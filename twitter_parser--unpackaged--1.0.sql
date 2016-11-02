/* contrib/twitter_parser/twitter_parser--unpackaged--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION twitter_parser FROM unpackaged" to load this file. \quit

ALTER EXTENSION twitter_parser ADD function twitterprs_start(internal,integer);
ALTER EXTENSION twitter_parser ADD function twitterprs_getlexeme(internal,internal,internal);
ALTER EXTENSION twitter_parser ADD function twitterprs_end(internal);
ALTER EXTENSION twitter_parser ADD function twitterprs_lextype(internal);
ALTER EXTENSION twitter_parser ADD text search parser twitterparser;
