#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <stdio.h>
#include <cstring>
using namespace std;

int main(){
    int cnt,min,max;
    bool flag;
    vector<int> table;
    set<int> ans;
    table.resize(1000001);
    
    fill(table.begin(), table.end(),1);
    table[0]=table[1]=0;
    for(int i=2;i<table.size();i++){
        if(table[i]==1){
            for(int j=i+i;j<table.size();j+=i){
                table[j]=0;
            }
        }
    }
    //cout<<"here";
    for(int i=2;i<table.size();i++){
        if(table[i]==1){
            string temp=to_string(i);
            int sum=0;
            for(int j=0;j<temp.size();j++){
                sum+=temp[j]-'0';
            }            
            if(table[sum]==1){
                ans.insert(i);
            }
        }
    }
    //cout<<"hello"<<endl;
    fill(table.begin(), table.end(),0);
    //cout<<"hello"<<endl;
    int fill=0;
    int i=0;
    for(const auto &s:ans){
        while(i!=s&&i<table.size()){
            table[i]=fill;
            i++;
        }
        table[i++]=++fill;
    }
    //fill++;
    for(i++;i<table.size();i++){
        table[i]=fill;
    }
    //cout<<"hello"<<endl;
    scanf("%d",&cnt);
    while(cnt--){
        int myanswer=0;
        scanf("%d%d",&min,&max);
        myanswer=table[max]-table[min-1];
        printf("%d\n",myanswer);
    }
}