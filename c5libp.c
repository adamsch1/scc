/*********************************************************/
/** Module: C5LIBp.C  By: s_dubrovich@yahoo.com	        **/
/** Last: 8:00 AM 8/16/2008                             **/
/** Vers: 0.2.3	-set to pause for users press anykey    **/
/**  modified puts();					**/
/*********************************************************/
/** Module: C5LIB.C  By: s_dubrovich@yahoo.com          **/
/** Last:  10:29 PM 8/14/2008                           **/
/** Vers: 0.2.2                                         **/
/**  Chg to C5LIB.C for further development.  Added in  **/
/**  mainmain() for initialization of user source.      **/
/**  file Was IOC5.C                                    **/
/**  Added puts();                                      **/
/*********************************************************/
/** IOC5.C -- stripped main for small-c include         **/
/** Call 5 IO Interface Library                         **/
/*********************************************************/
/** Last: 30-Jul-08 05:08:51 PM.                        **/
/** Prev: 08:15:42 AM 17-Jul-08                         **/
/** Vers: 0.1.7	modified re_ubuff()                     **/
/** chg index update for ol(ptr)                        **/
/*********************************************************/
/** Last: 07:32:44 AM 15-Jul-08                         **/
/** Vers: 0.1.5 -filter 0Dh -> 20h for eol problem    ****/
/**  chgd getc() to filter & 127, or, ret EOF         ****/
/*********************************************************/
/** Last: 05:23:25 AM 07/13/2008 01:31:05 PM 07/12/2008 **/
/** Init: 11:18 PM 7/5/2008                             **/
/*********************************************************/
/** CALL5_IO.C test file & IO Functions for small-c     **/
/** Last: 11:44 AM 7/5/2008 :: 11:28 AM 7/4/2008        **/
/** Init: 7:24 AM 5/31/2008                             **/
/** Vers: 0.1.0  By: s_dubrovich@yahoo.com              **/
/** Goal: minimal IO to Call_5 interface to run small-c **/
/** Note: Only Character, not binary Files.             **/
/** -Cloned From SINKIO01.C as IO LIB base Fn's         **/
/** -includes some testing code as main()               **/
/*********************************************************/
/** NOTE: for cmd.exe edit NASM src for vstart=0100h,   **/
/** and comment out ';;' the two SECTION .dseg align=16 **/
/** directives so that the pcdos 'small' model is used. **/
/*********************************************************/
/** Req'd IO Functions for Small-c                      **/
/**getc(){;} gets(){;} putc(){;}                        **/
/**putchar(){;} fopen(){;} fclose(){;}                  **/

/*
** STDIO.H -- Standard Small-C Definitions -- 
*/
#define stdin    0
#define stdout   1
#define stderr   2
#define ERR   (-2)
#define EOF   (-1)
#define YES      1
#define NO       0
/** #define NULL     0 **/
#define BAD	(-1)
#define CR      13
#define LF      10
#define BELL     7
#define SPACE  ' '
#define NEWLINE LF

#define EOL 10   /** LF **/
#define NL  10
#define TAB  9
#define EOL2    13 /*;end-of-line character          */
                   /*  (=carriage return,CR)         */
#define CTRLZ	26 /** 1Ah, Ctrl-Z, cpm textfile EOF */
/**-----------------------------------------------------**/
#define MAX_IOD   6 /** Max Streams, incl stdin,out,err **/
#define FILE  char /** Cain's C doesn't know struc FILE **/
#define VERBOSE   1 /** debugging, report runtime state **/
/**-----------------------------------------------------**/
/** #define PCDOS 1 * IBM PC DOS, Same as CP/M-80   */
/** #if PCDOS		*****************************/
#define EXIT	   0	/* Exit to BDOS		    */
#define CONIN	   1	/* direct echoing con input */
#define CONOUT	   2	/* Direct console output    */
#define LSTOUT	   5	/* Direct list device output*/
#define CONIO	   6	/* Direct console I/O	    */
#define C_WRITESTR 9	/* Console string output    */
#define CONBUF	   10	/* Read console buffer	    */
#define C_STAT	   11	/* Get console status	    */
#define OPEN	   15	/* OPEN a disk file	    */
#define CLOSE	   16	/* Close a disk file	    */
#define SEARCHF	   17	/* Search for first	    */
#define SEARCHN	   18	/* Search for next	    */
#define DELETE	   19	/* Delete a disk file	    */
#define SEQREAD    20	/* seq read		    */
#define SEQWRITE   21	/* seq write		    */
#define CREATE	   22	/* Create a disk file	    */
#define F_RENAME   23	/* Rename a disk file	    */
#define SETDMA	   26	/* Set DMA address	    */
#define B_READ	   33	/* Read Random record	    */
#define B_WRITE	   34	/* Write Random record	    */
#define FILSIZ	   35	/* Compute File Size	    */
/** #endif		*****************************/

/***	File State  bit definitions                       ***/
#define STATE_FR 0	/* Available State, FREE CCB        */
#define STATE_FO 1	/* else File Opened                 */
#define STATE_DT 2	/* at least 1 rec put to file, ndx0 */
#define STATE_PW 4	/* Pending Write, Flush before close*/
#define STATE_EF 8	/* EOF state on last read	    */
#define STATE_ER BAD	/* Error State                      */

/***	File Mode, Character String                         */
#define MODE_RD	'R'	/* Read mode parameter for open     */
#define MODE_WR	'W'	/* Write mode                       */
#define MODE_NU	'N'	/* Null Mode for reuse              */ 

