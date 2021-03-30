#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
void prompt(){
    printf("[ client@HW2 ]$ ");
}

int clientlink(int *fd,char *ip,char *port);

int sendfile(int *fd,char *filename);

int main(){
    int fd,linkflag=0;
    char command[10],ipbuf[20],portbuf[10],filenamebuf[128];
    memset(command,0,sizeof(command));
    memset(ipbuf,0,sizeof(ipbuf));
    memset(portbuf,0,sizeof(portbuf));
    memset(filenamebuf,0,sizeof(filenamebuf));

    //terminal
    while(1){
        prompt();
        scanf("%s",command);
        if( !strcmp(command,"link") ){
            scanf("%s%s",ipbuf,portbuf);
            if(clientlink(&fd,ipbuf,portbuf)<0){
                printf("link failed\n");
            }else{
                linkflag = 1;
            }
        }

        else if( !strcmp(command,"send") ){
            scanf("%s",filenamebuf);
            if(!linkflag){
                printf("You have to link first\n");
            }
            else{
                sendfile(&fd,filenamebuf);
            }
        }

        else if( !strcmp(command,"leave") ){
            break;
        }

        else 
            printf("invalid command\n");
    }

    //link example
    //clientlink(&fd,"192.168.50.11","1500");

    //sendfile example
    //sendfile(&fd,"test.jpg");
}

int clientlink(int *fd,char *ip,char *port){
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;

    if( getaddrinfo(ip,port,&hints,&res) <0 ){
        perror("get addrinfo ");
        return -1;
    }

    *fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    printf("New Socket fd: %d\n",*fd);

    if(connect(*fd,res->ai_addr,res->ai_addrlen)<0){
        perror("connect");
        return -1;
    }
    printf("Connected\n");
    return 1;
}

int sendfile(int *fd,char *filename){
    int bytesread;
    char buf[512];
    
    if( write(*fd,filename,sizeof(filename)) < 0){
        perror("write filename");
        exit(1);
    }
    printf("filename:%s, size:%ld\n",filename,sizeof(filename));
    printf("filename sent\n");

    int sendfile = open(filename,O_RDONLY);
    if(sendfile<0){
        perror("Open file");
        exit(1);
    }

    while( (bytesread=read(sendfile,buf,sizeof(buf))) > 0){
        if( write(*fd,buf,sizeof(buf)) < 0 ){
            perror("write file");
            exit(1);
        }
    }
    write(*fd,buf,sizeof(buf));
    printf("file sent\n");
    close(sendfile);
    close(*fd);
}