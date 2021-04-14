#include "huff2.h"
#define ENDEBUG 0
#define DEDEBUG 0
#define OFFSET 128 
using namespace std;
int huff2_encode(string path,int *compressedfilesize,int *codebooksize){
    long long original,compress,book;
    fstream input;
    long freqcnt[256]={0};
    map<int,string> codebook;
    
    input.open(path.c_str(),ios::in|ios::binary);
    if(!input){
        cout<<"File path doesn't exist\n";
        return -1;
    }  

    filefreq(input,freqcnt);
    input.close();
    if(ENDEBUG){
        cout<<"COUNT FREQ DONE"<<endl;
    }

    huff2_gencode(freqcnt,codebook);
    if(ENDEBUG){
        cout<<"HUFF CODE DONE"<<endl;
    }
    
    input.open(path,ios::in|ios::binary);
    huff2_output(input,path,codebook);
    if(ENDEBUG){
        cout<<"OUTPUT FILE DONE"<<endl;
    }

    input.open(path.c_str(),ios::in|ios::binary);
    input.seekg(0,ios::end);
    
    original=input.tellg();
    input.close();
    input.open((path+"-coded").c_str(),ios::in|ios::binary);
    input.seekg(0,ios::end);
    compress=input.tellg();
    input.close();
    input.open((path+"-codebook").c_str(),ios::in|ios::binary);
    input.seekg(0,ios::end);
    *codebooksize=input.tellg();
    input.close();
    cout<<"Original file length: "<<original<<" bytes"<<endl;
    cout<<"Compressed file length: "<<compress<<" bytes"<<endl;
    cout<<"Ratio: "<<(float)compress*100/original<<'%'<<endl;
    *compressedfilesize=compress;

    return 0;
}
int huff2_gencode(long int *table,map<int,string> &codebook){
    int variables=0;
    int code=0,len=0,temp=1;
    for(int i=0;i<256;i++){
        if(table[i]){
            variables++;
        }
    }
    len = ceil(log2(variables));
    for(int i=0;i<256;i++){
        if(table[i]){
            codebook[i]=decimal2binary(code++,len);
            if(ENDEBUG){
                cout<<(char)i<<" :"<<codebook[i]<<endl;
            }
        }
        
    }
    return 0;
}

int huff2_output(fstream &input,string outputpath,map<int,string> &codebook){
    char temp;
    unsigned char bitoperate;
    int extrabits,eightbitchar;
    string binarystring="",codebookpath,codedpath;
    input>>noskipws;
    while(input>>temp){
        binarystring+=codebook[temp+OFFSET];
    }
    

    fstream output;
    codebookpath=outputpath+"-codebook";
    codedpath = outputpath+"-coded";
    extrabits=binarystring.length()%8;

    output.open(codebookpath.c_str(),ios::out|ios::trunc|ios::binary);
    if(!output){
        cout<<"Write code book failed"<<endl;
    }
    output<<extrabits<<endl;
    for(int i=0;i<256;i++){
        if(codebook.find(i)!=codebook.end()){
            if(ENDEBUG)
                cout<<i<<": "<<codebook[i]<<endl;
            output<<i-OFFSET<<" "<<codebook[i]<<endl;
        }
    }
    output.close();

    output.open(codedpath.c_str(),ios::out|ios::trunc|ios::binary);
    if(!output){
        cout<<"Write coded file failed"<<endl;
    }
    
    for(int i=0;i<extrabits;i++){
        binarystring+='0';
    }
    for(int i=0;i<binarystring.length();i+=8){
        eightbitchar=0;
        for(int j=0;j<8;j++){
            bitoperate = binarystring[i+j]-'0';
            eightbitchar+=bitoperate<<7-j;
        }
        output<<(char)eightbitchar;
    }

    input.close();
    output.close();
    return extrabits;
}

string decimal2binary(int input,int len){
    string output="";
    for(int i=len-1;i>=0;i--){
        if(input>=pow(2,i)){
            output+="1";
            input-=pow(2,i);
        }
        else{
            output+="0";
        }
    }
    return output;
}