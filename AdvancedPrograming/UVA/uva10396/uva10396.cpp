#include <iostream>
#include <cstring>
#include <set>
#include <cmath>
using namespace std;
bool check(int a,int b,int num){
    int check[10];
    int c = a*b;
    if(c/pow(10,num)==0){
        return 0;
    }
    memset(check,0,sizeof(check));
    while(a>0){
        check[a%10]++;
        a/=10;
    }
    while(b>0){
        check[b%10]++;
        b/=10;
    }
    while(c>0){
        check[c%10]--;
        c/=10;
    }
    for(int i=0;i<10;i++){
        if(check[i]!=0){
            return 0;
        }
    }
    return 1;
}

int main(){
    set<int> ans[9];
    int i,j;
    for(i=10;i<100;i++){
        for(j=10;j<100;j++){
            //cout<<i<<" "<<j<<endl;
            if(((i*j)%2)==1)
                continue;
            if(((i%10)==0) && ((j%10)==0))
                continue;
            if(check(i,j,4)){
                ans[4].insert(i*j);
            }
            
        }
    }
    for(i=100;i<1000;i++){
        for(j=100;j<1000;j++){
            if((i*j)%2==1)
                continue;
            if(i%10==0 && j%10==0)
                continue;
            if(check(i,j,6)){
                ans[6].insert(i*j);
            }
        }
    }
    for(i=1000;i<10000;i++){
        for(j=1000;j<10000;j++){
            if((i*j)%2==1)
                continue;
            if(i%10==0 && j%10==0)
                continue;
            if(check(i,j,8)){
                ans[8].insert(i*j);
            }
        }
    }
    int check;
    while(cin>>check){
        for(const auto &s:ans[check]){
            cout<<s<<endl;
        }
        cout<<endl;
    }
    
}