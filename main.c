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

// lexer 
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
  TOKEN_EOF,     // end of file
  TOKEN_ERROR    // error token
} TokenType;

typedef struct
{
  TokenType type;
  int value;   // for integer tokens
  char *start; // for error reporting
  int length;
} Token;

Token current_token;
const char *input;
char *current_position;

void next_token()
{
  while (isspace(*current_position))
    current_position++;

  current_token.start = current_position;
  current_token.length = 1;

  switch (*current_position)
  {
  case '\0':
    current_token.type = TOKEN_EOF;
    break;
  case '+':
    current_token.type = TOKEN_PLUS;
    current_position++;
    break;
  case '-':
    current_token.type = TOKEN_MINUS;
    current_position++;
    break;
  case '*':
    current_token.type = TOKEN_MULTIPLY;
    current_position++;
    break;
  case '/':
    current_token.type = TOKEN_DIVIDE;
    current_position++;
    break;
  case '(':
    current_token.type = TOKEN_LPAREN;
    current_position++;
    break;
  case ')':
    current_token.type = TOKEN_RPAREN;
    current_position++;
    break;
  default:
    if (isdigit(*current_position))
    {
      current_token.type = TOKEN_INT;
      current_token.value = 0;
      while (isdigit(*current_position))
      {
        current_token.value = current_token.value * 10 + (*current_position - '0');
        current_position++;
      }
    }
    else
    {
      current_token.type = TOKEN_ERROR;
      current_position++;
    }
  }
}

// parser
ASTNode *parse_expression();
ASTNode *parse_primary()
{
  if (current_token.type == TOKEN_INT)
  {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_INT;
    node->value = current_token.value;
    next_token();
    return node;
  }
  else if (current_token.type == TOKEN_LPAREN)
  {
    next_token();
    ASTNode *expr = parse_expression();
    if (current_token.type != TOKEN_RPAREN)
    {
      fprintf(stderr, "Expected ')'\n");
      exit(1);
    }
    next_token();
    return expr;
  }
  else
  {
    fprintf(stderr, "Unexpected token\n");
    exit(1);
  }
}

ASTNode *parse_term()
{
  ASTNode *node = parse_primary();

  while (current_token.type == TOKEN_MULTIPLY || current_token.type == TOKEN_DIVIDE)
  {
    TokenType op = current_token.type;
    next_token();

    ASTNode *right = parse_primary();
    ASTNode *new_node = malloc(sizeof(ASTNode));
    new_node->type = NODE_BINARY_OPERATOR;
    new_node->BinaryOperator.operator = (op == TOKEN_MULTIPLY) ? '*' : '/';
    new_node->BinaryOperator.left = node;
    new_node->BinaryOperator.right = right;
    node = new_node;
  }
  return node;
}

ASTNode *parse_expression()
{
  ASTNode *node = parse_term();

  while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS)
  {
    TokenType op = current_token.type;
    next_token();

    ASTNode *right = parse_term();
    ASTNode *new_node = malloc(sizeof(ASTNode));
    new_node->type = NODE_BINARY_OPERATOR;
    new_node->BinaryOperator.operator = (op == TOKEN_PLUS) ? '+' : '-';
    new_node->BinaryOperator.left = node;
    new_node->BinaryOperator.right = right;
    node = new_node;
  }
  return node;
}