/*	CP/M FCB definition - similar to PC Dos	*/
#define EOFCHR	26	/* End of file character-^Z, 0x1a   */
				/****************************/
#define SECSIZ		128	/* size of CP/M sectorrecord*/
#define _MAXSXFR	1	/* max # sectors xferrable  */
#define _MAXSHFT	12	/* shift right BDOS rtn val */
				/*   to obtain nsecs on err */
				/****************************/
#define BYTE	char
#define INT	int
#define WORD	int

/********************************************************************
 *	Channel Control Block (CCB), _iod, I/O Descriptor.
 *									    
 * One CCB is allocated (statically) for each of the possible open  
 * files under C (including STDIN, STDOUT, STDERR).  Permanent data    
 * regarding the channel is kept here.				    
 *	-= note: initially. use 6 CCB's, adequate for small-c =-
 ********************************************************************/
  /** Define File Descriptor Indexes - _IOD **/
#define DRV	0
#define FNAME	1
#define FTYP	9
#define FBLK	12 /* current 16k Block */
#define LRS	14 /* Logical Record Size, 0080h */
#define FSZ	16 /* FileSize in bytes */
#define FDATE	20 /* Date */
#define FTCHG	22 /* Time of Last Change */
#define FDCHG	24 /* Date of Last Change */
#define FCLUS	26 /* First Cluster Number for FAT Entry */
#define FSIZE	28 /* File Size 28..31 */
#define FCREC	32 /* Current Record in current 16k Block */
#define FR0	33 /* lo byte of Random Rec Number */
#define FR1	34 /* mid byte of Random Rec Number */
#define FR2	35 /* hi byte of Random Rec Number */
#define FR3	36 /* Overflo byte under certain circumstances */
  /** -= File Operational Elements of Struct =- **/
#define IOTYP   37 /* TYPES: 'S'torage(file), 'M'emory(file) **/
#define FMODE	38 /* File Mode Indication Bytes [3] ex.'wb+' **/
#define FSTATE	41 /* Current Operational State Byte **/
#define FBUFNDX 42 /* Buffer index DW to next free position, 0..127 **/
#define FBUFFER	44 /* File Transfer Buffer **/ 
#define LENFDD	172 /* Next Position after FCB Structure, */
		/** Length of File Descriptor Area **/
#define LEN_Y	171 /** LENFDD - 1, for addressing f_units **/
/**-------------------------------------------------------------*/
/*** -= DECLARATIONS =- ***/
/**-------------------------------------------------------------*/

/** FCB STUCTURES ARRAY AREA ***** CCB's	**/
/** First three are STDIN, STDOUT, STDERR	**/

	int  *_iod[MAX_IOD]; /** holds addrs of f_units at runtime **/

  /** File Descriptor Area **/
	char f_unit0;	/** Location is DRV in struct **/
	char fdd_01[LEN_Y];  /** one less than LENFDD **/

	char f_unit1;
	char fdd_02[LEN_Y];

	char f_unit2;
	char fdd_03[LEN_Y];

	char f_unit3;
	char fdd_04[LEN_Y];

	char f_unit4;
	char fdd_05[LEN_Y];

	char f_unit5;
	char fdd_06[LEN_Y];


	char fdta[128]; /** default fp buff **/
  /** TBUFF Area for CONIO *********************/
	char tbuff[257];
	char ubuff[257];
	char cp[];

/*       1         2         3         4         5   */
/*****************************************************/
/* Module: OSFN.C				     */
/* Last:  12:10 PM 4/12/2008                         */
/* Init:  9:28 PM 12/28/2007                         */
/* By:  s_dubrovich@yahoo.com                        */
/* Goal: test module for small-c & nasm              */
/* testb.c -  to demo "SMALLC-NASM"                  */
/* -f bin -l testb.lst -o testb.com testb.nsm        */
/* #asm                                              */
/* ;-;	org 0100h ;; for .com locate main()	     */
/* ;-; Doesn't work, for pcDos, edit SECTION .text   */
/* ;-;  to vstart=0100h                              */
/* #endasm                                           */

/***-----------------------------------------------***/
/***                   M A I N                     ***/
/***-----------------------------------------------***/
/*** Main Block, testing File I/O Functions.  Also, **/
/** provides startup initialization for FCB's for   **/
/** User codes' main() -this depends on including   **/
/**  #include C5LIB.C ** first in users's source    **/
/**  in order for mainmain() to execute first. If   **/
/**  included elsewhere in user's source then this  **/
/**  mainmain() doesn't execute, but the lib fn's   **/
/**  are still available and the user must explicit- */
/*   ly call initfcb() first before file operations. */

mainmain()
{
	initfcb(); /** in this file **/
	main();    /** call users main **/
	exit(0);	/** user's main returns, handle exit */
}
/** eo main **/

/***-----------------------------------------------***/
/***             S U B R O U T I N E S             ***/
/***-----------------------------------------------***/

/*** codify EOL sequence...			   ***/
NLfn()
{
	putchar(CR); putchar(LF);
}

/* Test if given character is alpha - internal FN !? */
/****---- alpha(c)
	char c;
{	c=c&127;
	return(((c>='a')&(c<='z'))|
		((c>='A')&(c<='Z'))|
		(c=='_'));
} ----****/


