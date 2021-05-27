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
#define IP "192.168.50.11"
#define DEBUG 1 
#define ACK 1
#define SYN 4
#define DNS 1
using namespace std;

void test(int func,const char* errormsg);

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

class TCP{
public:
    int handshake();
    int dns(string website);
    TCP();
    ~TCP(){}
private:
    pkt recvPkt,sendPkt;
    bool connected;
    int fd;
    struct sockaddr_in srv;
    socklen_t srvlen;
};

TCP::TCP(){
    test( (fd=socket(AF_INET,SOCK_DGRAM,0)),"socket" );
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr(IP);
    srvlen = sizeof(srv);
}

int TCP::handshake(){
    int nbytes;
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));
    
    cout<<"===Start the 3-way handshake==="<<endl;
    sendPkt.flag[SYN]=1;
    sendPkt.seqNum=rand()%10001;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    cout<<"Sending  a SYN     packet, seqNUM: "<<sendPkt.seqNum<<endl;
    memset(&sendPkt,0,sizeof(sendPkt));
    
    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
    cout<<"Received a SYN&ACK packet, seqNUM: "<<recvPkt.seqNum<<" ackNUM: "<<recvPkt.ackNum<<endl;

    sendPkt.flag[ACK]=1;
    sendPkt.ackNum=recvPkt.seqNum+1;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    cout<<"Sending  a ACK     packet, ackNUM: "<<sendPkt.ackNum<<endl;
    cout<<"===complete the 3-way handshake==="<<endl;
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));

    return 0;
}

int TCP::dns(string website){
    memset(&sendPkt,0,sizeof(sendPkt));
    memset(&recvPkt,0,sizeof(recvPkt));

    sendPkt.mode = DNS;
    strcpy(sendPkt.data,website.c_str());
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    cout<<"Sending dns request to server "<<endl;
    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
    cout<<"Received IP address from server :"<<recvPkt.data<<endl;

    memset(&sendPkt,0,sizeof(sendPkt));
    memset(&recvPkt,0,sizeof(recvPkt));
}

int main(){
    srand(time(NULL));
    TCP client;
    client.handshake();
    client.dns("www.google.com");
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