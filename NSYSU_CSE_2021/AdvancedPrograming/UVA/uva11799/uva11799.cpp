#include <iostream>
using namespace std;
int main(){
    int num,cnt,time=1,temp,a;
    cin >> cnt;
    while(cnt--){
        temp=0;
        cin>>num;
        while(num--){
            cin>>a;
            if(a>temp){
                temp=a;
            }
        }
        cout<<"Case "<<time++<<": "<<temp<<endl;
    }
}