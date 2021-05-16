#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unordered_map>
#include <iostream>
#define PORT "3000"

using namespace std;

fd_set master,read_fds;
int listenfd,fdmax,newfd;

void *get_in_addr(struct sockaddr *sa);
void broadcast(char *name,char *ip);

int main(){
    struct sockaddr_storage remoteaddr; //client address
    socklen_t addrlen;

    char buf[512];
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1; //for reuse address
    int i,j,serverinfo;

    struct addrinfo hints,*ai,*p;

    unordered_map<string,int> userfd;
    unordered_map<string,bool> offlinemessage;
    unordered_map<string,string> message;

    //init fd sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;  //IPV4
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;    //ANY IP

    if( (serverinfo = getaddrinfo(NULL,PORT,&hints,&ai))!=0 ){
        perror("getaddrinfo");
        exit(1);
    }

    listenfd = socket(ai->ai_family,ai->ai_socktype,0);
    if(listenfd<0){
        perror("socket");
        exit(1);
    }

    // in case of address in use
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

    if(bind(listenfd,ai->ai_addr,ai->ai_addrlen) <0){
        perror("bind");
        exit(1);
    }

    freeaddrinfo(ai);

    if(listen(listenfd,10)==-1){
        perror("listen");
        exit(1);
    }
    cout<<"Server listening on 192.168.50.11:3000"<<endl;

    FD_SET(listenfd,&master);
    fdmax = listenfd;

    while(1){
        read_fds = master;
        if(select(fdmax+1,&read_fds,NULL,NULL,NULL) == -1){
            perror("select");
            exit(1);
        }

        //find ready fd
        for(i=0;i<=fdmax;i++){
            if(FD_ISSET(i,&read_fds)){  //found ready socket
                if(i==listenfd){    //new connection
                    addrlen  = sizeof(remoteaddr);
                    newfd = accept(listenfd,(struct sockaddr *)&remoteaddr,&addrlen);
                    if(newfd == -1){
                        perror("accept");
                        exit(1);
                    }else{
                        FD_SET(newfd,&master);
                        if(newfd>fdmax){
                            fdmax = newfd;
                        }
                        inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP,INET6_ADDRSTRLEN);
                        recv(newfd,buf,sizeof(buf),0);
                        printf("selectserver: new connection from %s on socket %d Username: %s\n",remoteIP,newfd,buf);
                        broadcast(buf,remoteIP);
                        
                    }
                }

                else{   //data from client socket
                    if( (nbytes = recv(i,buf,sizeof(buf),0)) <=0 ){ //got error or fd closed
                        if(nbytes == 0){
                            //close fd
                            printf("selectserver: socket %d hung up\n",i);
                        }
                        else{
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i,&master);
                    }
                    else{
                        for(j=0;j<=fdmax;j++){
                            if(FD_ISSET(j,&master)){
                                if(j != listenfd && j != i){
                                    //don't brocast to listenfd and sender
                                    if(send(j,buf,sizeof(buf),0)==-1){
                                        perror("send");
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void broadcast(char *name,char *ip){
    char buf[512]="User ";
    strcat(buf,name);
    strcat(buf," is online,IP address:");
    strcat(buf,ip);
    for(int i=0;i<=fdmax;i++){
        if(FD_ISSET(i,&master) && i!=newfd && i!=listenfd){
            if(send(i,buf,sizeof(buf),0) <0 ){
                perror("broadcast");
                exit(1);
            }
        }
    }
}