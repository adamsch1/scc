#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {ident, number, lparen, rparen, times, slash, plus,
    minus, eql, neq, lss, leq, gtr, geq, callsym, beginsym, semicolon,
    endsym, ifsym, whilesym, becomes, thensym, dosym, constsym, comma,
    varsym, procsym, period, oddsym, colon} Symbol;

struct _rw {
  char *name;
  Symbol s;
} rw[] = {
  { "call",  callsym },
  { "begin", beginsym },
  { "end",   endsym },
  { "if",    ifsym },
  { "while", whilesym },
  { "then",  thensym },
  { "do",    dosym },
  { "const", constsym },
  { "var",   varsym },
  { "procedure",   procsym },
  { "odd",   oddsym },
};

Symbol sym;
void expression(void);
char ch= ' '; 

int  cmax =256;
char id[256];
int  num;
int  imax = 1000000;
int  lineno;
int  level;

void error2( const char *msg ) {
  printf("%s\n",msg);
  exit(1);
}

void getsym() { 
  sym = lex();
}

char gch() {
  char tch = ch = getchar();
  if( ch == '=' ) ch = getchar();
  return tch;
}

int lex() {
   int digit, endstring, i, j, k;
   char tch;

   k = 0;
   while( 1 ) {
     if( ch == '\n' ) ch=getchar();
     else if( isspace(ch)) ch=getchar();
     else break;
   }

   if( isalpha(ch) ) {
     do {
       id[k++] = ch;
       ch = getchar();
     } while( (isalpha(ch) || isdigit(ch)) && k < cmax-1 );
     id[k] = 0;
 
     for( k=0; k<sizeof(rw)/sizeof(struct _rw); k++ ) {
       if( strcmp( rw[k].name, id ) == 0 ) return rw[k].s;
     }
     return ident;
   } else if (isdigit(ch)) {
      num = 0;
      num = ch -'0';
      while( (ch=getchar()) && isdigit(ch) ) {
        num = num*10 + ch-'0'; 
      }
      return number;
   } else if (ch == '(') {
      ch = getchar();
      if (ch != '*')
        sym = lparen;
      else  /* comment */
      {
        ch = getchar();
        do {
           while (ch != '*') {
              ch = getchar();
           }
           ch = getchar();
        } while (ch != ')');
        ch = getchar();
      }
      return sym;
    } else {
      if( ch == ':' ) {
        if( gch() == '=' ) return becomes;
        return colon;
      } else if( ch == '<' ) {
        if( gch() == '=' ) return leq;
        return lss;
      } else if( ch == '>' ) {
        if( gch() == '=' ) return geq;
        return gtr;
      } else {
      if( ch == ';' ) { ch = getchar(); return semicolon;  }
      if( ch == '*' ) { ch = getchar(); return times;  }
      if( ch == '.' ) { ch = getchar(); return period;  }
      if( ch == '+' ) { ch = getchar(); return plus; }
      if( ch == '/' ) { ch = getchar(); return slash; }
      if( ch == '-' ) { ch = getchar(); return minus; }
      if( ch == ',' ) { ch = getchar(); return comma; }
      if( ch == '=' ) { ch = getchar(); return eql; }
      if( ch == '#' ) { ch = getchar(); return neq; }
      if( ch == EOF ) return 0;
      printf("Unrecognzied: %c\n", ch );
      }
    }
}

int accept(Symbol s) {
    if (sym == s) {
        getsym();
        return 1;
    }
    return 0;
}
 
int expect(Symbol s) {
    if (accept(s))
        return 1;
    printf("expect: unexpected symbol: %d:%s\n", lineno,id);
    return 0;
}
 
void factor(void) {
    if (accept(ident)) {
      printf("\tmovl %s, %%eax\n", id );
    } else if (accept(number)) {
      printf("\tmovl $%d, %%eax\n", num );
        ;
    } else if (accept(lparen)) {
        expression();
        expect(rparen);
    } else {
        error2("factor: syntax error2");
        getsym();
    }
}
 
void term(void) {
    factor();
    while (sym == times || sym == slash) {
        printf("\tpushl %%eax\n");
        getsym();
        factor();
        printf("\tpopl %%edx\n");
        printf("\timull %%edx\n");
    }
}
 
void expression(void) {
    if (sym == plus || sym == minus)
        getsym();
    term();
    while (sym == plus || sym == minus) {
        printf("\tpushl %%eax\n");
        getsym();
        term();
        printf("\tpopl %%edx\n");
        printf("\taddl %%edx, %%eax\n");
    }
}
 
void condition(void) {
    if (accept(oddsym)) {
        expression();
    } else {
        expression();
        if (sym == eql || sym == neq || sym == lss || sym == leq || sym == gtr || sym == geq) {
            getsym();
            expression();
        } else {
            error2("condition: invalid operator");
            getsym();
        }
    }
}
 
void statement(void) {
    char temp[256];
    if (accept(ident)) {
        strcpy( temp, id );
        expect(becomes);
        expression();
        printf("\tmovl %%eax, %s\n", temp );
    } else if (accept(callsym)) {
        expect(ident);
    } else if (accept(beginsym)) {
        if( level == 0 ) {
          printf(".globl main\n", id );
          printf("\t.TYPE main,@function\n", id );
          printf("main:\n");
          printf("\tpushl %%ebp\n");
        }
        do {
            if( sym == endsym ) break;
            statement();
        } while( accept(semicolon) );
        expect(endsym);
        if( sym == semicolon ) {
          // close 
          printf("\tmovl %%ebp, %%esp\n");
          printf("\tpopl %%ebp\n");
          printf("\tret\n");
        }
    } else if (accept(ifsym)) {
        condition();
        expect(thensym);
        statement();
    } else if (accept(whilesym)) {
        condition();
        expect(dosym);
        statement();
    } else {
        printf("statement: syntax error2: %d:%s\n", lineno,id);
        getsym();
    }
}
 
void block(void) {
    if (accept(constsym)) {
        do {
            expect(ident);
            expect(eql);
            expect(number);
        } while (accept(comma));
        expect(semicolon);
    }
    if (accept(varsym)) {
        do {
            expect(ident);
        } while (accept(comma));
        expect(semicolon);
    }
    while (accept(procsym)) {
        level++;
        expect(ident);
        printf(".globl %s\n", id );
        printf("\t.TYPE %s,@function\n", id );
        printf("%s:\n", id );
        printf("\tpushl %%ebp\n");
        printf("\tmovl %%esp, %%ebp\n");
        expect(semicolon);
        block();
        expect(semicolon);
        level--;
    }
    statement();
}
 
void program(void) {
    getsym();
    block();
    printf("\tpopl %%ebp\n");
    printf("\tret\n");
    expect(period);
}

int main() {

  printf(".text\n");
  printf(".align 16\n");

  program();
  return 0;
}
