#include "huff.h"
#define DEBUG 1
using namespace std; 

int huff_encode(string path){
    fstream input,output;
    int freqcnt[256]={0};
    map<int,string> codebook;
    string outputpath=path+"-coded";
    priority_queue<Node*,vector<Node*>,Node> tree;
    
    input.open(path,ios::in|ios::binary);
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

    huffcode(tree.top(),"",codebook);
    if(DEBUG){
        cout<<"HUFF CODE DONE"<<endl;
    }
    
    input.open(path,ios::in|ios::binary);
    output.open(outputpath,ios::out|ios::trunc|ios::binary);
    encodeoutput(input,output,codebook);
    if(DEBUG){
        cout<<"OUTPUT FILE DONE"<<endl;
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
    input>>noskipws;
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
int buildtree(int *table, priority_queue<Node*,vector<Node*>,Node> &tree){
    for(int i=0;i<256;i++){
        if(table[i]){
            Node *temp= new Node(table[i],i);
            tree.push(temp);
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
int hufftree( priority_queue<Node*,vector<Node*>,Node> &tree){
    int parentnodefreq;
    Node *left,*right;
    
    while(tree.size()!=1){
        left = tree.top();
        tree.pop();
        right = tree.top();
        tree.pop();
        parentnodefreq=(left->frequency)+(right->frequency);
        Node *parent= new Node(parentnodefreq,-1);
        parent->left=left;
        parent->right=right;
        tree.push(parent);
    }

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
int huffcode(Node* head, string code,map<int,string> &codebook){
    if(head->data!=-1){
        head->code=code;
        codebook[head->data]=code;
        if(DEBUG){
            cout<<(char)head->data<<": "<<head->code<<endl;
        }
        return 0;
    }
    if(head->left){
        huffcode(head->left,code+'0',codebook);
    }
    if(head->right){
        huffcode(head->right,code+'1',codebook);
    }
    return 0;
}

/**------------------------------------------------------------
 * encodeoutput: output encoded data to file
 * [arguments]
 *  input : input file being encoded
 *  output : output file for encoded data
 *  codebook : huffman code we made for each char byte
---------------------------------------------------------------
*/
int encodeoutput(fstream &input,fstream &output,map<int,string> &codebook){
    char temp;
    unsigned char bitoperate;
    int extrabits,eightbitchar;
    string binarystring="";
    while(input>>temp){
        binarystring+=codebook[(int)temp];
    }
    
    extrabits=binarystring.length()%8;
    for(int i=0;i<extrabits;i++){
        binarystring+='0';
    }
    for(int i=0;i<binarystring.length();i+=8){
        eightbitchar=0;
        for(int j=0;j<8;j++){
            bitoperate = binarystring[i+j]-'0';
            eightbitchar+=bitoperate<<7-j;
        }
        cout<<eightbitchar<<endl;
    }

    input.close();
    output.close();
    return extrabits;
}