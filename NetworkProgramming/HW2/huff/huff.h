#include <fstream>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
#include <bitset>
using namespace std;

int huff_encode(string path);
int huff_decode(string path);
int filefreq(fstream& input,long int *table);
int encodeoutput(fstream &input,string outputpath,map<int,string> &codebook);

class Node{
    friend int buildtree(long int *table, priority_queue<Node*,vector<Node*>,Node> &tree);
    friend int hufftree( priority_queue<Node*,vector<Node*>,Node> &tree);
    friend int huffcode(Node* head, string code, map<int,string> &codebook);
    
    public:
        Node():frequency(0),data(0),code(""),left(NULL),right(NULL){}
        Node(int f,int d):frequency(f),data(d),code(""),left(NULL),right(NULL){}
        bool operator ()(const Node *a,const Node *b) const{
            return a->frequency > b->frequency;
        }
    private:
        int frequency;
        int data;
        string code;
        Node *left;
        Node *right;
};

