#ifndef SRC_CALCULATOR_H_
#define SRC_CALCULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node Node;
struct Node {
    char data;              // char data
    Node *next;             // pointer to the next node
};

typedef struct Date Date;
struct Date {
    double data;
    Date *next;
};

int isEmptyOper(Node *list);
int isEmptyOperand(Date *roster);
void pushOperand(Date **list, double temp);
double popOperand(Date **list);
void parser(char *str, int *err, char *res);
void numbersFun(char **str, char *res, int **err);
void pointCounter(char *arr, int ***err);
Node *getOper(char **str, char *res, Node *list);
int findOper(char oper);
int prior(char oper);
Node *getBracket(char **str, char *res, int **err, Node *list, int bracket_start, int bracket_end);
int findBracket(char oper, int *bracket_start, int *bracket_end);
void operCounter(char *str, int **err);
void modFun(char *str, int **err);
Node *pushFun(char *fun, char *res, Node *list);
void modFun(char *str, int **err);
void unaryGet(char **str, char **fun);
char convert(char *fun);
void checkFunc(char *func, int **err);
int findFunc(char oper);
void getFunc(char **str, char **func);
Date* calculator(Date *roaster, char *str, int **err);
double polishNotation(char *str, int *err);
void unarFun(char *str);


#endif  // SRC_CALCULATOR_H_
