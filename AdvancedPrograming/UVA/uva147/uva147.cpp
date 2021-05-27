#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

int main(){
    int coin[]={5,10,20,50,100,200,500,1000,2000,5000,10000};
    long long dp[30001]={0};
    dp[0]=1;
    
    for(int i=0;i<11;i++){
        for(int j=coin[i];j<30001;j+=5){
            dp[j]+=dp[j-coin[i]];
        }
    }

    double input;

    while(cin>>input&&input!=0){
        int tmp = input*100;
        if(tmp%5!=0){
            tmp++;
        }
        printf("%6.2f%17lld\n",input,dp[tmp]);
    }

}