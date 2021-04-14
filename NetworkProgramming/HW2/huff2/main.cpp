#include "huff2.h"
using namespace std;
int main(){
    int a,b;
    huff2_encode("test.txt",&a,&b);
    cout<<"encode done"<<endl;
    huff_decode("test.txt");
    cout<<"decode done"<<endl;

}