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
int main(){
    int fd,bytesleft;
    struct addrinfo hints,*res;

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;

    getaddrinfo("192.168.50.11","1500",&hints,&res);

    fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    printf("New Socket fd: %d\n",fd);

    if(connect(fd,res->ai_addr,res->ai_addrlen)<0){
        perror("connect");
        exit(1);
    }
    printf("Connected\n");

    char buf[512]="testpic.png";
    int nbytes;
    if( nbytes = write(fd,buf,sizeof(buf)) < 0){
        perror("write filename");
        exit(1);
    }
    printf("filename sent\n");

    int sendfile = open(buf,O_RDONLY);
    if(sendfile<0){
        perror("Open file");
        exit(1);
    }

    while(bytesleft=read(sendfile,buf,sizeof(buf)) >=0){
        printf("Bytesleft: %d\n",bytesleft);
        if( write(fd,buf,sizeof(buf)) < 0 ){
            perror("write file");
            exit(1);
        }
    }
    
    printf("file sent\n");
    close(fd);
    close(sendfile);
}