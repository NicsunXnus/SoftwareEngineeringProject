#ifndef SPCONSTANTS_H
#define SPCONSTANTS_H

#include <string>

using namespace std;

static inline const string IF_STRING = "if";
static inline const string WHILE_STRING = "while";
static inline const string THEN_STRING = "then";
static inline const string ELSE_STRING = "else";

static inline const string READ_WHITESPACE_STRING = "read ";
static inline const string CALL_WHITESPACE_STRING = "call ";
static inline const string PRINT_WHITESPACE_STRING = "print ";
static inline const string PROCEDURE_STRING = "procedure";

static inline const string SEMICOLON_STRING = ";";
static inline const string EQUALS_STRING = "=";

static inline const string AND_BOOL_OP_STRING = "&&";
static inline const string OR_BOOL_OP_STRING = "&&";

static inline const int IF_STR_LEN = 2;
static inline const int WHILE_STR_LEN = 5;
static inline const int THEN_STR_LEN_MINUS_ONE = 3;

static inline const char OPEN_BRACKET_CHAR = '(';
static inline const char CLOSE_BRACKET_CHAR = ')';
static inline const char OPEN_CURLY_CHAR = '{';
static inline const char CLOSE_CURLY_CHAR = '}';
static inline const char EXCLAMATION_MARK_CHAR = '!';

static inline const int SEPARATOR_LEN = 1; // because separators are one character long

static inline const string_view OPEN_BRACKET_STRING_VIEW = "(";
static inline const string_view CLOSE_BRACKET_STRING_VIEW = ")";
static inline const string_view PLUS_STRING_VIEW = "+";
static inline const string_view MINUS_STRING_VIEW = "-";
static inline const string_view MULTIPLY_STRING_VIEW = "*";
static inline const string_view DIVIDE_STRING_VIEW = "/";
static inline const string_view MODULO_STRING_VIEW = "%";

#endif