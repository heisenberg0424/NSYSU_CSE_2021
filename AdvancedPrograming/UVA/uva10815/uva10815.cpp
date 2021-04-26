#include <iostream>
#include <set>
#include <algorithm>
#include <string>
using namespace std;
int main(){
    string input;
    set<string> dict;
    while(cin>>input){
        string temp = "";
        for(int i=0;i<input.size();i++){
            if(isupper(input[i])){
                temp += tolower(input[i]);
            }
            else if(islower(input[i])){
                temp += input[i];
            }
            else {
                dict.insert(temp);
                temp = "";
            }
        }
        if(temp != ""){
            dict.insert(temp);
        }
        
    }
    for(const auto &s : dict){
        if(s == ""){
            continue;
        }
        cout<<s<<endl;
    }

    return 0;
}