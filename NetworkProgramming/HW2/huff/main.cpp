#include "huff.h"
int main(){
    int a,b;
    //huff_encode("test.txt",&a,&b);
    //cout<<"Encode done\n";
    //cout<<"size: "<<a<<" ,"<<b<<endl;
    huff_decode("../test.txt");
    cout<<"Decode done\n";
    return 0;
}