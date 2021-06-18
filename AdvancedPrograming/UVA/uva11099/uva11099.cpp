#include <iostream>
#include <algorithm>
#include <set>
#include <cstring>
#include <vector>
using namespace std;
bool prime[1001];
int main(){
    memset(prime, 0, sizeof(prime));
    for(int i=2;i<1001;i++){
        if(prime[i]==0){
            for(int j=i+i;j<1001;j+=i){
                prime[j]=1;
            }
        }
    }
    int input;
    while(cin>>input){
        if(input == 1){
            cout<<"Not Exist!"<<endl;
            continue;
        }

        int original = input;
        vector<int> nums;
        set<long long> myset;
        long long start=1;
        for(int i=2;i<1001;i++){
            if(prime[i]==0 && input%i==0){
                start*=i;
                nums.push_back(i);
                while(input%i==0){
                    input/=i;
                }
                //cout<<"Caught "<<i<<endl;
            }
            if(input==1){
                break;
            }
        }
        if(input!=1){
            nums.push_back(input);
            start*=input;
        }
        if(start==1){
            start=input;
        }

        for(long long i=0;i<nums.size();i++){
            if(nums[i]*start<2000000){    
                myset.insert(nums[i]*start);
                //cout<<"insert "<<nums[i]*start<<endl;
            }
        }
        
        long long first = *myset.begin();

        if(first<=0){
            cout<<"Not Exist!"<<endl;
            continue;
        }
        //cout<<first<<endl;
        while(first<=original){

            for(int i=0;i<nums.size();i++){
                if(nums[i]*first<2000000){
                    myset.insert(nums[i]*first);
                    //cout<<"insert "<<nums[i]*first<<endl;
                }
            }
            if(myset.size()<2){
                break;
            }
            myset.erase(first);
            //cout<<"erase "<<first<<endl;
            first = *myset.begin();
            //cout<<"current first"<<first<<endl;
        }
        if(first>2000000 || first == original){
            cout<<"Not Exist!"<<endl;
        }else {
            cout<<first<<endl;
        }

    }

    return 0;
}