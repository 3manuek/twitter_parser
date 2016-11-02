/* contrib/twitter_parser/test_parser--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION twitter_parser" to load this file. \quit

CREATE FUNCTION twitterprs_start(internal, int4)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION twitterprs_getlexeme(internal, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION twitterprs_end(internal)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION twitterprs_lextype(internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE TEXT SEARCH PARSER twitterparser (
    START    = twitterprs_start,
    GETTOKEN = twitterprs_getlexeme,
    END      = twitterprs_end,
    HEADLINE = pg_catalog.prsd_headline,
    LEXTYPES = twitterprs_lextype
);
