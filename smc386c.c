
/************************************************/
/*            */
/*    small-c compiler    */
/*            */
/*      by Ron Cain      */
/*            */
/************************************************/

/* with minor mods by RDK */
/* Hacked for IA32/Linux by Evgueniy Vitchev - 
   provided 'for' and 'do' statements */
/*

This fella outpus GAS assembler suitable for GNU toolchain - nice!

http://www.physics.rutgers.edu/~vitchev/smallc-i386.html

The compiler can be bootstrapped by using gcc in the following way:

    Build a stage 1 compiler:
    gcc -o smc386c1 smc386c.c
    Using the stage 1 compiler build a stage 2 compiler:
    ./smc386c1
    Output filename? smc386c2.s
    Input filename? smc386c.c
    Input filename? <enter>
    There were 0 errors in compilation.

    gcc -o smc386c2 smc386c2.s
    In order to make sure everything went properly, go to stage 3:
    ./smc386c2
    Output filename? smc386c3.s
    Input filename? smc386c.c
    Input filename? <enter>

    diff smc386c2.s smc386c3.s
    If diff doesn't produce output, this means the bootstrap was successful, and you can use the stage 2 compiler smc386c2.

*/
#define BANNER  "<><><>   Small-C  V1.2  DOS--CP/M Cross Compiler   <><><>"

#define VERSION "<><><><><>   CP/M Large String Space Version   <><><><><>"

#define AUTHOR  "<><><><><><><><><><>   By Ron Cain   <><><><><><><><><><>"

#define HCK     "<><><> Hacked for IA32/Linux by Evgueniy Vitchev <><><><>"

#define LINE    "<><><><><><><><><><><><><><>X<><><><><><><><><><><><><><>"

#define IDNT    "Small C"


/*#asm
  DB  'SMALL-C COMPILER V.1.2 DOS--CP/M CROSS COMPILER',0
  #endasm*/


/*  Define system dependent parameters  */

/*  Stand-alone definitions      */

/* INCLUDE THE LIBRARY TO COMPILE THE COMPILER (RDK) */

/* #include smallc.lib */ /* small-c library included in source now */

/* IN DOS USE THE SMALL-C OBJ LIBRARY RATHER THAN IN-LINE ASSEMBLER */

#define NULL 0
#define eol 10 /* was 13 */

/*  UNIX definitions (if not stand-alone)  */

/* #include "stdio.h"  /* was <stdio.h> */

/* #define eol 10  */

/*  Define the symbol table parameters  */

#define  SYMSIZ  14
#define  SYMTBSZ  5040
#define NUMGLBS 300
#define  STARTGLB SYMTAB
#define  ENDGLB  STARTGLB+NUMGLBS*SYMSIZ
#define  STARTLOC ENDGLB+SYMSIZ
#define  ENDLOC  SYMTAB+SYMTBSZ-SYMSIZ

/*  Define symbol table entry format  */

#define  NAME  0
#define  IDENT  9
#define  TYPE  10
#define  STORAGE  11
#define  OFFSET  12

/*  System wide NAME size (for symbols)  */

#define  NAMESIZE 9
#define NAMEMAX  8

/*  Define possible entries for "IDENT"  */

#define  VARIABLE 1
#define  ARRAY  2
#define  POINTER  3
#define  FUNCTION 4

/*  Define possible entries for "TYPE"  */

#define  CCHAR  1
#define  CINT  2

/*  Define possible entries for "STORAGE"  */

#define  STATIK  1
#define  STKLOC  2

/*  Define the "while" statement queue  */

#define  WQTABSZ  300
#define  WQSIZ  4
#define  WQMAX  wq+WQTABSZ-WQSIZ

/*  Define entry OFFSETs in while queue  */

#define  WQSYM  0
#define  WQSP  1
#define  WQLOOP  2
#define  WQLAB  3

/*  Define the literal pool      */

#define  LITABSZ  8000
#define  LITMAX  LITABSZ-1

/*  Define the input line      */

#define  LINESIZE 80
#define  LINEMAX  LINESIZE-1
#define  MPMAX  LINEMAX

/*  Define the macro (define) pool    */

#define  MACQSIZE 3000
#define  MACMAX  MACQSIZE-1

/*  Define statement TYPEs (tokens)    */

#define  STIF  1
#define  STWHILE  2
#define  STRETURN 3
#define  STBREAK  4
#define  STCONT  5
#define  STASM  6
#define  STEXP  7
#define STFOR   9
#define STDO    10


/* Define how to carve up a NAME too long for the assembler */

#define ASMPREF  7
#define ASMSUFF  7

/*Added by E.V.*/
#define LITSTKSZ 5000
#define LITSTKNUM 10
int tolitstk;
char litstk[LITSTKSZ];
char litstk2[LITSTKSZ];
int litstklens[LITSTKNUM];
int litstkptrs[LITSTKNUM];/*0 is reserved for the file output!*/
putlitstk(c)
   char c;
{
  if(litstkptrs[tolitstk]+litstklens[tolitstk]>=LITSTKSZ-1)
  {error("too large code from FUNCTION arguments");return 0;}
  /*litstk[litstkptrs[tolitstk]+litstklens[tolitstk]++]=c;*/
  litstk[litstkptrs[tolitstk]+litstklens[tolitstk]]=c;
  litstklens[tolitstk]=litstklens[tolitstk]+1;
  return c;
}
getlitstk()
{
  if(tolitstk>=LITSTKNUM-1)
  {error("too many FUNCTION arguments");return 0;}
  tolitstk++;
  litstklens[tolitstk]=0;
  litstkptrs[tolitstk]=litstkptrs[tolitstk-1]+litstklens[tolitstk-1];
  return tolitstk;
}
dumpltstk(tl)
   int tl;
{
  int i,p;
  char*q;
  q=litstk2;
  while(tolitstk>=tl)
  {
    i=litstklens[tolitstk];
    p=litstkptrs[tolitstk];
    while(i--)
    {
      /*printf("litstk[p]=%c,%d\n",litstk[p],litstk[p]);*/
      *q++=litstk[p++];
      /*outbyte1(litstk[p++]);*/
    }
    tolitstk--;
  }
  /*printf("tolitstk=%d\n",tolitstk);*/
  p=q;
  q=litstk2;
  while(q<p)
  {
    /*printf("*q=%c,%d\n",*q,*q);*/
    outbyte(*q);
    q++;
  }
}
/*End- Added by E.V.*/

/*  Now reserve some STORAGE words    */

char  SYMTAB[SYMTBSZ];  /* symbol table */
char  *glbptr,*locptr;    /* ptrs to next entries */

int  wq[WQTABSZ];    /* while queue */
int  *wqptr;      /* ptr to next entry */

char  litq[LITABSZ];    /* literal pool */
int  litptr;      /* ptr to next entry */

char  macq[MACQSIZE];    /* macro string buffer */
int  macptr;      /* and its index */

char  line[LINESIZE];    /* parsing buffer */
char  mline[LINESIZE];  /* temp macro buffer */
int  lptr,mptr;    /* ptrs into each */

/*  Misc STORAGE  */

int  nxtlab,    /* next avail label # */
  litlab,    /* label # assigned to literal pool */
  Zsp,    /* compiler relative stk ptr */
  argstk,    /* FUNCTION arg sp */
  argtop,/*added by E.V.*/
  ncmp,    /* # open compound statements */
  errcnt,    /* # errors in compilation */
  errstop,  /* stop on error      gtf 7/17/80 */
  eof,    /* set non-zero on final input eof */
  input,    /* iob # for input file */
  output,    /* iob # for output file (if any) */
  input2,    /* iob # for "include" file */
  glbflag,  /* non-zero if internal globals */
  ctext,    /* non-zero to intermix c-source */
  cmode,    /* non-zero while parsing c-code */
      /* zero when passing assembly code */
  lastst,    /* last executed statement TYPE */
  mainflg,  /* output is to be first asm file  gtf 4/9/80 */
  saveout,  /* holds output ptr when diverted to console     */
      /*          gtf 7/16/80 */
  kandr,    /* Current function decl K&R style? */
  fnstart,  /* line# of start of current fn.  gtf 7/2/80 */
  lineno,    /* line# in current file    gtf 7/2/80 */
  infunc,    /* "inside FUNCTION" flag    gtf 7/2/80 */
  savestart,  /* copy of fnstart "  "    gtf 7/16/80 */
  saveline,  /* copy of lineno  "  "    gtf 7/16/80 */
  saveinfn;  /* copy of infunc  "  "    gtf 7/16/80 */

char   *currfn,    /* ptr to SYMTAB entry for current fn.  gtf 7/17/80 */
       *savecurr;  /* copy of currfn for #include    gtf 7/17/80 */
char  quote[2];  /* literal string for '"' */
char  *cptr;    /* work ptr to any char buffer */
int  *iptr;    /* work ptr to any int buffer */
/*  >>>>> start cc1 <<<<<<    */

