#include <iostream>
#include <string>
using namespace std;
int main(){
    int time,i,cnt=1,j;
    string input;
    cin>>time;
    while(time--){
        int who=0;
        int sum=0;
        bool flag=1;
        cin>>input;
        //cout << "=" << input << "=" << endl;
        for(i=0;i<input.length();i++){
            sum+=input[i]-'0';
        }
        //cout << "sum=" << sum << endl;
        if(input.length()==1){
            cout<<"Case "<<cnt++<<": "<<"S"<<endl;
            continue;
        }
        else if(sum%3!=0){
            bool threeflag=0;
            for(i=0;i<input.length();i++){
                string temp=input;
                sum=0;
                temp.erase(temp.begin()+i);
                //cout << "temp= " << temp << endl;
                for(j=0;j<temp.length();j++){
                     sum+=temp[j]-'0';
                }
                if(sum%3==0){
                    //cout << "sum= " << sum << endl;
                    //cout << "hhhh" << endl;
                    threeflag=1;
                    input=temp;
                    who++;
                    break;
                }
            }
            if(!threeflag){
                //cout << "HIHI" << endl;
                cout<<"Case "<<cnt++<<": "<<"T"<<endl;
                continue;
            }
           
        }
        while(flag){
            int templen=input.length();
            for(i=0;i<input.length();i++){
                if(input[i]=='3'||input[i]=='6'||input[i]=='9'){
                    input.erase(input.begin()+i);
                    //cout << "input= " << input << endl;
                    who++;
                    break;
                }
            }
            //cout << i << " " << input << endl;
            if(input.length()==1 || i==input.length())
                break;
        }
        if(input.length()==1)
            who++;
        //cout << "who= " << who << endl;
        if(who%2)
            cout<<"Case "<<cnt++<<": "<<"S"<<endl;
        else
            cout<<"Case "<<cnt++<<": "<<"T"<<endl;
    }
}
