# include "stdio.h"
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
#undef ECHO
#include "dis.h"
#ifndef AMIGA
int lineno = 0;
#endif
char *strcpy();
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
{ ; }
break;
case 2:
{	lineno++;
				return '\n';
}
break;
case 3:
{ 
				return EQ;
}
break;
case 4:
{ 
				return LI;
}
break;
case 5:
{ 
				return EQ;
}
break;
case 6:
{ 
				return LI;
}
break;
case 7:
{
				return TSTART;
}
break;
case 8:
{
				return TSTOP;
}
break;
case 9:
{
				(void)sscanf(yytext, "%d", &token.ival); 
				return NUMBER;
}
break;
case 10:
{
				(void)sscanf(yytext+1, "%x", &token.ival);
				return NUMBER;
}
break;
case 11:
{
				token.sval = emalloc((unsigned) strlen(yytext)+1);
				(void)strcpy((char *)token.sval, (char *)yytext);
				return NAME;
}
break;
case 12:
{
				return COMMENT;
}
break;
case 13:
		{ return yytext[0]; }
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

char *
emalloc(n)
unsigned n;
{
	char *ptr, *malloc();

	if ((ptr = malloc(n)) == (char *) 0) {
		(void) fprintf(stderr,"out of core");
		exit(1);
	}
	return ptr;
}
int yyvstop[] = {
0,

13,
0,

1,
13,
0,

2,
0,

13,
0,

12,
13,
0,

13,
0,

9,
13,
0,

11,
13,
0,

10,
0,

12,
0,

9,
0,

11,
0,

3,
0,

4,
0,

5,
0,

6,
0,

8,
0,

7,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
7,12,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
7,12,	7,0,	12,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,6,	2,6,	0,0,	
0,0,	0,0,	0,0,	1,7,	
2,7,	0,0,	0,0,	1,8,	
2,8,	1,9,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
6,11,	6,11,	6,11,	6,11,	
6,11,	6,11,	6,11,	6,11,	
6,11,	6,11,	1,10,	7,12,	
0,0,	0,0,	0,0,	0,0,	
1,10,	6,11,	6,11,	6,11,	
6,11,	6,11,	6,11,	8,13,	
0,0,	0,0,	0,0,	0,0,	
7,12,	0,0,	8,14,	0,0,	
0,0,	0,0,	7,12,	0,0,	
0,0,	14,22,	0,0,	0,0,	
1,3,	0,0,	0,0,	0,0,	
0,0,	13,21,	0,0,	0,0,	
0,0,	6,11,	6,11,	6,11,	
6,11,	6,11,	6,11,	8,15,	
0,0,	0,0,	7,12,	0,0,	
0,0,	0,0,	8,16,	0,0,	
0,0,	0,0,	26,28,	0,0,	
0,0,	8,17,	8,18,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	15,23,	
16,24,	17,25,	18,26,	25,27,	
27,29,	28,30,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
30,31,	0,0,	0,0,	0,0,	
10,20,	0,0,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-2,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+8,	0,		yyvstop+8,
yycrank+-19,	0,		yyvstop+10,
yycrank+10,	0,		yyvstop+13,
yycrank+79,	0,		yyvstop+15,
yycrank+89,	0,		yyvstop+18,
yycrank+0,	yysvec+6,	yyvstop+21,
yycrank+-20,	yysvec+7,	yyvstop+23,
yycrank+20,	0,		0,	
yycrank+20,	0,		0,	
yycrank+34,	0,		0,	
yycrank+43,	0,		0,	
yycrank+33,	0,		0,	
yycrank+36,	0,		0,	
yycrank+0,	yysvec+9,	yyvstop+25,
yycrank+0,	yysvec+10,	yyvstop+27,
yycrank+0,	0,		yyvstop+29,
yycrank+0,	0,		yyvstop+31,
yycrank+0,	0,		yyvstop+33,
yycrank+0,	0,		yyvstop+35,
yycrank+40,	0,		0,	
yycrank+25,	0,		0,	
yycrank+40,	0,		0,	
yycrank+54,	0,		0,	
yycrank+0,	0,		yyvstop+37,
yycrank+79,	0,		0,	
yycrank+0,	0,		yyvstop+39,
0,	0,	0};
struct yywork *yytop = yycrank+211;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,01  ,01  ,01  ,01  ,'_' ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
#ifndef lint
static	char ncform_sccsid[] = "@(#)ncform 1.2 86/10/08 SMI"; /* from S5R2 1.2 */
#endif

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
