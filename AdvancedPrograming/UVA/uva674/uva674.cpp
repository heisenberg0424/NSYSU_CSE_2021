#include <iostream>
#include <cstring>
using namespace std;
int main(){
    int arr[7490],i,j,input;
    int coin[4]={5,10,25,50};
    for (i=0;i<7490;i++){
        arr[i]=1;
    }
    
    for(i=0;i<4;i++){
        for (j=0;j<7490;j++){
            if(j>=coin[i]){
                arr[j]+=arr[j-coin[i]];
            }
        }
    }

    while(cin>>input){
        cout<<arr[input]<<endl;
    }
}