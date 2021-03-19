#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef struct node {
    string pos;
    int val;
} Node;

bool istree;

bool cmp(Node a, Node b) {
    if (a.pos.length() != b.pos.length())
        return a.pos.length() > b.pos.length();
    for (int i = 0; i < a.pos.length(); i++) {
        if (a.pos[i] == b.pos[i]) continue;
        return a.pos[i] > b.pos[i];
    }
    istree = 0;
    return 0;
}

int main() {
    int i, j;
    string in;
    Node temp;
    bool endflag = 0;
    vector<Node> tree;
    tree.clear();
    istree = 1;
    while (cin >> in) {
        if (in.length() == 2) {
            endflag = 1;
        } else {
            string tempint, temppos;
            bool valflag = 1, posflag = 0;
            for (i = 1; i < in.length()-1; i++) {
                if (in[i] == ',') {
                    valflag = 0;
                    posflag = 1;
                    continue;
                }
                if (valflag) tempint += in[i];
                if (posflag) temppos += in[i];
            }
            temp.val = stoi(tempint);
            temp.pos = temppos;
            tree.push_back(temp);
        }

        if (endflag) {

            sort(tree.begin(), tree.end(), cmp);
            int target = 0;
            for (i = tree.size() - 1; i >= 0; i--) {
                if (tree[i].pos.length() == target) target++;
                if (tree[i].pos.length() > target) {
                    istree = 0;
                    break;
                }
            }

            if (istree) {
                for (i = tree.size() - 1; i >= 0; i--) {
                    cout << tree[i].val;
                    if (i != 0) cout << ' ';
                }
                cout << endl;
            } else {
                cout << "not complete" << endl;
            }
            tree.clear();
            endflag = 0;
            istree = 1;
        }
    }

    // for (i=0;i<tree.size();i++){
    //     cout<<tree[i].val<<" "<<tree[i].pos<<endl;
    // }
    return 0;
}