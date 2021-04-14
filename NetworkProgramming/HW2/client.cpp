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
#include "huff/huff.h"

void prompt(){
    printf("[ client@HW2 ]$ ");
}

int clientlink(int *fd,char *ip,char *port);

int sendfile(int *fd,char *filename);

long long filesize(char *filename);

int main(){
    int fd,linkflag=0;
    char command[10],ipbuf[128],portbuf[10],filenamebuf[128];
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

    if( getaddrinfo(ip,port,&hints,&res)!=0 ){
        perror("get addrinfo");
        return -1;
    }


    *fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    //printf("New Socket fd: %d\n",*fd);

    if(connect(*fd,res->ai_addr,res->ai_addrlen)<0){
        perror("connect");
        return -1;
    }
    printf("The server with IP address \"%s\" has accepted your connection.\n",ip);
    return 1;
}

int sendfile(int *fd,char *filename){
    int bytesread;
    int codedsize,codebooksize,filenamelen;
    char buf[512];
    memset(buf,0,sizeof(buf));
    huff_encode(filename,&codedsize,&codebooksize);
    
    if( write(*fd,&codedsize,sizeof(int)) < 0){
        perror("write filesize");
        return 1;
    }
    printf("filesize1 sent\n");
    if( write(*fd,&codebooksize,sizeof(int)) < 0){
        perror("write filesize");
        return 1;
    }
    printf("filesize2 sent\n");

    filenamelen=strlen(filename);
    if( write(*fd,&filenamelen,sizeof(int)) < 0){
        perror("write filenamelen");
        return 1;
    }
    printf("filenamelen sent\n");
    if( write(*fd,filename,(size_t)filenamelen) < 0){
        perror("write filename");
        return 1;
    }
    printf("filename sent \n");
    
    char codedfilepath[128]="";
    char codebookpath[128]="";
    strcat(codedfilepath,filename);
    strcat(codedfilepath,"-coded");
    strcat(codebookpath,filename);
    strcat(codebookpath,"-codebook");

    int sendfile = open(codedfilepath,O_RDONLY);
    if(sendfile<0){
        perror("Open coded file");
        return 1;
    }

    memset(buf,0,sizeof(buf));
    while( (bytesread=read(sendfile,buf,sizeof(buf))) > 0){
        cout<<"read: "<<bytesread<<endl;
        if( write(*fd,buf,sizeof(buf)) < 0 ){
            perror("write file");
            return 1;
        }
        memset(buf,0,sizeof(buf));
    }
    printf("Coded file sent\n");
    close(sendfile);

    sendfile = open(codebookpath,O_RDONLY);
    if(sendfile<0){
        perror("Open codebook");
        return 1;
    }

    memset(buf,0,sizeof(buf));
    while( (bytesread=read(sendfile,buf,sizeof(buf))) > 0){
        if( write(*fd,buf,sizeof(buf)) < 0 ){
            perror("write file");
            return 1;
        }
        memset(buf,0,sizeof(buf));
        cout<<"read: "<<bytesread<<endl;
    }
    printf("Codebook sent\n");
    close(sendfile);

    close(*fd);
    return 0;
}