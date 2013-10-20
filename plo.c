#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {ident, number, lparen, rparen, times, slash, plus,
    minus, eql, neq, lss, leq, gtr, geq, callsym, beginsym, semicolon,
    endsym, ifsym, whilesym, becomes, thensym, dosym, constsym, comma,
    varsym, procsym, period, oddsym, colon, bang, ob, cb, charsym,
    quotesym } Symbol;

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
  { "char",  charsym }
};

enum { INT, ARRAY, CHAR };

struct sym_t {
  char name[16];
  int  level;
  int  constant;
  int  zsp;
  int  type;
  int  isarray;
  int  isfunc;
  int  size;
  int  isparam;
} table[100];
int table_count = 0;

Symbol sym;
void expression(void);
void docall( struct sym_t *p);
void function_header( char * name );
char ch= ' '; 

int  cmax =256;
char id[256], temp[256];
int  num, lid;
int  imax = 1000000;
int  lineno;
int  level;
char litpool[8000];
int  litptr;

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
  int k,j;
  printf("cc1:\n");
 
  k=0;
  while( k<litptr ) {
    printf("\t.byte ");
    j=10;
    while( j > 0 ) {
      printf("%d", litpool[k] );
      if( j==0 || k >= litptr ) {
        printf("\n");
        break;
      }
      printf(",");
      k++;
      j--;
    }
  }
 
  printf("\t.byte 37,100,10,0\n");
  while( table_count-- > 0 ) {
    if( !table[table_count].constant && !table[table_count].isfunc ) printf("\t.comm %s,%d,%d\n", table[ table_count ].name, table[table_count].size, table[table_count].type == CHAR ? 1 : 4  );
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
       temp[k++] = ch;
       ch = getchar();
     } while( (isalpha(ch) || isdigit(ch)) && k < cmax-1 );
     temp[k] = 0;
 
     for( k=0; k<sizeof(rw)/sizeof(struct _rw); k++ ) {
       if( strcmp( rw[k].name, temp ) == 0 ) return rw[k].s;
     }

     strcpy(id, temp );
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
      if( ch == ')' ) { ch = getchar(); return rparen;  }
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
      if( ch == '[' ) { ch = getchar(); return ob; }
      if( ch == ']' ) { ch = getchar(); return cb; }
      if( ch == '"' ) { ch = getchar(); return quotesym; }
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

// Get address of symbol and aset into eax
void getref( struct sym_t *p ) {
  if( p->level > 0 ) {
    // Locals are stack offsets
    printf("\tleal %d(%%ebp), %%eax\n", p->zsp ); 
    printf("\tmovl (%%eax), %%eax\n") ;
  } else {
    // Globals are just symbols
    if( p->isarray == ARRAY )  {
      // Global array - copy address of array
      printf("\tmovl $%s, %%eax\n", p->name ); 
      printf("\tpushl %%eax\n");
      expect(ob);
      expression();
      if( p->type == INT ) printf("\tsall $2, %%eax\n"); /* Mul by 4 */
      printf("\tpopl %%edx\n");
      printf("\taddl %%edx, %%eax\n");
      // Move value at calculated offset into eax
      printf("\tmovl (%%eax), %%eax\n");
      expect(cb);
    } else {
      // Global non pointer - just copy value
      printf("\tmovl %s, %%eax\n", p->name ); 
    }
  }
}

