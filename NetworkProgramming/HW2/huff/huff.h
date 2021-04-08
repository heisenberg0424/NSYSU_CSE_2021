#include <fstream>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <deque>
#include <algorithm>
using namespace std;

int huff_encode(string path);
int huff_decode(string path);
int filefreq(fstream& input,int *table);

class Node{
    friend bool comp(Node a,Node b); //for frequency compare
    friend int buildtree(int *table,deque<Node> &tree);
    friend int hufftree(deque<Node> &tree);
    friend int huffcode(Node *head, string code);
    
    public:
        Node(int f,int d):frequency(f),data(d),code(""),left(NULL),right(NULL){}
    private:
        int frequency;
        int data;
        string code;
        Node *left;
        Node *right;
};

bool comp(Node a,Node b){
    return a.frequency<b.frequency;
}
