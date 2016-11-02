CREATE EXTENSION twitter_parser;

-- make test configuration using parser

CREATE TEXT SEARCH CONFIGURATION twittercfg (PARSER = twitterparser);

ALTER TEXT SEARCH CONFIGURATION twittercfg ADD MAPPING FOR word WITH simple;

-- ts_parse

SELECT * FROM ts_parse('twitterparser', 'That''s simple parser can''t parse urls like http://some.url/here/');

SELECT to_tsvector('twittercfg','That''s my first own parser');

SELECT to_tsquery('twittercfg', '@star is a mention');

SELECT ts_headline('twittercfg','Supernovae #stars are the brightest #phenomena in galaxies #LadyGaga',
       to_tsquery('twittercfg', 'stars'));
