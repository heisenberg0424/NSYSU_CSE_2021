#include "huff.h"
#define DEBUG 1
using namespace std;

int huff_encode(string path){
    fstream input;
    int freqcnt[256]={0};
    deque<Node> tree;
    
    input.open(path,ios::in);
    if(!input){
        cout<<"File path doesn't exist\n";
        return -1;
    }

    filefreq(input,freqcnt);
    input.close();

    buildtree(freqcnt,tree);
    if(DEBUG){
        cout<<"BUILD TREE DONE"<<endl;
    }
    hufftree(tree);
    if(DEBUG){
        cout<<"HUFF TREE DONE"<<endl;
    }
    huffcode( &tree.front(),"");
    if(DEBUG){
        cout<<"HUFF CODE DONE"<<endl;
    }
    return 0;
}

/*------------------------------------------------------
 * filefreq : count the frequency of chars in file
 * [Arguments]
 *  input : input file
 *  table : frequency table stored in array
 *------------------------------------------------------
 */
int filefreq(fstream &input,int *table){
    char temp;
    bool flag=0;
    
    while(input>>temp){
        table[(int)temp]+=1;
        flag=1;
    }
    
    if(!flag){
        cout<<"Empty file!"<<endl;
        return -1;
    }

    if(DEBUG){
        cout<<"freq table:"<<endl;
        for(int i=0;i<256;i++){
            if(!table[i])
                continue;
            cout<<(char)i<<": "<<table[i]<<endl;
        }
    }

    return 0;
}

/*------------------------------------------------------
 * buildtree : insert nodes from freqtable to a deque
 * [Arguments]
 *  tree : a deque storing 
 *  table : frequency table stored in array
 *------------------------------------------------------
 */
int buildtree(int *table,deque<Node> &tree){
    for(int i=0;i<256;i++){
        if(table[i]){
            Node *temp = new Node(table[i],i);
            tree.push_back(*temp);
        }
    }
    
    sort(tree.begin(),tree.end(),comp);
    
    if(DEBUG){
        cout<<endl<<"build tree:"<<endl;
        for(int i=0;i<tree.size();i++){
            cout<<(char)tree[i].data<<": "<<tree[i].frequency<<endl;
        }
    }

    return 0;
}

/*------------------------------------------------------
 * hufftree : build the huffman-code tree 
 * [Arguments]
 *  tree: queue with unbounded nodes inside
 * [Algorithms]
 *  pop out 2 nodes (least freqency), combine them
 *  to a new node and then push back.
 *  sort again to ensure we pop nodes out with least
 *  freq. Repeat until there is 1 node left in the queue.
 * -----------------------------------------------------
 */
int hufftree(deque<Node> &tree){
    int parentnodefreq;
    Node *left,*right;
    
    while(tree.size()!=1){
    //int i=5;
    //while(i--){
        left = &tree.front();
        tree.pop_front();
        right = &tree.front();
        tree.pop_front();
        parentnodefreq=(left->frequency)+(right->frequency);
        Node *parent =new Node(parentnodefreq,-1);
        parent->left=left;
        parent->right=right;
        tree.push_back(*parent);
        sort(tree.begin(),tree.end(),comp);
    }
    //cout<<"Debug: "<<tree[0].right->right->left->left->frequency<<endl;
    //exit(1);
    return 0;

}

/**-----------------------------------------------------
 * huffcode : fill in the node's huffman-code 
 * [Arguments]
 *  head: the head of the huffman tree
 *  code: the code string that will be filled in the node
 * [Algorithm]
 *  The function will perform a BFS using recursive.
 *  left node will add '0' in code string, and the right
 *  node will add '0'
 *  the function will stop if the node is a real node,
 *  not the one we combine ( data=-1 )
 */
int huffcode(Node *head, string code){
    if(head->data!=-1){
        head->code=code;
        
        if(DEBUG){
            cout<<(char)head->data<<": "<<head->code<<endl;
        }
        return 0;
    }
    if(head->left){
        huffcode(head->left,code+'0');
    }
    if(head->right){
        huffcode(head->right,code+'1');
    }
    return 0;
}

bool comp(Node a,Node b){
    return a.frequency<b.frequency;
}