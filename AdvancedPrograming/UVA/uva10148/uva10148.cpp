#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstring>
using namespace std;

int check(int a,int b,int target,bool arr[]){
    int cnt=0;
    for(int i=a;i<=b;i++){
        if(arr[i]){
            cnt++;
            //cout<<"sfdsfd:"<<i<<endl;
        }
    }
    //cout<<a<<" "<<b<<" "<<target<<" "<<cnt<<endl;
    return target-cnt;
}

int main(){
    int cases;
    int flags;
    int joggers;
    int a,b;
    cin>>cases;
    while(cases--){
        int offset=0;
        int min=10000,max=-10000,dist=0;
        vector<pair<int,int>> data;
        cin>>flags;
        cin>>joggers;
        while(joggers--){
            cin>>a;
            cin>>b;
            
            if(a>b){
                swap(a,b);
            }
            
            if(a<min){
                min=a;
            }
            if(b>max){
                max = b;
            }

            if(a<offset){
                offset=a;
            }
            data.push_back(make_pair(a,b));
        }
        sort(data.begin(),data.end(),[](pair<int,int> c,pair<int,int> d){
            return c.second<d.second;
        });
        dist = max - min;
        offset = min;
        //cout <<dist<<endl;
        for(int i=0;i<data.size();i++){
            data[i].first-=offset;
            data[i].second-=offset;
        }

        bool ads[dist+1];
        memset(ads,0,sizeof(ads));
        int result;
        int s=0;
        for(int i=0;i<data.size();i++){

            //cout<<data[i].first+offset<<" "<<data[i].second+offset<<endl;
            result = check(data[i].first,data[i].second,flags,ads);
            for(int j=data[i].second;j>=data[i].first;j--){
                if(result<=0){
                    break;
                }
                if(ads[j]!=1){
                    //cout<<"insert: "<<j+offset<<endl;
                    ads[j]=1;
                    result--;
                    s++;
                }
            }
        }
        cout<<s<<endl;
        for(int i=0;i<dist+1;i++){
            if(ads[i]){
                cout<<i+offset<<endl;
            }
        }
        if(cases!=0){
            cout<<endl;
        }
        


    }
}
