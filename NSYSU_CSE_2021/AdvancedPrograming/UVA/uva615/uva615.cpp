#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <cstring>
#include <algorithm>
using namespace std;
int copymat[100][100];
int matrix[100][100];
void clear(int row){
    for(int i=0;i<100;i++){
        copymat[row][i]=0;
    }
}

void bfs(int root){
    clear(root);
    for(int i=0;i<100;i++){
        if(matrix[root][i])
            bfs(i);
    }
}

int main(){
    int x,y,i,j,cnt=1;
    bool findx,findy,treeflag;

    
    while(1){
        vector <pair<int,int>> input;
        vector <int> already;
        treeflag=1;
        while(1){
            cin>>x>>y;

            if(x<0&&y<0)
                return 0;

            if(!x&&!y)
                break;
            input.push_back(make_pair(x,y));
            
            if(x==y)
                treeflag=0;
            
            findx=findy=0;
            for (i=0;i<already.size();i++){
                if(already[i]==x)
                    findx=1;
                if(already[i]==y)
                    findy=1;
            }

            if(!findx)
                already.push_back(x);
            if(!findy)
                already.push_back(y);
        }
        //int matrix[already.size()+1][already.size()];
        memset(matrix,0,sizeof(matrix));
        memset(copymat,0,sizeof(copymat));
        map<int,int> arrmap;

        for(i=0;i<already.size();i++){
            arrmap[already[i]]=i;
        }

        for(i=0;i<input.size();i++){
            if(matrix[arrmap[input[i].first]][arrmap[input[i].second]]){
                treeflag=0;
                //cout<<"Same"<<endl;
            }
            matrix[arrmap[input[i].first]][arrmap[input[i].second]]=1;
        }

        for(j=0;j<already.size();j++){
            for(i=0;i<already.size();i++){
                matrix[already.size()][j]+=matrix[i][j];
            }
        }

        int s[already.size()];
        for(i=0;i<already.size();i++){
            s[i]=matrix[already.size()][i];
        }
        sort(s,s+already.size());
        if(s[0]!=0)
            treeflag=0;
        if(s[1]==0)
            treeflag=0;
        if(s[already.size()-1]>1)
            treeflag=0;

        if(treeflag){

            for(i=0;i<already.size();i++){
                for(j=0;j<already.size();j++){
                    copymat[i][j]=matrix[i][j];
                }
            }

            int fndroot;
            int cnt=1;
            for(i=0;i<already.size();i++){
                if(!matrix[already.size()][i]){
                    fndroot=i;
                    break;
                }
            }
            bfs(fndroot);

            for(i=0;i<already.size();i++){
                for(j=0;j<already.size();j++){
                    if(copymat[i][j])
                        treeflag=0;
                        break;
                }
            }

            
        }

        // for(i=0;i<already.size()+1;i++){
        //     for (j=0;j<already.size();j++){
        //         cout<<matrix[i][j]<<" ";
        //     }
        //     cout<<endl;
        // }

        // for(i=0;i<already.size();i++){
        //     cout<<already[i]<<endl;
        // }

        // for(i=0;i<input.size();i++){
        //     cout<<input[i].first<<" "<<input[i].second<<endl;
        // }
        if(input.empty())
            treeflag=1;
        
        if(treeflag)
            cout<<"Case "<<cnt++<<" is a tree."<<endl;
        else
            cout<<"Case "<<cnt++<<" is not a tree."<<endl;
    }
}