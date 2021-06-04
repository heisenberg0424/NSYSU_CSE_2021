#include<iostream>
#include<string>
#define N 2000

using namespace std;

string rev(string s){
    string temp = s;
    for(int i=0;i<s.length();i++)
        temp[i] = s[s.length()-1-i];
    return temp;
}


int main(){
    string input,in;

    while(cin>>input){
        if(input=="1"){
            cout<<"Not an Automorphic number."<<endl;
            continue;
        }


        long ans[N+1];

        string in = input;

        for(int i=0;i<N+1;i++)
            ans[i]=0;

        in = rev(in);//reverse this string
        for(int i=0;i<in.length();i++)
            in[i]-='0';//adjust

        for(int i=0;i<in.length();i++)
            for(int j=0;j<in.length();j++){
                ans[i+j]+=in[i]*in[j];
            }
        for(int i=0;i<N+1;i++){
            ans[i+1]+=ans[i]/10;
            ans[i]%=10;
        }

        long temp[N+1];
        for(int i=0;i<N+1;i++)
            temp[i] = ans[N-i];
        int zero_counter = 0;

        for(int i=0;i<N+1;i++){
            if(temp[i]!=0)
                break;
            zero_counter++;
        }

        string real_ans;
        real_ans.resize(N+1-zero_counter);

        for(int i=0;i<N+1-zero_counter;i++)
            real_ans[i] = temp[i+zero_counter]+'0';

        //cout<<real_ans<<endl;


        bool is_Automorphic=true;

        for(int i=0;i<input.length();i++)
            if(input[i]!=real_ans[real_ans.length()-input.length()+i]){
                is_Automorphic = false;
                break;
            }

        if(is_Automorphic)
            cout<<"Automorphic number of "<<input.length()<<"-digit."<<endl;
        else
            cout<<"Not an Automorphic number."<<endl;


    }


    return 0;
}