/*          */
/*  Compiler begins execution here  */
/*          */
main(/*argc,argv*/)
   /*int argc;char*argv[];*/
  {
  glbptr=STARTGLB;  /* clear global symbols */
  locptr=STARTLOC;  /* clear local symbols */
  wqptr=wq;    /* clear while queue */
  tolitstk=
  litstkptrs[0]=
  litstklens[0]=
  macptr=    /* clear the macro pool */
  litptr=    /* clear literal pool */
    Zsp =    /* stack ptr (relative) */
  errcnt=    /* no errors */
  errstop=  /* keep going after an error    gtf 7/17/80 */
  eof=    /* not eof yet */
  input=    /* no input file */
  input2=    /* or include file */
  output=    /* no open units */
  saveout=  /* no diverted output */
  ncmp=    /* no open compound states */
  lastst=    /* no last statement yet */
  mainflg=  /* not first file to asm     gtf 4/9/80 */
  fnstart=  /* current "FUNCTION" started at line 0 gtf 7/2/80 */
  lineno=    /* no lines read from file    gtf 7/2/80 */
  infunc=    /* not in FUNCTION now      gtf 7/2/80 */
  quote[1]=
  0;    /*  ...all set to zero.... */
  quote[0]='"';    /* fake a quote literal */
  currfn=NULL;  /* no FUNCTION yet      gtf 7/2/80 */
  cmode=1;  /* enable preprocessing */
  /*        */
  /*  compiler body    */
  /*        */
  ask();      /* get user options */
  openout();    /* get an output file */
  openin();    /* and initial input file */
  header();    /* intro code */
  parse();     /* process ALL input */
  dumplits();    /* then dump literal pool */
  dumpglbs();    /* and all static memory */
  trailer();    /* follow-up code */
  closeout();    /* close the output (if any) */
  errorsummary();    /* summarize errors (on console!) */
  return;      /* then exit to system */
  }

/*          */
/*  Abort compilation    */
/*    gtf 7/17/80    */
zabort()
{
  if(input2)
    endinclude();
  if(input)
    fclose(input);
  closeout();
  toconsole();
  pl("Compilation aborted.");  nl();
  exit(0);
/* end zabort */}

/*          */
/*  Process all input text    */
/*          */
/* At this level, only static declarations, */
/*  defines, includes, and FUNCTION */
/*  definitions are legal...  */
parse()
  {
  while (eof==0)    /* do until no more input */
    {
    if(amatch("char",4)){declglb(CCHAR);ns();}
    else if(amatch("int",3)){declglb(CINT);ns();}
    else if(match("#asm"))doasm();
    else if(match("#include"))doinclude();
    else if(match("#define"))addmac();
    else newfunc();
    blanks();  /* force eof if pending */
    }
  }
/*          */
/*  Dump the literal pool    */
/*          */
dumplits()
  {int j,k;
  if (litptr==0) return;  /* if nothing there, exit...*/
  ot(".section");ot(".rodata");nl();
  printlabel(litlab);col();nl(); /* print literal label */
  k=0;      /* init an index... */
  while (k<litptr)  /*   to loop with */
    {defbyte();  /* pseudo-op to define byte */
    j=10;    /* max bytes per line */
    while(j--)
      {outdec((litq[k++]&127));
      if ((j==0) | (k>=litptr))
        {nl();    /* need <cr> */
        break;
        }
      outbyte(',');  /* separate bytes */
      }
    }
  }
/*          */
/*  Dump all static VARIABLEs  */
/*          */
dumpglbs()
  {
  int j;
  if(glbflag==0)return;  /* don't if user said no */
  cptr=STARTGLB;
  while(cptr<glbptr)
    {if(cptr[IDENT]!=FUNCTION)
      /* do if anything but FUNCTION */
      {/*col();*/
        /* output NAME as label... */
      defstorage();outname(cptr);  /* define STORAGE */
      comma();
      j=((cptr[OFFSET]&255)+
        ((cptr[OFFSET+1]&255)<<8));
          /* calc # bytes */
      if((cptr[TYPE]==CINT)|
        (cptr[IDENT]==POINTER))
        j=j*4;/*modified by E.V.*/
      outdec(j);  /* need that many */
      if(cptr[TYPE]==CINT|cptr[IDENT]==POINTER)
        outasm(",4");
      else
        outasm(",1");
      nl();
      }
    cptr=cptr+SYMSIZ;
    }
  }
/*          */
/*  Report errors for user    */
/*          */
errorsummary()
  {
  /* see if anything left hanging... */
  if (ncmp) error("missing closing bracket");
    /* open compound statement ... */
  nl();
  outstr("There were ");
  outdec(errcnt);  /* total # errors */
  outstr(" errors in compilation.");
  nl();
  }
/*          */
/*  Get options from user    */
/*          */
ask() {
  int k,num[1];
  kill();      /* clear input line */
  outbyte(12);    /* clear the screen */
  nl();nl();    /* print banner */
  pl(LINE);
  pl(BANNER);
  pl(AUTHOR);
  /*pl(VERSION);*/
  pl(HCK);
  pl(LINE);
  nl();nl();
  ctext=1;    /* assume yes */
    glbflag=1;  /* define globals */
  mainflg=1;  /* first file to assembler */
  nxtlab =0;  /* start numbers at lowest possible */
  errstop=0;
  litlab=getlabel();  /* first label=literal pool */ 
  kill();      /* erase line */
}
/*          */
/*  Get output filename    */
/*          */
openout()
  {
  kill();      /* erase line */
  output=0;    /* start with none */
  pl("Output filename? "); /* ask...*/
  gets(line);  /* get a filename */
  if(ch()==0)return;  /* none given... */
  if((output=fopen(line,"w"))==NULL) /* if given, open */
    {output=0;  /* can't open */
    error("Open failure!");
    }
  kill();      /* erase line */
}
/*          */
/*  Get (next) input file    */
/*          */
openin()
{
  input=0;    /* none to start with */
  while(input==0){  /* any above 1 allowed */
    kill();    /* clear line */
    if(eof)break;  /* if user said none */
    pl("Input filename? ");
    gets(line);  /* get a NAME */
    if(ch()==0)
      {eof=1;break;} /* none given... */
    if((input=fopen(line,"r"))!=NULL)
      newfile();      /* gtf 7/16/80 */
    else {  input=0;  /* can't open it */
      pl("Open failure");
      }
    }
  kill();    /* erase line */
  }

/*          */
/*  Reset line count, etc.    */
/*      gtf 7/16/80  */
newfile()
{
  lineno  = 0;  /* no lines read */
  fnstart = 0;  /* no fn. start yet. */
  currfn  = NULL;  /* because no fn. yet */
  infunc  = 0;  /* therefore not in fn. */
/* end newfile */}

/*          */
/*  Open an include file    */
/*          */
doinclude()
{
  blanks();  /* skip over to NAME */

  toconsole();          /* gtf 7/16/80 */
  outstr("#include "); outstr(line+lptr); nl();
  tofile();

  if(input2)          /* gtf 7/16/80 */
    error("Cannot nest include files");
  else if((input2=fopen(line+lptr,"r"))==NULL)
    {input2=0;
    error("Open failure on include file");
    }
  else {  saveline = lineno;
    savecurr = currfn;
    saveinfn = infunc;
    savestart= fnstart;
    newfile();
    }
  kill();    /* clear rest of line */
      /* so next read will come from */
      /* new file (if open */
}

/*          */
/*  Close an include file    */
/*      gtf 7/16/80  */
endinclude()
{
  toconsole();
  outstr("#end include"); nl();
  tofile();

  input2  = 0;
  lineno  = saveline;
  currfn  = savecurr;
  infunc  = saveinfn;
  fnstart = savestart;
/* end endinclude */}

/*          */
/*  Close the output file    */
/*          */
closeout()
{
  tofile();  /* if diverted, return to file */
  if(output)fclose(output); /* if open, close it */
  output=0;    /* mark as closed */
}
/*          */
/*  Declare a static VARIABLE  */
/*    (i.e. define for use)    */
/*          */
/* makes an entry in the symbol table so subsequent */
/*  references can call symbol by NAME  */
declglb(typ)    /* typ is CCHAR or CINT */
  int typ;
{  int k,j;char sname[NAMESIZE];
  while(1)
    {while(1)
      {if(endst())return;  /* do line */
      k=1;    /* assume 1 element */
      if(match("*"))  /* POINTER ? */
        j=POINTER;  /* yes */
        else j=VARIABLE; /* no */
       if (symname(sname)==0) /* NAME ok? */
        illname(); /* no... */
      if(findglb(sname)) /* already there? */
        multidef(sname);
      if (match("["))    /* ARRAY? */
        {k=needsub();  /* get size */
        if(k)j=ARRAY;  /* !0=ARRAY */
        else j=POINTER; /* 0=ptr */
        }
      addglb(sname,j,typ,k); /* add symbol */
      break;
      }
    if (match(",")==0) return; /* more? */
    }
  }
