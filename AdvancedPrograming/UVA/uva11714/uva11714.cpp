#include <iostream>
#include <cmath>
using namespace std;
int main(){
    int input,temp;
    while(cin>>input){
        temp=input-1+ ceil(log2(input))-1;
        cout<<(int)temp<<endl;
    }

}