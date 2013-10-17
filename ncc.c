/************************************************/
/** Last: 8:53 PM 8/14/2008 NCC.C               */
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: c0.1.5 New: inc macro pool 1664->2176 */
/** Stat: compiles itself! Vers for .com by mod */
/**  vstart=0100h, and commenting out section   */
/**  .dseg to form small model CS=DS=SS, the    */
/**  CMD.EXE dpmi server sets SP=FFF0h on load- */
/**  ing a .COM file at segment:0100h and the   */
/**  Program Segment Prefix is initialized with */
/**  the legacy Location 0005h Vector for the   */
/**  CP/M-80 API Emulation under dos fcb struct-*/
/**  ures, also using the CP/M-80 ABI register  */
/**  usage. see: http://www.patersontech.com/   */
/**   dos/Docs/86_Dos_inst_.pdf                 */
/**  also, added user input hints..             */
/************************************************/
/** Last: 8:41 PM 8/2/2008                      */
/** Vers: c0.1.4 New: inc macro pool 1400->1664 */
/** Stat: Successfully compiles itself with the */
/**  current IO library!                        */
/************************************************/
/** Last: 11:36 AM 8/2/2008                     */
/** Vers: 0.1.3 New: incr macro pool 1200->1400 */
/**             gains about +18                 */
/************************************************/
/** Last: 25-Jul-08 07:26:13 AM                 */
/** Vers: 0.1.3 New: chg parse return to exit(),*/
/**  reverted nl().                             */
/************************************************/
/** Last: 24-Jul-08 08:15:15 PM                 */
/** Vers: 0.1.2 New: DIV(), SDIV()              */
/************************************************/
/** SCNXa.C for LVL1 compile w/ POWERC          */
/** SCNX.C for LVL2 compile w/ SCNXa.EXE        */
/** Extern Files Req'd: IOC5.C for LVL2.        */
/** POWERC compiles K&R with pcDos iolib which  */
/** runs under win XP thru CMD.EXE, a DPMIv.9   */
/** server, this is a LVL1 build.  LVL2 build   */
/** binds a Call_5 legacy io-lib, which mimics  */ 
/** CP/M-80 io functions under pcDos.  LVL2 is a*/
/** 'stand-in' for your hobby io-lib.  It shows */
/** the cross development step needed to boot-  */
/** strap mimimalist small-c for your hobby OS. */
/** Since this demonstrates small-c compiling   */
/** itself, along with grafting a minimal custom*/
/** io-lib, it shows how the migration path to a*/
/** new hobby OS platform can be achieved with  */
/** small-c initially.  The back-end assembler  */
/** syntax chosen is for NASM, 16bit small model*/
/** with an eye for 32bit multi-segmented, or   */
/** 64 bit later on.                            */
/************************************************/
/** Modified for NASM as SCNX.C Small-C, nasm   */
/** Last: 7:45 AM 7/6/2008                      */
/** Prev: 29 JUL 2007 Init: 31 MAR 2007         */
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.1.2b  Goal: NASM ouput, Call5 i.f. **/
/** Base: from SCN_all.c &increase macro tblsz **/
/************************************************/
/** Modified for NASM as SCN_I.C Small-C, nasm **/
/** Last: 27 APR 2008                           */
/** Prev: 29 JUL 2007 Init: 31 MAR 2007        **/
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.0.4b  Goal: NASM ouput             **/
/** Stat: incomplete modules                    */
/************************************************/
/* 9:03 AM 4/27/2008                            */
/* remove angle brkts of include stdio.h        */
/* for self compile testing.                    */
/************************************************/
/* 9:17 AM 5/21/2008                            */
/* added outhexb(int) in scn_v.c                */
/** for PowerC             **/
/** #include "scn_i.c"     **/
/** #include "scn_ii.c"    **/
/** #include "scn_iii.c"   **/
/** #include "scn_iv.c"    **/
/** #include "scn_v.c"     **/
/**                        **/
/************************************************/
/** Modified for NASM as SCN_I.C Small-C, nasm **/
/** Last: 29 JUL 2007 Init: 31 MAR 2007        **/
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.0.3b  Goal: NASM ouput             **/
/**                                            **/
/************************************************/
/* ToBe Modified for NASM as SCCN.C --          */
/*  Small-C Compiler - Nasm                     */
/* Last: 15 JUL 2007 Init: 14 JUL 2007          */
/* By: s_dubrovich@yahoo.com                    */
/* Vers: 0.0.1b  Goal: NASM ouput               */
/* Comments:                                    */
/*  This is a new offshoot of development from  */
/*  NCn.C as changes beyond CC8.C are intended. */
/*  NCn.C is Archived, Closed to further        */
/*  development, superceded by this effort SCCN.*/
/* EndComments:                                 */
/** SCCN.C is parked                           **/
/************************************************/
/** New Development Branch                     **/
/************************************************/
/** Re-Codified as:                            **/
/**     I.   Global Storage Defn, main()       **/
/**     II.  Recursive Descent Parser          **/
/**     III. Utility Functions                 **/
/**     IV.  U.I. & I/O                        **/
/**     V.   Backend, NASM Object Output       **/
/************************************************/
/************************************************/
/*                                              */
/*              small-c compiler                */
/*                  rev. 1.1                    */
/*                by Ron Cain                   */
/*                  (1980)                      */
/************************************************/
/* #define NULL 0 -for PowerC */
/* #define eol 13 -for PowerC */

/*	UNIX definitions (if not stand-alone)	*/
/* #include <stdio.h>	*/
/* #define eol 10	*/

/* Note use PowerC Library */
/* #include <stdio.h>		**/
/* #include stdio.h * remove angle brkts */
/* #define eol 10	*/

/** SCNX.C & CALL5_IO.C attempt self compile   **/
#define eol 10	/** this is correct **/
#define NULL 0
/** move to the end --> #include IOC5.C         */
/** However, initfcb() still must be called     */
/**  before main(), so include stub function,   */
/**  just ahead of file io in main().           */
/************************************************/
/** Appendix A                                 **/
/**  4. C bases interp. of Identifier upon two **/
/**     attributes: Storage Class and its Type.**/
/** Storage Class determines Location, Lifetime**/
/** Type determines the meaning of the values  **/
/**  found in the identifier's storage.        **/
/** Storage Classes[4]={auto,static,extern,reg}**/
/** Type[Object]={char,int[3],float,double}    **/
/**  Type[derived]={array,functions,pointer,   **/
/**                 structures,unions}         **/
/**  5. Objects and lvalues - An Object is a   **/
/** manipulatable region of storage, an lvalue **/
/** is an expression refering to an object.    **/
/************************************************/

/*	Define the symbol table parameters	*/

#define	symsiz	16 /* incr sz by 2 for name */

/** #define	symtbsz	5040	**/
#define symtbsz	5760  /** 360x16 **/
#define numglbs 300
#define	numlocals	60	/* new def */
#define	startglb symtab
#define	endglb	startglb+numglbs*symsiz /**4200**/
#define	startloc endglb+symsiz          /**4214**/
#define	endloc	symtab+symtbsz-symsiz /**net812**/
/** 812 div by symsiz = 58 locals              **/
/**--------------------------------------------**/
/*	Define symbol table entry format	*/
/** modified for longer symbol name 		*/
#define	name	0	/** length 11 {0..10}  **/
#define	ident	11	/** length 1           **/
#define	type	12      /** length 1           **/
#define	storage	13      /** length 1           **/
#define	offset	14      /** length 2 {12..13}  **/

/*	System wide name size (for symbols)	*/
#define	namesize 9
#define	namemax  8

/*	Define possible entries for "ident"	*/
/** Derived Types-                             **/
#define	variable 1
#define	array	2
#define	pointer	3
#define	function 4

/*	Define possible entries for "type"	*/
#define	cchar	1  /** 8 bits **/
#define	cint	2  /** 16 bits **/

/*	Define possible entries for "storage"	*/
/** Storage Class-                             **/
#define	statik	1
#define	stkloc	2

/**--------------------------------------------**/
/**	Define possible entries for "segment"  **/
#define cseg 1  /** code **/
#define dseg 2  /** data **/
#define eseg 3  /** extra **/
#define sseg 4  /** stack **/
#define fseg 5  /** FS    **/
#define gseg 6  /** GS    **/
#define aux1 7  /** local heap **/
#define aux2 8  /** DTA, data transfer area **/
#define init 9  /** initialize on loading **/
#define ntry 10 /** module entry **/
#define stat 11 /** module status, exceptions **/

/**--------------------------------------------**/
/*	Define the "while" statement queue	*/
#define	wqtabsz	100	/** {0..24} entries    **/
#define	wqsiz	4   /** ea. entry size, 4 bytes**/
#define	wqmax	wq+wqtabsz-wqsiz /**lim offs 96**/

/*	Define entry offsets in while queue	*/
/** Entry of byte elements of indexes:         **/
#define	wqsym	0
#define	wqsp	1
#define	wqloop	2
#define	wqlab	3

/**--------------------------------------------**/
/*      Define the literal pool                 */
/*#define	litabsz	2000                    */
/** 2000 limit reached on self compile [1]      */
#define litabsz 4000  
#define	litmax	litabsz-1

/**--------------------------------------------**/
/*      Define the input line                   */
#define	linesize 80
#define	linemax	linesize-1
#define	mpmax	linemax

