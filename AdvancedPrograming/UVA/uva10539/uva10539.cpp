#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>
#include <cmath>
using namespace std;

int main(){
    set<long long> table;
    long long temp;
    vector<bool> prime;
    prime.resize(1000002);
    fill(prime.begin(), prime.end(), 0);
    for(int i=2; i<prime.size();i++){
        if(!prime[i]){
            for(int j=i+i; j<prime.size(); j+=i){
                prime[j]=1;
            }
        }
    }
    for(int i=2; i<prime.size(); i++){
        if(prime[i]==0){
            int powcnt=2;
            temp = pow(i,powcnt++);
            while(temp<1000000000000 && temp >0){
                table.insert(temp);
                temp = pow(i,powcnt++);
            }
        }
    }

    //main
    int cases;
    long long low,high;
    cin>>cases;
    while(cases--){
        int ans=0;
        bool flag=0;
        long long tt;
        cin>>low>>high;
        for(const auto a:table){
            //cout<<"table: "<<a<<endl;
            if(flag){
                if(a>high){
                    //cout<<"End "<<a<<endl;
                    break;
                }
                ans++;
                continue;
            }
            if(a>=low && a<high){
                //cout<<"found "<<a<<endl;
                flag=1;
                ans++;
            }
            
        }
        cout<<ans<<endl;
    }
}
    
