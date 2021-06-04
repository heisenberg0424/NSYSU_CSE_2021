#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

int main(){
    string input;
    int origin[2000],calculate[4005],tmp[4005];
    int ans=0,cnt=0,ttt;
    while(cin>>input){
        if(input=="0"||input=="1"){
            cout<<"Not an Automorphic number."<<endl;
            continue;
        }  
        ans=0;
        cnt=0;
        ttt=0;
        memset(origin,0,sizeof(origin));
        memset(calculate,0,sizeof(calculate));
        memset(tmp,0,sizeof(tmp));
        for(int i=input.size()-1;i>=0;i--){
            origin[cnt++] = input[i]-'0';
            //calculate[i] = origin[i];
        }

        for(int i=0;i<input.size();i++){
            ttt+=pow(10,i)*origin[i];
        }

        if(ttt==1||ttt==0){
            cout<<"Not an Automorphic number."<<endl;
            continue;
        }

        
        cnt=0;
        for(int i=0;i<input.length();i++){
            memset(tmp,0,sizeof(tmp));

            for(int j=0;j<input.length();j++){
                tmp[cnt+j]=origin[j]*origin[i];
            }

            for(int j=0;j<input.length();j++){
                calculate[j]+=tmp[j];
            }

            // for(int j=0;j<5;j++){
            //     cout<<tmp[j]<<" ";
            // }
            // cout<<endl;
            
            if(calculate[i]>9){
                calculate[i+1]+=calculate[i]/10;
                calculate[i]%=10;
            }
            
            

            //cout<<"i="<<i<<": "<<calculate[i]<<endl;
            //cout<<"i-1 ="<<i-1<<": "<<calculate[i-1]<<endl;
            
            if(calculate[i]!=origin[i]){
                ans=-1;
                break;
            }
            cnt++;
        }

        if(ans==-1){
            cout<<"Not an Automorphic number."<<endl;
        }
        else{
            cout<<"Automorphic number of "<<input.length()<<"-digit."<<endl;
        }

    }
    return 0;
}