/**--------------------------------------------**/
/*      Define the macro (define) pool          */
#define	macqsize 2176 /** was 1664 **/
#define	macmax	macqsize-1

/**--------------------------------------------**/
/*      Define statement types (tokens)         */
#define	stif	1
#define	stwhile	2
#define	streturn 3
#define	stbreak	4
#define	stcont	5
#define	stasm	6
#define	stexp	7


/*      Now reserve some storage words           */
char	symtab[symtbsz];  /* symbol table        */
char	*glbptr,*locptr;  /* ptrs to next entries*/
int	wq[wqtabsz];      /* while queue         */
int	*wqptr;           /* ptr to next entry   */

char	litq[litabsz];    /* literal pool        */
int	litptr;           /* ptr to next entry   */

char	macq[macqsize];   /* macro string buffer */
int	macptr;           /* and its index       */

char	line[linesize];   /* parsing buffer      */
char	mline[linesize];  /* temp macro buffer   */
int	lptr,mptr;        /* ptrs into each      */

/*      Misc storage                             */
int	nxtlab,           /* next avail label #  */
	litlab,  /* label # assigned to literal pool*/
	c_sp,    /* compiler relative stk ptr       */
	argstk,  /* function arg c_sp               */
	ncmp,    /* # open compound statements      */
	errcnt,  /* # errors in compilation         */
	eof,     /* set non-zero on final input eof */
	input,   /* iob # for input file            */
	output,  /* iob # for output file (if any)  */
	input2,  /* iob # for "include" file        */
	glbflag, /* non-zero if internal globals    */
	ctext,   /* non-zero to intermix c-source   */
	cmode,   /* non-zero while parsing c-code   */
                 /* zero when passing assembly code */
	lastst;  /* last executed statement type    */

char	quote[2];/* literal string for '"'      */
char	*cptr;   /* work ptr to any char buffer */
int	*iptr;   /* work ptr to any int buffer  */

/************************************************/
/** Module: SCCN1.C                            **/
/** Last: 20 JUL 2007  Init: 14 JUL 2007       **/
/** Vers: 0.0.2b  Goal: Self Compile, Nasm out **/
/** Modified for NASM by s_dubrovich@yahoo.com **/
/************************************************/
/** modified dumpglbs, dumplits                **/

/*	>>>>> start cc1 <<<<<<		*/
/*					*/
/*	Compiler begins execution here	*/
/*					*/
main()
	{
	glbptr=startglb; /* clear global symbols */
	locptr=startloc; /* clear local symbols  */
	wqptr=wq;       /* clear while queue     */
	macptr=         /* clear the macro pool  */
	litptr=         /* clear literal pool    */
  	c_sp =          /* stack ptr (relative)  */
	errcnt=         /* no errors             */
	eof=            /* not eof yet           */
	input=          /* no input file         */
	input2=         /* or include file       */
	output=         /* no open units         */
	ncmp=           /* no open compound states*/
	lastst=         /* no last statement yet*/
	quote[1]=
	0;              /*  ...all set to zero..*/
	quote[0]='"';   /* fake a quote literal */
	cmode=1;        /* enable preprocessing */
/***--------------------------------------------*/
/*** IO Init code before file use, in IOC5.C ****/
	/*initfcb(); */
/***--------------------------------------------*/
/*	                        */
/*	compiler body           */
/*	                        */
	ask();       /* get user options        */
	openout();   /* get an output file      */
	openin();    /* and initial input file  */
	header();    /* intro code              */
	parse();     /* process ALL input       */
		 /** added back in **/
	dumplits();  /* then dump literal pool  */
/** dump global static defs. to output source, required **/
	dumpglbs();  /* and all static memory   */
		/** original place, added back in **/
	errorsummary(); /* summarize errors     */
	trailer();   /* follow-up code          */
	closeout();  /* close the output (if any) */
/***	return;	** then exit to system */
	exit(0); /*** sjd 25-JUL-2008 ***/
	}
/********************************/
/*				*/
/** for PowerC 			**/
/** see "scn_all.c"		**/
/** #include "scn_ii.c"		**/
/** #include "scn_iii.c"	**/
/** #include "scn_iv.c"		**/
/** #include "scn_v.c"		**/
/** 				**/
/** 				**/
/**  SCN_I.C **/
/** Next is SCN_II.C **/
/************************************************/
/** Modified for NASM as SCN_II.C Small-C, nasm**/
/** Last: 29 JUL 2007 Init: 31 MAR 2007        **/
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.0.3b  Goal: NASM ouput             **/
/**					       **/
/************************************************/
/* ToBe Modified for NASM as SCCN.C --          */
/*  Small-C Compiler - Nasm                     */
/* Last: 15 JUL 2007 Init: 14 JUL 2007          */
/* By: s_dubrovich@yahoo.com                    */
/* Vers: 0.0.1b  Goal: NASM ouput               */
/* Comments:                                    */
/*  This is a new offshoot of development from  */
/*  NCn.C as changes beyond CC8.C are intended. */
/*  NCn.C is Archived, Closed to further        */
/*  development, superceded by this effort SCCN.*/
/* EndComments:                                 */
/** SCCN.C is parked                           **/
/************************************************/
/** New Development Branch                     **/
/************************************************/
/** Re-Codified as:                            **/
/**	I.   Global Storage Defn, main()       **/
/**     II.  Recursive Descent Parser          **/
/**     III. Utility Functions                 **/
/**     IV.  U.I. & I/O                        **/
/**     V.   Backend, NASM Object Output       **/
/************************************************/
/************************************************/
/*						*/
/*		small-c compiler		*/
/*                  rev. 1.1                    */
/*		  by Ron Cain			*/
/*		    (1980)			*/
/************************************************/
/** Fn's in this file **/
/** parse(); 	** process ALL input */
/** declgbl, newfunc, addmac,  doasm **/
/** declloc(), put in here  **/
/** doinclude(), is in SCN_IV **/
/************************************************/

/*					*/
/*	Process all input text		*/
/*					*/
/* At this level, only static declarations, */
/*	defines, includes, and function */
/*	definitions are legal...	*/
parse()
	{
	while (eof==0)		/* do until no more input */
		{
		if(amatch("char",4)){declglb(cchar);ns();}
		else if(amatch("int",3)){declglb(cint);ns();}
		else if(match("#asm"))doasm();
		else if(match("#include"))doinclude();
		else if(match("#define"))addmac();
		else newfunc();
		blanks();	/* force eof if pending */
		}
	}
/************************************************/

/*					*/
/*	Declare a static variable	*/
/*	  (i.e. define for use)		*/
/*					*/
/* makes an entry in the symbol table so subsequent */
/*  references can call symbol by name	*/
declglb(typ)		/* typ is cchar or cint */
	int typ;
{	int k,j;char sname[namesize];
	while(1)
		{while(1)
			{if(endst())return;	/* do line */
			k=1;		/* assume 1 element */
			if(match("*"))	/* pointer ? */
				j=pointer;	/* yes */
				else j=variable; /* no */
			if (symname(sname)==0) /* name ok? */
				illname(); /* no... */
			if(findglb(sname)) /* already there? */
				multidef(sname);
			if (match("["))		/* array? */
				{k=needsub();	/* get size */
				if(k)j=array;	/* !0=array */
				else j=pointer; /* 0=ptr */
				}
			addglb(sname,j,typ,k); /* add symbol */
			break;
			}
		if (match(",")==0) return; /* more? */
		}
	}

/*					*/
/*	Declare local variables		*/
/*	(i.e. define for use)		*/
/*					*/
/* works just like "declglb" but modifies machine stack */
/*	and adds symbol table entry with appropriate */
/*	stack offset to find it again			*/
declloc(typ)		/* typ is cchar or cint */
	int typ;
	{
	int k,j;char sname[namesize];
	while(1)
		{while(1)
			{if(endst())return;
			if(match("*"))
				j=pointer;
				else j=variable;
			if (symname(sname)==0)
				illname();
			if(findloc(sname))
				multidef(sname);
			if (match("["))
				{k=needsub();
				if(k)
					{j=array;
					if(typ==cint)k=k+k;
					}
				else
					{j=pointer;
					k=2;
					}
				}
			else
				if((typ==cchar)
					&(j!=pointer))
					k=1;else k=2;
			/* change machine stack */
			c_sp=mnem_modstk(c_sp-k);
			addloc(sname,j,typ,c_sp);
			break;
			}
		if (match(",")==0) return;
		}
	}


/************************************************/

addmac()
{	char sname[namesize];
	int k;
	if(symname(sname)==0)
		{illname();
		kill();
		return;
		}
	k=0;
	while(putmac(sname[k++]));
	while(chr()==' ' | chr()==9) gchr();
	while(putmac(gchr()));
	if(macptr>=macmax)error("macro table full");
	}

putmac(c)
	char c;
{	macq[macptr]=c;
	if(macptr < macmax)macptr++;
	return c;
}

findmac(sname)
	char *sname;
{	int k;
	k=0;
	while(k < macptr)
		{if(astreq(sname,macq+k,namemax))
			{while(macq[k++]);
			return k;
			}
		while(macq[k++]);
		while(macq[k++]);
		}
	return 0;
}

/************************************************/
/*					*/
/*	Get required array size		*/
/*					*/
/* invoked when declared variable is followed by "[" */
/*	this routine makes subscript the absolute */
/*	size of the array. */

