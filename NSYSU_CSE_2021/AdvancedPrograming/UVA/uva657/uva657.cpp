#include <iostream>
#include <algorithm>
using namespace std;
int w,h;
string input[50];
void bfs_dot(int i, int j){
    input[i][j]='.';
    if(i-1>=0){
        if (input[i-1][j]=='X')
            bfs_dot(i-1,j);
    }
    if(i+1<h){
        if (input[i+1][j]=='X')
            bfs_dot(i+1,j);
    }
    if(j-1>=0){
        if (input[i][j-1]=='X')
            bfs_dot(i,j-1);
    }
    if(j+1<w){
        if (input[i][j+1]=='X')
            bfs_dot(i,j+1);
    }
}

void bfs_dice(int i,int j,int *cnt){
    if(input[i][j]=='X'){
        bfs_dot(i,j);
        *cnt++;
    }
    else
        input[i][j]='.';

    if(i-1>=0){
        if (input[i-1][j]!='.')
            bfs_dice(i-1,j,cnt);
    }
    if(i+1<h){
        if (input[i+1][j]!='.')
            bfs_dice(i+1,j,cnt);
    }
    if(j-1>=0){
        if (input[i][j-1]!='.')
            bfs_dice(i,j-1,cnt);
    }
    if(j+1<w){
        if (input[i][j+1]!='.')
            bfs_dice(i,j+1,cnt);
    }
}

int main(){
    cin>>w>>h;
    for(int i=0;i<h;i++){
        cin>>input[i];
    }
    int cnt=0;
    bfs_dice(1,1,&cnt);

    cout<<endl;
    for(int i;i<h;i++){
        cout<<input[i]<<endl;
    }
    cout<<cnt<<endl;



}