/*          */
/*  Declare local VARIABLEs    */
/*  (i.e. define for use)    */
/*          */
/* works just like "declglb" but modifies machine stack */
/*  and adds symbol table entry with appropriate */
/*  stack OFFSET to find it again      */
declloc(typ)    /* typ is CCHAR or CINT */
  int typ;
  {
  int k,j;char sname[NAMESIZE];
  while(1)
    {while(1)
      {if(endst())return;
      if(match("*"))
        j=POINTER;
        else j=VARIABLE;
      if (symname(sname)==0)
        illname();
      if(findloc(sname))
        multidef(sname);
      if (match("["))
        {k=needsub();
        if(k)
          {j=ARRAY;
          if(typ==CINT)k=4*k;/*modifyed by E.V.*/
          }
        else
          {j=POINTER;
          k=4;/*modified by E.V.*/
          }
        }
      else
        if((typ==CCHAR)
          &(j!=POINTER))
          k=1;else k=4;/*modified by E.V.*/
      if(k&3)k=k+4-(k&3);/*align, by E.V.*/
      /* change machine stack */
      Zsp=modstk(Zsp-k);
      addloc(sname,j,typ,Zsp);
      break;
      }
    if (match(",")==0) return;
    }
  }
/*  >>>>>> start of cc2 <<<<<<<<  */

/*          */
/*  Get required ARRAY size    */
/*          */
/* invoked when declared VARIABLE is followed by "[" */
/*  this routine makes subscript the absolute */
/*  size of the ARRAY. */
needsub()
  {
  int num[1];
  if(match("]"))return 0;  /* null size */
  if (number(num)==0)  /* go after a number */
    {error("must be constant");  /* it isn't */
    num[0]=1;    /* so force one */
    }
  if (num[0]<0)
    {error("negative size illegal");
    num[0]=(-num[0]);
    }
  needbrack("]");    /* force single dimension */
  return num[0];    /* and return size */
  }
/*          */
/*  Begin a FUNCTION    */
/*          */
/* Called from "parse" this routine tries to make a FUNCTION */
/*  out of what follows.  */
newfunc() {
  char n[NAMESIZE];  /* ptr => currfn,  gtf 7/16/80 */
  int  tidx;
  if (symname(n)==0) {
    error("illegal FUNCTION or declaration");
    kill();  /* invalidate line */
    return;
  }
  fnstart=lineno;    /* remember where fn began  gtf 7/2/80 */
  infunc=1;    /* note, in FUNCTION now.  gtf 7/16/80 */
  /* already in symbol table ? */
  if(currfn=findglb(n))  {
    if(currfn[IDENT]!=FUNCTION)multidef(n);
      /* already VARIABLE by that NAME */
    else if(currfn[OFFSET]==FUNCTION)multidef(n);
      /* already FUNCTION by that NAME */
    else currfn[OFFSET]=FUNCTION;
      /* otherwise we have what was earlier*/
      /*  assumed to be a FUNCTION */
  } else {
    /* if not in table, define as a FUNCTION now */
    currfn=addglb(n,FUNCTION,CINT,FUNCTION);
  }

  toconsole();          /* gtf 7/16/80 */
  /*outstr("====== "); outstr(currfn+NAME); outstr("()"); nl();*/
  tofile();

  /* we had better see open paren for args... */
  if(match("(")==0)error("missing open paren");
  ol(".text");
  ol(".align 16");
  outasm(".globl ");outname(n);nl();
  ot(".TYPE");tab();outname(n);outasm(",@function");nl();
  outname(n);col();nl();  /* print FUNCTION NAME */
  argstk=0;    /* init arg count */

  locptr=STARTLOC;  /* "clear" local symbol table*/
  Zsp=0;      /* preset stack ptr */

 
  /* Parse twice, once for arg count so second pass we can pass proper 
     stack offsets in emitted asm code - SA */ 
  tidx=lptr;
  blanks();
  
  /* Assume K&R style  - SA */
  kandr = 1;
  while( match(")" ) == 0 ) {
    /* Found a type? we now treat this function as a non K&R style */
    if( match("int",3) ) { kandr = 0; }
    else if( match("char", 4) ) { kandr = 0; }

    if( streq(line+lptr, ",") ) {
      /* Still our goal is to find the number of arguments  */
      argstk = argstk + 4;
    }
    lptr++;
    if( argstk == 0 ) argstk = argstk + 4;
  }
  /* Record stack depth based on # of parameters */
  argtop = argstk;

  /* If we are not K&R re-parse the params, we needed an arg count for this 
     parse code to work however so we are doing it twice */
  if( !kandr ) {
      /* Reset lptr so we can reparse - SA */
      lptr=tidx;  
      while(match(")")==0) {
        if( amatch("int",3) ) {
          getarg(CINT);
        } else if( amatch("char", 4) ) {
          getarg(CINT);
        } else if(streq(line+lptr,")")==0) {
          if(match(",")==0) { 
            error("expected comma");
            break;
          }
        }
      }
  } else {
      blanks();
      /* We are K&R - parse arg name and types after the ) and before the { */
      locptr=STARTLOC;  /* "clear" local symbol table*/ 
      Zsp=0;      /* preset stack ptr */
      argtop=argstk;
      while(argstk)  {
        /* now let user declare what TYPEs of things */
        /*  those arguments were */
        if(amatch("char",4)){getarg(CCHAR);ns();}
        else if(amatch("int",3)){getarg(CINT);ns();}
        else{error("wrong number args");break;}
      }
  }
  ol("pushl %ebp");
  ol("movl %esp, %ebp");
  if(statement()!=STRETURN) /* do a statement, but if */
    ;
        /* it's a return, skip */
        /* cleaning up the stack */
  {/*modstk(0);*/
    ol("movl %ebp, %esp");
    ol("popl %ebp");
    zret();
    }
  Zsp=0;      /* reset stack ptr again */
  locptr=STARTLOC;  /* deallocate all locals */
  infunc=0;    /* not in fn. any more    gtf 7/2/80 */
  }
/*          */
/*  Declare argument TYPEs    */
/*          */
/* called from "newfunc" this routine adds an entry in the */
/*  local symbol table for each NAMEd argument */
getarg(t)    /* t = CCHAR or CINT */
  int t;
  {
  char n[NAMESIZE],c;int j;
  while(1)
    {if(argstk==0)return;  /* no more args */
    if(match("*"))j=POINTER;
      else j=VARIABLE;
    if(symname(n)==0) illname();
    if(findloc(n))multidef(n);
    if(match("["))  /* POINTER ? */
    /* it is a POINTER, so skip all */
    /* stuff between "[]" */
      {while(inbyte()!=']')
        if(endst())break;
      j=POINTER;
      /* add entry as POINTER */
      }
    addloc(n,j,t,8+argtop-argstk);
    argstk=argstk-4;  /* cnt down *//*modified by E.V.*/
    /* K&R handling conditionally - SA */
    if( kandr ) {
      if(endst())return;
      if(match(",")==0)error("expected comma"); 
    } else {
      return;
    }
    }
}
/*          */
/*  Statement parser    */
/*          */
/* called whenever syntax requires  */
/*  a statement.        */
/*  this routine performs that statement */
/*  and returns a number telling which one */
statement()
{
        /* NOTE (RDK) --- On DOS there is no CPM FUNCTION so just try */
        /* commenting it out for the first test compilation to see if */
        /* the compiler basic framework works OK in the DOS environment */
  /* if(cpm(11,0) & 1)  /* check for ctrl-C gtf 7/17/80 */
    /* if(getchar()==3) */
      /* zabort(); */

  if ((ch()==0) & (eof)) return;
  else if(amatch("char",4))
    {declloc(CCHAR);ns();}
  else if(amatch("int",3))
    {declloc(CINT);ns();}
  else if(match("{"))compound();
  else if(amatch("if",2))
    {doif();lastst=STIF;}
  else if(amatch("while",5))
    {dowhile();lastst=STWHILE;}
  else if(amatch("for",3))
    {dofor();lastst=STFOR;}
  else if(amatch("do", 2))
    {dodo();lastst=STDO;}
  else if(amatch("return",6))
    {doreturn();ns();lastst=STRETURN;}
  else if(amatch("break",5))
    {dobreak();ns();lastst=STBREAK;}
  else if(amatch("continue",8))
    {docont();ns();lastst=STCONT;}
  else if(match(";"));
  else if(match("#asm"))
    {doasm();lastst=STASM;}
  /* if nothing else, assume it's an expression */
  else{expression();ns();lastst=STEXP;}
  return lastst;
}
/*          */
/*  Semicolon enforcer    */
/*          */
/* called whenever syntax requires a semicolon */
ns()  {if(match(";")==0)error("missing semicolon");}
/*          */
/*  Compound statement    */
/*          */
/* allow any number of statements to fall between "{}" */
compound()
  {
  ++ncmp;    /* new level open */
  while (match("}")==0) statement(); /* do one */
  --ncmp;    /* close current level */
  }
