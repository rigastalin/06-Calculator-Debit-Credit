#include "calculator.h"

// **************
// STACK FUNCTIONS
// **************


// function to add an element 'x' to the stack
void pushOperator(Node **list, char buffer) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->data = buffer;
    p->next = *list;
    *list = p;
}

// function to pop a list element from the stack remove at the beginning
char popOperator(Node **list) {
    Node *p = *list;
    char res = p->data;
    p->data = 0;
    *list = p->next;
    free(p);
    return res;
}

// function to check if the stack is empty or not
int isEmptyOper(Node *list) {
    return list == NULL ? 0 : 1;
}

int isEmptyOperand(Date *roster) {
    return roster == NULL ? 0 : 1;
}

void pushOperand(Date **roster, double buffer) {
    Date *p = (Date *)malloc(sizeof(Date));
    p->data = buffer;
    p->next = *roster;
    *roster = p;
}

double popOperand(Date **roster) {
    Date *p = *roster;
    double res = p->data;
    p->data = 0;
    *roster = p->next;
    free(p);
    return res;
}


// **************
// PARCING FUNCTIONS
// **************

void parser(char *string, int *err, char *out_string) {
    int openBracket = 0;
    int closeBracket = 0;
    Node *list = NULL;

    operCounter(string, &err);
    unarFun(string);
    modFun(string, &err);
    while (*string != '\0' && *err == 0) {
        if ((string[0] >= '0' && string[0] <= '9') || string[0] == '.') {
            numbersFun(&string, out_string, &err);
        } else if (findOper(string[0]) == 1) {
            list = getOper(&string, out_string, list);
        } else if (findBracket(string[0], &openBracket, &closeBracket) == 1) {
            list = getBracket(&string, out_string, &err, list, openBracket, closeBracket);
        } else if (string[0] == 'u' || string[0] == 'p' || string[0] == 'k' || string[0] == 'b') {
            char *grep = (char *)malloc(50 * sizeof(char));
            unaryGet(&string, &grep);
            list = pushFun(grep, out_string, list);
            free(grep);
        } else if (findFunc(string[0]) == 1) {
            char *fun = (char *)malloc(30 * sizeof(char));
            getFunc(&string, &fun);
            checkFunc(fun, &err);
            if (*err == 0) {
                list = pushFun(fun, out_string, list);
            }
            free(fun);
        } else {
            *err = 9;
        }
    }
    if (isEmptyOper(list) == 1 && *err == 0) {
        while (isEmptyOper(list) == 1) {
            char buf;
            buf = popOperator(&list);
            out_string[strlen(out_string)] = buf;
            snprintf(out_string + strlen(out_string),
                    sizeof(out_string + strlen(out_string)) + 250, "%s", " ");
        }
    }
    if (openBracket != closeBracket && *err == 0)
        *err = 3;
    out_string[strlen(out_string) + 1] = '\0';
}

double polishNotation(char *string, int *err) {
    char *one_string;
    double result = 0;
    Date* roster = NULL;
    one_string = strtok(string, " ");
    while (one_string != NULL) {
        if (one_string[0] >= '0' && one_string[0] <= '9') {
            double num = strtod(one_string, NULL);
            pushOperand(&roster, num);
        } else {
            roster = calculator(roster, one_string, &err);
        }
        one_string = strtok(NULL, " ");
    }
    if (*err == 0)  {
        if (isEmptyOperand(roster) == 1) {
            result = popOperand(&roster);
        }
    }
    if (isEmptyOperand(roster) == 1) {
        *err = 7;
        while (isEmptyOperand(roster) == 0) {
            popOperand(&roster);
        }
    }
    return result;
}

Date *calculator(Date *roster, char *one_string, int **err) {
    double result = 0;
    if (findOper(one_string[0]) == 1 || one_string[0] == 'm') {
        if (isEmptyOperand(roster) == 1) {
            double num2 = popOperand(&roster);
            if (isEmptyOperand(roster) == 1) {
                double num1 = popOperand(&roster);
                if (strcmp(one_string, "+") == 0) result = num1 + num2;
                if (strcmp(one_string, "-") == 0) result = num1 - num2;
                if (strcmp(one_string, "/") == 0) result = num1 / num2;
                if (strcmp(one_string, "*") == 0) result = num1 * num2;
                if (strcmp(one_string, "m") == 0) result = fmod(num1, num2);
                if (strcmp(one_string, "^") == 0) result = pow(num1, num2);
            } else {
                **err = 8;
            }
        } else {
            **err = 8;
        }
    } else {
        if (isEmptyOperand(roster) == 1) {
            double num1 = popOperand(&roster);
            if (strcmp(one_string, "s") == 0) result = sin(num1);
            if (strcmp(one_string, "c") == 0) result = cos(num1);
            if (strcmp(one_string, "t") == 0) result = tan(num1);
            if (strcmp(one_string, "i") == 0) result = asin(num1);
            if (strcmp(one_string, "o") == 0) result = acos(num1);
            if (strcmp(one_string, "a") == 0) result = atan(num1);
            if (strcmp(one_string, "q") == 0) result = sqrt(num1);
            if (strcmp(one_string, "n") == 0) result = log(num1);
            if (strcmp(one_string, "l") == 0) result = log10(num1);
            if (strcmp(one_string, "p") == 0 || strcmp(one_string, "b") == 0) {
                result = num1;
            }
            if (strcmp(one_string, "u") == 0 || strcmp(one_string, "k") == 0) {
                result = num1 * (-1);
            }
        } else {
            **err = 8;
        }
    }
    if (**err == 0) pushOperand(&roster, result);
    return roster;
}

