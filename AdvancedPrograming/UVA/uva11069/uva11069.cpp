#include <iostream>
using namespace std;
int main(){
    int ans[77];
    ans[1]=1;
    ans[2]=2;
    ans[3]=2;
    for(int i=4;i<77;i++){
        ans[i]=ans[i-2]+ans[i-3];
    }
    int target;
    while(cin>>target){
        cout<<ans[target]<<endl;
    }
}