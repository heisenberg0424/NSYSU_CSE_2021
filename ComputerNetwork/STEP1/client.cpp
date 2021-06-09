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
#define MSS 1000
#define PORT 3000
#define IP "192.168.50.11"
#define DEBUG 0
#define ACK 1
#define SYN 4
#define FIN 5
#define DNS 1
#define MATH 2
#define FILE 3
using namespace std;

void test(int func,const char* errormsg);

typedef struct segment{
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
    int math(string function);
    int file(string filename);
    void help();
    TCP();
    ~TCP(){}
private:
    pkt recvPkt,sendPkt;
    bool connected;
    int fd;
    struct sockaddr_in srv;
    socklen_t srvlen;
    void pktClear(){
        memset(&sendPkt,0,sizeof(sendPkt));
        memset(&recvPkt,0,sizeof(recvPkt));
    }
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
    pktClear();

    sendPkt.mode = DNS;
    strcpy(sendPkt.data,website.c_str());
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    cout<<"Sending dns request to server "<<endl;
    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
    cout<<"Received IP address from server :"<<recvPkt.data<<endl;

    pktClear();
    return 0;
}

int TCP::math(string function){
    pktClear();
    
    sendPkt.mode = MATH;
    strcpy(sendPkt.data , function.c_str());
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    cout<<"Sending mathematical equation to server "<<endl;
    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
    cout<<"Recevied answer from server :"<<recvPkt.data<<endl;

    pktClear();
    return 0;
}

int TCP::file(string filename){
    pktClear();
    int size,offset,ack=0;
    int file = open(("recv/"+filename).c_str(),O_WRONLY|O_TRUNC|O_CREAT,S_IRWXU);

    strcpy(sendPkt.data,filename.c_str());
    sendPkt.mode = FILE;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
    size = recvPkt.ackNum;
    offset = recvPkt.seqNum;
    pktClear();
    sendPkt.ackNum=ack;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));

    while(size>MSS){
        recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
        cout<<"Received pkt from server (seq: "<<recvPkt.seqNum<<" )"<<endl;
        if(recvPkt.seqNum-offset==ack){
            test(write(file,recvPkt.data,MSS),"write file");
            ack+=MSS;
            size-=MSS;
        }
        pktClear();
        sendPkt.ackNum=ack;
        sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));
    }
    recvfrom(fd,&recvPkt,sizeof(recvPkt),0,(struct sockaddr*) &srv,&srvlen);
    write(file,recvPkt.data,size);
    pktClear();
    sendPkt.flag[FIN]=1;
    sendto(fd,&sendPkt,sizeof(sendPkt),0,(struct sockaddr*) &srv,sizeof(srv));

    close(file);
    pktClear();
    return 0;
}

void TCP::help(){
    cout<<"Commands Example: "<<endl;
    cout<<"dns www.example.com"<<endl;
    cout<<"math add 3.14 2"<<endl;
    cout<<"file myfile.txt"<<endl;
}

int main(){
    string mode, command;
    srand(time(NULL));
    TCP client;
    client.handshake();

    if(DEBUG){
        client.dns("www.google.com");
        client.math("add 3.14 2");
        client.math("sub 2 5");
        client.math("mul 2 5.7");
        client.math("divide 40.5 5");
        client.math("power 2 3");
        client.math("sqrt 26");
        client.file("test");
        return 0;
    }
    
    cout<<"Welcome to udp client, enter command or '--help' "<<endl;
    while(1){
        cout<<"[UDP@client]: ";
        cin>>mode;
        if(mode=="help"){
            client.help();
            continue;
        }
        cin.ignore();
        getline(cin,command);
        
        if(mode=="dns"){
            client.dns(command);
        }
        if(mode=="math"){
            client.math(command);
        }
        if(mode=="file"){
            client.file(command);
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