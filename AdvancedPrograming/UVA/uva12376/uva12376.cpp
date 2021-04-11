#include <iostream>
#include <cstring>
using namespace std;

int main(){
    int node,edge,i,j,edgea,edgeb,cnt=1;
    int fff;
    cin>>fff;
    while(fff--){
    cin>>node>>edge;
    int map[node][node];
    int points[node];
    memset(map,0,sizeof(map));
    for(i=0;i<node;i++){
        cin>>points[i];
    }
    //cout<<"here"<<endl;
    while(edge--){
        cin>>edgea>>edgeb;
        map[edgea][edgeb]=1;
    }
    //cout<<"here"<<endl;
    int currpos=0,currpoint=0,maxpoint=0,maxpos=0;
    while(1){
        bool anyroad=0;
        for(i=0;i<node;i++){
            if(map[currpos][i]){
                anyroad=1;
                if(points[i]>maxpoint){
                    maxpoint=points[i];
                    maxpos=i;
                }
            }
        }
        if(!anyroad)
            break;
        
        currpos=maxpos;
        currpoint+=maxpoint;
        maxpoint=maxpos=0;
    }
    cout<<"Case "<<cnt++<<": "<<currpoint<<" "<<currpos<<endl;
    
    }
}