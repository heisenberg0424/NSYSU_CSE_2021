#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>
using namespace std;

int main(){
    set<long long> table;
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
            long long temp = i*i;
            while(temp<1000000000000 && temp >0){
                //cout<<"i: "<<i<<" temp: "<<temp<<endl;
                table.insert(temp);
                temp*=i;
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
            cout<<"table: "<<a<<endl;
            if(flag){
                if(a>high){
                    cout<<"End "<<a<<endl;
                    break;
                }
                ans++;
                continue;
            }
            if(a>=low){
                cout<<"found "<<a<<endl;
                flag=1;
                ans++;
            }
            
        }
        cout<<ans<<endl<<tt<<endl;
    }
}
    