needsub()
	{
	int num[1];
	if(match("]"))return 0;	/* null size */
	if (number(num)==0)	/* go after a number */
		{error("must be constant");	/* it isn't */
		num[0]=1;		/* so force one */
		}
	if (num[0] < 0)
		{error("negative size illegal");
		num[0]=(-num[0]);
		}
	needbrack("]");		/* force single dimension */
	return num[0];		/* and return size */
	}


/*					*/
/*	Begin a function		*/
/*					*/
/* Called from "parse" this routine tries to make a function */
/*	out of what follows.	*/

newfunc()
	{
	char n[namesize],*ptr;
	if (symname(n)==0)
		{error("illegal function or declaration");
		kill();	/* invalidate line */
		return;
		}
	if(ptr=findglb(n))	/* already in symbol table ? */
		{if(ptr[ident]!=function)multidef(n);
			/* already variable by that name */
		else if(ptr[offset]==function)multidef(n);
			/* already function by that name */
		else ptr[offset]=function;
			/* otherwise we have what was earlier*/
			/*  assumed to be a function */
		}

	/* if not in table, define as a function now */

	else addglb(n,function,cint,function);

	/* we had better see open paren for args... */

	if(match("(")==0)error("missing open paren");
	outstr(n);col();nl();	/* print function name */
	argstk=0;		/* init arg count */
	while(match(")")==0)	/* then count args */
		/* any legal name bumps arg count */
		{if(symname(n))argstk=argstk+2;
		else{error("illegal argument name");junk();}
		blanks();
		/* if not closing paren, should be comma */
		if(streq(line+lptr,")")==0)
			{if(match(",")==0)
			error("expected comma");
			}
		if(endst())break;
		}

	locptr=startloc;	/* "clear" local symbol table */
	c_sp=0;			/* preset stack ptr */
	while(argstk)
		/* now let user declare what types of things */
		/*	those arguments were */
		{if(amatch("char",4)){getarg(cchar);ns();}
		else if(amatch("int",3)){getarg(cint);ns();}
		else{error("wrong number args");break;}
		}

	if(statement()!=streturn) /* do a statement, but if */
				/* it's a return, skip */
				/* cleaning up the stack */
		{mnem_modstk(0);
		mnem_ret();
		}
	c_sp=0;			/* reset stack ptr again */
	locptr=startloc;        /* deallocate all locals */
	}

/*					*/
/*	Declare argument types		*/
/*					*/
/* called from "newfunc" this routine adds an entry in the */
/*	local symbol table for each named argument */

getarg(t)		/* t = cchar or cint */
	int t;
	{
	char n[namesize],c;int j;
	while(1)
		{if(argstk==0)return;	/* no more args */
		if(match("*"))j=pointer;
			else j=variable;
		if(symname(n)==0) illname();
		if(findloc(n)) multidef(n);
		if(match("["))	/* pointer ? */

		/* it is a pointer, so skip all */
		/* stuff between "[]" */

			{while(inbyte()!=']')
				if(endst())break;
			j=pointer;
			/* add entry as pointer */
			}
		addloc(n,j,t,argstk);
		argstk=argstk-2;	/* cnt down */
		if(endst())return;
		if(match(",")==0)error("expected comma");
		}
	}

/*					*/
/*	Statement parser		*/
/*					*/
/* called whenever syntax requires	*/
/*	a statement. 			 */
/*  this routine performs that statement */
/*  and returns a number telling which one */

statement()
{
	if ((chr()==0) & (eof)) return;
	else if(amatch("char",4))
		{declloc(cchar);ns();}
	else if(amatch("int",3))
		{declloc(cint);ns();}
	else if(match("{"))compound();
	else if(amatch("if",2))
		{doif();lastst=stif;}
/*	else if(amatch("do",2))		added by codeworks */
/*		{dodo();lastst=stwhile;}	version */
	else if(amatch("while",5))
		{dowhile();lastst=stwhile;}
	else if(amatch("return",6))
		{doreturn();ns();lastst=streturn;}
	else if(amatch("break",5))
		{dobreak();ns();lastst=stbreak;}
	else if(amatch("continue",8))
		{docont();ns();lastst=stcont;}
	else if(match(";"));
	else if(match("#asm"))
		{doasm();lastst=stasm;}
	/* if nothing else, assume it's an expression */
	else{expression();ns();lastst=stexp;}
	return lastst;
}

/*					*/
/*	Semicolon enforcer		*/
/*					*/
/* called whenever syntax requires a semicolon */

ns()	{if(match(";")==0)error("missing semicolon");}


/*					*/
/*	Compound statement		*/
/*					*/
/* allow any number of statements to fall between "{}" */

compound()
	{
	++ncmp;		/* new level open */
	while (match("}")==0)
		if(eof) return;
		else statement();
	--ncmp;		/* close current level */
	}

/*					*/
/*		"if" statement		*/
/*					*/

doif()
	{
	int flev,fsp,flab1,flab2;
	flev=locptr;	/* record current local level */
	fsp=c_sp;		/* record current stk ptr */
	flab1=getlabel(); /* get label for false branch */
	m_test(flab1);	/* get expression, and branch false */
	statement();	/* if true, do a statement */
	c_sp=mnem_modstk(fsp);	/* then clean up the stack */
	locptr=flev;	/* and deallocate any locals */
	if (amatch("else",4)==0)	/* if...else ? */
		/* simple "if"...print false label */
		{printlabel(flab1);col();nl();
		return;		/* and exit */
		}
	/* an "if...else" statement. */

	mnem_jump(flab2=getlabel());	/* jump around false code */
	printlabel(flab1);col();nl();	/* print false label */
	statement();		/* and do "else" clause */
	c_sp=mnem_modstk(fsp);		/* then clean up stk ptr */
	locptr=flev;		/* and deallocate locals */
	printlabel(flab2);col();nl();	/* print true label */
	}

/*					*/
/*	"while" statement		*/
/*					*/

dowhile()
	{
	int wq[4];		/* allocate local queue */
	wq[wqsym]=locptr;	/* record local level */
	wq[wqsp]=c_sp;		/* and stk ptr */
	wq[wqloop]=getlabel();	/* and looping label */
	wq[wqlab]=getlabel();	/* and exit label */
	addwhile(wq);		/* add entry to queue */
				/* (for "break" statement) */
	printlabel(wq[wqloop]);col();nl(); /* loop label */
	m_test(wq[wqlab]);	/* see if true */
	statement();		/* if so, do a statement */
	mnem_jump(wq[wqloop]);	/* loop to label */
	printlabel(wq[wqlab]);col();nl(); /* exit label */
	locptr=wq[wqsym];	/* deallocate locals */
	c_sp=mnem_modstk(wq[wqsp]);	/* clean up stk ptr */
	delwhile();		/* delete queue entry */
	}

/*					*/
/*	"return" statement		*/
/*					*/

doreturn()
	{
	/* if not end of statement, get an expression */
	if(endst()==0)expression();
	mnem_modstk(0);	/* clean up stk */
	mnem_ret();	/* and exit function */
	}

/*					*/
/*	"break" statement		*/
/*					*/

dobreak()
	{
	int *ptr;
	/* see if any "whiles" are open */
	if ((ptr=readwhile())==0) return;	/* no */
	mnem_modstk((ptr[wqsp]));	/* else clean up stk ptr */
	mnem_jump(ptr[wqlab]);	/* jump to exit label */
	}

/*					*/
/*	"continue" statement		*/
/*					*/

docont()
	{
	int *ptr;
	/* see if any "whiles" are open */
	if ((ptr=readwhile())==0) return;	/* no */
	mnem_modstk((ptr[wqsp]));	/* else clean up stk ptr */
	mnem_jump(ptr[wqloop]);	/* jump to loop label */
	}

/*					*/
/*	"asm" pseudo-statement		*/
/*					*/
/* enters mode where assembly language statement are */
/*	passed intact through parser	*/

doasm()
	{
	cmode=0;		/* mark mode as "asm" */
	while (1)
		{inline2();	/* get and print lines */
		if (match("#endasm")) break;	/* until... */
		if(eof)break;
		outstr(line);
		nl();
		}
	kill();		/* invalidate line */
	cmode=1;		/* then back to parse level */
	}

/*					*/
/*	Perform a function call		*/
/*					*/
/* called from heir11, this routine will either call */
/*	the named function, or if the supplied ptr is */
/*	zero, will call the contents of HL- Primary */

callfunction(ptr)
	char *ptr;	/* symbol table entry (or 0) */
{	int nargs;
	nargs=0;
	blanks();	/* already saw open paren */
	if(ptr==0)mnem_push();	/* calling HL- Primary */
	while(streq(line+lptr,")")==0)
		{if(endst())break;
		expression();	/* get an argument */
		if(ptr==0)swapstk(); /* don't push addr */
		mnem_push();	/* push argument */
		nargs=nargs+2;	/* count args*2 */
		if (match(",")==0) break;
		}
	needbrack(")");
	if(ptr)mnem_call(ptr);
	else mnemcallstk();
	c_sp=mnem_modstk(c_sp+nargs);	/* clean up arguments */
}

junk()
{	if(an(inbyte()))
		while(an(chr()))gchr();
	else while(an(chr())==0)
		{if(chr()==0)break;
		gchr();
		}
	blanks();
}

endst()
{	blanks();
	return ((streq(line+lptr,";")|(chr()==0)));
}