/*          */
/*    "if" statement    */
/*          */
doif()
  {
  int flev,fsp,flab1,flab2;
  flev=locptr;  /* record current local level */
  fsp=Zsp;    /* record current stk ptr */
  flab1=getlabel(); /* get label for false branch */
  test(flab1);  /* get expression, and branch false */
  statement();  /* if true, do a statement */
  Zsp=modstk(fsp);  /* then clean up the stack */
  locptr=flev;  /* and deallocate any locals */
  if (amatch("else",4)==0)  /* if...else ? */
    /* simple "if"...print false label */
    {printlabel(flab1);col();nl();
    return;    /* and exit */
    }
  /* an "if...else" statement. */
  jump(flab2=getlabel());  /* jump around false code */
  printlabel(flab1);col();nl();  /* print false label */
  statement();    /* and do "else" clause */
  Zsp=modstk(fsp);    /* then clean up stk ptr */
  locptr=flev;    /* and deallocate locals */
  printlabel(flab2);col();nl();  /* print true label */
  }
/*          */
/*  "while" statement    */
/*          */
dowhile()
  {
  int wq[4];    /* allocate local queue */
  wq[WQSYM]=locptr;  /* record local level */
  wq[WQSP]=Zsp;    /* and stk ptr */
  wq[WQLOOP]=getlabel();  /* and looping label */
  wq[WQLAB]=getlabel();  /* and exit label */
  addwhile(wq);    /* add entry to queue */
        /* (for "break" statement) */
  printlabel(wq[WQLOOP]);col();nl(); /* loop label */
  test(wq[WQLAB]);  /* see if true */
  statement();    /* if so, do a statement */
  Zsp = modstk(wq[WQSP]);  /* zap local vars: 9/25/80 gtf */
  jump(wq[WQLOOP]);  /* loop to label */
  printlabel(wq[WQLAB]);col();nl(); /* exit label */
  locptr=wq[WQSYM];  /* deallocate locals */
  delwhile();    /* delete queue entry */
  }
dodo()
{
  int wq[4];
  wq[WQSYM]=locptr;
  wq[WQSP]=Zsp;
  wq[WQLOOP]=getlabel();
  wq[WQLAB]=getlabel();
  addwhile(wq);

  printlabel(wq[WQLOOP]);col();nl();
  statement();
  Zsp = modstk(wq[WQSP]);
  if(amatch("while",5)==0)
  {error("'while' needed");}
  needbrack("(");
  expression();
  ol("testl %eax, %eax");
  ot("jne");tab();
  printlabel(wq[WQLOOP]);nl();
  printlabel(wq[WQLAB]);col();nl();
  
  needbrack(")");
  ns();
  locptr=wq[WQSYM];
  delwhile();
}
dofor()
{
  int wq[4];
  int bl;
  int tl,tl1;
  bl=getlabel();
  wq[WQSYM]=locptr;
  wq[WQSP]=Zsp;
  wq[WQLOOP]=getlabel();
  wq[WQLAB]=getlabel();
  addwhile(wq);
  
  needbrack("(");
  expression();/*i=0*/
  ns();
  printlabel(wq[WQLOOP]);col();nl();
  expression();/*i<N*/
  testjump(wq[WQLAB]);
  ns();
  tl=getlitstk();
  expression();/*i++*/
  getlitstk();
  needbrack(")");
  statement();
  Zsp = modstk(wq[WQSP]);  /* zap local vars: 9/25/80 gtf */
  printf("dumpltstk...\n");
  /*dumpltstk(tl1);*/
  dumpltstk(tl);
  jump(wq[WQLOOP]);  /* loop to label */
  printlabel(wq[WQLAB]);col();nl(); /* exit label */
  locptr=wq[WQSYM];  /* deallocate locals */
  delwhile();    /* delete queue entry */
}
/*          */
/*  "return" statement    */
/*          */
doreturn()
  {
  /* if not end of statement, get an expression */
  if(endst()==0)expression();
  /*modstk(0);*/  /* clean up stk */
  ol("movl %ebp, %esp");
  ol("popl %ebp");
  zret();    /* and exit FUNCTION */
  }
/*          */
/*  "break" statement    */
/*          */
dobreak()
  {
  int *ptr;
  /* see if any "whiles" are open */
  if ((ptr=readwhile())==0) return;  /* no */
  modstk((ptr[WQSP]));  /* else clean up stk ptr */
  jump(ptr[WQLAB]);  /* jump to exit label */
  }
/*          */
/*  "continue" statement    */
/*          */
docont()
  {
  int *ptr;
  /* see if any "whiles" are open */
  if ((ptr=readwhile())==0) return;  /* no */
  modstk((ptr[WQSP]));  /* else clean up stk ptr */
  jump(ptr[WQLOOP]);  /* jump to loop label */
  }
/*          */
/*  "asm" pseudo-statement    */
/*          */
/* enters mode where assembly language statement are */
/*  passed intact through parser  */
doasm()
  {
  cmode=0;    /* mark mode as "asm" */
  while (1)
    {insline();  /* get and print lines */
    if (match("#endasm")) break;  /* until... */
    if(eof)break;
    outstr(line);
    nl();
    }
  kill();    /* invalidate line */
  cmode=1;    /* then back to parse level */
  }
/*  >>>>> start of cc3 <<<<<<<<<  */

/*          */
/*  Perform a FUNCTION call    */
/*          */
/* called from heir11, this routine will either call */
/*  the NAMEd FUNCTION, or if the supplied ptr is */
/*  zero, will call the contents of HL    */
callfunction(ptr)
  char *ptr;  /* symbol table entry (or 0) */
{  int nargs,tl;
  nargs=0;
  tl=getlitstk();
  blanks();  /* already saw open paren */
  if(ptr==0)zpush();  /* calling HL */
  while(streq(line+lptr,")")==0)
    {if(endst())break;
    expression();  /* get an argument */
    /*if(ptr==0)swapstk();*/ /* don't push addr */
    zpush();  /* push argument */
    getlitstk();
    nargs=nargs+4;  /* count args*2 *//*4, modified by E.V.*/
    if (match(",")==0) break;
    }
  needbrack(")");
  dumpltstk(tl);
  if(ptr)zcall(ptr);
  else callstk(nargs);
  Zsp=modstk(Zsp+nargs);  /* clean up arguments */
}
junk()
{  if(an(inbyte()))
    while(an(ch()))gch();
  else while(an(ch())==0)
    {if(ch()==0)break;
    gch();
    }
  blanks();
}
endst()
{  blanks();
  return ((streq(line+lptr,";")|(ch()==0)));
}
illname()
{  error("illegal symbol NAME");junk();}
multidef(sname)
  char *sname;
{  error("already defined");
  comment();
  outstr(sname);nl();
}
needbrack(str)
  char *str;
{  if (match(str)==0)
    {error("missing bracket");
    comment();outstr(str);nl();
    }
}
needlval()
{  error("must be lvalue");
}
findglb(sname)
  char *sname;
{  char *ptr;
  ptr=STARTGLB;
  while(ptr!=glbptr)
    {if(astreq(sname,ptr,NAMEMAX))return ptr;
    ptr=ptr+SYMSIZ;
    }
  return 0;
}
findloc(sname)
  char *sname;
{  char *ptr;
  ptr=STARTLOC;
  while(ptr!=locptr)
    {if(astreq(sname,ptr,NAMEMAX))return ptr;
    ptr=ptr+SYMSIZ;
    }
  return 0;
}
addglb(sname,id,typ,value)
  char *sname,id,typ;
  int value;
{  char *ptr;
  if(cptr=findglb(sname))return cptr;
  if(glbptr>=ENDGLB)
    {error("global symbol table overflow");
    return 0;
    }
  cptr=ptr=glbptr;
  while(an(*ptr++ = *sname++));  /* copy NAME */
  cptr[IDENT]=id;
  cptr[TYPE]=typ;
  cptr[STORAGE]=STATIK;
  cptr[OFFSET]=value;
  cptr[OFFSET+1]=value>>8;
  glbptr=glbptr+SYMSIZ;
  return cptr;
}
addloc(sname,id,typ,value)
  char *sname,id,typ;
  int value;
{  char *ptr;
  if(cptr=findloc(sname))return cptr;
  if(locptr>=ENDLOC)
    {error("local symbol table overflow");
    return 0;
    }
  cptr=ptr=locptr;
  while(an(*ptr++ = *sname++));  /* copy NAME */
  cptr[IDENT]=id;
  cptr[TYPE]=typ;
  cptr[STORAGE]=STKLOC;
  cptr[OFFSET]=value;
  cptr[OFFSET+1]=value>>8;
  locptr=locptr+SYMSIZ;
  return cptr;
}
/* Test if next input string is legal symbol NAME */
symname(sname)
  char *sname;
{  int k;char c;
  blanks();
  if(alpha(ch())==0)return 0;
  k=0;
  while(an(ch()))sname[k++]=gch();
  sname[k]=0;
  return 1;
  }
