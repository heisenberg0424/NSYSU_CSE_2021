#include <iostream>
#include <cstring>
using namespace std;

int main(){
    int cnt[100]={0};
    int queue[100]={0};
    int top=0;
    string input;
    int count=0;
    int ans=1;
    cin>>count;
    
    while(count--){
        cin>>input;
        queue[top++]=input[0];
        //cnt[input[0]]++;
        for(int i=1;i<input.length();i++){
            if(queue[top-1] == input[i]){
                queue[top-1] = 0;
                top--;
                if(top!=0){
                    cnt[queue[top-1]]++;
                }
                
            }
            else{
                queue[top++]=input[i];
                cnt[input[i]]++;
            }
        }

        cout<<"Case "<<ans++<<endl;
        for(int i=0;i<100;i++){
            if(cnt[i]!=0){
                cout<<(char)i<<" = "<<cnt[i]<<endl;
            }
        }

        // for(int i=0;i<100;i++){
        //     cnt[i]=0;
        // }
        memset(cnt,0,sizeof(cnt));
        memset(queue,0,100);
        top=0;
        
    }

}