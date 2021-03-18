#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
int w,h;
int ti,tj;
string input[50];
void bfs_dot(int i, int j){
    input[i][j]='*';
    //ti=i;
    //tj=j;
    
    if(j-1>=0){
        if (input[i][j-1]=='X')
            bfs_dot(i,j-1);
    }
    if(j+1<w){
        if (input[i][j+1]=='X')
            bfs_dot(i,j+1);
    }
    if(i-1>=0){
        if (input[i-1][j]=='X')
            bfs_dot(i-1,j);
    }
    if(i+1<h){
        if (input[i+1][j]=='X')
            bfs_dot(i+1,j);
    }
}

void bfs_dice(int i,int j,int *cnt){
    // for(int i=0;i<h;i++){
    //         cout<<input[i]<<endl;
    //     }
    //     cout<<endl<<endl;

    if(input[i][j]=='X'){
        bfs_dot(i,j);
        *cnt+=1;
        //i=ti;
        //j=tj;
        //cout<<i<<" "<<j<<endl;
    }
    else
        input[i][j]='.';
    if(j-1>=0){
        if (input[i][j-1]!='.')
            bfs_dice(i,j-1,cnt);
    }
    if(j+1<w){
        if (input[i][j+1]!='.')
            bfs_dice(i,j+1,cnt);
    }
    if(i-1>=0){
        if (input[i-1][j]!='.')
            bfs_dice(i-1,j,cnt);
    }
    if(i+1<h){
        if (input[i+1][j]!='.')
            bfs_dice(i+1,j,cnt);
    }
    
}

int main(){
    int c=1;
    vector <int> output;
    while(1){
        output.clear();
        cin>>w>>h;
        if(!w&&!h)
            break;
        for(int i=0;i<h;i++){
            cin>>input[i];
        }
        int cnt=0;
        for(int i=0;i<h;i++){
            for (int j=0;j<w;j++){
                if(input[i][j]!='.'){
                    bfs_dice(i,j,&cnt);
                    output.push_back(cnt);
                    cnt=0;
                }
            }
        }
        sort(output.begin(),output.end());
        cout<<"Throw "<<c++<<endl;
        for (int i=0;i<output.size()-1;i++){
            if(!output[i])
                continue;
            cout<<output[i]<<" ";
        }
        cout<<output[output.size()-1]<<endl<<endl;
    }
    //cout<<endl;





}