/* Return next avail internal label number */
getlabel()
{  return(++nxtlab);
}
/* Print specified number as label */
printlabel(label)
  int label;
{  outasm("cc");
  outdec(label);
}
/* Test if given character is alpha */
alpha(c)
  char c;
{  c=c&127;
  return(((c>='a')&(c<='z'))|
    ((c>='A')&(c<='Z'))|
    (c=='_'));
}
/* Test if given character is numeric */
numeric(c)
  char c;
{  c=c&127;
  return((c>='0')&(c<='9'));
}
/* Test if given character is alphanumeric */
an(c)
  char c;
{  return((alpha(c))|(numeric(c)));
}
/* Print a carriage return and a string only to console */
pl(str)
  char *str;
{  int k;
  k=0;
  putchar(eol);
  while(str[k])putchar(str[k++]);
}
addwhile(ptr)
  int ptr[];
 {
  int k;
  if (wqptr==WQMAX)
    {error("too many active whiles");return;}
  k=0;
  while (k<WQSIZ)
    {*wqptr++ = ptr[k++];}
}
delwhile()
  {if(readwhile()) wqptr=wqptr-WQSIZ;
  }
readwhile()
 {
  if (wqptr==wq){error("no active whiles");return 0;}
  else return (wqptr-WQSIZ);
 }
ch()
{  return(line[lptr]&127);
}
nch()
{  if(ch()==0)return 0;
    else return(line[lptr+1]&127);
}
gch()
{  if(ch()==0)return 0;
    else return(line[lptr++]&127);
}
kill()
{  lptr=0;
  line[lptr]=0;
}
inbyte()
{
  while(ch()==0)
    {if (eof) return 0;
    insline();
    preprocess();
    }
  return gch();
}
inchar()
{
  if(ch()==0)insline();
  if(eof)return 0;
  return(gch());
}
insline()
{
  int k,unit;
  while(1)
    {if (input==0)openin();
    if(eof)return;
    if((unit=input2)==0)unit=input;
    kill();
    while((k=getc(unit))>0)
      {if((k==eol)|(lptr>=LINEMAX))break;
      line[lptr++]=k;
      }
    line[lptr]=0;  /* append null */
    lineno++;  /* read one more line    gtf 7/2/80 */
    if(k<=0)
      {fclose(unit);
      if(input2)endinclude();    /* gtf 7/16/80 */
        else input=0;
      }
    if(lptr)
      {if((ctext)&(cmode))
        {comment();
        outstr(line);
        nl();
        }
      lptr=0;
      return;
      }
    }
}
/*  >>>>>> start of cc4 <<<<<<<  */

keepch(c)
  char c;
{  mline[mptr]=c;
  if(mptr<MPMAX)mptr++;
  return c;
}
preprocess()
{  int k;
  char c,sname[NAMESIZE];
  if(cmode==0)return;
  mptr=lptr=0;
  while(ch())
    {if((ch()==' ')|(ch()==9))
      {keepch(' ');
      while((ch()==' ')|
        (ch()==9))
        gch();
      }
    else if(ch()=='"')
      {keepch(ch());
      gch();
      while(ch()!='"')
        {if(ch()==0)
          {error("missing quote");
          break;
          }
        keepch(gch());
        }
      gch();
      keepch('"');
      }
    else if(ch()==39)
      {keepch(39);
      gch();
      while(ch()!=39)
        {if(ch()==0)
          {error("missing apostrophe");
          break;
          }
        keepch(gch());
        }
      gch();
      keepch(39);
      }
    else if((ch()=='/')&(nch()=='*'))
      {inchar();inchar();
      while(((ch()=='*')&
        (nch()=='/'))==0)
        {if(ch()==0)insline();
          else inchar();
        if(eof)break;
        }
      inchar();inchar();
      }
    else if((ch()=='0')&(nch()=='x'))/*added by E.V.*/
      {
      keepch(gch());keepch(gch());
      while(an(ch())|((ch()>='a')&(ch()<='f')))
          keepch(gch());
      }
    else if(alpha(ch()))  /* from an(): 9/22/80 gtf */
      {k=0;
      while(an(ch()))
        {if(k<NAMEMAX)sname[k++]=ch();
        gch();
        }
      sname[k]=0;
      if(k=findmac(sname))
        while(c=macq[k++])
          keepch(c);
      else
        {k=0;
        while(c=sname[k++])
          keepch(c);
        }
      }
    else keepch(gch());
    }
  keepch(0);
  if(mptr>=MPMAX)error("line too long");
  lptr=mptr=0;
  while(line[lptr++]=mline[mptr++]);
  lptr=0;
  }
addmac()
{  char sname[NAMESIZE];
  int k;
  if(symname(sname)==0)
    {illname();
    kill();
    return;
    }
  k=0;
  while(putmac(sname[k++]));
  while(ch()==' ' | ch()==9) gch();
  while(putmac(gch()));
  if(macptr>=MACMAX)error("macro table full");
  }
putmac(c)
  char c;
{  macq[macptr]=c;
  if(macptr<MACMAX)macptr++;
  return c;
}
findmac(sname)
  char *sname;
{  int k;
  k=0;
  while(k<macptr)
    {if(astreq(sname,macq+k,NAMEMAX))
      {while(macq[k++]);
      return k;
      }
    while(macq[k++]);
    while(macq[k++]);
    }
  return 0;
}
/* direct output to console    gtf 7/16/80 */
toconsole()
{
  saveout = output;
  output = 0;
/* end toconsole */}

/* direct output back to file    gtf 7/16/80 */
tofile()
{
  if(saveout)
    output = saveout;
  saveout = 0;
/* end tofile */}

outbyte(c)
   char c;
{
  if(output==0)
  {putchar(c);return c;}
  if(tolitstk==0)
  return outbyte1(c);
  return putlitstk(c);
}
outbyte1(c)
  char c;
{
  if(c==0)return 0;
  if(output)
    {if((putc(c,output))<=0)
      {closeout();
      error("Output file error");
      zabort();      /* gtf 7/17/80 */
      }
    }
  else putchar(c);
  return c;
}
outstr(ptr)
  char ptr[];
 {
  int k;
  k=0;
  while(outbyte(ptr[k++]));
 }

/* write text destined for the assembler to read */
/* (i.e. stuff not in comments)      */
/*  gtf  6/26/80 */
outasm(ptr)
char *ptr;
{
  while(outbyte(*ptr++));
/* end outasm */}
outasm1(ptr)
char *ptr;
{
  while(outbyte1(*ptr++));
}
nl(){outbyte(eol);}
nl1(){outbyte1(eol);}
tab(){outbyte(9);}
tab1(){outbyte1(9);}
col(){outbyte(58);}
col1(){outbyte1(58);}
comma(){outbyte(',');}
comma1(){outbyte1(',');}

bell()        /* gtf 7/16/80 */
  {outbyte(7);}
/*        replaced 7/2/80 gtf
 * error(ptr)
 *  char ptr[];
 * {
 *  int k;
 *  comment();outstr(line);nl();comment();
 *  k=0;
 *  while(k<lptr)
 *    {if(line[k]==9) tab();
 *      else outbyte(' ');
 *    ++k;
 *    }
 *  outbyte('^');
 *  nl();comment();outstr("******  ");
 *  outstr(ptr);
 *  outstr("  ******");
 *  nl();
 *  ++errcnt;
 * }
 */

error(ptr)
char ptr[];
{  int k;
  char junk[81];

  toconsole();
  bell();
  outstr("Line "); outdec(lineno); outstr(", ");
  if(infunc==0)
    outbyte('(');
  if(currfn==NULL)
    outstr("start of file");
  else  outstr(currfn+NAME);
  if(infunc==0)
    outbyte(')');
  outstr(" + ");
  outdec(lineno-fnstart);
  outstr(": ");  outstr(ptr);  nl();

  outstr(line); nl();

  k=0;  /* skip to error position */
  while(k<lptr){
    if(line[k++]==9)
      tab();
    else  outbyte(' ');
    }
  outbyte('^');  nl();
  ++errcnt;

  if(errstop){
    pl("Continue (Y,n,g) ? ");
    gets(junk);    
    k=junk[0];
    if((k=='N') | (k=='n'))
      zabort();
    if((k=='G') | (k=='g'))
      errstop=0;
    }
  tofile();
/* end error */}

ol(ptr)
  char ptr[];
{
  ot(ptr);
  nl();
}
ot(ptr)
  char ptr[];
{
  tab();
  outasm(ptr);
}
streq(str1,str2)
  char str1[],str2[];
 {
  int k;
  k=0;
  while (str2[k])
    {if ((str1[k])!=(str2[k])) return 0;
    k++;
    }
  return k;
 }
astreq(str1,str2,len)
  char str1[],str2[];int len;
 {
  int k;
  k=0;
  while (k<len)
    {if ((str1[k])!=(str2[k]))break;
    if(str1[k]==0)break;
    if(str2[k]==0)break;
    k++;
    }
  if (an(str1[k]))return 0;
  if (an(str2[k]))return 0;
  return k;
 }