/** ch > chr, and > m_and, pop > mnem_pop         **/
/** push > mnem_push, or > m_or, xor > m_xor **/


/** The hierarchical recursive descent steps have  **/
/** the pattern of, on entry, call the next higher **/
/** precedence level, and then at return, filter   **/
/** for lexeme operators for its level.  If false, **/
/** returns control to the lower precedence level. **/
/** If true, select for matching lexeme for its    **/
/** precedence level and perform the production    **/
/** actions to generate code.			   **/
/** expression() forms a control array: lval[2] to **/
/** which rvalue(lval) responds after primary(lval)**/
/** has made its determinations on the nature of   **/
/** the identifier parsed in a statement held in a **/
/** line buffer.  Two symbol tables are maintained **/
/** one for Local Symbols (auto storage class) and **/
/** one for Global Symbols (static storage class). **/
/** operations on auto variables take place on the **/
/** Stack, static variable operations involve their**/
/** storage in the Data Segment.  Symbol record    **/
/** structure, and the possible field values are   **/
/** defined at the begining of the source through  **/
/** #define statements.  This C subset language    **/
/** hasn't structures, unions, switch statements,  **/
/** etc., but has enough to bootstrap those elements*/
/** in later editions.  The primary(lval) sets lval**/
/** fields as a control mechanism on identifiers for*/
/** rvalue and the rest of the expression analyzer **/
/** as well, it sets a return value of zero or one **/
/** as an additional control flag.  Consequently,  **/
/** the following control table develops:	   **/
/**	 ident	lval[0]	[1]	ret_val	   	   **/
/**						   **/
/**    const	0	0	0		   **/
/**    funct	ptr	0	0		   **/
/**    array	ptr	typ	0		   **/
/**  pointer	ptr	0	1		   **/
/** variable	ptr	0	1		   **/
/** such that.. ptr is identifier location in symbol*/
/**  table and typ for array is one of cchar.1 or  **/
/**  ccint.2, and an identifier of pointer or var  **/
/** return(1), array and function identifiers      **/
/** return(0).					   **/


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
		if (lval[1])mnem_push();
		if(heir1(lval2))rvalue(lval2);
		store(lval);
		return 0;
		}
	else return k;
}

heir2(lval)
	int lval[];
{	int k,lval2[2];
	k=heir3(lval);
	blanks();
	if(chr()!='|')return k;
	if(k)rvalue(lval);
	while(1)
		{if (match("|"))
			{mnem_push();
			if(heir3(lval2)) rvalue(lval2);
			mnem_pop();
			m_or();
			}
		else return 0;
		}
}

heir3(lval)
	int lval[];
{	int k,lval2[2];
	k=heir4(lval);
	blanks();
	if(chr()!='^')return k;
	if(k)rvalue(lval);
	while(1)
		{if (match("^"))
			{mnem_push();
			if(heir4(lval2))rvalue(lval2);
			mnem_pop();
			m_xor();
			}
		else return 0;
		}
}

