#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

#define MAX 2000001
using namespace std;
struct ttt{
    short num;
    int id;
};
bool compare (struct ttt a,struct ttt b){
    return a.num<b.num;
}

int main(){
    vector <int> lastfactor;
    vector<struct ttt> dp;
    dp.resize(MAX);
    lastfactor.resize(MAX);
    int i,j;
    for(i=0;i<MAX;i++){
        lastfactor[i]=1;
        dp[i].id=i;
        dp[i].num=0;
    }
    for(i=2;i<MAX;i++){
        if(lastfactor[i]!=1)
            continue;
        for(j=i+i;j<MAX;j+=i){
            if(lastfactor[j]==1)
                lastfactor[j]=j/i; 
        }
    }
    for(i=2;i<MAX;i++){
        dp[i].num=dp[lastfactor[i]].num+1;
    }

    stable_sort(dp.begin(),dp.end(),compare);
    int temp;
    int cs=1;
    while(cin>>temp && temp!=0){
        cout<<"Case "<<cs++<<": "<<dp[temp].id<<endl;
    }
}
    
