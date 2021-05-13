#include <iostream>
#include <string>
using namespace std;
int main(){
    int head,tail,i,j;
    bool flag=0;
    string input;
    while(cin>>input){
        flag=0;
        if(input=="0")
            break;
        
        for(i=input.length()-1;i>=0;i--){
            if(flag&&input[i]=='0'){
                if(head==tail){
                    flag=0;
                    continue;
                }
                if(head==input.length()-1){
                    input.push_back('-');
                }else{
                    input[head-1]='-';
                }
                for(j=head;j>=tail;j--){
                    input[j]='0';
                }
                input[i]='+';
                flag=0;
            }
                

            if(!flag&&input[i]=='1'){
                flag=1;
                head=i;
                tail=i; 
            }
                
            if(flag&&input[i]=='1'){
                tail=i;
            }
        }

        if(flag&&tail!=head){
            if(head==input.length()-1){
                input.push_back('-');
            }else{
                input[head-1]='-';
            }
            for(j=head;j>=tail;j--){
                input[j]='0';
            }
            input='+'+input;
            flag=0;
        }

        for(i=0;i<input.length();i++){
            if(input[i]=='1')
                input[i]='+';
        }

        cout<<input<<endl;
        
    }
}