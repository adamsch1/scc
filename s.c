/*******************************************************************
   Program :  Recursive Descent Compiler for PL/0
   Module  :  PL0SCAN - Scanner: lexical analysis
   File    :  pl0scan.c
   Compiler:  Borland C 3.1 - 4.5, GNU C 2.7.1
   Author  :  H. Weber
   Revision:  Aug. 1998
********************************************************************/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "pl0const.h"
#include "pl0glob.h"

FILE *source, *listing, *token;

int  eofflag = 0;
int  sym, num;
int  lineno = 1;
char id[cmax];
char str[strmax];
char ch = ' ';      /* to begin */


void error()
{
   fprintf(stderr, "Premature end of file\n");
   exit(1);
}

void initsym()
/* initialization of the ssym array */
{
   int i;

   for (i=0; i<256; i++)
      ssym[i] = 0;
   ssym['+'] = plus;
   ssym['-'] = minus;
   ssym['*'] = times;
   ssym['/'] = slash;
   ssym['('] = lparen;
   ssym[')'] = rparen;
   ssym['.'] = period;
   ssym['#'] = neq;
   ssym['='] = eql;
   ssym[','] = comma;
   ssym['<'] = lss;
   ssym['>'] = gtr;
   ssym[';'] = semicolon;
   ssym[':'] = colon;
   return;
}

int readchar()
/* read a character from source input and
   write it on listing file */
{
   char c;

   c = fgetc(source);
   if (c == EOF) eofflag = 1;
   fputc(c, listing);
   if (c == '\n') {
      fprintf(listing, "%3d  ", ++lineno);
      fprintf(token, "%2d\n", lf);
   }

   return c;
}

void findresw()
/* find reserved words */
{
   int i, j, k;

   i = 1;
   j = norw;                   /* check for reserved word */
   do {
      k = (i+j)/2;
      if (strcmp(id, word[k]) <= 0) j = k-1;
      if (strcmp(id, word[k]) >= 0) i = k+1;
   } while (i <= j);
   if (i-1 > j)
      sym = wsym[k];          /* symbol is reserved word */
   else
      sym = ident;            /* symbol is an identifier */
}

int getsym()
/*------------------------------------------------------------------
    The function getsym provides the next symbol from the source
    program. The result is output in the following variables:

    sym:     contains the type or internal representation of the
             next symbol
    num:     contains the value of a number
    id :     contains the character string defining an identifier
             or reserved word
    str:     contains a character string belonging to a write-
             statement
-------------------------------------------------------------------*/
{
   int digit, endstring, i, j, k;

 go:

   if (eofflag) return 0;
   /* pass over leading blanks, tabs, etc. */
   while (isspace(ch)) ch = readchar();

   /* next symbol is identifier or reserved word */
   if (isalpha(ch) && islower(ch))
   {
      strcpy(id, "");
      k = 0;
      do {
         if (k < cmax-1)
            id[k++] = ch;
         ch = readchar();
      } while (isalpha(ch) || isdigit(ch));
      id[k] = 0;
      findresw();
   }

   /* next symbol is a number */
   else if (isdigit(ch))
   {
      num = 0;
      sym = number;
      do {
        digit = ch - '0';
        if ((num < imax/10) ||
            ((num == imax/10) && (digit <= imax % 10)))
           num = num*10 + digit;
        else {
	   num = 0;
	   fprintf(stderr, "Wrong number in input file\n");
        }
        ch = readchar();
      } while (isdigit(ch));
   }

   /* next symbol is  ':'  or  ':='  */
   else if (ch == ':')
   {
      ch = readchar();
      if (ch == '=') {
         sym = becomes;
         ch = readchar();
      }
      else
         sym = colon;
   }

   /* next symbol is '<' or '<=' */
   else if (ch == '<')      /* next symbol is '<' or '<=' */
   {
      ch = readchar();
      if (ch == '=') {
         sym = leq;
         ch = readchar();
      }
      else
         sym = lss;
   }

   /* next symbol is '>' or '>=' */
   else if (ch == '>')
   {
      ch = readchar();
      if (ch == '=') {
         sym = geq;
         ch = readchar();
      }
      else
         sym = gtr;
   }

   /* next symbol is string */
   else if (ch == '\'')
   {
      num = 0;
      ch = readchar();
      sym = stringsym;
      endstring = 0;
      do {
         if (ch == '\'') {
            ch = readchar();
            endstring = (ch != '\'');
         }
         if (!endstring) {
            str[num++] = ch;
            ch = readchar();
            if (eofflag) error();
         }
      } while (!endstring);
      if (!endstring) num = 0;
      str[num++] = 0;
   }

   /* next symbol is '(' or possibly comment */
   else if (ch == '(')
   {
      ch = readchar();
      if (ch != '*')
        sym = lparen;
      else  /* comment */
      {
        ch = readchar();
        do {
           while (ch != '*') {
              if (eofflag) error();
              ch = readchar();
           }
           ch = readchar();
        } while (ch != ')');
        ch = readchar();
        if (eofflag) error();
        goto go;
      }
    }

    else if (ch == EOF)
       return 0;

    /* special symbol */
    else
    {
       sym = ssym[ch];
       if (sym == 0) {
          fprintf(stderr,
                 "Wrong character: '%c' found in source.\n", ch);
          ch = readchar();
          goto go;
       }
       else
          ch = readchar();
    }

    return 1;
}

main(int argc, char *argv[])
/*------------------------------------------------------------------
   The main function of the scanner reads the source file and
   writes the symbols of the source program on an intermediate
   file.
------------------------------------------------------------------*/
{
   char sname[20], lname[20], tname[20];
   char *p;

   printf("PL/0-Compiler: Scanner\n");

   /* check arguments */
   if (argc == 1) {
      fprintf(stderr, "Usage: %s source\n", argv[0]);
      exit(1);
   }

   /* set up filenames */
   strcpy(sname, argv[1]);
   p = strchr(sname, '.');
   if (p) *p = 0;
   strcpy(lname, sname);
   strcpy(tname, sname);
   strcat(sname, ".pl0");
   strcat(lname, ".lst");
   strcat(tname, ".tok");

   /* open files */
   if ((source = fopen(sname, "r")) == NULL) {
      fprintf(stderr, "Cannot open source file %s\n", sname);
      exit(1);
   }
   if ((listing = fopen(lname, "w")) == NULL) {
      fprintf(stderr, "Cannot open listing file %s\n", lname);
      exit(1);
   }
   if ((token = fopen(tname, "w")) == NULL) {
      fprintf(stderr, "Cannot open token file %s\n", tname);
      exit(1);
   }
   fprintf(listing, "Source file: %s\n\n", sname);
   fprintf(listing, "  1  ");

   /* initialize symbols */
   initsym();

   /* main loop */
   while (getsym()) {
      fprintf(token, "%2d  ", sym);
      if (sym == ident)
         fprintf(token, "%s\n", id );
      else if (sym == number)
         fprintf(token, "%d\n", num);
      else if (sym == stringsym)
         fprintf(token, "%s\n", str);
      else
         fprintf(token, "\n");
   }

   /* close files */
   fclose(source);
   fclose(listing);
   fclose(token);
   return;
}

