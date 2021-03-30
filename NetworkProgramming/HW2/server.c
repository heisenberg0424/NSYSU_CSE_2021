#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int fd;
    struct sockaddr_in srv;
    struct sockaddr_in cli;

    fd = socket(AF_INET,SOCK_STREAM,0);
    printf("New socket \n");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(1500);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd,(struct sockaddr* ) &srv, sizeof(srv))<0){
        perror("bind");
        exit(1);
    }
    printf("Bind complete\n");

    if(listen(fd,5)<0){
        perror("listen");
        exit(1);
    }
    printf("Listening...\n");

    int newfd;
    socklen_t cli_len=sizeof(cli);
    newfd = accept(fd,(struct sockaddr*) &cli,&cli_len);
    if(newfd < 0){
        perror("accept");
        exit(1);
    }
    printf("Acceped\n");

    char buf[512];
    char pwd[512]="rev/";
    int nbytes;
    memset(buf,0,sizeof(buf));
    if( (nbytes = read(newfd,buf,sizeof(buf))) < 0){
        perror("read filename");
        exit(1);
    }
    printf("Filename : %s\n",buf);

    strcat(pwd,buf);
    int revfile = open(pwd,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU);

    memset(buf,0,sizeof(buf));
    while(read(newfd,buf,sizeof(buf))>0){
        write(revfile,buf,sizeof(buf));
    }
    printf("File recieved\n");

    close(revfile);
    close(newfd);
    close(fd);

}