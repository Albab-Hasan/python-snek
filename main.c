// Snek - A python interpreter in C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Abstract Syntax Tree node types
typedef enum
{
  NODE_INT,            // a number
  NODE_BINARY_OPERATOR // an operator like +, -, *, /
} NodeType;

// Abstract Syntax Tree node
// Abstract Syntax Tree is a tree representation of the syntactic structure of source code, used in compilers
typedef struct ASTNode
{
  NodeType type;
  union
  {
    int value; // if the node is a number
    struct
    {
      char operator;         // an operator like +, -, *, /
      struct ASTNode *left;  // left side of the operator
      struct ASTNode *right; // right side of the operator
    } BinaryOperator;
  };
} ASTNode;

// parser
typedef enum
{
  TOKEN_INT,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIVIDE,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_UNKNOWN, // unknown token
  TOKEN_EOF      // end of file
} TokenType;

typedef struct
{
  TokenType type;
  int value;   // for integer tokens
  char *start; // for error reporting
  int length;
} Token;

Token current_token;
const char* input;
char* current_position;

void next_token(){
  while (isspace(*current_position)) current_position++;

  current_token.start = current_position;
  current_token.length = 1;
}
