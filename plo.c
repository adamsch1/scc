#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {ident, number, lparen, rparen, times, slash, plus,
    minus, eql, neq, lss, leq, gtr, geq, callsym, beginsym, semicolon,
    endsym, ifsym, whilesym, becomes, thensym, dosym, constsym, comma,
    varsym, procsym, period, oddsym, colon, bang} Symbol;

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

struct sym_t {
  char name[16];
  int  level;
  int  zsp;
} table[100];
int table_count = 0;

Symbol sym;
void expression(void);
char ch= ' '; 

int  cmax =256;
char id[256];
int  num, lid;
int  imax = 1000000;
int  lineno;
int  level;

void error2( const char *msg ) {
  printf("%s\n",msg);
  exit(1);
}

void cleansym() {
  if( table_count > 0 ) {
    while( table_count > 0 && table[ table_count-1 ].level > level-1 ) {
      table_count--;
    }
  }
}

void dump()  {
  printf("cc1:\n");
  printf("\t.byte 37,100,10,0\n");
  while( table_count-- > 0 ) {
    printf("\t.comm %s,4,4\n", table[ table_count ].name );
  }
}

struct sym_t * look( const char *name ) {
  int k;

  for( k=0; k<table_count; k++) {
    if( strcmp( table[k].name, name ) == 0 ) {
      return &table[k];
    }
  }

  if( k < 100 ) {
    strcpy( table[k].name, name );
    table[k].level = level;
    table_count++;
  }

  return &table[k];
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
      if( ch == '!' ) { ch = getchar(); return bang; }
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
    struct sym_t *sym;
    if (accept(ident)) {
      sym = look(id);
      if( sym->level > 0 ) { /* Locals - stack offset reference  */
        printf("\tleal %d(%%ebp), %%eax\n", sym->zsp ); 
        printf("\tmovl (%%eax), %%eax\n") ;
      } else { /* Globals - just a symbol */
        printf("\tmovl %s, %%eax\n", id );
      }
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
    Symbol tsym; 
    factor();
    while (sym == times || sym == slash) {
        tsym = sym;
        printf("\tpushl %%eax\n");
        getsym();
        factor();
        printf("\tpopl %%edx\n");
        if( tsym == times ) {
          printf("\timull %%edx\n");
        } else {
          printf("\txchgl %%eax, %%edx\n");
          printf("\tmovl %%edx, %%ecx\n");
          printf("\tcltd\n");
          printf("\tidivl %%ecx\n");
        }
    }
}
 
void expression(void) {
    Symbol tsym;
    if (sym == plus || sym == minus)
        getsym();
    term();
    while (sym == plus || sym == minus) {
        tsym = sym;
        printf("\tpushl %%eax\n");
        getsym();
        term();
        printf("\tpopl %%edx\n");
        if( tsym == plus ) {
          printf("\taddl %%edx, %%eax\n");
        } else {
          printf("\tsub %%eax, %%edx\n");
          printf("\tmovl %%edx, %%eax\n");
        }
    }
}
 
void condition(void) {
    if (accept(oddsym)) {
        expression();
    } else {
        expression();
        if (sym == eql || sym == neq || sym == lss || sym == leq || sym == gtr || sym == geq) {
            printf("\tpushl %%eax\n");
            getsym();
            expression();
            printf("\tpopl %%edx\n");
            printf("\tcmpl %%eax, %%edx\n");
            printf("\tsete %%al\n");
            printf("\tmovzbl %%al, %%eax\n");

        } else {
            error2("condition: invalid operator");
            getsym();
        }
    }
}
 
void statement(void) {
    struct sym_t *p;

    if (accept(ident)) {
        p = look(id);
        if( p->level > 0 ) {
          printf("\tleal  %d(\%%ebp), %%eax\n", p->zsp ); 
          printf("\tpushl %%eax\n");
        }
        expect(becomes);
        expression();
        if( p->level == 0 ) {
          printf("\tmovl %%eax, %s\n", p->name );
        } else if( p->level == level ) { // Local
          printf("\tpopl %%edx\n");
          printf("\tmovl %%eax, (%%edx)\n");
        }
    } else if (accept(bang)){ 
        accept(ident); /* Printf basically */
        p = look(id);
        printf("\tmovl %s, %%eax\n", p->name );
        printf("\tpushl %%eax\n");
        printf("\tmovl $cc1+0, %%eax\n");
        printf("\tpushl %%eax\n");
        printf("\tcall printf\n");
        printf("\taddl $8, %%esp\n"); 
    } else if (accept(callsym)) {
        expect(ident); /* Call a function */
        printf("\tcall %s\n", id );
    } else if (accept(beginsym)) {
        if( level == 0 ) { /* Start of main def */
          printf(".globl main\n", id );
          printf("\t.TYPE main,@function\n", id );
          printf("main:\n");
          printf("\tpushl %%ebp\n");
          printf("\tmovl %%esp, %%ebp\n");
        }
        do {
            if( sym == endsym ) break;
            statement();
        } while( accept(semicolon) );
        expect(endsym);
        if( sym == semicolon ) {
          /* Ret from main */
          printf("\tmovl %%ebp, %%esp\n");
          printf("\tpopl %%ebp\n");
          printf("\tret\n");
        }
    } else if (accept(ifsym)) {
        int lfalse;
        lfalse = lid++; 
        condition();
        printf("\ttestl %%eax, %%eax\n");
        printf("\tje L%d\n", lfalse );
        expect(thensym);
        statement();
        printf("L%d:\n", lfalse );
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
    int zsp = 0;
    struct sym_t *p;
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
            p = look(id);
            if( level > 0 ) { /* Local */
              zsp = zsp - 4;
              printf("\tpushl %%edx\n");
            }
            p->zsp = zsp;
        } while (accept(comma));
        expect(semicolon);
    }
    while (accept(procsym)) {
        level++; /* Start of a procedure def */
        expect(ident);
        printf(".globl %s\n", id );
        printf("\t.TYPE %s,@function\n", id );
        printf("%s:\n", id );
        printf("\tpushl %%ebp\n");
        printf("\tmovl %%esp, %%ebp\n");
        expect(semicolon);
        block();
        expect(semicolon);
        cleansym(); /* Clean locals */
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
    dump();
}

int main() {

  printf(".text\n");
  printf(".align 16\n");

  program();
  return 0;
}
