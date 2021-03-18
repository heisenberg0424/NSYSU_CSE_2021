#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
/*
AACATGAAGG 
TTTTGGCCAA
TTTGGCCAAA
GATCAGATTT
CCCGGGGGGA
ATCGATGCAT
*/
struct input{
    string s;
    int sortness=0;
    void cal(int length){
        for (int i=0;i<length;i++){
            for (int j=i+1;j<length;j++){
                if( (int)s[i] > (int)s[j] ){
                    sortness++;
                }
            }
        }
    }
};
bool inputcmp(struct input a,struct input b){
    if(a.sortness<b.sortness)
        return 1;
    return a.sortness<b.sortness;
}
struct input a[100];
int main(){
    int dataset,length,num;
    string temp;
    cin>>dataset;
    while(dataset--){
        cin>>length>>num;
        for (int i=0;i<num;i++){
            cin>>temp;
            a[i].s=temp;
            a[i].cal(length);
        }
        stable_sort(a,a+num,inputcmp);
        for (int i=0;i<num;i++){
            cout<<a[i].s<<endl;
        }
        if(dataset)
            cout<<endl;
        memset(a,0,sizeof(a));
    }
    
    
}