match(lit)
  char *lit;
{
  int k;
  blanks();
  if (k=streq(line+lptr,lit))
    {lptr=lptr+k;
    return 1;
    }
   return 0;
}
amatch(lit,len)
  char *lit;int len;
 {
  int k;
  blanks();
  if (k=astreq(line+lptr,lit,len))
    {lptr=lptr+k;
    while(an(ch())) inbyte();
    return 1;
    }
  return 0;
 }
blanks()
  {while(1)
    {while(ch()==0)
      {insline();
      preprocess();
      if(eof)break;
      }
    if(ch()==' ')gch();
    else if(ch()==9)gch();
    else return;
    }
  }
/* output a decimal number - rewritten 4/1/81 gtf */
outdec(n)
int n;
{
  if(n<0)
    outbyte('-');
  else  n = -n;
  outint(n);
/* end outdec */}

outint(n)  /* added 4/1/81 */
int n;
{  int q;

  q = n/10;
  if(q) outint(q);
  outbyte('0'-(n-q*10));
/* end outint */}

/* return the length of a string */
/* gtf 4/8/80 */
strlen(s)
char *s;
{  char *t;

  t = s;
  while(*s) s++;
  return(s-t);
/* end strlen */}

/* convert lower case to upper */
/* gtf 6/26/80 */
raise(c)
char c;
{
  if((c>='a') & (c<='z'))
    c = c - 'a' + 'A';
  return(c);
/* end raise */}

/* ------------------------------------------------------------- */

/*  >>>>>>> start of cc5 <<<<<<<  */

/* as of 5/5/81 rj */

expression()
{
  int lval[2];
  if(heir1(lval))rvalue(lval);
}
heir1(lval)
  int lval[];
{
  int k,lval2[2];
  k=heir2(lval);
  if (match("="))
    {if(k==0){needlval();return 0;}
    if (lval[1])zpush();
    if(heir1(lval2))rvalue(lval2);
    store(lval);
    return 0;
    }
  else return k;
}
heir2(lval)
  int lval[];
{  int k,lval2[2];
  k=heir3(lval);
  blanks();
  if(ch()!='|')return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("|"))
      {zpush();
      if(heir3(lval2)) rvalue(lval2);
      zpop();
      zor();
      }
    else return 0;
    }
}
heir3(lval)
  int lval[];
{  int k,lval2[2];
  k=heir4(lval);
  blanks();
  if(ch()!='^')return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("^"))
      {zpush();
      if(heir4(lval2))rvalue(lval2);
      zpop();
      zxor();
      }
    else return 0;
    }
}
heir4(lval)
  int lval[];
{  int k,lval2[2];
  k=heir5(lval);
  blanks();
  if(ch()!='&')return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("&"))
      {zpush();
      if(heir5(lval2))rvalue(lval2);
      zpop();
      zand();
      }
    else return 0;
    }
}
heir5(lval)
  int lval[];
{
  int k,lval2[2];
  k=heir6(lval);
  blanks();
  if((streq(line+lptr,"==")==0)&
    (streq(line+lptr,"!=")==0))return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("=="))
      {zpush();
      if(heir6(lval2))rvalue(lval2);
      zpop();
      zeq();
      }
    else if (match("!="))
      {zpush();
      if(heir6(lval2))rvalue(lval2);
      zpop();
      zne();
      }
    else return 0;
    }
}
heir6(lval)
  int lval[];
{
  int k,lval2[2];
  k=heir7(lval);
  blanks();
  if((streq(line+lptr,"<")==0)&
    (streq(line+lptr,">")==0)&
    (streq(line+lptr,"<=")==0)&
    (streq(line+lptr,">=")==0))return k;
    if(streq(line+lptr,">>"))return k;
    if(streq(line+lptr,"<<"))return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("<="))
      {zpush();
      if(heir7(lval2))rvalue(lval2);
      zpop();
      if(cptr=lval[0])
        if(cptr[IDENT]==POINTER)
        {ule();
        continue;
        }
      if(cptr=lval2[0])
        if(cptr[IDENT]==POINTER)
        {ule();
        continue;
        }
      zle();
      }
    else if (match(">="))
      {zpush();
      if(heir7(lval2))rvalue(lval2);
      zpop();
      if(cptr=lval[0])
        if(cptr[IDENT]==POINTER)
        {uge();
        continue;
        }
      if(cptr=lval2[0])
        if(cptr[IDENT]==POINTER)
        {uge();
        continue;
        }
      zge();
      }
    else if((streq(line+lptr,"<"))&
      (streq(line+lptr,"<<")==0))
      {inbyte();
      zpush();
      if(heir7(lval2))rvalue(lval2);
      zpop();
      if(cptr=lval[0])
        if(cptr[IDENT]==POINTER)
        {ult();
        continue;
        }
      if(cptr=lval2[0])
        if(cptr[IDENT]==POINTER)
        {ult();
        continue;
        }
      zlt();
      }
    else if((streq(line+lptr,">"))&
      (streq(line+lptr,">>")==0))
      {inbyte();
      zpush();
      if(heir7(lval2))rvalue(lval2);
      zpop();
      if(cptr=lval[0])
        if(cptr[IDENT]==POINTER)
        {ugt();
        continue;
        }
      if(cptr=lval2[0])
        if(cptr[IDENT]==POINTER)
        {ugt();
        continue;
        }
      zgt();
      }
    else return 0;
    }
}
/*  >>>>>> start of cc6 <<<<<<  */

heir7(lval)
  int lval[];
{
  int k,lval2[2];
  k=heir8(lval);
  blanks();
  if((streq(line+lptr,">>")==0)&
    (streq(line+lptr,"<<")==0))return k;
  if(k)rvalue(lval);
  while(1)
    {if (match(">>"))
      {zpush();
      if(heir8(lval2))rvalue(lval2);
      zpop();
      asr();
      }
    else if (match("<<"))
      {zpush();
      if(heir8(lval2))rvalue(lval2);
      zpop();
      asl();
      }
    else return 0;
    }
}
heir8(lval)
  int lval[];
{
  int k,lval2[2];
  k=heir9(lval);
  blanks();
  if((ch()!='+')&(ch()!='-'))return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("+"))
      {zpush();
      if(heir9(lval2))rvalue(lval2);
      if(cptr=lval[0])
        if((
          (cptr[IDENT]==ARRAY)|
           (cptr[IDENT]==POINTER))&
         (cptr[TYPE]==CINT))/* modified by E.V. */
          /*doublereg();*/ol("sall $2, %eax");
      zpop();
      zadd();
      }
    else if (match("-"))
      {zpush();
      if(heir9(lval2))rvalue(lval2);
      if(cptr=lval[0])
        if(((cptr[IDENT]==POINTER)|(cptr[IDENT]==ARRAY))&
        (cptr[TYPE]==CINT))
          /*doublereg();*/ol("sall $2, %eax");
      zpop();
      zsub();
      }
    else return 0;
    }
}
heir9(lval)
  int lval[];
{
  int k,lval2[2];
  k=heir10(lval);
  blanks();
  if((ch()!='*')&(ch()!='/')&
    (ch()!='%'))return k;
  if(k)rvalue(lval);
  while(1)
    {if (match("*"))
      {zpush();
      if(heir9(lval2))rvalue(lval2);
      zpop();
      mult();
      }
    else if (match("/"))
      {zpush();
      if(heir10(lval2))rvalue(lval2);
      zpop();
      div();
      }
    else if (match("%"))
      {zpush();
      if(heir10(lval2))rvalue(lval2);
      zpop();
      zmod();
      }
    else return 0;
    }
}
heir10(lval)
  int lval[];
{
  int k;
  char *ptr;
  if(match("++"))
    {if((k=heir10(lval))==0)
      {needlval();
      return 0;
      }
    if(lval[1])zpush();
    rvalue(lval);
    inc();
    ptr=lval[0];
    if((ptr[IDENT]==POINTER)&
      (ptr[TYPE]==CINT))
      {inc();inc();inc();}
    store(lval);
    return 0;
    }
  else if(match("--"))
    {if((k=heir10(lval))==0)
      {needlval();
      return 0;
      }
    if(lval[1])zpush();
    rvalue(lval);
    dec();
    ptr=lval[0];
    if((ptr[IDENT]==POINTER)&
      (ptr[TYPE]==CINT))
      {dec();dec();dec();}
    store(lval);
    return 0;
    }
  else if (match("-"))
    {k=heir10(lval);
    if (k) rvalue(lval);
    neg();
    return 0;
    }
  else if(match("*"))
    {k=heir10(lval);
    if(k)rvalue(lval);
    lval[1]=CINT;
    if(ptr=lval[0])lval[1]=ptr[TYPE];
    lval[0]=0;
    return 1;
    }
  else if(match("!"))/*added by E.V.*/
    {
    k=heir10(lval);
    if(k)rvalue(lval);
    lnot();
    return 0;
    }
  else if(match("~"))/*added by E.V.*/
    {
    k=heir10(lval);
    if(k)rvalue(lval);
    bnot();
    return 0;
    }
  else if(match("&"))
    {
    k=heir10(lval);
    if(k==0)
      {
      error("illegal address");
      return 0;
      }
    else if(lval[1])return 0;
    else
      {
        immed();
        ot("$");
        outasm(ptr=lval[0]);
        outasm(", %eax");
        nl();
        lval[1]=ptr[TYPE];
        return 0;
      }
    }
  else 
    {k=heir11(lval);
    if(match("++"))
      {if(k==0)
        {needlval();
        return 0;
        }
      if(lval[1])zpush();
      rvalue(lval);
      inc();
      ptr=lval[0];
      if(ptr)
        if((ptr[IDENT]==POINTER)&
         (ptr[TYPE]==CINT))
        {inc();inc();inc();}
      store(lval);
      dec();
      if(ptr)
        if((ptr[IDENT]==POINTER)&
         (ptr[TYPE]==CINT))
        {dec();dec();dec();}
      return 0;
      }
    else if(match("--"))
      {if(k==0)
        {needlval();
        return 0;
        }
      if(lval[1])zpush();
      rvalue(lval);
      dec();
      ptr=lval[0];
      if(ptr)
        if((ptr[IDENT]==POINTER)&
         (ptr[TYPE]==CINT))
        {dec();dec();dec();}
      store(lval);
      inc();
      if(ptr)
        if((ptr[IDENT]==POINTER)&
         (ptr[TYPE]==CINT))
        {inc();inc();inc();}
      return 0;
      }
    else return k;
    }
  }
