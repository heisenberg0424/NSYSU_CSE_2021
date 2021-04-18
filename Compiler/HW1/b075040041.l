%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned char_count = 1, id_count = 0, line_count = 1;
int commentnewline(char *input,int len);
int tableptr=0;
struct sym{
    char *data;
};
struct sym table[100];
void create();
int lookup(char *input);
int insert(char *input,int len);
void dump();

%}
/*  Reserved word */
reserved_word boolean|break|byte|case|char|catch|class|const|continue|default|do|double|else|extends|false|final|finally|float|for|if|implements|int|long|main|new|print|private|protected|public|return|short|static|string|switch|this|true|try|void|while

/*  Operators */
plus \+
plus_plus \+\+
minus \-
minus_minus \-\-
multiply \*
divide \/
mod \%
assign \=
calculate {plus}|{plus_plus}|{minus}|{minus_minus}|{multiply}|{divide}|{mod}|{assign}
less \<
less_equal \<\=
greater \>
greater_equal \>\=
equal \=\=
not_equal \!\=
compare {less}|{less_equal}|{greater}|{greater_equal}|{equal}|{not_equal}
and \&\&
or \|\|
not \!
logic {and}|{or}|{not}
operators {calculate}|{compare}|{logic}

/* Symbols */
comma \,
colon \:
semicolon \;
parentheses [\(\)]
square_brackets [\[\]]
brackets [\{\}]
symbols {comma}|{colon}|{semicolon}|{parentheses}|{square_brackets}|{brackets}

/* IDs */
identifier [a-zA-Z_$][a-zA-Z0-9_$]*
badid [0-9\^\#][a-zA-Z_$]+
/* Integers */
integer \-?[0-9]*

/* Floats */
scientific \-?[0-9]+(\.[0-9]+)?[eE][+-][0-9]+
point \-?[0-9]+\.[0-9]+
float {point}|{scientific}

/* String */
string \"([^\\\'\"]*(\\[\\\'\"])*)*\"

/* Comments */
cpp \/\/.*\n
c \/\*(.|[\r\n])*?\*\/
comments {cpp}|{c}

/*  Spaces */
new_line \n
space [ \t]

%%
{operators} {
    printf("Line: %d, 1st char: %d, \"%s\" is an \"operator\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}

{reserved_word} {
    printf("Line: %d, 1st char: %d, \"%s\" is a \"reserved word\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}

{symbols} {
    printf("Line: %d, 1st char: %d, \"%s\" is a \"symbol\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}

{identifier} {
    printf("Line: %d, 1st char: %d, \"%s\" is an \"identifier\" .\n",line_count,char_count,yytext);
    if(lookup(yytext)==-1){
        insert(yytext,yyleng);
    }
    char_count += yyleng;
}

{badid} {
    printf("\"%s\" is a invalid ID.\n",yytext);
    char_count += yyleng;
}

{integer} {
    printf("Line: %d, 1st char: %d, \"%s\" is an \"integer\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}

{float} {
    printf("Line: %d, 1st char: %d, \"%s\" is a \"float\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}

{string} {
    printf("Line: %d, 1st char: %d, %s is a \"string\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}

{comments} {
    printf("Line: %d, 1st char: %d, \"%s\" is a \"comment\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
    line_count += commentnewline(yytext,yyleng);
}

{new_line} {
    line_count++;
    char_count=1;
}

{space} {
    char_count++;
}

%%
int commentnewline(char *input,int len){
    int i,lines=0;
    for(i=0;i<len;i++){
        if(input[i]=='\n')
            lines++;
    }
    return lines;
}

void create(){
    tableptr=0;
}
int lookup(char *input){
    int i,ret=-1;
    for(i=0;i<tableptr;i++){
        if(!strcmp(table[i].data,input)){
            ret=i;
        }
    }
    return ret;
}
int insert(char *input,int len){
    char *temp = (char*)malloc((len+1)*sizeof(char));
    strcpy(temp,input);
    table[tableptr].data=temp;
    tableptr++;
    return tableptr-1;
}
void dump(){
    int i;
    printf("The symbol table contains:\n");
    for(i=0;i<tableptr;i++){
        printf("%s\n",table[i].data);
    }
}

int main(){
    yylex();
    dump();
    return 0;
}