/***-----------------------------------------------***/
/** Test for Printable Character, EOF		---***/
/**  ret bool := 1 if not a printable char, else 0 ***/
/**  ret`0 .if. e|{' '..'~'} .else. 1		   ***/

isnprint(c) char c;
{
	if ((c>=' ')&(c<='~')) return 0;
	else return 1;
}

/***-----------------------------------------------***/
toupper(c) char c;
{
	if (c) /** not null **/
		{
		if ((c>='a')&(c<='z')) return (c-32);
		else return (c);
		}
	return (c);
}

/***-----------------------------------------------***/
/*** put a char array of len length to conout.	---***/
putchara(str,len) char str[]; int len;
{
	int cnt, ix;
	cnt = len;
	ix  = 0;
	while (len)
		{
		putchar(str[ix++]);
		len--;
		}
}

/***-----------------------------------------------***/
/*** to Upper Case conversion of string 'str$'     ***/

uc_str(pstr) char *pstr;
{ /** takes a ptr to a '$' terminated string **/
	char chr, *ip;

	while (*pstr != '$')
		{
		chr=toupper(*pstr++);
		putchar(chr);
		*ip++ = chr; /** ip++; **/
		} /** str conversion **/
}

/***-----------------------------------------------***/
/*** to Upper Case conversion of string 'str\0'    ***/

uc_nstr(pstr) char *pstr;
{ /** takes a ptr to a null terminated string **/
	char chr, *ip;

	while (*pstr)
		{
		chr=toupper(*pstr++);
		putchar(chr);
		*ip++ = chr; /** ip++; **/
		} /** str conversion **/
}

/***-----------------------------------------------***/
/*** NOTE- limited function, copy filespec 11 chars***/

copystr(str1, str2) char str1[], str2[]; 
{
	int i,cnt;
	cnt = 11;
	i   = 0;
	while (cnt)
		{
		str2[i] = str1[i];
		cnt--;
		i++;
		}
}

/***-----------------------------------------------***/
copymem(blk1, blk2, ccnt) char blk1[], blk2[]; int ccnt; 
{
	int i;
	i   = 0;
	while (ccnt)
		{
		blk2[i] = blk1[i];
		ccnt--;
		i++;
		}
}

/***-----------------------------------------------***/
/*** Div DW as DD`DX:AX -> AX`quot,DX`rem.  Word   ***/
/** value is passed on the stack.                  ***/
/** Use Division by Subtraction to avoid ovfl      ***/
/*** Status: works ok, wordsz => 0..max`65535      ***/


outdeci(number) int number;
{
#asm
;;outdeci(number)int number;{int k,zs;char c;zs = 0;
;; k=10000;/**	if (number<0){number=(-number);
;; outbyte('-');}***/while (k>=1){c=number/k + '0';
;; if ((c!='0')|(k==1)|(zs)){zs=1;putchar(c);}
;; number=number%k;k=k/10;} }

	mov  dx,0
	pop  di		;; 0[ret]2[val]
	pop  ax
	push ax		;; caller cleans the stack
	push di		;; restore return address.
	pusha		;; save entry state...

	mov  cx,0
	mov  bx,10000
	call Lnnd
	mov  cx,0
	mov  bx,1000
	call Lnnd
	mov  cx,0
	mov  bx,100
	call Lnnd
	mov  cx,0
	mov  bx,10
	call Lnnd
	mov  cx,0
	mov  bx,1
	call Lnnd
  ;; Cleanup Stack, and return..
	popa
	jmp  Cont
Lnnd:
	cmp  ax,bx
	jb   .Lnxt1	;; less than 10k, else count.
	sub  ax,bx
	inc  cx
	jmp  Lnnd
.Lnxt1:
	add  cx,30h ;; make ascii number
	pusha
	push cx
	call putchar
	pop  cx
	popa
	RET
Cont:

#endasm
}

/***-----------------------------------------------***/
/** ----------= S I M P L E   I / O =-------------- **/
/***-----------------------------------------------***/
/** define putchar() putc(c,stdout)                 **/
/** -= Simple I/O =- **/
putchar(c)
 char c;
{
	OSfn_(CONOUT,c);
/**	if(c==CR)		**/
/**	OSfn_(CONOUT,LF);	**/
/**	return(c & 255);	**/
}

/***-----------------------------------------------***/
/** get buffered input from stdin, CONBUF #10       **/
/** 1st byte of tbuff on call holds max_len,1..255  **/
/** 2nd byte is set to string length inc terminator **/
/** byte CR`0Dh.  Buffer must be preset before call **/
/** Users returned string begins at third byte.     **/

/** Call 5 interface for internal use. **/
c5gets(ubuff) char *ubuff;  
{  /** use temp buffer with copy to userbuffer **/
	char *ip,*pb,chr;

	tbuff[0]=255;  /** use default max **/
	ip = &tbuff[1];
	pb = &tbuff[2];
	OSfn_(CONBUF,&tbuff[0]); /** Fn 10, DS:DX **/
  /** copy string part to users buffer **/
	while (*pb != 13)
		{
		chr = *pb++;
		*ubuff++ = chr;
		}
  /* chgs termination from CR to '$' */
	*ubuff = '$';
	return (*ip);
}

/***-----------------------------------------------***/
/**** Note: gets() for C functionality, append null ****/
/**** Rets- caller's buffer ptr, or NULL if eof or err */

