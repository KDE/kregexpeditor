typedef union {
  struct {
    int min;
    int max;
  } range;
  int backRef;
  RegExp* regexp;
  char ch;
} YYSTYPE;
#define	TOK_Dot	257
#define	TOK_Dollar	258
#define	TOK_Carat	259
#define	TOK_MagicLeftParent	260
#define	TOK_PosLookAhead	261
#define	TOK_NegLookAhead	262
#define	TOK_LeftParen	263
#define	TOK_RightParent	264
#define	TOK_Bar	265
#define	TOK_Quantifier	266
#define	TOK_BackRef	267
#define	TOK_CharClass	268
#define	TOK_Char	269
#define	TOK_EscapeChar	270
#define	TOK_PosWordChar	271
#define	TOK_PosNonWordChar	272


extern YYSTYPE yylval;
