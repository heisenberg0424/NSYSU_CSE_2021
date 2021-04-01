#include <iostream>
#include <cmath>
using namespace std;
int main(){
    int input,temp;
    while(cin>>input){
        temp=input + ceil(log2(input)) - 2;
        cout<<temp<<endl;
    }

}