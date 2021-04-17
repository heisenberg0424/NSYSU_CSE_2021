%{
#include <stdio.h>
unsigned char_count = 1, id_count = 0, line_count = 1;
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

/*  Spaces */
new_line \n


%%
{operators} {
    printf("Line: %d, 1st char: %d, \"%s\" is an \"operator\" .\n",line_count,char_count,yytext);
    char_count += yyleng;
}
{new_line} {
    line_count++;
    char_count=1;
}
{reserved_word} {
    char_count++;
}
\. char_count++;

%%
int main(){
    yylex();
    return 0;
}