void unarFun(char *string) {
    if (string[0] == '-') string[0] = 'u';
    if (string[0] == '+') string[0] = 'p';
    for (unsigned int i = 0; i < strlen(string); i++) {
        if ((string[i] == 'n' || string[i] == 's' || string[i] == 'g' ||
            string[i] == 't' || string[i] == 'd') && (string[i + 1] == '-')) {
            string[i + 1] = 'k';
        }
        if ((string[i] == '(') && (string[i + 1] == '-')) {
            string[i + 1] = 'u';
        }
        if ((string[i] == 'n' || string[i] == 's' || string[i] == 'g' || string[i] == 't' ||
            string[i] == 'd') && string[i + 1] == '+') {
            string[i + 1] = 'b';
        }
        if ((string[i] == '(') && string[i + 1] == '+') {
            string[i + 1] = 'p';
        }
    }
}

void numbersFun(char **string, char *out_string, int **err)  {
    char *numArr = "1234567890.";
    int num;
    char arr[256] = "\0";
    num = strspn(*string, numArr);
    strncpy(arr, *string, num);
    pointCounter(arr, &err);
    snprintf(out_string + strlen(out_string), sizeof(out_string + strlen(out_string)) + 250, "%s", arr);
    snprintf(out_string + strlen(out_string), sizeof(out_string + strlen(out_string)) + 250, "%s", " ");
    *string += num;
}

void pointCounter(char *arr, int ***err) {
    int point = 0;
    int lenght = strlen(arr);
    if (arr[0] == '.') {
        point = 2;
    } else {
        for (int i = 1; i < lenght; i++) {
            if (arr[i] == '.') point++;
        }
    }
    if (point > 1) ***err = 1;
}

void operCounter(char *string, int **err) {
    int count = 0;
    int len = strlen(string);
    for (int i = 0; i < len && count < 2; i++) {
        if (findOper(string[i]) == 1) {
            count++;
        } else {
            count = 0;
        }
    }
    if (count > 1) {
        **err = 4;
    }
}

int findOper(char oper) {
    int find = 0;
    if (oper == '+' || oper == '-' || oper == '/' || oper == '*' || oper == '^')
        find = 1;
    return find;
}

int findUnar(char un) {
    int find = 0;
    if (un == 'u' || un == 'p' || un == 'k' || un == 'b') {
        find = 1;
    }
    return find;
}

int findFunc(char oper) {
    int find = 0;
    if (oper == 's' || oper == 'c' || oper == 't' || oper == 'a' || oper == 'l' || oper == 'm') {
        find = 1;
    }
    return find;
}

void checkFunc(char *func, int **err) {
    **err = 5;
    char *token;
    char temp[] = "sin cos tan asin acos atan sqrt ln log mod u p b k";
    token = strtok(temp, " ");
    while (token != NULL) {
        if (strcmp(func, token) == 0)
            **err = 0;
        token = strtok(NULL, " ");
    }
}


void getFunc(char **string, char **func) {
    char *funcString = "sincotaqrlgmd";
    int num;
    char arr[256] = "\0";
    num = strspn(*string, funcString);
    strncpy(arr, *string, num);
    strncpy(*func, arr, strlen(arr));
    *string += num;
}

Node *getOper(char **string, char *out_string, Node *list) {
    if (isEmptyOper(list) == 0) {
        pushOperator(&list, *string[0]);
        *string += 1;
    } else {
        int count = 0;
        while ((count == 0) && (prior(*string[0]) <= prior(list->data))) {
            char buf;
            buf = popOperator(&list);
            out_string[strlen(out_string)] = buf;
            snprintf(out_string + strlen(out_string),
                    sizeof(out_string + strlen(out_string)) + 250, "%s", " ");
            if (isEmptyOper(list) == 0) {
                count = 1;
            }
        }
        pushOperator(&list, *string[0]);
        *string += 1;
    }
    return list;
}

int prior(char oper) {
    int rank = 0;
    if (oper == 'k' || oper == 'b') {
        rank = 5;
    } else if (oper == 's' || oper == 'c' || oper == 't' || oper == 'i' || oper == 'o'
                || oper == 'a' || oper == 'q' || oper == 'n' || oper == 'l') {
        rank = 4;
    } else if (oper == '^' || oper == 'u' || oper == 'm') {
        rank = 3;
    } else if (oper == '/' || oper == '*' || oper == 'm') {
        rank = 2;
    } else if (oper == '-' || oper == '+') {
        rank = 1;
    }
    return rank;
}

