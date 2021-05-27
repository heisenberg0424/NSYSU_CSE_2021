#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctime>
#define MSS 1000
#define PORT 3000
#define DEBUG 1
#define ACK 1
#define SYN 4 
#define DNS 1
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
int dns(pkt request);



int fd;
pkt recvPkt,sendPkt;
struct sockaddr_in srv,cli;
socklen_t clilen = sizeof(cli);

int main(){
    srand(time(NULL));
    int nbytes;
    
    
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));
    
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    test( (fd=socket(AF_INET,SOCK_DGRAM,0)),"socket" );
    test( bind(fd,(struct sockaddr*) &srv,sizeof(srv)) , "bind");

    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen);
    cout<<"Received a SYN     packet, seqNum: "<<recvPkt.seqNum<<endl;
    sendPkt.seqNum = rand()%10001;
    sendPkt.ackNum = recvPkt.seqNum+1;
    sendPkt.flag[SYN]=sendPkt.flag[ACK]=1;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));
    cout<<"Sending  a SYN&ACK packet, seqNUM: "<<sendPkt.seqNum<<" ackNUM: "<<sendPkt.ackNum<<endl;
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));

    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen);
    cout<<"Received a ACK     packet, ackNum: "<<recvPkt.ackNum<<endl;
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));

    while(1){
        recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen);
        cout<<"New request incomming!"<<endl;
        
        switch(recvPkt.mode){
            case DNS:
                dns(recvPkt);
                break;
        }

        memset(&recvPkt,0,sizeof(recvPkt));
        memset(&sendPkt,0,sizeof(sendPkt));

    }


}

int dns(pkt request){
    cout<<"Checking on DNS server..."<<endl;
    struct addrinfo hints,*res;
    void *addr;
    memset(&hints,0,sizeof(hints));
    memset(&sendPkt,0,sizeof(sendPkt));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char ipstr[INET6_ADDRSTRLEN];

    getaddrinfo(request.data,NULL,&hints,&res);
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    addr = &(ipv4->sin_addr);
    inet_ntop(AF_INET,addr,ipstr,sizeof(ipstr));
    cout<<"Sending result back :"<<ipstr<<endl;
    strcpy(sendPkt.data,ipstr);
    sendPkt.mode=DNS;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));

    memset(&sendPkt,0,sizeof(sendPkt));
    return 0;
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