#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <vector>
#define DEBUG 1
using namespace std;

int clientlink(int *fd,char *ip,char *port);


int main(){
    int clientfd,i,j;
    char buf[512],ip[128],port[8],username[16];
    string input,tmp;
    fd_set master,read_fds;
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_SET(0,&master);

    cout<<"         Welcome back!"<<endl;
    cout<<"-------------------------------"<<endl;
    cout<<"Login to server"<<endl;
    cout<<"IP: ";
    //cin>>ip;
    cout<<"Port: ";
    //cin>>port;
    cout<<"User name: ";
    cin>>username;

    if(DEBUG){
        cout<<ip<<":"<<port<<"  "<<username<<endl;
    }
    clientlink(&clientfd,"192.168.50.11","3000");
    send(clientfd,username,sizeof(username),0);
    //clientlink(&clientfd,ip,port);
    FD_SET(clientfd,&master);
    
    while(1){
        read_fds=master;
        select(clientfd+1,&read_fds,NULL,NULL,NULL);
        if(FD_ISSET(0,&read_fds)){
            cin>>input;
            if(input=="bye"){
                close(clientfd);
                cout<<"Logged out\n";
                return 0;
            }
            
            if(input=="send"){
                vector<string> destname;
                cin>>input;
                while(input[0]!='\"'){
                    destname.push_back(input);
                    cin>>input;
                }
                getline(cin,tmp);
                input+=tmp;
                
                for(i=0;i<destname.size();i++){
                    if(send(clientfd,destname[i].c_str(),destname[i].size(),0)<0){
                        perror("sendname");
                        exit(1);
                    }
                    if(send(clientfd,input.c_str(),input.size(),0)<0){
                        perror("sendmessage");
                        exit(1);
                    }
                    if(DEBUG){
                        cout<<"Sent "<<destname[i]<<"Size: "<<destname[i].size()<<endl;
                        cout<<"Sent "<<input<<"Size: "<<input.size()<<endl;
                    }
                }
            }

        }
        else if(FD_ISSET(clientfd,&read_fds)){
            recv(clientfd,buf,sizeof(buf),0);
            cout<<buf<<endl;
        }
        else{
            perror("unknown fd");
            exit(1);
        }
    }
}

/**---------------------------------------------------------------
 * clientlink : link the client to server
 * [variables]
 *  fd : the file descriptor of socket connects to server
 *  ip : server's ip 
 *  port : server's port
 -----------------------------------------------------------------
 */
int clientlink(int *fd,char *ip,char *port){
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;

    if( getaddrinfo(ip,port,&hints,&res)!=0 ){
        perror("get addrinfo");
        return -1;
    }


    *fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol); //get file descriptor

    if(connect(*fd,res->ai_addr,res->ai_addrlen)<0){
        perror("connect");
        return -1;
    }
    printf("The server with IP address \"%s\" has accepted your connection.\n",ip);
    return 1;
}