int findBracket(char oper, int *openBracket, int *closeBracket) {
    int bracket = 0;
    if (oper == '(') {
        (*openBracket)++;
        bracket = 1;
    }
    if (oper == ')') {
        (*closeBracket)++;
        bracket = 1;
    }
    return bracket;
}

Node *getBracket(char **string, char *out_string, int **err, Node *list, int openBracket, int closeBracket) {
    if (openBracket < closeBracket)
        **err  = 2;
    if (**err == 0) {
        if (*string[0] == '(')
            pushOperator(&list, *string[0]);
        if (*string[0] == ')') {
            int empty = 0;
            while (empty == 0 && list->data != '(') {
                char buf;
                buf = popOperator(&list);
                out_string[strlen(out_string)] = buf;
                snprintf(out_string + strlen(out_string),
                        sizeof(out_string + strlen(out_string)) + 250, "%s", " ");
                if (isEmptyOper(list) == 0)
                    empty = 1;
            }
            if (isEmptyOper(list) == 1) {
                popOperator(&list);
            }
        }
        *string += 1;
    }
    return list;
}

void modFun(char *string, int **err) {
    if (string[0] == 'm') {
        **err = 6;
    } else {
        for (size_t i = 1; i < strlen(string); i++) {
            if (string[i] == 'm' && (string[i - 1] == '-' || string[i - 1] == '+' ||
                                string[i - 1] == '*' || string[i - 1] == '/' ||
                                string[i - 1] == 'u' || string[i - 1] == 'p' ||
                                string[i - 1] == '(')) {
                **err = 6;
            }
            if (string[i] == 'd' && (string[i + 1] == '-' || string[i + 1] == '+' ||
                                    string[i + 1] == '*' || string[i + 1] == '/' ||
                                    string[i + 1] == 'u' || string[i + 1] == 'p' ||
                                    string[i + 1] == ')')) {
                **err = 6;
            }
        }
    }
}

Node *pushFun(char *fun, char *out_string, Node *list) {
    char findFunc = 0;
    findFunc = convert(fun);
    if (isEmptyOper(list) == 0) {
        pushOperator(&list, findFunc);
    } else {
        int empty = 0;
        while ((empty == 0) && (prior(findFunc) <= prior(list->data))) {
            char buf;
            buf = popOperator(&list);
            out_string[strlen(out_string)] = buf;
            snprintf(out_string + strlen(out_string),
                    sizeof(out_string + strlen(out_string)) + 250, "%s", " ");
            if (isEmptyOper(list) == 0) {
                empty = 1;
            }
        }
        pushOperator(&list, findFunc);
    }
    return list;
}

char convert(char *fun) {
    char findFunc = '0';
    if (strcmp(fun, "sin") == 0) findFunc = 's';
    if (strcmp(fun, "cos") == 0) findFunc = 'c';
    if (strcmp(fun, "tan") == 0) findFunc = 't';
    if (strcmp(fun, "asin") == 0) findFunc = 'i';
    if (strcmp(fun, "acos") == 0) findFunc = 'o';
    if (strcmp(fun, "atan") == 0) findFunc = 'a';
    if (strcmp(fun, "sqrt") == 0) findFunc = 'q';
    if (strcmp(fun, "ln") == 0) findFunc = 'n';
    if (strcmp(fun, "log") == 0) findFunc = 'l';
    if (strcmp(fun, "mod") == 0) findFunc = 'm';
    if (strcmp(fun, "u") == 0) findFunc = 'u';
    if (strcmp(fun, "p") == 0) findFunc = 'p';
    if (strcmp(fun, "k") == 0) findFunc = 'k';
    if (strcmp(fun, "b") == 0) findFunc = 'b';
    return findFunc;
}

void unaryGet(char **string, char **fun) {
    char arr[256] = "\0";
    strncpy(arr, *string, 1);
    strncpy(*fun, arr, 1);
    *string += 1;
}

// int main() {
//     int err = 0;

//     // char *string = "tan(10)*(ln(4))";
//     // char *string = "tan(10)*((5-3)*ln(4)-log(8))*2+7";
//     char *string = "sin(-4)";
//     // char *string = "cos(12)+sin(30)-tan(14)*ln(29)+log(12)+(32*3)-3^3-sqrt(sqrt(12))";
//     // char *string = "ln(10)";
//     // char *string = "(15-5)mod(5^3)";
//     // char *string = "56+21-376+(56-22)-(13+10)+(11+(7-(3+2)))";
//     // char *str = "10mod1";
//     printf("string: %s\n", string);
//     char * out_res = (char *)malloc(sizeof(char) * 1000);
//     double result;
//     parser(string, &err, out_res);
//     printf("out_str: %s\n", out_res);
//     if (err == 0) {
//         result = polishNotation(out_res, &err);
//         printf("result: %f\n", result);
//     } else {
//         printf("error: %d\n", err);
//     }

//     return 0;
// }