gets(ubuff) char *ubuff;  /** CONIN Fn **/
{  /** use temp buffer with copy to userbuffer **/
	char *ip,*pb, *tmpp, chr;

	tmpp = ubuff;  /** local copy **/
	tbuff[0]=255;  /** use default max inbuffsz **/
	ip = &tbuff[1]; /** fn10 sets to strlen entered **/
	pb = &tbuff[2]; /** entered str start **/
	OSfn_(CONBUF,&tbuff[0]); /** Fn 10, DS:DX struct **/

	if (0 == *ip) return (NULL); /** empty line **/
  /** copy string part to users buffer **/
	while (*pb != 13) /** input CR terminated **/
		{
		chr = *pb++;
		*tmpp++ = chr;
		}
  /* chgs termination from CR to C's '0' */
	*tmpp = 0;
	return (ubuff);
}

/***-----------------------------------------------***/
getch()
{ /* rets AH.01,AL.CharIn, echos to console   ***/
	OSfn_(CONIN,0); 
}

/***-----------------------------------------------***/
putstr(prntstr) /** must be '$' terminated, conout  **/
 int *prntstr;
{
	OSfn_(C_WRITESTR,prntstr); /* Fn 9, DS:DX */
}

/***-----------------------------------------------***/
/*** 1:46 PM 8/15/2008				---***/
/*** int puts(char *string) ; rets 0`success .else.***/
/***  non-zero.  C strings are null terminated.	   ***/
puts(str) char str[];
{
	char pstr[],*ptb,*ip,*op,*mx,chr;
	int  ndx;

	mx = &tbuff[0];  /** holds byte len of tbuff **/
	ip = &tbuff[1];  /** returns len entered **/
	ptb = &tbuff[2]; /** buffer data area **/
	op = ptb;	/** save start **/
	pstr = str;
	ndx = 0;

	*mx = 127; /** set max length for string 0..126 **/
	*ip = 0;
		

	while (1)
		{
		if (ndx == 127)
			{ /** internal max str len **/
			NLfn();
			putstr("-= String maxed error =-$");
			return (BAD);
			}
		if (pstr[ndx] == 0)
			{	/** found null **/
			*ptb = '$';   /** chg termination **/
			*ip = ndx;    /** length of str in tbuff **/
			putstr(op);
			return (ndx); /** pass back numb of chrs **/
			}
		else	{  /** not EO string yet, copy chr **/
			chr = pstr[ndx];
			*ptb++ = chr;
			}
		ndx++;
		}
}

/***-----------------------------------------------***/
/*** dump2cons(fp_); write DTA to console, primitive.*/
dump2cons(fp_) int fp_;
{	/** change index to _iod address **/
	int *fptr, i, index;
	char fbuf[], byteval, *pccb;

	pccb = _iod[fp_]; /** log address of CCB fcb **/
	fbuf = &pccb[FBUFFER];
	i = 128;
	index = 0;
	while (i)
		{
		byteval = fbuf[index++];
		if (byteval == EOFCHR) break;
		if (isnprint (byteval)) 
			{ /** non printable substitution **/
			putchar('^');
			continue;
			}
		putchar(byteval);
		i--;
		}
	return (index);	
}

/***-----------------------------------------------***/
/** ----------= F I L E   I / O =------------------ **/
/***-----------------------------------------------***/
/*** OS_ := direct OS calls which map to 'C' Fns.  ***/
/***-----------------------------------------------***/
/*** Program Terminate is Fn# 0, but use exit();   ***/
/***-----------------------------------------------***/
/** These minimal file io primitives req'd for sm-c **/
/** int putc(int chr,FILE *fp); ret`chr`ok .else. EOF*/
/** int getc(FILE *fp); ret`nxtchr`ok .else. EOF    **/
/** fopen(name,mode) fclose(fp)			    **/
/***-----------------------------------------------***/
/** File Open 15 | On entry DS:DX := unopened FCB,  **/
/** Ret: int fp_, index of iob structure assigned.  **/
/** search thru CCB's, i.e. File Descriptors, for   **/
/** free descriptor to use. Copy filename and mode  **/
/** to descriptor fields.  OS Call 5 returns NULL if**/
/** either open or create is successful, but fopen()**/
/** returns fp#, or NULL if error opening the file. **/

fopen(filespec, mode) char *filespec, *mode; 
{
	int result, fpi;
	char *ufdesc, *indx, *p_ccb, sval;
  /** available slot, first free _iod index is our **/
  /** fp#, return this after further processing    **/
	while (1) 
		{
		fpi = 3;
		p_ccb = _iod[fpi]; /** infile **/
		sval = p_ccb[FSTATE];
		if ( sval == STATE_FR) {break;}

		fpi = 4;
		p_ccb = _iod[fpi]; /** outfile **/
		sval = p_ccb[FSTATE];
		if ( sval == STATE_FR) {break;}

		fpi = 5;
		p_ccb = _iod[fpi]; /** temp **/
		sval = p_ccb[FSTATE];
		if ( sval == STATE_FR) {break;}
	/** else none of the 3 CCB's are available **/
		sval = STATE_ER;
		putstr("No Available IOD$");
		NLfn();
		break;
		}
  /** catch error, return null  **/
	if ( sval == STATE_ER )
		{
		putstr("fopen STATE_ER error.$");
		NLfn();
		return (NULL);
		}
  /** else presumed free descriptor **/
  /** fpi holds index in _iod, which has addr of desc **/
  /** p_ccb holds that addr. Copy it as ufdesc        **/
	ufdesc = p_ccb;
  /** Set FMODE to mode **/
	p_ccb[FMODE] = *mode;
	p_ccb[DRV] = 0; /** current default drive **/
				/** chg behavior in future version **/

  /** Copy Filefilespec as upper case to FNAME, FTYP **/
	clearfcb(ufdesc); /** overwrite with spaces to clear **/
	p_ccb = &p_ccb[FNAME];
	indx = filespec;
	while (1)
		{ /** null terminated strings **/
		if (*indx == 0) break;
		if (*indx == '.')
			{
			p_ccb = &ufdesc[FTYP];
			indx++;
			}
		if (*indx) 
			{
			*p_ccb = toupper(*indx);
			p_ccb++; indx++;
			}
		else break;
		}

  /** call OS_fo, pass user file descriptor ptr -------------**/
	result = OS_fo(ufdesc);

  /** process low level fopen result **/
	if (result == NULL)
		{ /** successful open from lower level **/
		  /** conout state information **/
		if (VERBOSE)
			{
			putstr("CCB File Buffer assigned.$");
			NLfn();
			putstr ("fopen result reports not null.$");
			NLfn();outdeci(fpi);NLfn();
			}
		return fpi;
 		}
	else 	{ /** unsuccessful open **/
		if (VERBOSE)
			{
			putstr ("fopen result reports null.$");
			NLfn();
			}
		return (NULL);
		}
}