void factor(void) {
    struct sym_t *p;
    if (accept(ident)) {
      p = look(id);
      if( sym == lparen ) { /* Call */
          docall(p); 
          return;
      } else { 
        getref(p);
      }
    } else if (accept(number)) {
      printf("\tmovl $%d, %%eax\n", num );
    } else if (accept(lparen)) {
      expression();
      expect(rparen);
    } else if ( sym == quotesym ) {
       printf("\tmovl $cc1+%d, %%eax\n", litptr );
       while( ch != '"' ) {
         litpool[ litptr ] = ch;
         litptr++;
         ch = getchar();
       }
       litpool[litptr] = 0;
       accept(quotesym);
       accept(quotesym);
       accept(semicolon);
    } else {
        //error2("factor: syntax error2");
        //getsym();
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
    Symbol tsym;
    if (accept(oddsym)) {
        expression();
    } else {
        expression();
        if (sym == eql || sym == neq || sym == lss || sym == leq || sym == gtr || sym == geq) {
            tsym = sym;
            printf("\tpushl %%eax\n");
            getsym();
            expression();
            printf("\tpopl %%edx\n");
            printf("\tcmpl %%eax, %%edx\n");
            if( tsym == eql ) printf("\tsete %%al\n");
            else if( tsym == neq ) printf("\tsetne %%al\n");
            else if( tsym == lss ) printf("\tsetl  %%al\n");
            else if( tsym == leq ) printf("\tsetle %%al\n");
            else if( tsym == gtr ) printf("\tsetg  %%al\n");
            else if( tsym == geq ) printf("\tsetge %%al\n");
            printf("\tmovzbl %%al, %%eax\n");

        } else {
            error2("condition: invalid operator");
            getsym();
        }
    }
}

void docall( struct sym_t *p) {
  accept(lparen);
  do {
    expression();
    accept(comma);
    printf("\tpushl %%eax\n");
  } while( !accept(rparen ) );
  printf("\tcall %s\n", p->name );
}

void statement(void) {
    struct sym_t *p;
    Symbol tsym;
    if (accept(ident)) {
        p = look(id);

        if( sym == lparen ) { /* Call */
          docall(p);
          return;
        }

        if( p->isarray == ARRAY && sym == ob ) {
          expect(ob);
          expression();
          printf("\tpushl %%eax\n"); // Array index
          expect(cb);
        }

        expect(becomes);
        expression();

        if( p->level == 0 ) {
          if( p->isarray == ARRAY ) {
            printf("\tpopl %%edx\n"); // Array index
            printf("\tpushl %%eax\n"); // save rval

            // Calculate array offset
            printf("\tmovl $%s, %%eax\n", p->name ); 
            if( p->type == INT ) printf("\tsall $2, %%edx\n"); 
            printf("\taddl %%edx, %%eax\n");
            printf("\tpopl %%edx\n"); // rval
            printf("\tmovl %%edx, (%%eax)\n"); // Move rval into mem location
          } else {
            // No array - just intrinsic
            printf("\tmovl %%eax, %s\n",p->name);
          }
        } else {
          // Locals
          if( p->type == INT ) {
            // Load address of local into edx
            printf("\tleal %d(%%ebp), %%edx\n", p->zsp ); 
            printf("\tmovl %%eax, (%%edx)\n"); 
          } 
        }
    } else if (accept(bang)){ 
        accept(ident); /* Printf basically */
        p = look(id);
        printf("\tmovl %s, %%eax\n", p->name );
        printf("\tpushl %%eax\n");
        printf("\tmovl $cc1+%d, %%eax\n", litptr == 0 ? litptr : litptr + 1);
        printf("\tpushl %%eax\n");
        printf("\tcall printf\n");
        printf("\taddl $8, %%esp\n"); 
    } else if (accept(callsym)) {
        expect(ident); /* Call a function */
        printf("\tcall %s\n", id );
    } else if (accept(beginsym)) {
        if( level == 0 ) { /* Start of main def */
          function_header("main");
        }
        do {
            if( sym == endsym ) break;
            statement();
            accept(semicolon);
        } while( 1 ) ; //accept(semicolon)); // accept(semicolon) );
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
        int lfalse = lid++;
        int ltrue  = lid++;
        printf("L%d:\n", ltrue );
        condition();
        printf("\ttestl %%eax, %%eax\n");
        printf("\tje L%d\n", lfalse );
        expect(dosym);
        statement();
        printf("\tjmp L%d\n", ltrue );
        printf("L%d:\n", lfalse );
    } else {
        printf("statement: syntax error2: %d:%s\n", lineno,id);
        getsym();
    }
}
 
void block(void) {
    int zsp = 0;
    struct sym_t *p;
    int type = INT; 
    if (accept(constsym)) {
        do {
            expect(ident);
            p = look(id); 
            p->constant = 1;
            printf("\t.globl %s\n", id );
            printf("\t.align 4\n");
            printf("\t.type %s,@object\n", id );
            printf("\t.size %s, 4\n", id );
            expect(eql);
            expect(number);
            printf("%s:\n", p->name );
            printf("\t.long %d\n", num );
        } while (accept(comma));
        expect(semicolon);
    }
    if (accept(varsym)) {
        do {
            type = INT;
            if( sym == charsym ) {
              accept(charsym);
              type = CHAR;
            }
            expect(ident);
            p = look(id);
            if( p->level > 0 ) { /* Local */
              zsp = zsp - 4;
              printf("\tpushl %%edx\n");
            }
            p->zsp = zsp;
            p->type = type;
            if( type == CHAR ){  p->size = 4; } else { p->size = 1; }
            if( sym == ob ) {
              expect(ob);
              expect(number);
              p->size = num * (type == CHAR ? 1 : 4 ) ;
              expect(cb);
              p->isarray = ARRAY;
            }
        } while (accept(comma));
        expect(semicolon);
    }
    while (accept(procsym)) {
        level++; /* Start of a procedure def */
        expect(ident);
        p = look(id);
        p->isfunc = 1;
        function_header(id);
        expect(lparen);
        if( sym != rparen ) {
            /* Looks like they are specifying arguments */
            zsp = 4;
            do {
              type = INT;
              if( sym == charsym ) { 
                accept(charsym);
                type = CHAR;
              } 
              expect(ident);
              p = look(id);
              p->isparam = 1;
              zsp = zsp + 4;
              p->zsp = zsp;
              p->type = type;
              if( type == CHAR ){  p->size = 4; } else { p->size = 1; }
              if( sym == ob ) {
                expect(ob);
                //expect(number);
                p->size = num * (type == CHAR ? 1 : 4 ) ;
                expect(cb);
                p->isarray = ARRAY;
              }
            } while( accept(comma));
        }
        expect(rparen);
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
    printf("\tmovl %%ebp, %%esp\n");
    printf("\tpopl %%ebp\n");
    printf("\tret\n");
    expect(period);
    dump();
}

void function_header( char * name ) {
  printf(".globl %s\n", name );
  printf("\t.TYPE %s,@function\n", name );
  printf("%s:\n", name );
  printf("\tpushl %%ebp\n");
  printf("\tmovl %%esp, %%ebp\n");
}

int main() {

  printf(".text\n");
  printf(".align 16\n");

  program();
  return 0;
}
