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
#include <random>
#define MSS 1000
#define PORT 3000
#define DEBUG 0
#define ACK 1
#define CHECKSUM 2
#define SYN 4 
#define FIN 5
#define DNS 1
#define MATH 2
#define FILE 3
using namespace std;

// Global variables
struct sockaddr_in srv,cli;
socklen_t clilen = sizeof(cli);

// functions
void test(int func,const char* errormsg);
int filesize(const char *filename);
int fdlisten(int port);

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

class Server{
public:
    Server(){}
    Server(int port,pkt packet);
    void handleRequest();
    void pktClear();
private:
    int fd,ack;
    pkt recvPkt,sendPkt;
    int dns(pkt request);
    int math(pkt request);
    int file(pkt request);
    int filesize(const char *filename);
};

Server::Server(int port,pkt packet){
    fd = fdlisten(port);
    // C++ 11 random
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> genrand(1,10000);
    pktClear();

    //Three-way handshake
    sendPkt.seqNum = genrand(gen);
    sendPkt.ackNum = packet.seqNum+1;
    sendPkt.flag[SYN]=sendPkt.flag[ACK]=1;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));
    cout<<"Sending  a SYN&ACK packet, seqNUM: "<<sendPkt.seqNum<<" ackNUM: "<<sendPkt.ackNum<<endl;
    pktClear();

    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen);
    cout<<"     Received a ACK     packet, ackNum: "<<recvPkt.ackNum<<endl;
    ack = recvPkt.ackNum+1;
    pktClear();
}

void Server::pktClear(){
    memset(&recvPkt,0,sizeof(recvPkt));
    memset(&sendPkt,0,sizeof(sendPkt));
}

void Server::handleRequest(){
    while(1){
        recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen);
        cout<<"New request incomming!"<<endl;
            
        switch(recvPkt.mode){
            case DNS:
                dns(recvPkt);
                break;
            case MATH:
                math(recvPkt);
                break;
            case FILE:
                file(recvPkt);
                break;
        }

        pktClear();
    }
}

int Server::dns(pkt request){
    cout<<"Checking on DNS server..."<<endl;
    struct addrinfo hints,*res;
    void *addr;
    memset(&hints,0,sizeof(hints));
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

    pktClear();
    return 0;
}

int Server::math(pkt request){
    pktClear();
    int i;
    string symbol="",split="";
    float x,y,ans;

    for(i=0;i<MSS;i++){
        if(request.data[i]!=' '){
            symbol+= request.data[i];
        }
        else{
            i++;
            break;
        }
    }

    if(DEBUG){
        cout<<"got math: "<<symbol<<endl;
    }

    for(i;i<MSS;i++){
        if(request.data[i]=='\0'){
            i=MSS;
            x = stof(split);
            split = "";
            break;
        }
        else if(request.data[i]!=' '){
            split+= request.data[i];
        }
        else{
            i++;
            x = stof(split);
            split = "";
            break;
        }
    }

    if(DEBUG){
        cout<<"x= "<<x;
    }

    for(i;i<MSS;i++){
        if(request.data[i]!='\0'){
            split+= request.data[i];
        }
        else{
            i++;
            y = stof(split);
            split = "";
            break;
        }
    }

    if(DEBUG){
        cout<<" y= "<<y<<endl;
    }

    cout<<"Calcualting function: "<<symbol<<", x = "<<x<<", y = "<<y<<endl;

    if(symbol=="add"){
        ans = x+y;
    }
    if(symbol=="sub"){
        ans = x-y;
    }
    if(symbol=="mul"){
        ans = x*y;
    }
    if(symbol=="divide"){
        ans = x/y;
    }
    if(symbol=="power"){
        ans = pow(x,y);
    }
    if(symbol=="sqrt"){
        ans = sqrt(x);
    }

    cout<<"Sending result back..."<<endl;
    strcpy(sendPkt.data,to_string(ans).c_str());
    sendPkt.mode = MATH;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));
    pktClear();
    
    return 0;
}

int Server::file(pkt request){
    pktClear();
    string filename = request.data;
    filename = "snd/"+filename;
    int file = open(filename.c_str(),O_RDONLY);
    int size = filesize(filename.c_str());
    const int offset = ack;
    int seq = offset;

    //Random init
    default_random_engine generator;
    bernoulli_distribution distribution(0.00001);

    sendPkt.seqNum = seq;
    sendPkt.ackNum = size;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));
    cout<<"Size: "<<sendPkt.ackNum<<endl;
    pktClear();
    cout<<"Transfer file to client..."<<endl;
    
    while(recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &cli,&clilen)&&recvPkt.flag[FIN]!=1){
        cout<<"     Received Packet: ACK: "<<recvPkt.ackNum<<endl;
        if(distribution(generator)){
            cout<<"!!!!! LOSS PACKET !!!!!"<<endl;
            sendPkt.flag[CHECKSUM]=1;
            sleep(1);
        }

        lseek(file,recvPkt.ackNum,SEEK_SET);
        read(file,sendPkt.data,MSS);
        sendPkt.seqNum = offset + recvPkt.ackNum;
        sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));
        cout<<"Sending file Pkt (seq: "<<sendPkt.seqNum<<")\n";
        ack = sendPkt.seqNum;
        pktClear();

        // 2 ACK
        if(distribution(generator)){
            cout<<"!!!!! LOSS PACKET !!!!!"<<endl;
            sendPkt.flag[CHECKSUM]=1;
            sleep(1);
        }

        read(file,sendPkt.data,MSS);
        sendPkt.seqNum = ack+MSS;
        sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &cli,sizeof(cli));
        cout<<"Sending file Pkt (seq: "<<sendPkt.seqNum<<")\n";
        ack = sendPkt.seqNum;
        pktClear();
    }

    cout<<"File transfer done"<<endl;
    close(file);
    pktClear();
    return 0;
}

int Server::filesize(const char *filename){
    int size;
    ifstream cnt(filename,ios::in|ios::binary);
    cnt.seekg(0,ios::end);
    size=cnt.tellg();
    cnt.close();
    return size;
}




int main(){
    int nbytes,newports=0;
    pkt packet;
    memset(&packet,0,sizeof(packet));
    
    int listenfd = fdlisten(PORT);

    while(1){
        recvfrom(listenfd,&packet,sizeof(packet),0,(struct sockaddr*) &cli,&clilen);
        cout<<"=== New Client Comming ==="<<endl;
        cout<<"     Received a SYN     packet, seqNum: "<<packet.seqNum<<endl;

        int pid = fork();
        newports++;
        if(pid==0){
            Server server(PORT+newports,packet);
            server.handleRequest();
            return 0;
        }
    }
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

int fdlisten(int port){
    int fd;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    test( (fd=socket(AF_INET,SOCK_DGRAM,0)),"socket" );
    test( bind(fd,(struct sockaddr*) &srv,sizeof(srv)) , "bind");
    return fd;
}