/***-----------------------------------------------***/
/** File Open 15 | On entry DS:DX := unopened FCB,  **/
/** the dir is searched searched for the named file **/
/** and returns AL := FFh if not found, .else. AL:= **/
/** 0, and the entry FCB is filled in, DRV:=current **/
/** drv, Current Block :=0.  OS Call 5 returns NULL **/
/** if either open or create is successful.         **/

OS_fo (ufdesc) char ufdesc[];
{ /** User File Descriptor*/
	char cresult, *cbuff;
	int iresult, i;
	cresult = ufdesc[FMODE];

  /** if fmode := read, f must exist for std C **/
	if ((cresult == 'r') | (cresult == 'R'))
		{
		cresult = OSfn_(OPEN, ufdesc);

	  /** OS Open op searches for f, returns FFh if not found **/
		if (cresult == BAD)
			{
			putstr("File Not Found for reading, Error..$");
			NLfn();
			return (BAD);
			}
		else	{	/** Open Successful **/
			putstr("File Found and Opened for Reading.$");
			NLfn();
			ufdesc[FSTATE] = STATE_FO;
		/** assign DTA _and_ seed it from file **/
			cbuff = &ufdesc[FBUFFER];
			setdta(cbuff);
			iresult = OS_sr(ufdesc);
			if (iresult)	/** 0`ok, 1`EOF **/
				{  /** true if EOF, else null returned **/
				ufdesc[FSTATE] = STATE_EF;
				ufdesc[FBUFNDX] = 0;
				}
			else  {
				ufdesc[FBUFNDX] = 0;
				ufdesc[FSTATE] = STATE_DT;
				}
			return (cresult);
			}
		}
	  /** if fmode := write, f created or f overwritten for std C **/

	else if ((cresult == 'w') | (cresult == 'W'))
		{ /** fcb ptr to unopened fcb, FFh ret if no file, else ret 0 **/
		cresult = OSfn_(OPEN, ufdesc);
	
		if (cresult == BAD)
			{  /** fille not found in current directory **/
			putstr("File Not Found for writing, Creating..$");
			NLfn();
			cresult = creatf(ufdesc);
			if (cresult == BAD) return BAD;
			else	{  /** Successful File Create for Writing **/
				putstr(" -File Created for Writing- $");
				NLfn();
				ufdesc[FSTATE] = STATE_FO;
				  /** assign DTA _and_ seed it with ^Z **/
				cbuff = &ufdesc[FBUFFER];
				setdta(cbuff);
				i=127;
				while (i) cbuff[i--] = CTRLZ;
				cbuff[i] = CTRLZ; /** position index 0 **/
				return (NULL);
				}
			}
		else	{  /** File Found and Successfully Opened for Writing. **/
		  /** Note that file position is at the beginning of the file. **/
		  /** seed buffer with ctl-z, eof marker **/
			i=127;
			cbuff = &ufdesc[FBUFFER];
			while (i) cbuff[i--] = CTRLZ;
			cbuff[i] = CTRLZ; /** position index 0 **/
			ufdesc[FSTATE] = STATE_FO;
			return (cresult);
			}
		}
  /** else some major error **/
	putstr(" -Major FMODE error- $");
	return BAD;
}

/***-----------------------------------------------***/
/** File Close 16 | Req'd to update dir info. DS:DX **/
/*** fclose() ret NULL if successful, else nz err  ***/
/*** Call 5 ret FFh, BAD, means err, NULL is ok    ***/

fclose(fp_) int fp_; 
{
	int *fptr, result;
  /** don't close standard files **/
	if ((fp_==stdin)|(fp_==stdout)|(fp_==stderr)) return NULL;
  /** convert fp_ index to CCB ptr, then call lower level **/
	fptr = _iod[fp_];
	result = OS_fc(fptr);
	if (VERBOSE)
		{
		if (result == NULL)
			{
			putstr ("fclose result reports null, success.$");
			NLfn();
			}
		else	{
			putstr ("fclose result reports not null, fail.$");
			NLfn();outdeci(fp_);NLfn();
			}
		}
	return result;
}

