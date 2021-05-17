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
#include <ctime>
#define PORT "3000"
#define DEBUG 1

using namespace std;

fd_set master,read_fds;
int listenfd,fdmax,newfd;
unordered_map<string,int> user2fd;
unordered_map<int,string> fd2user;
unordered_map<string,bool> offlinemessage;
unordered_map<string,string> message;

void *get_in_addr(struct sockaddr *sa);
void broadcast(int src,const char *name,const char *ip,bool online);
void unicast(int src,int dest,const char *msg);

int main(){
    struct sockaddr_storage remoteaddr; //client address
    socklen_t addrlen;

    char buf[512];
    int nbytes,src,dest;

    char remoteIP[INET6_ADDRSTRLEN];
    int yes = 1; //for reuse address
    int i,j,serverinfo;

    struct addrinfo hints,*ai,*p;

    

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
                        broadcast(newfd,buf,remoteIP,1);
                        fd2user[newfd]=buf;
                        user2fd[buf]=newfd;
                    }
                }

                else{   //data from client socket
                    if( (nbytes = recv(i,buf,sizeof(buf),0)) <=0 ){ //got error or fd closed
                        if(nbytes == 0){
                            //close fd
                            printf("selectserver: socket %d hung up\n",i);
                            broadcast(i,fd2user[i].c_str(),NULL,0);
                        }
                        else{
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i,&master);
                    }
                    else{
                        dest = user2fd[buf];
                        recv(i,buf,sizeof(buf),0);

                        if(DEBUG){
                            cout<<"recvedfrom : "<<dest<<" :"<<buf<<endl;
                        }

                        unicast(i,dest,buf);
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

void broadcast(int src,const char *name,const char *ip,bool online){
    char buf[512]="User ";
    strcat(buf,name);
    if(online){
        strcat(buf," is online,IP address:");
        strcat(buf,ip);
    }
    else{
        strcat(buf," is offline.");
    }
    
    for(int i=0;i<=fdmax;i++){
        if(FD_ISSET(i,&master) && i!=src && i!=listenfd){
            if(send(i,buf,sizeof(buf),0) <0 ){
                perror("broadcast");
                exit(1);
            }
        }
    }
}

void unicast(int src,int dest,const char *msg){
    char buf[512]="User ";
    time_t curtime;
    time(&curtime);

    strcat(buf,fd2user[src].c_str());
    strcat(buf," has sent you a message \"");
    strcat(buf,msg);
    strcat(buf,"\" at ");
    strcat(buf,ctime(&curtime));

    if(send(dest,buf,sizeof(buf),0) <0 ){
        perror("unicast");
        exit(1);
    }

}