/*  >>>>>> start of cc7 <<<<<<  */

heir11(lval)
  int *lval;
{  int k;char *ptr;
  k=primary(lval);
  ptr=lval[0];
  blanks();
  if((ch()=='[')|(ch()=='('))
  while(1)
    {if(match("["))
      {if(ptr==0)
        {error("can't subscript");
        junk();
        needbrack("]");
        return 0;
        }
      else if(ptr[IDENT]==POINTER)rvalue(lval);
      else if(ptr[IDENT]!=ARRAY)
        {error("can't subscript");
        k=0;
        }
      zpush();
      expression();
      needbrack("]");
      if(ptr[TYPE]==CINT)/*doublereg();*/ol("sall $2, %eax");
      zpop();
      zadd();
      lval[1]=ptr[TYPE];
        /* 4/1/81 - after subscripting, not ptr anymore */
      lval[0]=0;
      k=1;
      }
    else if(match("("))
      {if(ptr==0)
        {callfunction(0);
        }
      else if(ptr[IDENT]!=FUNCTION)
        {rvalue(lval);
        callfunction(0);
        }
      else callfunction(ptr);
      k=lval[0]=0;
      }
    else return k;
    }
  if(ptr==0)return k;
  if(ptr[IDENT]==FUNCTION)
    {immed();
    outname(ptr);
    outasm(", %eax");
    nl();
    return 0;
    }
  return k;
}
primary(lval)
  int *lval;
{  char *ptr,sname[NAMESIZE];int num[1];
  int k;
  if(match("("))
    {k=heir1(lval);
    needbrack(")");
    return k;
    }
  if(symname(sname))
    {if(ptr=findloc(sname))
      {getloc(ptr);
      lval[0]=ptr;
      lval[1]=ptr[TYPE];
      if(ptr[IDENT]==POINTER)lval[1]=CINT;
      if(ptr[IDENT]==ARRAY)return 0;
        else return 1;
      }
    if(ptr=findglb(sname))
      if(ptr[IDENT]!=FUNCTION)
      {lval[0]=ptr;
      lval[1]=0;
      if(ptr[IDENT]!=ARRAY)return 1;
      
      immed();ot("$");/*handling the ARRAY address- by E.V.*/
      outasm(ptr);
      outasm(", %eax");nl();
      lval[1]=ptr[TYPE];
      return 0;
      }
    ptr=addglb(sname,FUNCTION,CINT,0);
    lval[0]=ptr;
    lval[1]=0;
    return 0;
    }
  if(constant(num))
    return(lval[0]=lval[1]=0);
  else
    {error("invalid expression");
    immed();outdec(0);
    comma();outasm(", %eax");nl();
    junk();
    return 0;
    }
  }
store(lval)
  int *lval;
{  if (lval[1]==0)putmem(lval[0]);
  else putstk(lval[1]);
}
rvalue(lval)
  int *lval;
{  if((lval[0] != 0) & (lval[1] == 0))
    getmem(lval[0]);
    else indirect(lval[1]);
}
test(label)
  int label;
{
  needbrack("(");
  expression();
  needbrack(")");
  testjump(label);
}
constant(val)
  int val[];
{  if (number(val))
  {immed();outasm("$");}
  else if (pstr(val))
    {immed();outasm("$");}
  else if (qstr(val))
    {immed();outasm("$");printlabel(litlab);
    outbyte('+');/*outasm("$");*/}
  else return 0;  

  outdec(val[0]);
  outasm(", %eax");
  nl();
  return 1;
}
number(val)
  int val[];
{  int k,minus;char c;
 int d; 
  k=minus=1;
  if(match("0x"))
    {
    k=0;
    while(numeric(ch())|((ch()>='a')&(ch()<='f')))
      {
      c=inbyte();
      if(numeric(c))
        {
        k=(k<<4)+(c-'0');
        }
      else
        {
        k=(k<<4)+(c-'a'+10);
        }
      }
    val[0]=k;
    return 1;
    }
  while(k)
    {k=0;
    if (match("+")) k=1;
    if (match("-")) {minus=(-minus);k=1;}
    }
  if(numeric(ch())==0)return 0;
  while (numeric(ch()))
    {c=inbyte();
    k=k*10+(c-'0');
    }
  if (minus<0) k=(-k);
  val[0]=k;
  return 1;
}
pstr(val)
  int val[];
{  int k;char c;
  k=0;
  if (match("'")==0) return 0;
  while((c=gch())!=39)
    k=(k&255)*256 + (c&127);
  val[0]=k;
  return 1;
}
qstr(val)
  int val[];
{  char c;
  if (match(quote)==0) return 0;
  val[0]=litptr;
  while (ch()!='"')
    {if(ch()==0)break;
    if(litptr>=LITMAX)
      {error("string space exhausted");
      while(match(quote)==0)
        if(gch()==0)break;
      return 1;
      }
    c=gch();
    if(c!=92)
      litq[litptr++]=c;
    else
      {
      c=gch();
      if(c==0)break;
      if(c=='n')c=10;
      else if(c=='t')c=9;
      else if(c=='b')c=8;
      else if(c=='f')c==12;
      litq[litptr++]=c;
      }
    }
  gch();
  litq[litptr++]=0;
  return 1;
}
/*  >>>>>> start of cc8 <<<<<<<  */

/* Begin a comment line for the assembler */
comment()
{  outbyte('#');outbyte(':');
}

/* Put out assembler info before any code is generated */
header()
{  comment();
  outstr(BANNER);
  nl();
  comment();
  outstr(VERSION);
  nl();
  comment();
  outstr(AUTHOR);
  nl();
  comment();
  nl();
  /*(if(mainflg){*/    /* do stuff needed for first */
  /*ol("ORG 100h");*/ /* assembler file. */       
  /*ol("LHLD 6");*/  /* set up stack */
  /*ol("SPHL");*/
  /*callrts("ccgo");*/  /* set default drive for CP/M */
  /*zcall("main");*/  /* call the code generated by small-c */
  /*zcall("exit");*/  /* do an exit    gtf 7/16/80 */
  /*}*/
}
/* Print any assembler stuff needed after all code */
trailer()
{  /* ol("END"); */  /*...note: commented out! */

  nl();      /* 6 May 80 rj errorsummary() now goes to console */
  comment();
  outstr(" --- End of Compilation ---");
  nl();
  ot(".IDENT");tab();outstr(quote);
  outstr(IDNT);outstr(quote);
  nl();
}
/* Print out a NAME such that it won't annoy the assembler */
/*  (by matching anything reserved, like opcodes.) */
/*  gtf 4/7/80 */
outname(sname)
char *sname;
{  int len, i,j;

/*outasm("qz");*/
  len = strlen(sname);
  if(len>(ASMPREF+ASMSUFF)){
    i = ASMPREF;
    len = len-ASMPREF-ASMSUFF;
    while(i-- > 0)
      outbyte(raise(*sname++));
    while(len-- > 0)
      sname++;
    while(*sname)
      outbyte(raise(*sname++));
    }
  else  outasm(sname);
/* end outname */}
/* Fetch a static memory cell into the primary register */
getmem(sym)
  char *sym;
{  if((sym[IDENT]!=POINTER)&(sym[TYPE]==CCHAR))
    {ot("LDA ");
    outname(sym+NAME);
    nl();
    callrts("ccsxt");
    }
  else
    {ot("movl ");
    outname(sym+NAME);
    outasm(", %eax");
    nl();
    }
  }