/***-----------------------------------------------***/
/** File Close 16 | Req'd to update dir info. DS:DX **/
/** := Close FCB, AL:=0 means ok, AL:=FFh means     **/
/** error. (BAD).                                   **/
/** at exit FCB must appear as unopened, clear FMODE */
/**  FSTATE **/

OS_fc(ufdesc) char ufdesc[]; /** User File Descriptor*/
{
	int result,cnt,idx;
	char stflg;

  /** note default open read is STATE_FO **/
  /** A Read changes FCB Block Values to current block **/
  /** check for pending write before closing **/
	stflg = ufdesc[FSTATE];

	if (stflg == STATE_PW) flushf(ufdesc);
  /** ok, close. **/
  /** successful close returns AL:=0, else FFh err  **/
	result = OSfn_(CLOSE, ufdesc);
	if (result == NULL)
		{
		putstr ("OS_fc result reports null, success.$");
		NLfn();
		}
	else	{
		putstr ("OS_fc result reports non-null, error.$");
		NLfn();
		}
  /** Clear CCB, Free FMODE, FSTATE, fn **/

	ufdesc[FMODE] = 0;
	ufdesc[FSTATE] = ufdesc[DRV] = 0;
/***	cnt = 171; idx = 0; while (cnt--) { ufdesc[idx++] = 0; } **/
  /** Return reported result, note we clear CCB to **/
  /** new, even on error. **/
	if (result == BAD) return BAD;
	else return NULL;
}

/***--- Write Record according to current blk/rec  ***/
/***--- settings in fcb                            ***/
wr_rec()
{
}

/***-----------------------------------------------***/
/** int getc(FILE *fp); ret`nxtchr`ok .else. EOF    **/
/**  init is fp_ index into _iob                    **/

getc(unit) int unit;
{
	int ichr;
  /** fetch ascii character from file and return it **/
	ichr = rd_ubuff(unit);
	if (ichr == EOF) return (EOF);
	return (ichr & 127);
}
/***-----------------------------------------------***/
/** int puts(char *string); ret`0`ok .else. `NZ`err **/
/**  takes a pointer value to target string, decom- **/
/**  pose targets' characters and pass each one to  **/
/**  putc() until NULL in string. CONOUT.           **/

/***-----------------------------------------------***/
/** int fputs(char *string, FILE *fp);             ***/
/**  ret`0`ok .else. `NZ`err                        **/
/**  takes a pointer value to target string, decom- **/
/**  pose targets' characters and pass each one to  **/
/**  putc() until NULL in string.  Does not append  **/
/**  the new-line character to the end of the string**/
/** Target string is C string, null terminated.     **/

fputs(str,fp_) char str[]; int fp_;
{
	char chr; int i;

	i=0; chr = str[0];
	if (chr == 0) return BAD;
	while (chr)
		{
		putc(chr,fp_);
		chr = str[++i]; /** read chr from ds:str **/
		}
}

/***-----------------------------------------------***/
/** int putc(int chr,FILE *fp); ret`chr`ok .else. EOF*/
/** note: only retun EOF on File Error, ret null or **/
/** chr only.  (echo back chr means success)		*/
/** _get ptr to CCB; //DTA is CCB[FBUFFER]; putc to **/
/**  //[FBUFFER]+[FBUFNDX];                         **/
/** _get FBUFNDX,				    **/
/** .if. == 0, set FSTATE == STATE_PW		    **/
/** .if. == 128, flush buffer to file, set FBUFNDX==0*/
/**      set FSTATE == STATE_DT;		    **/
/** .else. place chr to FBUFNDX and incr FBUFNDX    **/
/** NOTE: small-c will close outfile if putc rets<=0**/

putc(chr,fp_) char chr; int fp_;
{
	int *fptr, i, index;
	char fbuf[], byteret, *pccb;
  /** First off, test for printable character passed **/
  /**  isnprint returns 1 if not printable           **/
  /**	if (isnprint(chr)) return (BAD);	**/

  /** convert fp_ index to CCB ptr **/
	pccb = _iod[fp_]; /** log address of CCB fcb **/
	fbuf = &pccb[FBUFFER]; /** get buffer location **/
	setdta(fbuf); /** indicate current DTA for write **/
	index = pccb[FBUFNDX];

  /** put char to buffer **/
	fbuf = &pccb[FBUFFER]; /** get buffer location **/
	fbuf = fbuf + index; /** base + index **/
	*fbuf = chr; /** chr placed into current buffer **/

	if (index == 0) pccb[FSTATE] = STATE_PW;

  /** ck if index is maxed, if so wr buffer and reset index **/
	if (index == 127) /** maxed, signed byte compare **/
		{ /** flushf full buffer **/
		byteret = flushf(pccb);
		if (byteret)  /** 0`ok .else. err **/
			{ /** error state **/
			pccb[FSTATE] = STATE_ER;
			return BAD;
			}
		else	{ /** flushed ok, purge buff upd index **/
			fbuf = &pccb[FBUFFER]; /** reset to begin **/
		 /** fill buffer with CTRLZ, EOF **/
			i=127;
			while (i) fbuf[i--] = CTRLZ;
			fbuf[i] = CTRLZ; /** position index 0 **/
			pccb[FSTATE] = STATE_DT; /** Closeable State **/
			pccb[FBUFNDX] = 0; /** reset for next chr **/
			return (chr);
			}
		} /** sector written, chr or err has been returned **/

  /** update index return chr placed -echo- **/ 
  /**	pccb[FBUFNDX]++; ??questionable	**/
	index++;
	pccb[FBUFNDX] = index;
	return (chr);
}

