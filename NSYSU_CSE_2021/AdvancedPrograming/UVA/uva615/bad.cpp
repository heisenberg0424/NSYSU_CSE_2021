#include <iostream>
#include <utility>
#include <vector>
using namespace std;
int main(){
    typedef pair<int,int> Node;
    int tempint1,tempint2,i,j,timer=1,root;
    Node tempnode;
    vector<Node> Tree;
    bool endflag=0,findrootflag=0,treeflag=1;    
    while(1){
        root=-1;
        Tree.clear();
        treeflag=1;
        while(1){
            cin>>tempint1>>tempint2;
            if(!tempint1 && !tempint2)
                break;
            if(tempint1<0 && tempint2<0){
                endflag=1;
                break;
            }
            Tree.push_back(make_pair(tempint1,tempint2));
        }
        
        if(endflag)
            break;
        for(i=0;i<Tree.size();i++){
            findrootflag=1;

            for(j=0;j<Tree.size();j++){
                
                if(i==j)
                    continue;

                //cout<<"j="<<j<<endl;

                if(Tree[i].second == Tree[j].second){
                    treeflag=0;
                    cout<<"caught same "<<i<<" "<<j<<endl;
                    break;
                }
                if(Tree[i].first == Tree[j].second){
                    findrootflag=0;
                    break;
                }
            }

            if(!treeflag)
                break;

            if(findrootflag){
                if(root==-1){
                    root=Tree[i].first;
                }
                else if(root!=Tree[i].first){
                    //cout<<"2 root"<<endl;
                    treeflag=0;
                    break;
                }
            }
        }
        if(!findrootflag){
            treeflag=0;
            //cout<<"no root"<<endl;
        }

        

        if(treeflag){
            cout<<"Case "<<timer++<<" is a tree."<<endl;
        }
        else{
            cout<<"Case "<<timer++<<" is not a tree."<<endl;
        }


        
    }
}