// Snek - A python interpreter in C

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

// Abstract Syntax Tree node types
typedef enum {
  NODE_INT,               // a number
  NODE_BINARY_OPERATION  // an operator like +, -, *, /
} NodeType;

// abstract syntax tree node
typedef struct ASTNode {
  NodeType type;
  union {
    int value;  // if the node is a number
    struct {
      char operator;          // an operator like +, -, *, /
      struct ASTNode *left;   // left side of the operator
      struct ASTNode *right;  // right side of the operator
    } BinaryOperation;
  };
} ASTNode;