/***-----------------------------------------------***/
/*** set to spaces the fcb filename,typ		---***/

clearfcb(fcb) char fcb[];
{
	int i;
	i=11;
	while (i) fcb[i--] = ' ';
}

/***-----------------------------------------------***/

/***-----------------------------------------------***/
/** Directory First Search 17**/

/***-----------------------------------------------***/
/** Directory Subsequent Search 18 **/

/***-----------------------------------------------***/
/** File Delete 19 **/

/***-----------------------------------------------***/
/** File Sequential Read 20 | On Entry, DS:DX:=FCB **/
/** The 128 byte record indicated by Current Block **/
/** of 16k, and Current Record Fields, is loaded to**/
/** the Disk Transfer Area, DTA, then the record   **/
/** addressed by those fields is incremented. IF EOF*/
/** THEN AL:=1, ELSE AL:=0.                        **/ 

OS_sr(ufcb) char ufcb[];
{
	int byte_result;

	byte_result = OSfn_(SEQREAD,ufcb);
	if (byte_result) return (EOF);
	else return (0);
}

/***-----------------------------------------------***/
/** rd_ubuff(int fp) read unit buffer, if last chr  **/
/**  re index and fetch next file record.           **/
/** Called by getc(fp_), so ret chr or EOF          **/
/** Note: short sector files are null padded, catch **/
/** partial buffer data.                            **/
/** 30-Jul-08 05:08:51 PM. OS_sr returns 0 if ok,   **/
/** .else. 1 if EOF. Fixed buffer clipping.         **/

rd_ubuff(fp_) int fp_;
{	/** change fp_ index into _iod address **/
	/** check _iod is read kind, else error **/
	/** chr_ptr points to next_to_read **/
	/** if chr_ptr == 0, ck STATE_EF for eof **/
	/**  if EOF state return EOF **/
	/** if chr_ptr == 127 retrieve chr, do seq **/
	/** read to fill buffer, reindex chr_ptr =0 **/
	/** if read_sq gives EOF, set state STATE_EF, **/
	/** but return last chr to caller. **/

	int chr, index, result;
	char *p_ccb, *cdta, *cp;

  /** Convert fp_ into addr of UFCB **/
	p_ccb = _iod[fp_];

  /** Also need to manage DTA for UFCB **/
	cdta = &p_ccb[FBUFFER];
	cp = cdta;
	setdta(cdta);

  /** check _iod is read kind, else error **/
	if (p_ccb[FMODE] != 'r')
		{ /** mode error **/
		putstr("File Mode Error, not opened for Reading$");
		NLfn(); return (EOF);
		}
	if ((p_ccb[FSTATE] == STATE_EF) & (p_ccb[FBUFNDX] == 127))
		{  /** last low level read returned eof **/
		putstr("-= End of File Reached =- $");
		NLfn(); return (EOF);
		}
  /** fetch ascii character from file buffer and hold it **/  
	index = p_ccb[FBUFNDX];
	cp = cp + index;  /** base + index to chr in buffer **/
	chr = *cp;		/** FBUFNDX points to next, got chr **/
  /** change null to sp ' ' char, these are text files only. **/
	if (chr == NULL) chr = ' ';
  /** !! Convert eol 0Dh,0Ah to 20h,0Ah for small-c parser. !! **/
	if (chr == 13) chr = ' ';

  /** Warning: an unprimed buffer could hold nulls, handle this **/
  /** by priming buffer on fopen(fp_,'r') for reading.          **/
  /** Still, should check here for ^Z, eof, marker and ret EOF. **/
  /** But NO, file can be exact byte length without terminators.**/

  /** ck if index is maxed, if so rd buffer and reset index **/
	if (index == 127)      /** maxed, signed byte compare **/
		{  /** eo buffer read, fetch next from seq read **/
		result = OS_sr(p_ccb);
		if (result)
			{  /** true if EOF, else null returned **/
			p_ccb[FSTATE] = STATE_EF;
			p_ccb[FBUFNDX] = 0;
			return (chr);
			}
		else  {
			p_ccb[FBUFNDX] = 0;
			p_ccb[FSTATE] = STATE_DT;
			return (chr); /** added to fix clipping **/
			}
		}
	p_ccb[FBUFNDX]++; /** update for next chr fetch from buffer **/
	return (chr);
}

/***-----------------------------------------------***/
/** File Sequential Write 21 | On Entry, DS:DX:=FCB **/
/** The 128 byte record indicated by Current Block  **/
/** of 16k, and Current Record Fields, is written   **/
/** from the Disk Transfer Area, DTA, to the file   **/
/** then the record address is incremented. IF the  **/
/** disk is too full, AL:=1, ELSE AL:=0.            **/
/**	=	=	=	=	=	=   **/
/*** write buffer, 80h long, to file sequentially. ***/
/***  file fp_ must select open file. DTA must be  ***/
/***  current to proper fp_.                       ***/
/***  calls OSfn_ 21d Sequential Write, which rets ***/
/*** al`0`ok .else. al`1`'disk full error'         ***/
/***  flushf() rets`0`ok .else. `BAD`error         ***/
/***-----------------------------------------------***/

