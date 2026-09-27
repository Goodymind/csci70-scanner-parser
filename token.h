struct token
{
    int id;
    char lexeme[1000];
};

extern const char *tokennames[];

// state definitions
#define IDENTIFIER  7
#define NUMBER      8
#define EXPONENTIAL 9
#define STRING      10
#define COMMENT     11
#define ASSIGN      12
#define SEMICOLON_S 13
#define PLUS_S      14
#define COLON_S     15
#define MINUS_S     16
#define COMMA_S     17
#define MULTIPLY    18
#define RAISE       19
#define DIVIDE      20
#define LESSTHAN    21
#define LTEQUAL     22
#define EQUAL_S     23
#define GREATERTHAN 24
#define GTEQUAL     25
#define NOTEQUAL    26
#define LEFTPAREN_S 27
#define RIGHTPAREN_S 28
#define EOF_S       29
#define ERROR       30
#define DECIMAL     31 // should be shifted around ngl but order doesnt matter
