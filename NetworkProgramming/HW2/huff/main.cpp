#include "huff.h"
int main(){
    huff_encode("test.jpg");
    cout<<"Encode done\n";
    huff_decode("test.jpg");
    cout<<"Decode done\n";
    return 0;
}