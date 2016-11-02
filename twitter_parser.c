/*-------------------------------------------------------------------------
 *
 * twitter_parser.c
 *	  Hashtag and mentions added to the parser
 *
 * Copyright (c) 2007-2016, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  contrib/twitter_parser/twitter_parser.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "fmgr.h"

PG_MODULE_MAGIC;


/*
 * types
 */

/* self-defined type */
typedef struct
{
	char	   *buffer;			/* text to parse */
	int			len;			/* length of the text in buffer */
	int			pos;			/* position of the parser */
} ParserState;



/*


postgres=# \dFp+ 
    Text search parser "pg_catalog.default"
     Method      |    Function    | Description 
-----------------+----------------+-------------
 Start parse     | prsd_start     | (internal)
 Get next token  | prsd_nexttoken | (internal)
 End parse       | prsd_end       | (internal)
 Get headline    | prsd_headline  | (internal)
 Get token types | prsd_lextype   | (internal)

        Token types for parser "pg_catalog.default"
   Token name    |               Description                
-----------------+------------------------------------------
 asciihword      | Hyphenated word, all ASCII
 asciiword       | Word, all ASCII
 blank           | Space symbols
 email           | Email address
 entity          | XML entity
 file            | File or path name
 float           | Decimal notation
 host            | Host
 hword           | Hyphenated word, all letters
 hword_asciipart | Hyphenated word part, all ASCII
 hword_numpart   | Hyphenated word part, letters and digits
 hword_part      | Hyphenated word part, all letters
 int             | Signed integer
 numhword        | Hyphenated word, letters and digits
 numword         | Word, letters and digits
 protocol        | Protocol head
 sfloat          | Scientific notation
 tag             | XML tag
 uint            | Unsigned integer
 url             | URL
 url_path        | URL path
 version         | Version number
 word            | Word, all letters
(23 rows)


#define ASCIIWORD               1
#define WORD_T                  2
#define NUMWORD                 3
#define EMAIL                   4
#define URL_T                   5
#define HOST                    6
#define SCIENTIFIC              7
#define VERSIONNUMBER   8
#define NUMPARTHWORD    9
#define PARTHWORD               10
#define ASCIIPARTHWORD  11
#define SPACE                   12
#define TAG_T                   13
#define PROTOCOL                14
#define NUMHWORD                15
#define ASCIIHWORD              16
#define HWORD                   17
#define URLPATH                 18
#define FILEPATH                19
#define DECIMAL_T               20
#define SIGNEDINT               21
#define UNSIGNEDINT             22
#define XMLENTITY               23

#define LASTNUM                 23

*/

typedef struct
{
	//  copy-paste from wparser.h of tsearch2 
	int			lexid;
	char	   *alias;
	char	   *descr;
} LexDescr;

/*
 * functions
 */
PG_FUNCTION_INFO_V1(twitterprs_start);
PG_FUNCTION_INFO_V1(twitterprs_getlexeme);
PG_FUNCTION_INFO_V1(twitterprs_end);
PG_FUNCTION_INFO_V1(twitterprs_lextype);

Datum
twitterprs_start(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));

	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);
	pst->pos = 0;

	PG_RETURN_POINTER(pst);
}

Datum
twitterprs_getlexeme(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	char	  **t = (char **) PG_GETARG_POINTER(1);
	int		   *tlen = (int *) PG_GETARG_POINTER(2);
	int			startpos = pst->pos;
	int			type;

	*t = pst->buffer + pst->pos;

/*
	If it is a character AND it has an @ AND at least 1 dot after it with no other
	characters. -> email
	If char is @ go until next white space -> mention
	If char is # go until next white space -> hashtag

	We need more elegance, detecting invalid characters. 
*/

	if (pst->pos < pst->len &&
		(pst->buffer)[pst->pos] == '#')
	{
		/* hashtag type */
		type = 25;
		/* go to the next non-space character */
		while (pst->pos < pst->len &&
			   (pst->buffer)[pst->pos] != ' ')
			(pst->pos)++;
	}
	else if (pst->pos < pst->len &&
		(pst->buffer)[pst->pos] == '@')
	{
		/* mention type */
		type = 24;
		/* go to the next space character */
		while (pst->pos < pst->len &&
			   (pst->buffer)[pst->pos] != ' ')
			(pst->pos)++;
	}

	*tlen = pst->pos - startpos;

	/* we are finished if (*tlen == 0) */
	if (*tlen == 0)
		type = 0;

	PG_RETURN_INT32(type);
}

Datum
twitterprs_end(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);

	pfree(pst);
	PG_RETURN_VOID();
}

Datum
twitterprs_lextype(PG_FUNCTION_ARGS)
{
	/*
	 * Remarks: - we have to return the blanks for headline reason - we use
	 * the same lexids like Teodor in the default word parser; in this way we
	 * can reuse the headline function of the default word parser.

	Above, you will see the current list. We picked up 2 new ids as there is no
	documentation wether it should use an existent id.

	 */
	LexDescr   *descr = (LexDescr *) palloc(sizeof(LexDescr) * (2 + 1));

	/* there are only two types in this parser */
	descr[0].lexid = 24;
	descr[0].alias = pstrdup("mention");
	descr[0].descr = pstrdup("Mention Twitter Style @name");
	descr[1].lexid = 25;
	descr[1].alias = pstrdup("hashtag");
	descr[1].descr = pstrdup("Hashtag #hashtag");
	descr[2].lexid = 0;

	PG_RETURN_POINTER(descr);
}

/*

*/