flushf(pccb) char *pccb;
{
	char byte_result;
	if (pccb[FSTATE] == STATE_PW)
		{
		byte_result = OSfn_(SEQWRITE,pccb);
		if (byte_result) /** indicates err **/
			{
			pccb[FSTATE] = STATE_ER;
			return (BAD);
			}
		pccb[FSTATE] = STATE_FO; /** mark as file open **/
		return byte_result;
		}
	else return (BAD); /** no pending write state **/
}

/***-----------------------------------------------***/
/** File Create 22 Rets: FFh if no dir space else   **/
/**  null == successful create                      **/

creatf(ufcb) char *ufcb;
{
	OSfn_(CREATE, ufcb);
}

/***-----------------------------------------------***/
/** ReName File 23 **/
OS_rn()
{
}
/***-----------------------------------------------***/
/** non-existent fn# 24 **/
/***-----------------------------------------------***/
/** Current Default Drive 25, 0`A,1`B,etc. **/

/***-----------------------------------------------***/
/** Set Disk Transfer Addr 26 **/
setdta(dta) int *dta;
{  /**  !MUST! Set DTA address prior to File I/O **/
	OSfn_(26,dta); /** Fn Set DTA -> DS:DX **/
}
/***-----------------------------------------------***/
/** Allocation Table Addr 27 ret: DS:BX, DX := # of..*/
/**  alloc units, AL := # rec per Alloc Unit.       **/
/***-----------------------------------------------***/
/** non-existent fn# 28, 29, 30 **/
/***-----------------------------------------------***/
/** Get Disk Parameter Address 31 ret: DS:BX table  **/
/**  of params for current default disk **/
/***-----------------------------------------------***/
/** non-existent fn# 32 **/
/***-----------------------------------------------***/
/** File Random Read 33 **/
/***-----------------------------------------------***/
/** File Random Write 34 **/
/***-----------------------------------------------***/
/** File Size 35 **/
/***-----------------------------------------------***/
/** File Set Random Record Field 36 **/
/***-----------------------------------------------***/
/** Interrupt Set Vector 37 **/
/***-----------------------------------------------***/
/** Alloc, Create New Program Segment 38 **/
/***-----------------------------------------------***/
/** File Random Block Read 39 **/
/***-----------------------------------------------***/
/** Random Block Write **/

/***-----------------------------------------------***/
/** initialize the fcb structure fields at main    ***/

initfcb()
{	char *cptr, *p_stdin, *p_stdout, *p_stderr;
	char *p_infile, *p_outfile, *p_tempfile;
	int  cnt;

	p_stdin    = "STDIN   IOB"; /** null term.. **/
	p_stdout   = "STDOUT  IOB"; /**  in dseg    **/
	p_stderr   = "STDERR  IOB";
	p_infile   = "INFILE  TXT";
	p_outfile  = "OUTFILE TXT";
	p_tempfile = "TEMP    TXT";
	;
	setdta(fdta); /** default catch basin **/
  /** setup _iod[] with addr of f_units **/
	_iod[0] = &f_unit0;
	_iod[1] = &f_unit1;
	_iod[2] = &f_unit2;
	_iod[3] = &f_unit3;
	_iod[4] = &f_unit4;
	_iod[5] = &f_unit5;

  /** copy char const to each f_unit **/
  /***  Also, **/
  /** mark stdin as R, stdout, stderr as W in fmode **/
  /** mark infile, outfile, temp as 'N'ull for now. **/
  /**  open these 3 default files in main if desired**/

	cptr = _iod[0];
	*cptr[FMODE] = "r";
	copystr(p_stdin, cptr+1);

	cptr = _iod[1];
	*cptr[FMODE] = "w";
	copystr(p_stdout, cptr+1);

	cptr = _iod[2];
	*cptr[FMODE] = "w";
	copystr(p_stderr, cptr+1);

	cptr = _iod[3];
	*cptr[FMODE] = "n";
	copystr(p_infile, cptr+1);

	cptr = _iod[4];
	*cptr[FMODE] = "n";
	copystr(p_outfile, cptr+1);

	cptr = _iod[5];
	*cptr[FMODE] = "n";
	copystr(p_tempfile, cptr+1);

  /*** otherwise, fields are init to null by nasm ***/
}


/*****************************************************/
/** Test OSfn_ Call 0005h Interface **/
/*****************************************************/
/* CP/M system call codes                            */
/*     -also-                                        */
/* pcDos .COM Call 5 Interface.                      */

 #asm
BDOS	EQU	5	;; call 5 INTERFACE as CALL BDOS.

CTRLZ	EQU	26	;end-of-file mark for text files
TBUFF	EQU	80H	;address of default I/O address

;;/** OSfn_(Fn#,Value) -> tos 0[ret]2[Value]4[Fn#] **/
OSfn_:  ;; small-c, top of stack[ret][Val][Fn]
	pop  bx   ;;
        pop  dx   ;;
        pop  cx   ;;
	push cx   ;; [Fn#]
	push dx   ;; [Value]
        push bx   ;; [ret] 
        call BDOS ;; values passed in registers.
;; Test AL`CBW returned value...
	cbw
	RET       ;; Caller cleans the stack.

exit:  ;; pause for keypress before return.
	mov  ah,0
	int  16h  ;; RomBios wait for keypress.
;;exit:		  ;; don't wait.
	int  19h  ;; reboot-> enviornment gets control.
	ret       ;; (actually, never executed)
 #endasm
/***-----------------------------------------------***/
/*****************************************************/
/*       1         2         3         4         5   */
/***-----------------------------------------------***/
/** EO C5LIB.C **/
