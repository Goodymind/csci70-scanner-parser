struct token
{
    int id;
    char lexeme[1000];
};

extern const char *tokennames[];

// state definitions
#define IDENTIFIER  10
#define NUMBER      11
#define DECIMAL     12
#define EXPONENTIAL 13
#define STRING      14
#define COMMENT     15
#define ASSIGN      16
#define SEMICOLON_S 17
#define PLUS_S      18
#define COLON_S     19
#define MINUS_S     20
#define COMMA_S     21
#define MULTIPLY    22
#define RAISE       23
#define DIVIDE      24
#define LESSTHAN    25
#define LTEQUAL     26
#define EQUAL_S     27
#define GREATERTHAN 28
#define GTEQUAL     29
#define NOTEQUAL    30
#define LEFTPAREN_S 31
#define RIGHTPAREN_S 32
#define EOF_S       33
#define ERROR       34
#define NUMSTATES   50