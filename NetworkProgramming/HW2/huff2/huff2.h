#include <fstream>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <bitset>
#include <cmath>
#include "../huff/huff.h"
using namespace std;

int huff2_encode(string path,int *compressedfilesize,int *codebooksize);
int huff2_decode(string path);
int huff2_output(fstream &input,string outputpath,map<int,string> &codebook);
int huff2_gencode(long int *table,map<int,string> &codebook);
string decimal2binary(int input,int len);
//int huff2_createmap(string path,map<string,int> &codebook);
//int huff2_decodedata(string path,map<string,int> &codebook,int len);