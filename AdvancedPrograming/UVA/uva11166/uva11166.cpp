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
                input[i]='+';
                for(j=tail;j<head;j++){
                    input[j]='0';
                }
                input[head]='-';
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

        if(flag&&tail!=head&&input.length()!=2){
            for(j=tail;j<head;j++){
                input[j]='0';
            }
            input[head]='-';
            input='+'+input;
        }

        for(i=0;i<input.length();i++){
            if(input[i]=='1')
                input[i]='+';
        }

        cout<<input<<endl;
        
    }
}