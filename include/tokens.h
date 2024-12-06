#ifndef TOKENS_H
#define TOKEN_H

// defines methods for tokens data structure
// tokens are seperated by whitespace, individual tokens are treated as whitespace/newline/null terminator terminated
// tokens is a c string which acts as a list of tokens, the list is null terminated.

// gets head of next token
const char *next_token(const char * tokens);

// returns 1 if lhs is equal to rhs
int token_is_equal(const char *, const char *);

// returns 1 if lhs is an int (i.e you can call atoi on it)
int token_is_int(const char *);

// returns the length of the token
int token_length(const char*);
#endif