#include "huff.h"
#define ENDEBUG 0
#define DEDEBUG 0
#define OFFSET 128 
using namespace std; 

//ENCODE  
//***DECODE func in lin 218
int huff_encode(string path,int *compressedfilesize,int *codebooksize){
    long long original,compress,book;
    fstream input;
    long freqcnt[256]={0};
    map<int,string> codebook;
    priority_queue<Node*,vector<Node*>,Node> tree;
    
    input.open(path.c_str(),ios::in|ios::binary);
    if(!input){
        cout<<"File path doesn't exist\n";
        return -1;
    }  

    filefreq(input,freqcnt);
    input.close();
    if(ENDEBUG){
        cout<<"COUNT FREQ DONE"<<endl;
    }

    buildtree(freqcnt,tree);
    if(ENDEBUG){
        cout<<"BUILD TREE DONE"<<endl;
    }
    hufftree(tree);
    if(ENDEBUG){
        cout<<"HUFF TREE DONE"<<endl;
    }

    huffcode(tree.top(),"",codebook);
    if(ENDEBUG){
        cout<<"HUFF CODE DONE"<<endl;
    }
    
    input.open(path,ios::in|ios::binary);
    encodeoutput(input,path,codebook);
    if(ENDEBUG){
        cout<<"OUTPUT FILE DONE"<<endl;
    }

    input.open(path.c_str(),ios::in|ios::binary);
    input.seekg(0,ios::end);
    
    original=input.tellg();
    input.close();
    input.open((path+"-coded").c_str(),ios::in|ios::binary);
    input.seekg(0,ios::end);
    compress=input.tellg();
    input.close();
    input.open((path+"-codebook").c_str(),ios::in|ios::binary);
    input.seekg(0,ios::end);
    *codebooksize=input.tellg();
    input.close();
    cout<<"Original file length: "<<original<<" bytes"<<endl;
    cout<<"Compressed file length: "<<compress<<" bytes"<<endl;
    cout<<"Ratio: "<<(float)compress*100/original<<'%'<<endl;
    *compressedfilesize=compress;

    return 0;
}

/*------------------------------------------------------
 * filefreq : count the frequency of chars in file
 * [Arguments]
 *  input : input file
 *  table : frequency table stored in array
 *------------------------------------------------------
 */
int filefreq(fstream &input,long int *table){
    char temp;
    bool flag=0;
    //input>>noskipws;
    while(input.get(temp)){
        table[temp+OFFSET]+=1;
        flag=1;
    }
    
    if(!flag){
        cout<<"Empty file!"<<endl;
        return -1;
    }

    if(ENDEBUG){
        cout<<"freq table:"<<endl;
        for(int i=0;i<256;i++){
            if(!table[i])
                continue;
            cout<<i<<": "<<table[i]<<endl;
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
int buildtree(long int *table, priority_queue<Node*,vector<Node*>,Node> &tree){
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
        if(ENDEBUG){
            cout<<head->data<<": "<<head->code<<endl;
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
 *  outputpath : output path for encoded data
 *  codebook : huffman code we made for each char byte
 * TODO:
 *  write the algo
---------------------------------------------------------------
*/
int encodeoutput(fstream &input,string outputpath,map<int,string> &codebook){
    char temp;
    unsigned char bitoperate;
    int extrabits,eightbitchar;
    string binarystring="",codebookpath,codedpath;
    input>>noskipws;
    while(input>>temp){
        binarystring+=codebook[temp+OFFSET];
    }
    

    fstream output;
    codebookpath=outputpath+"-codebook";
    codedpath = outputpath+"-coded";
    extrabits=binarystring.length()%8;

    output.open(codebookpath.c_str(),ios::out|ios::trunc|ios::binary);
    if(!output){
        cout<<"Write code book failed"<<endl;
    }
    output<<extrabits<<endl;
    for(int i=0;i<256;i++){
        if(codebook.find(i)!=codebook.end()){
            if(ENDEBUG)
                cout<<i<<": "<<codebook[i]<<endl;
            output<<i-OFFSET<<" "<<codebook[i]<<endl;
        }
    }
    output.close();

    output.open(codedpath.c_str(),ios::out|ios::trunc|ios::binary);
    if(!output){
        cout<<"Write coded file failed"<<endl;
    }
    
    for(int i=0;i<extrabits;i++){
        binarystring+='0';
    }
    for(int i=0;i<binarystring.length();i+=8){
        eightbitchar=0;
        for(int j=0;j<8;j++){
            bitoperate = binarystring[i+j]-'0';
            eightbitchar+=bitoperate<<7-j;
        }
        output<<(char)eightbitchar;
    }

    input.close();
    output.close();
    return extrabits;
}

int huff_decode(string path){
    int extrabit;
    map<string,int> codebook;
    extrabit = createmap(path,codebook);
    decodedata(path,codebook,extrabit);
    return 0;
}

/**------------------------------------------------------------
 * createmap: read the codebook from file and save it as map
 * will return extrabits from codedfile
 * [arguments]
 *  path: codebook file's file path
 *  codebook: map
----------------------------------------------------------------
 */
int createmap(string path,map<string,int> &codebook){
    fstream codebookfile;
    int extrabit;
    path+="-codebook";
    codebookfile.open(path.c_str(),ios::in|ios::binary);
    if(!codebookfile){
        cout<<"Fail to open codebook file"<<endl;
        return -1;
    }
    codebookfile>>extrabit;
    if(DEDEBUG){
        cout<<"offset: "<<extrabit<<endl;
    }
    int tempchar;
    string tempstring;
    while(codebookfile>>tempchar>>tempstring){
        codebook[tempstring]=tempchar;
        if(DEDEBUG){
            cout<<(char)tempchar<<": "<<tempstring<<endl;
        }
        
    }
    return extrabit;
}

int decodedata(string path,map<string,int> &codebook,int extrabit){
    fstream codedfile;
    char temp;
    string binarystring="";
    string codedpath=path+"-coded";
    codedfile.open(codedpath.c_str(),ios::in|ios::binary);
    if(!codedfile){
        cout<<"Fail to open coded file"<<endl;
        return -1;
    }
    while(codedfile.get(temp)){
        bitset<8> binarychar(temp);
        binarystring+=binarychar.to_string();
    }
    while(extrabit--){
        binarystring.pop_back();
    }

    
    // fstream debugfile(path+"binarystring",ios::out|ios::trunc);
    // debugfile<<binarystring;
    // debugfile.close();
    

    fstream decodefile;
    //path+="done";
    decodefile.open(path.c_str(),ios::out|ios::trunc);
    int i,j;
    for(i=0;i<binarystring.size();i+=j){
        for(j=1;j<binarystring.size()-i;j++){
            string compare2map = binarystring.substr(i,j);
            if(codebook.find(compare2map)!=codebook.end()){
                decodefile<<(char)codebook[compare2map];
                //cout<<compare2map;
                break;
            }
        }
    }
    return 0;

}