heir4(lval)
	int lval[];
{	int k,lval2[2];
	k=heir5(lval);
	blanks();
	if(chr()!='&')return k;
	if(k)rvalue(lval);
	while(1)
		{if (match("&"))
			{mnem_push();
			if(heir5(lval2))rvalue(lval2);
			mnem_pop();
			m_and();
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
			{mnem_push();
			if(heir6(lval2))rvalue(lval2);
			mnem_pop();
		/** cmp Secd. to Prim. ret1 in Prim. if TRUE **/
			eq(); 
			}
		else if (match("!="))
			{mnem_push();
			if(heir6(lval2))rvalue(lval2);
			mnem_pop();
			ne();
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
			{mnem_push();
			if(heir7(lval2))rvalue(lval2);
			mnem_pop();
			if(cptr=lval[0])
				if(cptr[ident]==pointer)
				{ule();
				continue;
				}
			le();
			}
		else if (match(">="))
			{mnem_push();
			if(heir7(lval2))rvalue(lval2);
			mnem_pop();
			if(cptr=lval[0])
				if(cptr[ident]==pointer)
				{uge();
				continue;
				}
			if(cptr=lval2[0])
				if(cptr[ident]==pointer)
				{uge();
				continue;
				}
			ge();
			}
		else if((streq(line+lptr,"<"))&
			(streq(line+lptr,"<<")==0))
			{inbyte();
			mnem_push();
			if(heir7(lval2))rvalue(lval2);
			mnem_pop();
			if(cptr=lval[0])
				if(cptr[ident]==pointer)
				{ult();
				continue;
				}
			if(cptr=lval2[0])
				if(cptr[ident]==pointer)
				{ult();
				continue;
				}
			lt();
			}
		else if((streq(line+lptr,">"))&
			(streq(line+lptr,">>")==0))
			{inbyte();
			mnem_push();
			if(heir7(lval2))rvalue(lval2);
			mnem_pop();
			if(cptr=lval[0])
				if(cptr[ident]==pointer)
				{ugt();
				continue;
				}
			if(cptr=lval2[0])
				if(cptr[ident]==pointer)
				{ugt();
				continue;
				}
			gt();
			}
		else return 0;
		}
}

/** ch > chr, div > m_div, push > mnem_push **/
/** add > m_add, sub > m_sub, mult > m_mult **/
/** inc > m_inc, dec > m_dec, neg > m_neg **/

heir7(lval)
	int lval[];
{
	int k,lval2[2];
	k=heir8(lval); /** recursive descent **/
	blanks();
	if((streq(line+lptr,">>")==0)&
		(streq(line+lptr,"<<")==0))return k;
		/** trap for >> << **/
	if(k)rvalue(lval);
	while(1)
		{if (match(">>"))
			{mnem_push();
			if(heir8(lval2))rvalue(lval2);
			mnem_pop();
			asr();
			}
		else if (match("<<"))
			{mnem_push();
			if(heir8(lval2))rvalue(lval2);
			mnem_pop();
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
	if((chr()!='+')&(chr()!='-'))return k;
	if(k)rvalue(lval);
	while(1)
		{if (match("+"))
			{mnem_push();
			if(heir9(lval2))rvalue(lval2);
			if(cptr=lval[0])
				if((cptr[ident]==pointer)&
				(cptr[type]==cint))
				doublereg();
			mnem_pop();
			m_add();
			}
		else if (match("-"))
			{mnem_push();
			if(heir9(lval2))rvalue(lval2);
			if(cptr=lval[0])
				if((cptr[ident]==pointer)&
				(cptr[type]==cint))
				doublereg();
			mnem_pop();
			m_sub();
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
	if((chr()!='*')&(chr()!='/')&
		(chr()!='%'))return k;
	if(k)rvalue(lval);
	while(1)
		{if (match("*"))
			{mnem_push();
			if(heir9(lval2))rvalue(lval2);
			mnem_pop();
			m_mult();
			}
		else if (match("/"))
			{mnem_push();
			if(heir10(lval2))rvalue(lval2);
			mnem_pop();
			m_div();
			}
		else if (match("%"))
			{mnem_push();
			if(heir10(lval2))rvalue(lval2);
			mnem_pop();
			mod();
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
		if(lval[1])mnem_push();
		rvalue(lval);
		m_inc();
		ptr=lval[0];
		if((ptr[ident]==pointer)&
			(ptr[type]==cint))
				m_inc();
		store(lval);
		return 0;
		}
	else if(match("--"))
		{if((k=heir10(lval))==0)
			{needlval();
			return 0;
			}
		if(lval[1])mnem_push();
		rvalue(lval);
		m_dec();
		ptr=lval[0];
		if((ptr[ident]==pointer)&
			(ptr[type]==cint))
			m_dec();
		store(lval);
		return 0;
		}
	else if (match("-"))
		{k=heir10(lval);
		if (k) rvalue(lval);
		m_neg();
		return 0;
		}
	else if(match("*"))
		{k=heir10(lval);
		if(k)rvalue(lval);
		if(ptr=lval[0])
			lval[1]=ptr[type];
		
		lval[0]=0;
		return 1;
		}
	else if(match("&"))
		{k=heir10(lval);
		if(k==0)
			{error("illegal address");
			return 0;
			}
		else if(lval[1])return 0;
		else
			{immed();
			outstr(ptr=lval[0]);
			nl();
			lval[1]=ptr[type];
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
			if(lval[1])mnem_push();
			rvalue(lval);
			m_inc();
			ptr=lval[0];
			if((ptr[ident]==pointer)&
				(ptr[type]==cint))
					m_inc();
			store(lval);
			m_dec();
			if((ptr[ident]==pointer)&
				(ptr[type]==cint))
				m_dec();
			return 0;
			}
		else if(match("--"))
			{if(k==0)
				{needlval();
				return 0;
				}
			if(lval[1])mnem_push();
			rvalue(lval);
			m_dec();
			if((ptr[ident]==pointer)&
				(ptr[type]==cint))
				m_dec();
			store(lval);
			m_inc();
			if((ptr[ident]==pointer)&
				(ptr[type]==cint))
				m_inc();
			return 0;
			}
		else return k;
		}
	}
/************************************************/
/** ch > chr, gch > gchr, add > m_add          **/
/** push > mnem_push, pop > mnem_pop, test > m_test  **/

heir11(lval)
	int *lval;
{	int k;char *ptr;
	k=primary(lval);
	ptr=lval[0];
	blanks();
	if((chr()=='[')|(chr()=='('))
	while(1)
		{if(match("["))
			{if(ptr==0)
				{error("can't subscript");
				junk();
				needbrack("]");
				return 0;
				}
			else if(ptr[ident]==pointer)rvalue(lval);
			else if(ptr[ident]!=array)
				{error("can't subscript");
				k=0;
				}
			mnem_push();
			expression();
			needbrack("]");
			if(ptr[type]==cint)doublereg();
			mnem_pop();
			m_add();
			lval[0]=0;
			lval[1]=ptr[type];
			k=1;
			}
		else if(match("("))
			{if(ptr==0)
				{callfunction(0);
				}
			else if(ptr[ident]!=function)
				{rvalue(lval);
				callfunction(0);
				}
			else callfunction(ptr);
			k=lval[0]=0;
			}
		else return k;
		}
	if(ptr==0)return k;
	if(ptr[ident]==function)
		{immed();	/*immedo*/
		outstr(ptr);
		nl();
		return 0;
		}
	return k;
}

primary(lval)
	int *lval;
{	char *ptr,sname[namesize];int num[1];
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
			lval[1]=ptr[type];
			if(ptr[ident]==pointer)lval[1]=cint;
			if(ptr[ident]==array)return 0;
					else return 1;
			}
		if(ptr=findglb(sname))
			if(ptr[ident]!=function)
			{lval[0]=ptr;
			lval[1]=0;
			if(ptr[ident]!=array)return 1;
			immed();
			outstr(ptr);nl(); /*outname*/
			lval[1]=ptr[type];
			return 0;
			}
		ptr=addglb(sname,function,cint,0);
		lval[0]=ptr;
		lval[1]=0;
		return 0;
		}
	if(constant(num))
		return(lval[0]=lval[1]=0);
	else
		{error("invalid expression");
		immed();outdec(0);nl();
		junk();
		return 0;
		}
	}

store(lval)
	int *lval;
{	if (lval[1]==0)putmem(lval[0]);
	else putstk(lval[1]);
}

rvalue(lval)
	int *lval;
{	if((lval[0] != 0) & (lval[1] == 0))
		getmem(lval[0]);
		else indirect(lval[1]);
}

m_test(label)
	int label;
{
	needbrack("(");
	expression();
	needbrack(")");
	testjump(label);
}

constant(val)
	int val[];
{	if (number(val))
		immed();
	else if (pstr(val))
		immed();
	else if (qstr(val))
		{immed();printlabel(litlab);outbyte('+');}
	else return 0;	
	outdec(val[0]);
	nl();
	return 1;
}

number(val)
	int val[];
{	int k,minus;char c;
	k=minus=1;
	while(k)
		{k=0;
		if (match("+")) k=1;
		if (match("-")) {minus=(-minus);k=1;}
		}
	if(numeric(chr())==0)return 0;
	while (numeric(chr()))
		{c=inbyte();
		k=k*10+(c-'0');
		}
	if (minus < 0) k=(-k);
	val[0]=k;
	return 1;
}

pstr(val)
	int val[];
{	int k;char c;
	k=0;
	if (match("'")==0) return 0;
	while((c=gchr())!=39)
		k=(k&255)*256 + (c&127);
	val[0]=k;
	return 1;
}

qstr(val)
	int val[];
{	char c;
	if (match(quote)==0) return 0;
	val[0]=litptr;
	while (chr()!='"')
		{if(chr()==0)break;
		if(litptr>=litmax)
			{error("string space exhausted");
			while(match(quote)==0)
				if(gchr()==0)break;
			return 1;
			}
		litq[litptr++]=gchr();
		}
	gchr();
	litq[litptr++]=0;
	return 1;
}

/****************************************/
/**  SCN_II.C 			       **/
/** Next is SCN_III.C 		       **/
/************************************************/
/** Modified for NASM as SCN_III.C Small-C,nasm**/
/** Last: 29 JUL 2007 Init: 31 MAR 2007        **/
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.0.3b  Goal: NASM ouput             **/
/**					       **/
/************************************************/
/* ToBe Modified for NASM as SCCN.C --          */
/*  Small-C Compiler - Nasm                     */
/* Last: 15 JUL 2007 Init: 14 JUL 2007          */
/* By: s_dubrovich@yahoo.com                    */
/* Vers: 0.0.1b  Goal: NASM ouput               */
/* Comments:                                    */
/*  This is a new offshoot of development from  */
/*  NCn.C as changes beyond CC8.C are intended. */
/*  NCn.C is Archived, Closed to further        */
/*  development, superceded by this effort SCCN.*/
/* EndComments:                                 */
/** SCCN.C is parked                           **/
/************************************************/
/** New Development Branch                     **/
/************************************************/
/** Re-Codified as:                            **/
/**	I.   Global Storage Defn, main()       **/
/**     II.  Recursive Descent Parser          **/
/**     III. Utility Functions                 **/
/**     IV.  U.I. & I/O                        **/
/**     V.   Backend, NASM Object Output       **/
/************************************************/
/************************************************/
/*						*/
/*		small-c compiler		*/
/*                  rev. 1.1                    */
/*		  by Ron Cain			*/
/*		    (1980)			*/
/************************************************/

findglb(sname)
	char *sname;
{	char *ptr;
	ptr=startglb;
	while(ptr!=glbptr)
		{if(astreq(sname,ptr,namemax))return ptr;
		ptr=ptr+symsiz;
		}
	return 0;
}

findloc(sname)
	char *sname;
{	char *ptr;
	ptr=startloc;
	while(ptr!=locptr)
		{if(astreq(sname,ptr,namemax))return ptr;
		ptr=ptr+symsiz;
		}
	return 0;
}

addglb(sname,id,typ,value)
	char *sname,id,typ;
	int value;
{	char *ptr;
	if(cptr=findglb(sname))return cptr;
	if(glbptr>=endglb)
		{error("global symbol table overflow");
		return 0;
		}
	cptr=ptr=glbptr;
	while(an(*ptr++ = *sname++));	/* copy name */
	cptr[ident]=id;
	cptr[type]=typ;
	cptr[storage]=statik;
	cptr[offset]=value;
	cptr[offset+1]=value>>8;
	glbptr=glbptr+symsiz;
	return cptr;
}

addloc(sname,id,typ,value)
	char *sname,id,typ;
	int value;
{	char *ptr;
	if(cptr=findloc(sname))return cptr;
	if(locptr>=endloc)
		{error("local symbol table overflow");
		return 0;
		}
	cptr=ptr=locptr;
	while(an(*ptr++ = *sname++));	/* copy name */
	cptr[ident]=id;
	cptr[type]=typ;
	cptr[storage]=stkloc;
	cptr[offset]=value;
	cptr[offset+1]=value>>8;
	locptr=locptr+symsiz;
	return cptr;
}

/* Test if next input string is legal symbol name */
symname(sname)
	char *sname;
{	int k;char c;
	blanks();
	if(alpha(chr())==0)return 0;
	k=0;
	while(an(chr()))sname[k++]=gchr();
	sname[k]=0;
	return 1;
	}

/* Return next avail internal label number */
getlabel()
{	return(++nxtlab);
}

/* Print specified number as label */
printlabel(label)
	int label;
{	outstr("cc");
	outdec(label);
}
/* Test if given character is alpha */
alpha(c)
	char c;
{	c=c&127;
	return(((c>='a')&(c<='z'))|
		((c>='A')&(c<='Z'))|
		(c=='_'));
}
/* Test if given character is numeric */
numeric(c)
	char c;
{	c=c&127;
	return((c>='0')&(c<='9'));
}
/* Test if given character is alphanumeric */
an(c)
	char c;
{	return((alpha(c))|(numeric(c)));
}

addwhile(ptr)
	int ptr[];
 {
	int k;
	if (wqptr==wqmax)
		{error("too many active whiles");return;}
	k=0;
	while (k < wqsiz)
		{*wqptr++ = ptr[k++];}
}
delwhile()
	{if(readwhile()) wqptr=wqptr-wqsiz;
	}
readwhile()
 {
	if (wqptr==wq){error("no active whiles");return 0;}
	else return (wqptr-wqsiz);
 }

chr()
{	return(line[lptr]&127);
}
nchr()
{	if(chr()==0)return 0;
		else return(line[lptr+1]&127);
}
gchr()
{	if(chr()==0)return 0;
		else return(line[lptr++]&127);
}
kill()
{	lptr=0;
	line[lptr]=0;
}
inbyte()
{
	while(chr()==0)
		{if (eof) return 0;
		inline2();
		preprocess();
		}
	return gchr();
}
inchar()
{
	if(chr()==0)inline2();
	if(eof)return 0;
	return(gchr());
}
inline2()
{
	int k,unit;
	while(1)
		{if (input==0)openin();
		if(eof)return;
		if((unit=input2)==0)unit=input;
		kill();
		while((k=getc(unit))>0)
			{if((k==eol)|(lptr>=linemax))break;
			line[lptr++]=k;
			}
		line[lptr]=0;	/* append null */
		if(k<=0)
			{fclose(unit);
			if(input2)input2=0;
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

/** ch > chr, gch > gchr, nch > nchr           **/
keepch(c)
	char c;
{	mline[mptr]=c;
	if(mptr < mpmax)mptr++;
	return c;
}

preprocess() /* Process Line in LineBuffer line[k] */
{	int k;
	char c,sname[namesize];
	if(cmode==0)return;
	mptr=lptr=0;
	while(chr())
		{if((chr()==' ')|(chr()==9)) /* filter sp, tab */
			{keepch(' ');
			while((chr()==' ')|
				(chr()==9))
				gchr();
			}
		else if(chr()=='"')         /* filter double quote */
			{keepch(chr());
			gchr();
			while(chr()!='"')
				{if(chr()==0)
				  {error("missing quote");
				  break;
				  }
				keepch(gchr());
				}
			gchr();
			keepch('"');
			}
		else if(chr()==39)          /* filter single quote */
			{keepch(39);
			gchr();
			while(chr()!=39)
				{if(chr()==0)
				  {error("missing apostrophe");
				  break;
				  }
				keepch(gchr());
				}
			gchr();
			keepch(39);
			}
		else if((chr()=='/')&(nchr()=='*')) /* filter bi-glyph, comment start */
			{inchar();inchar();
			while(((chr()=='*')&
				(nchr()=='/'))==0)
				{if(chr()==0)inline2();
					else inchar();
				if(eof)break;
				}
			inchar();inchar();
			}
		else if(an(chr()))  /* CK alpha-numeric */
			{k=0;
			while(an(chr()))
				{if(k < namemax)sname[k++]=chr();
				gchr();
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
		else keepch(gchr());
		}
	keepch(0);
	if(mptr>=mpmax)error("line too long");
	lptr=mptr=0;
	while(line[lptr++]=mline[mptr++]);
	lptr=0;
}

/************************************************/
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
	while (k < len)
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
		while(an(chr())) inbyte();
		return 1;
		}
	return 0;
 }

blanks()
	{while(1)
		{while(chr()==0)
			{inline2();
			preprocess();
			if(eof)break;
			}
		if(chr()==' ')gchr();
		else if(chr()==9)gchr();
		else return;
		}
	}

/**  SCN_III.C **/
/** Next is SCN_IV.C **/
/************************************************/
/** Modified for NASM as SCN_IV.C Small-C, nasm**/
/** Last: 29 JUL 2007 Init: 31 MAR 2007        **/
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.0.3b  Goal: NASM ouput             **/
/**					       **/
/************************************************/
/* ToBe Modified for NASM as SCCN.C --          */
/*  Small-C Compiler - Nasm                     */
/* Last: 15 JUL 2007 Init: 14 JUL 2007          */
/* By: s_dubrovich@yahoo.com                    */
/* Vers: 0.0.1b  Goal: NASM ouput               */
/* Comments:                                    */
/*  This is a new offshoot of development from  */
/*  NCn.C as changes beyond CC8.C are intended. */
/*  NCn.C is Archived, Closed to further        */
/*  development, superceded by this effort SCCN.*/
/* EndComments:                                 */
/** SCCN.C is parked                           **/
/************************************************/
/** New Development Branch                     **/
/************************************************/
/** Re-Codified as:                            **/
/**	I.   Global Storage Defn, main()       **/
/**     II.  Recursive Descent Parser          **/
/**     III. Utility Functions                 **/
/**     IV.  U.I. & I/O                        **/
/**     V.   Backend, NASM Object Output       **/
/************************************************/
/************************************************/
/*						*/
/*		small-c compiler		*/
/*                  rev. 1.1                    */
/*		  by Ron Cain			*/
/*		    (1980)			*/
/************************************************/
/** Fn's in this file **/
/**	ask();		* get user options */
/**	openout();	* get an output file */
/**	openin();	* and initial input file */
/**	header();	* intro code */
/** Logically Parse is here **/
/**	dumplits();	* then dump literal pool */
/** dump global static defs. to output source,  **/
/**  required **/
/**	dumpglbs();	* and all static memory  */
/** original place, added back in **/
/**	errorsummary();	* summarize errors */
/**	trailer();	* follow-up code */
/**	closeout();	* close the output (if any) */
/**	return;		* then exit to system */
/************************************************/
/** doinclude() **/
/************************************************/

/*					*/
/*	Get options from user		*/
/*					*/
ask()
	{
	int k,num[1];
	kill();			/* clear input line */
/**	outbyte(12);	* Form Feed, clear the screen */
	nl();nl();nl();		/* print banner */
	pl("     * * *  small-c compiler  * * *");
	nl();
	pl("              by Ron Cain");
	nl();
	pl("* NASM Vers c0.1.5, Aug. 2008, s_dubrovich@yahoo.com *");
	nl();
	/* see if user wants to interleave the c-text */
	/*	in form of comments (for clarity) */
	pl("Do you want the c-text to appear? (y) ");
	gets(line);		/* get answer */
	ctext=0;		/* assume no */
	if((chr()=='Y')|(chr()=='y'))
		ctext=1;	/* user said yes */
 	/* see if user wants us to allocate static */
 	/*  variables by name in this module	*/
 	/*	(pseudo external capability)	*/
 	pl("Do you wish the globals to be defined? (y) ");
 	gets(line);
 	glbflag=0;
 	if((chr()=='Y')|(chr()=='y'))
 		glbflag=1;	/* user said yes */
	/* get first allowable number for compiler-generated */
 	/*	labels (in case user will append modules) */
 	while(1){
 		pl("Starting number for labels? (0) ");
 		gets(line);
 		if(chr()==0){num[0]=0;break;}
		if(k=number(num))break;
		}
	nxtlab=num[0];
	litlab=getlabel();	/* first label=literal pool */ 
	kill();			/* erase line */
	}

/*					*/
/*	Get output filename		*/
/*					*/
openout()
	{
	kill();			/* erase line */
	output=0;		/* start with none */
	pl("Output filename? "); /* ask...*/
	gets(line);	/* get a filename */
	if(chr()==0)return;	/* none given... */
	if((output=fopen(line,"w"))==NULL) /* if given, open */
		{output=0;	/* can't open */
		error("Open failure!");
		}
	kill();			/* erase line */
}


/*					*/
/*	Get (next) input file		*/
/*					*/
openin()
{
	input=0;		/* none to start with */
	while(input==0){	/* any above 1 allowed */
		kill();		/* clear line */
		if(eof)break;	/* if user said none */
		pl("Input filename? ");
		gets(line);	/* get a name */
		if(chr()==0)
			{eof=1;break;} /* none given... */
		if((input=fopen(line,"r"))==NULL)
			{input=0;	/* can't open it */
			pl("Open failure");
			}
		}
	kill();		/* erase line */
	}

/*					*/
/*	Report errors for user		*/
/*					*/
errorsummary()
	{
	/* see if anything left hanging... */
	if (ncmp) error("missing closing bracket");
		/* open compound statement ... */
	nl();
	comment();
	outdec(errcnt);	/* total # errors */
	outstr(" errors in compilation.");
	nl();
	}

/*					*/
/*	Close the output file		*/
/*					*/
closeout()
{
	if(output)fclose(output); /* if open, close it */
	output=0;		/* mark as closed */
}


/*					*/
/*	Dump the literal pool		*/
/*					*/
dumplits()
	{int j,k;
	if (litptr==0) return;	/* if nothing there, exit...*/

	do_dseg();     /** in SCCN8.C for section .dseg **/
	nl();

	printlabel(litlab);col(); /* print literal label */
	k=0;			/* init an index... */
	while (k < litptr)	/* 	to loop with */
		{defbyte();	/* pseudo-op to define byte */
		j=10;		/* max bytes per line */
		while(j--)
			{outdec((litq[k++]&127));
			if ((j==0) | (k>=litptr))
				{nl();		/* need <cr> */
				break;
				}
			outbyte(',');	/* separate bytes */
			}
		}
	}

/*					*/
/*	Dump all static variables	*/
/*					*/
/** these are put to outfile, required **/
/** for nasm DSegment data ref. defs.  **/
/** Form; {Name,TIMES,numb,DB 0}       **/

dumpglbs()
	{
	int j;
	if(glbflag==0)return;	/* don't if user said no */
/** do SECTION here as well, there may not be lits, above **/
	do_dseg();     /** in SCCN8.C for section .dseg **/
	nl();	/** do in dumplits, it is first **/
	cptr=startglb;
	while(cptr < glbptr)
		{if(cptr[ident]!=function)
			/* do if anything but function */
			{outstr(cptr);col();
				/* output name as label... */
			defstorage();	/* define storage, Times */
			j=((cptr[offset]&255)+((cptr[offset+1]&255)<<8));
				/* calc # bytes lo,hi */
/**	why?		if((cptr[type]==cint)|(cptr[ident]==pointer)) **/
/**	why?			j=j+1;					**/
			outdec(j);	/* need that many */
			if((cptr[type]==cint)|(cptr[ident]==pointer))
				defword();
				else defbyte();
			ot("0"); /** filler value **/
			nl();
			}
		cptr=cptr+symsiz;  /** inc thru table **/
		}
	}

/************************************************/

/*					*/
/*	Open an include file		*/
/*					*/
doinclude()
{
	blanks();	/* skip over to name */
	if((input2=fopen(line+lptr,"r"))==NULL)
		{input2=0;
		error("Open failure on include file");
		}
	kill();		/* clear rest of line */
			/* so next read will come from */
			/* new file (if open */
}

/************************************************/

illname()
{	error("illegal symbol name");junk();}

multidef(sname)
	char *sname;
{	error("already defined");
	comment();
	outstr(sname);nl();
}

needbrack(str)
	char *str;
{	if (match(str)==0)
		{error("missing bracket");
		comment();outstr(str);nl();
		}
}

needlval()
{	error("must be lvalue");
}

/************************************************/
/* Print a carriage return and a string only to console */
pl(str)
	char *str;
{	int k;
	k=0;
	putchar(eol);
	while(str[k])putchar(str[k++]);
}


/************************************************/
/** putc <- outbyte <- nl, outstr <- ol        **/
/** Note: null, eof, cause closeout.           **/

outbyte(c)  /** Note usage: outbyte must return null or chr **/
	char c;
{
	if(c==0)return 0; /** don't pass null terminator in **/
	if(output)  /** output is file fp_ **/
		{
		if((putc(c,output))<=0)
			{
			closeout();
			error("Output file error");
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
  /** Note usage: outbyte must return null or chr **/
	while(outbyte(ptr[k++]));
}

/** was nl() {outbyte(eol);} **/
nl()	{outbyte(13); outbyte(10);}

tab()	{outbyte(9);}

col()	{outbyte(58);} /** colon **/

error(ptr)
	char ptr[];
{
	int k;
	comment();outstr(line);nl();comment();
	k=0;
	while(k < lptr)
		{if(line[k]==9) tab();
			else outbyte(' ');
			++k;
			}
	outbyte('^');
	nl();comment();outstr("******  ");
	outstr(ptr);
	outstr("  ******");
	nl();
	++errcnt;
}

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
	outstr(ptr);	/*outasm*/
}

/************************************************/

outdec(number)
	int number;
 {
	int k,zs;
	char c;
	zs = 0;
	k=10000;
	if (number < 0)
		{number=(-number);
		outbyte('-');
		}
	while (k>=1)
		{
		c=number/k + '0';
		if ((c!='0')|(k==1)|(zs))
			{zs=1;outbyte(c);}
		number=number%k;
		k=k/10;
		}
 }


/**  SCN_IV.C **/
/** Next is SCN_V.C **/
/************************************************/
/** Modified for NASM as SCN_V.C Small-C, nasm **/
/** Last: 26 APR 2008				*/
/** Prev: 29 JUL 2007 Init: 31 MAR 2007        **/
/** By: s_dubrovich@yahoo.com                  **/
/** Vers: 0.0.4b  Goal: NASM ouput             **/
/**					       **/
/************************************************/
/* ToBe Modified for NASM as SCCN.C --          */
/*  Small-C Compiler - Nasm                     */
/* Last: 15 JUL 2007 Init: 14 JUL 2007          */
/* By: s_dubrovich@yahoo.com                    */
/* Vers: 0.0.1b  Goal: NASM ouput               */
/* Comments:                                    */
/*  This is a new offshoot of development from  */
/*  NCn.C as changes beyond CC8.C are intended. */
/*  NCn.C is Archived, Closed to further        */
/*  development, superceded by this effort SCCN.*/
/* EndComments:                                 */
/** SCCN.C is parked                           **/
/************************************************/
/** New Development Branch                     **/
/************************************************/
/** Re-Codified as:                            **/
/**	I.   Global Storage Defn, main()       **/
/**     II.  Recursive Descent Parser          **/
/**     III. Utility Functions                 **/
/**     IV.  U.I. & I/O                        **/
/**     V.   Backend, NASM Object Output       **/
/************************************************/
/************************************************/
/*						*/
/*		small-c compiler		*/
/*                  rev. 1.1                    */
/*		  by Ron Cain			*/
/*		    (1980)			*/
/************************************************/
/** Functions in this file:			*/
/** comment, header, outhexb, trailer, getmem  **/
/** getloc, putmem, putstk, indirect, swap,	*/
/** immed, m_push, m_push_zr, m_pop, swapstk	*/
/** m_call, m_ret, callstk, jump, testjump,	*/
/** do_seg, defbyte, defstorage, defword, modstk*/
/** doublereg, m_add, m_sub, m_mult, m_div, mod */
/** m_or, m_xor, m_and, asr, asl, m_neg, com   **/
/** m_inc, m_dec, eq, ne, lt, le, gt, ult, ule **/
/** ugt, uge					*/
/**  SCN_V.C **/
/*       1         2         3         4         5   */
/*	>>>>>> start of cc8 <<<<<<<	*/
/************************************************/
/** Module: SCCN8.C                            **/
/** Last: 23 DEC 2007  Init: 14 JUL 2007       **/
/** Vers: 0.0.2b  Goal: Self Compile, Nasm out **/
/** Modified for NASM by s_dubrovich@yahoo.com **/
/************************************************/
/** Nasm Text Output Begins in This Module     **/
/** Primary: 8080.HL 8086.BX, Static Cell is DSEG **/
/** Secondary: 8080.DE 8086.DX                    **/
/**  -made 2 versions, SCCN8.C & SCCNT8.C test    **/
/** - appx A 6.1 char are conv to ints, which  **/
/**   are signed values. K&R C.                **/
/** 8:47 PM 7/27/2007 -Changes Primary = AX,	**/
/** Secondary = BX, chg due to sign extend AL	**/
/** 10:34 PM 12/23/2007 --                      **/
/** -Fixed 'indirect' mov prim,[prim]           **/

/* Begin a comment line for the assembler */
comment()
{	outbyte(';');
}
/*       1         2         3         4         5   */
/* Print all assembler info before any code          */
/*  is generated.                                    */

header()
{
	comment();
	outstr("Small-c Compiler rev 1.1 re:NASM c0.1.5 Aug 2008");
	nl();
	ol("[MAP ALL SCCN.MAP]");
/**	ol("SECTION .cseg vstart=0");	**/
	ol("SECTION .text vstart=0100h"); /*** .com vers. ***/
	nl();
}
/***------------------------------------------***/

/***------------------------------------------***/

outhexw(number)	int number;
 {
	leaderx();
	hex(number);
 }

hex(x) int x;
{
	hexb(x>>8);
	hexb(x);
}

hexb(x) int x;
{
	hexn(x>>4);
	hexn(x);
}

hexn(x) int x;
{
	x=x&15;
	if(x < 10) putchar(x+'0');
	else putchar(x-10+'A');
} /** EO outhexw **/

/***------------------------------------------***/

outhexb(number) int number;
{
	leaderx();
	hexb(number & 255);
} /** EO outhexb **/

/***------------------------------------------***/

leaderx()
{
/**	ot("0x"); **/
	putchar('0');
	putchar('x');
}  /** EO leaderx() **/

/***------------------------------------------***/


/***------------------------------------------***/
/* Print any assembler stuff needed after all code */
trailer()
{	comment();
	ol("-= END =-");
}

/***------------------------------------------***/
/* Fetch a static memory cell into the primary register */
/** Primary 8080.HL 8086.AX, Static Cell is DSEG **/
/** If byte, then affirm it is sign extended to wordsz **/
getmem(sym)
	char *sym;
{	if((sym[ident]!=pointer)&(sym[type]==cchar))
		{ot("MOV  AL,[");
		outstr(sym+name);
		ot("]");
		nl();
	/** sign extension **/
		ol("CBW");
		}
	else
		{ot("MOV  AX,[");
		outstr(sym+name);
		ot("]");
		nl();
		}
	}

/* Fetch the address of the specified symbol */
/** the Stack Address of the passed Parameter, **/
/** place that reference into the Primary Cell **/
getloc(sym)
	char *sym;
{	immed();
	outdec(((sym[offset]&255)+
		((sym[offset+1]&255)<<8))-
		c_sp);
	nl();
	ol("ADD  AX,SP");
	}

/* Store the primary register into the specified */
/*	static memory cell */
putmem(sym)
	char *sym;
{	if((sym[ident]!=pointer)&(sym[type]==cchar))
		{ot("MOV  [");
		outstr(sym+name);
		ot("],AL");nl();
		}
	else 
		{ot("MOV  [");
		outstr(sym+name);
		ot("],AX");
		nl();
		}
	}

/* Store the specified object type in the primary register */
/*	at the address on the top of the stack */
putstk(typeobj)
	char typeobj;
/**{pop();if(typeobj==cchar)call("ccpchar");else call("ccpint");}**/
{	mnem_pop();	/** BX <- Addr.TOS **/
			/** Effective Address must be in BX **/
	if(typeobj==cchar) ol("MOV  [BX],AL");
		else ol("MOV  [BX],AX");
}

/* Fetch the specified object type indirect through the */
/*	primary register into the primary register */
indirect(typeobj)
	char typeobj;
/**{if (typeobj==cchar)call("ccgchar");else call("ccgint");}**/
{	if (typeobj==cchar) 
		{ol("MOV  BX,AX");
		ol("MOV  AL,[Byte BX]");
		ol("CBW");
		}
		else {  ol("MOV  BX,AX");
			ol("MOV  AX,[BX]");
			}
}

/* Swap the primary and secondary registers */
swap()
{	ol("XCHG AX,BX");
}

/* Print partial instruction to get an immediate value */
/*	into the primary register */
immed()
{	ot("MOV  AX,");
}

/* Push the primary register onto the stack */
/** 24 callers **/
mnem_push()
{	ol("PUSH AX");
	c_sp=c_sp-2;
}

/** Push Zero onto the stack		**/
/** 10:30 AM 8/5/2007			**/
/** unlinked from callfunction() 11:54 AM 8/5/2007 **/
m_push_zr()
{	ol("PUSH 0");
	c_sp=c_sp-2;
}

/* Pop the top of the stack into the secondary register */
/** 17 callers **/
mnem_pop()
{	ol("POP  BX");
	c_sp=c_sp+2;
}

/* Swap the primary register and the top of the stack */
swapstk()
{	ol("XCHG [SP],AX");
}

/* Call the specified subroutine name */
/** 1 caller **/
mnem_call(sname)
	char *sname;
{	ot("CALL ");
	outstr(sname);
	nl();
}

/* Return from subroutine */
/** 2 callers **/
mnem_ret()
{	ol("RET");
}

/* Perform subroutine call to value on top of stack */
/** one caller **/
mnemcallstk()
{	immed();
	outstr("$+5");
	nl();
	swapstk();
	ol("CALL [AX]");
	c_sp=c_sp+2;
	}

/* Jump to specified internal label number */
/** 4 Callers **/
mnem_jump(label)
	int label;
{	ot("JMP  ");
	printlabel(label);
	nl();
	}

/* Test the primary register and jump if false to label */
/** Primary holds Bool on entry 1=TRUE, 0=FALSE the jump **/
/** often is not short, so change this... **/
/** testjump(label)int label;{ol("MOV  CX,AX"); **/
/** ot("JCXZ ");printlabel(label);nl();} **/

testjump(label)
	int label;
{	ol("CMP  AX,0"); /** ZF if equal **/
	ol("JNE  $+5"); /** next is past, jmp label **/
	ot("JMP  ");
	printlabel(label);
	nl();
}

/** .dseg section **/
do_dseg()
{
  ot(";-; SECTION .dseg align=16 ;; commented out for .com version");
}


/* Print pseduo-op to define a byte */
defbyte()
{	ot("DB ");
}

/* Print psedo-op to define storage */
/** Called by CC1 dumpglbs(), both will need modifications **/
/**   form: times nn d_sz val **/
defstorage()
{	ot("TIMES "); /**ot("DS ");**/
}

/* Print pseudo-op to define a word */
defword()
{	ot("DW ");
}

/* Modify the stack pointer to the new value indicated */
/** 9 callers, pertains to local stack index **/
mnem_modstk(newsp)
	int newsp;
{	int k;
	k=newsp-c_sp;
	if(k==0)return newsp;
	if(k>+0)
		{
		ot("ADD  SP,");
		outdec(k);
		nl();
		return newsp;
		}
	if(k < 0)
		{if(k>-7)
			{if(k&1)
				{ol("DEC  SP");
				k++;
				}
			while(k)
				{ol("PUSH AX");
				k=k+2;
				}
			return newsp;
			}
		}

/* 2 Modify the stack pointer to the new value indicated */
/** 9 callers, pertains to local stack index **/
/* mnem_modstk(newsp)int newsp;{int k;k=newsp-c_sp; **/
/* if(k==0)return newsp;if(k>+0){if(k<7){if(k&1)**/
/** handle odd alignment **/
/* {ol("INC  SP");k--;}while(k)**/
/** throw away **/
/** without overwriting primary */
/* {ol("POP  DX");k=k-2;}return newsp;}} **/
/* if(k<0){if(k>-7){if(k&1){ol("DEC  SP");k++;}while(k){ol("PUSH AX"); **/
/* k=k+2;}return newsp;}} **/

/* Modify the stack pointer to the new value indicated */
/* modstk(newsp)int newsp;{int k;k=newsp-Zsp; **/
/* if(k==0)return newsp;ot("ADD SP,");outdec(k); **/
/* nl();return newsp;} **/

/** Have a closer look at the following for improvement **/
	swap();
	immed();outdec(k);nl();
	ol("ADD  AX,SP");
	ol("MOV  SP,AX");
	swap();
	return newsp;
}

/* Double the primary register */
doublereg()
{	ol("ADD  AX,AX");
}

/* Add the primary and secondary registers */
/*	(results in primary) */
m_add()
{	ol("ADD  AX,BX");
}

/* Substract the primary register from the secondary */
/*	(results in the primary) */
/** sub() {call("ccsub");} **/
m_sub()
{	ol("SUB  BX,AX"); /** BX-AX -> BX **/
	ol("MOV  AX,BX");
}

/* Multiply the primary and secondary registers */
/*	(results in primary) */
/** mult() {call("ccmult");} **/
/** Unsigned Mult re:AX*BX -> DX:AX **/
m_mult()
{	
	ol("MUL  BX");
	
}

/* Divide the secondary by the primary */
/* (quotient in primary, remainder in secondary) */
/*** 21-Jul-08 07:41:16 PM add signed integer divide ***/
/**** 24-Jul-08 02:51:10 PM force DX := 0, prevent exception ****/
/** div() {call("ccdiv");} **/
/** swap Primary&Secondary **/
/** Unsigned Div re:DX.0:AX/BX -> AX.quot DX.rem **/
/** mov remainder into Secondary		**/
m_div()
{	ol("XCHG AX,BX");
	ol("MOV  DX,0");	/** clear extended reg DX **/
	ol("DIV  BX");	/** DX has remainder **/
	ol("MOV  BX,DX");
}

m_sdiv()
{	ol("XCHG AX,BX");
	ol("CWD");		/** sign extend into DX **/
	ol("IDIV BX");	/** DX has remainder **/
	ol("MOV  BX,DX");
}

/* Compute remainder (mod) of secondary register divided */
/*	by the primary */
/*	(remainder in primary, quotient in secondary) */
mod()
{	m_div();
	swap();
	}

/* Inclusive 'or' the primary and secondary registers */
/*	(results in the primary) */
/** or() {call("ccor");} **/
m_or()
{	ol("OR   AX,BX");
}

/* Exclusive 'or' the primary and secondary registers */
/*	(results in the primary) */
/** xor() {call("ccxor");} **/
m_xor()
{	ol("XOR  AX,BX");
}

/* 'And' the primary and secondary registers */
/*	(results in primary) */
/** and() {call("ccand");} **/
m_and()
{	ol("AND  AX,BX");
}

/* Arithmetic shift right the secondary register number of */
/*	times in the primary (results in primary) */
/** asr() {call("ccasr");} **/
asr()
{	ol("MOV  CL,AL"); /** BH is disregarded **/
	ol("MOV  AX,BX");
	ol("SAR  AX,CL");
}

/* Arithmetic left shift the secondary register number of */
/*	times in the primary (results in primary) */
/** asl() {call("ccasl");} **/
asl()
{	ol("MOV  CL,AL"); /** BH is disregarded **/
	ol("MOV  AX,BX");
	ol("SAL  AX,CL");
}

/* Form two's complement of primary register */
/** neg() {call("ccneg");} **/
m_neg()
{	ol("NEG  AX");
}

/* Form one's complement of primary register */
/** com() {call("cccom");} **/
com()
{	ol("NOT  AX");
}

/* Increment the primary register by one */
/** inc() {ol("INX H");} **/
m_inc()
{	ol("INC  AX");
}

/* Decrement the primary register by one */
/** dec() {ol("DCX H");} **/
m_dec()
{	ol("DEC  AX");
}

/* Following are the conditional operators */
/* They compare the secondary register against the primary */
/* and put a literal 1 in the primary if the condition is */
/* true, otherwise they clear the primary register */
/** Here for NASM, I've inline2d these fns instead of	**/
/**  leaving them for an include *.* 			**/

/* Test for equal */
eq()	/** {call("cceq");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JZ   $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for not equal */
ne()	/** {call("ccne");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JNZ  $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for less than (signed) */
lt()	/** {call("cclt");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JL   $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for less than or equal to (signed) */
le()	/** {call("ccle");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JLE  $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for greater than (signed) */
gt()	/** {call("ccgt");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JG   $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for greater than or equal to (signed) */
ge()	/** {call("ccge");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JGE  $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for less than (unsigned) */
ult()	/** {call("ccult");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JB   $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for less than or equal to (unsigned) */
ule()	/** {call("ccule");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JBE  $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for greater than (unsigned) */
ugt()	/** {call("ccugt");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JA   $+5");
	ol("MOV  AX,0");   /** is false **/
}

/* Test for greater than or equal to (unsigned) */
uge()	/** {call("ccuge");} **/
{	ol("CMP  BX,AX");  /** sets flags **/
	ol("MOV  AX,1");   /** assume true **/
	ol("JAE  $+5");
	ol("MOV  AX,0");   /** is false **/
}
/***------------------------------------------***/
/**-------------------------------------**/
/** extern functions needed by small-c: **/
/** fopen()				**/
/** fclose()				**/
/** putchar()				**/
/** getc()				**/
/** putc()				**/
/** gets()				**/
/**	Example..			**/
/** CALL gets | gets(line);		**/
/** CALL fopen | fopen(line,"w")	**/
/** CALL fclose | fclose(output);	**/
/*	<<<<< End of Compiler >>>>>	 */
/* #include IOC5.C * when self compiling */
/* Otherwise, the PowerC pcdos io lib is */
/* used: stdio.h			**/
/** SCNXa.C is PowerC Level 1 Compile	**/
/** SCNX.C is IOC5.C Level 2 Self-Compile*/
/** #include IOC5.C ** for self-compile **/
/** Name changed to...			**/
/**#include C5LIB.C  for self-compile	**/
/** -======== End of Compiler ========- **/