/* Fetch the address of the specified symbol */
/*  into the primary register */
getloc(sym)
  char *sym;
{
  int t;
  t=(sym[OFFSET]&255)+
  ((sym[OFFSET+1]&255)<<8);
  if(sym[OFFSET+1]&0x80)
  t=t|0xffff0000;/*patched for 32 bits by E.V.*/
  /*ot("getloc ");*/
  ot("leal");tab();
  outdec(t);outasm("(%ebp), %eax");nl();
  /*
  immed();
 
  outdec(((sym[OFFSET]&255)+
    ((sym[OFFSET+1]&255)<<8))-
    Zsp);
  outasm(", %eax");
  nl();
  ol("DAD SP");
  */
  }
/* Store the primary register into the specified */
/*  static memory cell */
putmem(sym)
  char *sym;
{  if((sym[IDENT]!=POINTER)&(sym[TYPE]==CCHAR))
  {/*ol("MOV A,L");*/
    ot("movb %al, ");
    }
  else ot("movl %eax, ");
  outname(sym+NAME);
  nl();
  }
/* Store the specified object TYPE in the primary register */
/*  at the address on the top of the stack */
putstk(typeobj)
char typeobj;
{  zpop();
  if(typeobj==CINT)
    {/*callrts("ccpint");*/
      ol("movl %eax, (%edx)");
    }
  else
    {/*ol("MOV A,L");*/    /* per Ron Cain: gtf 9/25/80 */
    /*ol("STAX D");*/
    ol("movb %al, (%edx)");
    }
}
/* Fetch the specified object TYPE indirect through the */
/*  primary register into the primary register */
indirect(typeobj)
  char typeobj;
{
  if(typeobj==CCHAR)
  {/*callrts("ccgchar");*/
    ol("movsbl (%eax),%eax");
  }
  else 
  {/*callrts("ccgint");*/
    ol("movl (%eax), %eax");
  }
}
/* Swap the primary and secondary registers */
swap()
{  ol("TTTTTxchgl %eax, %edx");
}
/* Print partial instruction to get an immediate value */
/*  into the primary register */
immed()
{  ot("movl ");
}
/* Push the primary register onto the stack */
zpush()
{  ol("pushl %eax");
 Zsp=Zsp-4;/*modified by E.V.*/
}
/* Pop the top of the stack into the secondary register */
zpop()
{  ol("popl %edx");
 Zsp=Zsp+4;/*modified by E.V.*/
}
/* Swap the primary register and the top of the stack */
swapstk()
{  ol("XTHL");
}
/* Call the specified subroutine NAME */
zcall(sname)
  char *sname;
{  ot("call ");
  outname(sname);
  nl();
}
/* Call a run-time library routine */
callrts(sname)
char *sname;
{
  ot("call ");
  outasm(sname);
  nl();
/*end callrts*/}

/* Return from subroutine */
zret()
{  ol("ret");
}
/* Perform subroutine call to value on top of stack */
callstk(nargs)
   int nargs;
{
  immed();outdec(nargs);outasm("(%esp), %eax");nl();
  ol("call *%eax");
  /*immed();
  outasm("$+5");
  nl();
  swapstk();
  ol("PCHL");
  Zsp=Zsp+2;*/ /* corrected 5 May 81 rj */
}
/* Jump to specified internal label number */
jump(label)
  int label;
{  ot("jmp ");
  printlabel(label);
  nl();
  }
/* Test the primary register and jump if false to label */
testjump(label)
  int label;
{  /*ol("MOV A,H");
  ol("ORA L");
  ot("JZ ");*/
  ol("testl %eax, %eax");
  ot("je");tab();
  printlabel(label);
  nl();
  }
/* Print pseudo-op to define a byte */
defbyte()
{  ot(".byte ");
}
/*Print pseudo-op to define STORAGE */
defstorage()
{  ot(".comm ");
}
/* Print pseudo-op to define a word */
defword()
{  ot("DW ");
}
/* Modify the stack POINTER to the new value indicated */
modstk(newsp)
  int newsp;
 {  int k;
  k=newsp-Zsp;
  if(k==0)return newsp;
  if(k>=0)
    {if(k<7)
      {if(k&1)
        {ol("INX SP");
        k--;
        }
      while(k)
        {ol("popl %edx");
        k=k-4;
        }
      return newsp;
      }
    }
  if(k<0)
    {
      /*ot("subl $");outdec(-k);outasm(", %esp");nl();*/
      if(k>-7)
      {
        if(k&1)
        {
          ol("decl %esp");
          k++;
        }
        if(k&2)
        {
          ol("decl %esp");
          k++;
          ol("decl %esp");
          k++;
        }
        while(k)
        {
          ol("pushl %edx");
          k=k+4;/*modified by E.V.*/
        }
        return newsp;
        }
    }
  /*swap();
  immed();outdec(k);outasm(", %eax");nl();
  ol("DAD SP");
  ol("SPHL");
  swap();*/
  if(k>0)
    {ot("addl $");outdec(k);outasm(", %esp");nl();}
  else
    {ot("subl $");outdec(-k);outasm(", %esp");nl();}
  return newsp;
}
/* Double the primary register */
doublereg()
{  ol("DAD H");
}
/* Add the primary and secondary registers */
/*  (results in primary) */
zadd()
{  ol("addl %edx, %eax");
}
/* Subtract the primary register from the secondary */
/*  (results in primary) */
zsub()
{  /*callrts("ccsub");*/
  ol("subl %eax, %edx");
  ol("movl %edx, %eax");
}
/* Multiply the primary and secondary registers */
/*  (results in primary */
mult()
{  /*callrts("ccmult");*/
  ol("imull %edx");
}
/* Divide the secondary register by the primary */
/*  (quotient in primary, remainder in secondary) */
div()
{/*callrts("ccdiv");*/
  ol("xchgl %eax, %edx");
  ol("movl %edx, %ecx");
  ol("cltd");
  ol("idivl %ecx");
}
/* Compute remainder (mod) of secondary register divided */
/*  by the primary */
/*  (remainder in primary, quotient in secondary) */
zmod()
{
  div();
  ol("movl %edx, %eax");
  /*swap();*/
}
/* Inclusive 'or' the primary and the secondary registers */
/*  (results in primary) */
zor()
{/*callrts("ccor");*/
  ol("orl %edx, %eax");
}
/* Exclusive 'or' the primary and seconday registers */
/*  (results in primary) */
zxor()
{/*callrts("ccxor");*/
  ol("xorl %edx, %eax");
}
/* 'And' the primary and secondary registers */
/*  (results in primary) */
zand()
{/*callrts("ccand");*/
  ol("andl %edx, %eax");
}
/* Arithmetic shift right the secondary register number of */
/*  times in primary (results in primary) */
asr()
{/*callrts("ccasr");*/
  ol("movl %eax, %ecx");
  ol("movl %edx, %eax");
  ol("sarl %cl, %eax");
}
/* Arithmetic left shift the secondary register number of */
/*  times in primary (results in primary) */
asl()
{/*callrts("ccasl");*/
  ol("movl %eax, %ecx");
  ol("movl %edx, %eax");
  ol("sall %cl, %eax");
}
/* Form two's complement of primary register */
neg()
{/*callrts("ccneg");*/
  ol("negl %eax");
}
lnot()/*added by E.V.*/
{
  ol("testl %eax,%eax");
  ol("sete %al");
  ol("movzbl %al, %eax");
}
bnot()/*added by E.V.*/
{
  ol("notl %eax");
}
/* Form one's complement of primary register */
com()
  {callrts("cccom");}
/* Increment the primary register by one */
inc()
  {ol("incl %eax");}
/* Decrement the primary register by one */
dec()
  {ol("decl %eax");}

/* Following are the conditional operators */
/* They compare the secondary register against the primary */
/* and put a literal 1 in the primary if the condition is */
/* true, otherwise they clear the primary register */

/* Test for equal */
zeq()
{/*callrts("cceq");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("sete");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for not equal */
zne()
{/*callrts("ccne");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setne");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for less than (signed) */
zlt()
{/*callrts("cclt");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setl");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for less than or equal to (signed) */
zle()
{/*callrts("ccle");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setle");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for greater than (signed) */
zgt()
{/*callrts("ccgt");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setg");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for greater than or equal to (signed) */
zge()
{/*callrts("ccge");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setge");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for less than (unsigned) */
ult()
{/*callrts("ccult");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setb");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for less than or equal to (unsigned) */
ule()
{/*callrts("ccule");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setbe");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for greater than (unsigned) */
ugt()
{/*callrts("ccugt");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("seta");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}
/* Test for greater than or equal to (unsigned) */
uge()
{/*callrts("ccuge");*/
  ot("cmpl");ot("%eax, %edx");nl();
  ot("setae");ot("%al");nl();
  ot("movzbl");ot("%al, %eax");nl();
}

/*  <<<<<  End of small-c compiler  >>>>>  */

