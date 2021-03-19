#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

typedef struct node{
    string pos;
    int val;
}Node;

bool istree;

bool cmp(Node a,Node b){
    if(a.pos.length() != b.pos.length())
        return a.pos.length()>b.pos.length();
    for(int i=0;i<a.pos.length();i++){
        if(a.pos[i]==b.pos[i])
            continue;
        return a.pos[i]>b.pos[i];
    }
    istree=0;
    return 0;
}



int main(){
    int i,j;
    bool eofflag;
    Node temp;
    vector<Node> tree;
    tree.resize(256);
    while(cin.get()){
        tree.clear();
        istree=1;
        while(1){
            if(cin.peek()==')'){
                cin.get();
                cin.get();
                
                break;
            }
            else{
                cin>>temp.val;
                cin.get();
                cin>>temp.pos;
                temp.pos.pop_back();
                cin.get();
                cin.get();
                tree.push_back(temp);
            }
        }
        
        sort(tree.begin(),tree.end(),cmp);
        int target=0;
        for (i=tree.size()-1;i>=0;i--){
            if(tree[i].pos.length()==target)
                target++;
            if(tree[i].pos.length()>target){
                istree=0;
                break;
            }
        }

        if(istree){
            for(i=tree.size()-1;i>=0;i--){
                cout<<tree[i].val;
                if(i!=0)
                    cout<<' ';
            }
            cout<<endl;
        }
        else{
            cout<<"not complete"<<endl;
        }
        
        // for (i=0;i<tree.size();i++){
        //     cout<<tree[i].val<<" "<<tree[i].pos<<endl;
        // }
    }
    return 0;
}