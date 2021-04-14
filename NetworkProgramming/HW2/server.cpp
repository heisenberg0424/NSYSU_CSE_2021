#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "huff/huff.h"
using namespace std;

int main(){
    int fd;
    int codedfilesize,codebooksize,filenamelen;
    struct sockaddr_in srv,cli,clientinfo;
    char clientip[16];
    int clientport;
    memset(clientip,0,sizeof(clientip));
    socklen_t clientinfo_len=sizeof(clientinfo);

    fd = socket(AF_INET,SOCK_STREAM,0);
    //printf("New socket \n");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(1500);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd,(struct sockaddr* ) &srv, sizeof(srv))<0){
        perror("bind");
        exit(1);
    }
    //printf("Bind complete\n");

    while(1){
        if(listen(fd,5)<0){
            perror("listen");
            exit(1);
        }
        printf("\n###################\n");
        printf("listening...\n");
        int newfd;
        socklen_t cli_len=sizeof(cli);
        newfd = accept(fd,(struct sockaddr*) &cli,&cli_len);
        if(newfd < 0){
            perror("accept");
            exit(1);
        }
        //printf("connected\n");

        getpeername(newfd, (struct sockaddr *) &clientinfo,&clientinfo_len);
        inet_ntop(AF_INET,&clientinfo.sin_addr,clientip,sizeof(clientip));
        clientport = ntohs(clientinfo.sin_port);
        printf("A client \"%s\" has connected via port num %d using SOCKET_STREAM(TCP)\n",clientip,clientport);
        char buf[512];
        char pwd[512]="rev/";
        int nbytes;
        memset(buf,0,sizeof(buf));

        if( (nbytes = read(newfd,&codedfilesize,sizeof(int))) < 0){
            perror("read filesize");
            exit(1);
        }
        printf("Filesize1 : %d\n",codedfilesize);
        if( (nbytes = read(newfd,&codebooksize,sizeof(int))) < 0){
            perror("read filesize");
            exit(1);
        }
        printf("Filesize2 : %d\n",codebooksize);
        if( (nbytes = read(newfd,&filenamelen,sizeof(int))) < 0){
            perror("read filesize");
            exit(1);
        }
        printf("Filenamelen : %d\n",filenamelen);

        char filename[filenamelen+1];
        memset(filename,0,sizeof(filename));
        if( (nbytes = read(newfd,filename,filenamelen)) < 0){
            perror("read filename");
            exit(1);
        }
        
        printf("Filename : %s\n",filename);

        strcat(pwd,filename);
        strcat(pwd,"-coded");
        int revfile = open(pwd,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);

        int temp;
       
        while(codedfilesize>=512){
            memset(buf,0,sizeof(buf));
            codedfilesize-=read(newfd,buf,sizeof(buf));
            write(revfile,buf,sizeof(buf));
        }
        read(newfd,buf,codedfilesize);
        write(revfile,buf,codedfilesize);
        read(newfd,buf,512-codedfilesize);

        printf("coded file recieved\n");
        close(revfile);

        memset(pwd,0,sizeof(pwd));
        strcat(pwd,"rev/");
        strcat(pwd,filename);
        strcat(pwd,"-codebook");
        revfile = open(pwd,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
        while(codebooksize>=512){
            memset(buf,0,sizeof(buf));
            codebooksize-=read(newfd,buf,sizeof(buf));
            write(revfile,buf,sizeof(buf));
        }
        memset(buf,0,sizeof(buf));
        read(newfd,buf,codebooksize);
        write(revfile,buf,codebooksize);
        printf("code book recieved\n");
        close(revfile);

        memset(pwd,0,sizeof(pwd));
        strcat(pwd,"rev/");
        strcat(pwd,filename);
        huff_decode(pwd);
        printf("File decoded\n");
        
        close(newfd);
    }
    close(fd);

}