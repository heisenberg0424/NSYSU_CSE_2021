#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctime>
#include <cmath>
#include <fstream>
#define MSS 1000
#define PORT 3000
#define DEBUG 1
#define ACK 1
#define SYN 4 
#define FIN 5
#define DNS 1
#define MATH 2
#define FILE 3
using namespace std;

typedef struct segment{
    char srcPort[2];
    char destPort[2];
    uint32_t seqNum;
    uint32_t ackNum;
    bool flag[6];
    uint16_t recvWindow;
    char checksum[2];
    char data[MSS];
    int mode;
}pkt;

void test(int func,const char* errormsg);

int fd;
pkt recvPkt,sendPkt;
struct sockaddr_in srv,cli;
socklen_t clilen = sizeof(cli);

int main(){
    srand(time(NULL));
    int nbytes,ports=1;
    
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));
    
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    test( (fd=socket(AF_INET,SOCK_DGRAM,0)),"socket" );
    test( bind(fd,(struct sockaddr*) &srv,sizeof(srv)) , "bind");

    while(1){
        recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen);
        cout<<"Received a SYN     packet, seqNum: "<<recvPkt.seqNum<<endl;

        int pid = fork();
        if(pid==0){
            cout<<"Hello!"<<endl;
            int newfd;
            srv.sin_port = htons(PORT+ports);
            test( (fd=socket(AF_INET,SOCK_DGRAM,0)),"socket" ); 
            test( bind(fd,(struct sockaddr*) &srv,sizeof(srv)) , "bind");
        }
    }
    
    
}


void test(int func,const char* errormsg){
    if(func<0){
        perror(errormsg);
        exit(1);
    }
    else if(DEBUG){
        cout<<errormsg<<" done"<<endl;
    }
}