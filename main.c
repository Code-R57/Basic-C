#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
    
#define MAX_TOKENS (1000000)
#define MAX_LINE_LENGTH (1000000)
#define EXIT_SUCCESS 0
    
char tokens[MAX_TOKENS][1000];
int num_tokens;
    
struct node {
    char* value;
    struct node* children[1000];
    int num_child;
};
    
struct variable {
    char variable_name[1000];
    int value;
};
    
struct variable variables[1000];
int num_variables = 0;
    
// Insert Node in the Syntax Tree
struct node* insertNode(char* value) {
    struct node* newNode = (struct node*) malloc(sizeof(struct node));
    
    newNode->value = value;
    newNode->num_child = 0;
    
    for(int i=0; i<1000; i++) {
        newNode->children[i] = NULL;
    }
    
    return newNode;
}
    
// Search if the Variable is Declared
int searchVariable(char *v) {
    for(int i=0; i<num_variables; i++) {
        if(strcmp(variables[i].variable_name, v) == 0) {
            return i;
        }
    }
    
    return -1;
}
    
/**
Reads the passed input file line by line.
Removes all space type characters from the lines, including \n (Carriage Return). 
Modifies the global tokens and num_tokens variables. 
*/
void tokenize(char* filename) {
    
    FILE* in_file = fopen(filename, "r");
    char line[MAX_TOKENS];
    char token[1000];
    int j=0;
    
    memset(token, 0, 1000);
    
    while (fgets(line, MAX_LINE_LENGTH, in_file)) {        
        for (int i = 0; i < strlen(line); ++i) {
            if(line[i] == '=') {
                if(j != 0 && token[0] != '\0') {
                    strcpy(tokens[num_tokens], token);
                    memset(token, 0, strlen(token));
                    num_tokens++;
                    j=0;
                }
    
                if(i+1 < strlen(line) && line[i+1] == '=') {
                    token[j] = line[i];
                    token[j+1] = line[i+1];
                    i++;
                    j+=2;
                }
                else {
                    token[j] = line[i];
                    j++;
                }
    
                strcpy(tokens[num_tokens], token);
                memset(token, 0, strlen(token));
                num_tokens++;
                j=0;
            }
            else if(line[i] == '(' || line[i] == ')' || line[i] == ','  || line[i] == ';' || line[i] == '>' || line[i] == '+' || line[i] == '*' || line[i] == '-' || line[i] == '/') {
                if(j != 0 && token[0] != '\0') {
                    strcpy(tokens[num_tokens], token);
                    num_tokens++;
                    memset(token, 0, strlen(token));
                    j=0;
                }
                tokens[num_tokens][0] = line[i];
                num_tokens++;
            }
            else if (line[i] == ' ' || line[i] == '\t' || line[i] == 10 /* ASCII 10 = Line Feed */ || line[i] == '\n') { 
                if(j != 0 && token[0] != '\0') {
                    strcpy(tokens[num_tokens], token);
                    memset(token, 0, strlen(token));
                    num_tokens++;
                    j=0;
                } 
            }
            else {
                token[j]=line[i];
                j++; 
            }
        }
    }

    if(strlen(token) != 0) {
        strcpy(tokens[num_tokens], token);
        memset(token, 0, strlen(token));
        num_tokens++;
        j=0;
    }
}

// Function Prototypes for Parsing  
int for_loop(struct node* root, int ind);
int operand(struct node* root, int ind, int end);
int factor(struct node* root, int ind, int end);
int term(struct node* root, int ind, int end);
int expression(struct node* root, int ind, int end);
int assignment(struct node* root, int ind);
int letter(struct node* root, char alphabet);
int var(struct node* root, int ind, int len);
int vars(struct node* root, int ind, int end);
int declaration(struct node* root, int ind);
int integer(struct node* root, char num);
int integer_const(struct node* root, int ind, int len);
int read(struct node* root, int ind);
int write(struct node* root, int ind);
int statement(struct node* root, int ind);
int statements(struct node* root, int ind, int end);
int program(struct node* root, int ind);

// Function Prototypes for Evaluation
int for_loop_evaluate(struct node* root);
int operand_evaluate(struct node* root);
int factor_evaluate(struct node* root);
int term_evaluate(struct node* root);
int expression_evaluate(struct node* root);
int assignment_evaluate(struct node* root);
char* letter_evaluate(struct node* root);
char* var_evaluate(struct node* root);
char* integer_evaluate(struct node* root);
char* integer_const_evaluate(struct node* root);
int read_evaluate(struct node* root);
int write_evaluate(struct node* root);
int statement_evaluate(struct node* root);
int statements_evaluate(struct node* root);
int program_evaluate(struct node* root);

// Print the Syntax Tree  
void printSyntaxTree(struct node* root, FILE *out) {

    if(root == NULL) {
        return;
    }
    
    fprintf(out, "[ %s ", root->value);
    
    for(int i=0; i<root->num_child; i++) {
        printSyntaxTree(root->children[i], out);
    }
    
    fprintf(out, "]");
}
    
int main(int argc, char** argv) {
    
    tokenize(argv[1]);
    
    struct node* root = insertNode("<program>");
    
    program(root, 0);
    
    FILE *out = fopen("parse_tree.txt", "w");
    printSyntaxTree(root, out);
    fclose(out);

    printf("\n\n/------------SIMULATION OF THE CODE------------/\n");

    program_evaluate(root);
    
    return EXIT_SUCCESS;
}

// Parsing

int letter(struct node* root, char alphabet) {
    char *ch = (char *) calloc(2, sizeof(char));
    
    ch[0] = alphabet;
    ch[1] = '\0';
    
    root->children[root->num_child] = insertNode(ch);
    root->num_child++;
    
    return 1;
}
    
int var(struct node* root, int ind, int len) {

    if (searchVariable(tokens[ind]) == -1) {
        printf("\nSyntax Error - Variable declaration not found - %s", tokens[ind]);
        exit(EXIT_FAILURE);
    }

    if(len == 1) {
        root->children[root->num_child] = insertNode("<letter>");
        root->num_child++;
        return letter(root->children[root->num_child - 1], tokens[ind][len-1]);
    }

    if (strcmp(tokens[ind], "for") == 0) {
        printf("\nSyntax Error - for Keyword Used");
        exit(EXIT_FAILURE);
    }
    else if (strcmp(tokens[ind], "int") == 0)
    {
        printf("\nSyntax Error - int Keyword Used");
        exit(EXIT_FAILURE);
    }
    else if (strcmp(tokens[ind], "read") == 0)
    {
        printf("\nSyntax Error - read Keyword Used");
        exit(EXIT_FAILURE);
    }
    else if (strcmp(tokens[ind], "write") == 0)
    {
        printf("\nSyntax Error - write Keyword Used");
        exit(EXIT_FAILURE);
    }

    if(isalpha(tokens[ind][len-1]) == 0 || islower(tokens[ind][len-1]) == 0) {
        printf("\nSyntax Error - var should be lowercase character - var");
        exit(EXIT_FAILURE);
    }
    
    root->children[root->num_child] = insertNode("<var>");
    root->num_child++;
    var(root->children[root->num_child - 1], ind, len-1);
    
    root->children[root->num_child] = insertNode("<letter>");
    root->num_child++;
    letter(root->children[root->num_child - 1], tokens[ind][len-1]);
    
    return ind+1;
}
    
int vars(struct node* root, int ind, int end) {
    if (ind > end) {
        printf("\nSyntax Error - Wrong declaration - vars");
        exit(EXIT_FAILURE);
    }

    if(ind == end) {
        root->children[root->num_child] = insertNode("<var>");
        root->num_child++;
        
        strcpy(variables[num_variables].variable_name, tokens[end]);
        variables[num_variables].value = 0;
        num_variables++;

        var(root->children[root->num_child - 1], ind, strlen(tokens[ind]));

        return end;
    }
    
    root->children[root->num_child] = insertNode("<vars>");
    root->num_child++;
    vars(root->children[root->num_child - 1], ind, end-2);
    
    if (strcmp(tokens[end - 1], ",")) {
        printf("\nSyntax Error - seperate variables by comma - vars");
        exit(0);
    }

    root->children[root->num_child] = insertNode(",");
    root->num_child++;
    
    root->children[root->num_child] = insertNode("<var>");
    root->num_child++;
    
    strcpy(variables[num_variables].variable_name, tokens[end]);
    variables[num_variables].value = 0;
    num_variables++;
    
    var(root->children[root->num_child - 1], end, strlen(tokens[end]));
    
    return end;
}
    
int declaration(struct node* root, int ind) {
    int vars_end = ind;
    
    while(vars_end < num_tokens && strcmp(tokens[vars_end], ";") != 0) {
        vars_end++;
    }

    if (vars_end == num_tokens) {
        printf("\nSyntax Error - Semicolon not found in declaration.");
        exit(EXIT_FAILURE);
    }
    
    root->children[root->num_child] = insertNode("int");
    root->num_child++;
    
    root->children[root->num_child] = insertNode("<vars>");
    root->num_child++;
    vars(root->children[root->num_child - 1], ind+1, vars_end-1);
    
    return vars_end;
}
    
int integer(struct node* root, char num) {
    char *digit = (char *) calloc(2, sizeof(char));
    
    digit[0] = num;
    digit[1] = '\0';
    
    root->children[root->num_child] = insertNode(digit);
    root->num_child++;
    
    return 1;
}
    
int integer_const(struct node* root, int ind, int len) {
    if(len == 1) {
        root->children[root->num_child] = insertNode("<integer>");
        root->num_child++;
        return integer(root->children[root->num_child - 1], tokens[ind][len-1]);
    }
    
    root->children[root->num_child] = insertNode("<integer_const>");
    root->num_child++;
    integer_const(root->children[root->num_child - 1], ind, len-1);
    
    if(isdigit(tokens[ind][len-1]) == 0) {
        printf("\nSyntax Error - integer should have numbers - integer_const");
        exit(EXIT_FAILURE);
    }

    root->children[root->num_child] = insertNode("<integer>");
    root->num_child++;
    integer(root->children[root->num_child - 1], tokens[ind][len-1]);
    
    return 1;
}
    
int read(struct node* root, int ind) {

    if (ind + 1 >= num_tokens || searchVariable(tokens[ind + 1]) == -1) {
        printf("\nSyntax Error - Variable not defined - read");
        exit(EXIT_FAILURE);
    }

    if (ind + 2 >= num_tokens || strcmp(tokens[ind + 2], ";") != 0) {
        printf("\nSyntax Error - Semicolon missing - read");
        exit(EXIT_FAILURE);
    }

    root->children[root->num_child] = insertNode("read");
    root->num_child++;
    
    root->children[root->num_child] = insertNode("<var>");
    root->num_child++;
    var(root->children[root->num_child - 1], ind+1, strlen(tokens[ind+1]));
    
    return ind+1;
}
    
int write(struct node* root, int ind) {

    if (ind + 2 >= num_tokens || strcmp(tokens[ind + 2], ";") != 0) {
        printf("\nSyntax Error - Semicolon missing - write");
        exit(EXIT_FAILURE);
    }

    root->children[root->num_child] = insertNode("write");
    root->num_child++;

    if (ind + 1 >= num_tokens) {
        printf("\nSyntax Error - Write value missing - write");
        exit(EXIT_FAILURE);
    }
    
    if(isdigit(tokens[ind+1][0])) {
        root->children[root->num_child] = insertNode("<integer_const>");
        root->num_child++;
        return ind + integer_const(root->children[root->num_child - 1], ind+1, strlen(tokens[ind+1]));
    }
    else if(isalpha(tokens[ind+1][0])) {
        if (searchVariable(tokens[ind + 1]) == -1) {
            printf("\nSyntax Error - Variable not defined - write");
            exit(EXIT_FAILURE);
        }

        root->children[root->num_child] = insertNode("<var>");
        root->num_child++;
        return ind + var(root->children[root->num_child - 1], ind+1, strlen(tokens[ind+1]));
    }
    else {
        printf("\nSyntax Error - Write should only be variable or constant - write");
        exit(EXIT_FAILURE);
    }
    
    return -1;
}
    
int for_loop(struct node* root, int ind) {
    root->children[root->num_child] = insertNode("for");
    root->num_child++;

    if(ind+1>=num_tokens || strcmp(tokens[ind+1], "(") != 0) {
        printf("\nSyntax Error - '(' missing  - for_loop");
        exit(EXIT_FAILURE);
    }
    
    root->children[root->num_child] = insertNode("(");
    root->num_child++;
    
    root->children[root->num_child] = insertNode("<assignment>");
    root->num_child++;
    assignment(root->children[root->num_child - 1], ind+2);
    
    root->children[root->num_child] = insertNode(";");
    root->num_child++;
    
    int expression_ind = ind+1;
    
    while(expression_ind < num_tokens && strcmp(tokens[expression_ind], ";") != 0) {
        expression_ind++;
    }
    
    int expression_end = expression_ind+1;
    
    while(expression_end < num_tokens && strcmp(tokens[expression_end], ";") != 0) {
        expression_end++;
    }
    
    root->children[root->num_child] = insertNode("<expression>");
    root->num_child++;
    expression(root->children[root->num_child - 1], expression_ind+1, expression_end);
    
    root->children[root->num_child] = insertNode(";");
    root->num_child++;
    
    root->children[root->num_child] = insertNode("<assignment>");
    root->num_child++;
    assignment(root->children[root->num_child - 1], expression_end+1);

    root->children[root->num_child] = insertNode(")");
    root->num_child++;
    
    int for_body_start = expression_end;
    
    while(for_body_start < num_tokens && strcmp(tokens[for_body_start], "{") != 0) {
        for_body_start++;
    }

    if(strcmp(tokens[for_body_start], "{") != 0) {
        printf("\nSyntax Error - '{' missing - for_loop");
        exit(EXIT_FAILURE);
    }

    if(strcmp(tokens[for_body_start-1], ")") != 0) {
        printf("\nSyntax Error - ')' missing - for_loop");
        exit(EXIT_FAILURE);
    }

    root->children[root->num_child] = insertNode("{");
    root->num_child++;
    
    int for_body_end = for_body_start+1;
    
    while(for_body_end < num_tokens && strcmp(tokens[for_body_end], "}") != 0) {
        for_body_end++;
    }

    if(strcmp(tokens[for_body_end], "}") != 0) {
        printf("\nSyntax Error - '}' missing  - for_loop");
        exit(EXIT_FAILURE);
    }
    
    root->children[root->num_child] = insertNode("<statements>");
    root->num_child++;
    statements(root->children[root->num_child - 1], for_body_start+1, for_body_end-1);
    
    root->children[root->num_child] = insertNode("}");
    root->num_child++;

    if(for_body_end+1 >= num_tokens || strcmp(tokens[for_body_end+1], ";") != 0) {
        printf("\nSyntax Error - ';' missing  - for_loop");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
    
int operand(struct node* root, int ind, int end) {
    if(isdigit(tokens[ind][0])) {
        root->children[root->num_child] = insertNode("<integer_const>");
        root->num_child++;
        return integer_const(root->children[root->num_child - 1], ind, strlen(tokens[ind]));
    }
    else if(isalpha(tokens[ind][0])) {
        root->children[root->num_child] = insertNode("<var>");
        root->num_child++;
        return var(root->children[root->num_child - 1], ind, strlen(tokens[ind]));
    }
    else if(strcmp(tokens[ind], "(") == 0 && strcmp(tokens[end-1], ")") == 0) {
        if (ind + 1 == end - 1) {
            printf("\nSyntax Error - operand not specified - operand");
            exit(EXIT_FAILURE);
        }
        
        root->children[root->num_child] = insertNode("(");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<expression>");
        root->num_child++;
        expression(root->children[root->num_child - 1], ind+1, end-1);
    
        root->children[root->num_child] = insertNode(")");
        root->num_child++;
    }
    else {
        printf("\nSyntax Error - Undefined operand use - operand");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
    
int factor(struct node* root, int ind, int end) {
    int factorEnd = end-1;
    
    int bracketCount = 0;

    for(int i = end-1; i >= ind; i--) {
        if(strcmp(tokens[i], ")") == 0) {
            bracketCount++;
        }
        else if(strcmp(tokens[i], "(") == 0) {
            bracketCount--;
        }

        if(bracketCount == 0 && (strcmp(tokens[i], "*") == 0 || strcmp(tokens[i], "/") == 0)) {
            factorEnd = i;
            break;
        }

        if(ind == i) {
            factorEnd = ind;
            break;
        }
    }
    
    if(ind == factorEnd) {
        root->children[root->num_child] = insertNode("<operand>");
        root->num_child++;
        return operand(root->children[root->num_child - 1], ind, end);
    }
    else if(strcmp(tokens[factorEnd], "*") == 0) {
        root->children[root->num_child] = insertNode("<factor>");
        root->num_child++;
        factor(root->children[root->num_child - 1], ind, factorEnd);
    
        root->children[root->num_child] = insertNode("*");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<operand>");
        root->num_child++;
        operand(root->children[root->num_child - 1], factorEnd+1, end);
    }
    else if(strcmp(tokens[factorEnd], "/") == 0) {
        root->children[root->num_child] = insertNode("<factor>");
        root->num_child++;
        factor(root->children[root->num_child - 1], ind, factorEnd);
    
        root->children[root->num_child] = insertNode("/");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<operand>");
        root->num_child++;
        operand(root->children[root->num_child - 1], factorEnd+1, end);
    }
    else {
        printf("\nSyntax Error - Undefined operator use - factor");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
    
int term(struct node* root, int ind, int end) {
    int termEnd = end-1;
    
    int bracketCount = 0;

    for(int i = end-1; i >= ind; i--) {
        if(strcmp(tokens[i], ")") == 0) {
            bracketCount++;
        }
        else if(strcmp(tokens[i], "(") == 0) {
            bracketCount--;
        }

        if(bracketCount == 0 && (strcmp(tokens[i], "+") == 0 || strcmp(tokens[i], "-") == 0)) {
            termEnd = i;
            break;
        }

        if(ind == i) {
            termEnd = ind;
            break;
        }
    }
    
    if(ind == termEnd) {
        root->children[root->num_child] = insertNode("<factor>");
        root->num_child++;
        return factor(root->children[root->num_child - 1], ind, end);
    }
    else if(strcmp(tokens[termEnd], "+") == 0) {
        root->children[root->num_child] = insertNode("<term>");
        root->num_child++;
        term(root->children[root->num_child - 1], ind, termEnd);
    
        root->children[root->num_child] = insertNode("+");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<factor>");
        root->num_child++;
        factor(root->children[root->num_child - 1], termEnd+1, end);
    }
    else if(strcmp(tokens[termEnd], "-") == 0) {
        root->children[root->num_child] = insertNode("<term>");
        root->num_child++;
        term(root->children[root->num_child - 1], ind, termEnd);
    
        root->children[root->num_child] = insertNode("-");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<factor>");
        root->num_child++;
        factor(root->children[root->num_child - 1], termEnd+1, end);
    }
    else {
        printf("\nSyntax Error - Undefined operator use - term");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
    
int expression(struct node* root, int ind, int end) {
    int expressionEnd = end-1;

    int bracketCount = 0;

    for(int i = end-1; i >= ind; i--) {
        if(strcmp(tokens[i], ")") == 0) {
            bracketCount++;
        }
        else if(strcmp(tokens[i], "(") == 0) {
            bracketCount--;
        }

        if(bracketCount == 0 && (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], "==") == 0)) {
            expressionEnd = i;
            break;
        }

        if(ind == i) {
            expressionEnd = ind;
            break;
        }
    }
    
    if(ind == expressionEnd) {
        root->children[root->num_child] = insertNode("<term>");
        root->num_child++;
        return term(root->children[root->num_child - 1], ind, end);
    }
    else if(strcmp(tokens[expressionEnd], "==") == 0) {
        root->children[root->num_child] = insertNode("<expression>");
        root->num_child++;
        expression(root->children[root->num_child - 1], ind, expressionEnd);
    
        root->children[root->num_child] = insertNode("==");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<term>");
        root->num_child++;
        term(root->children[root->num_child - 1], expressionEnd+1, end);
    }
    else if(strcmp(tokens[expressionEnd], ">") == 0) {
        root->children[root->num_child] = insertNode("<expression>");
        root->num_child++;
        expression(root->children[root->num_child - 1], ind, expressionEnd);
    
        root->children[root->num_child] = insertNode(">");
        root->num_child++;
    
        root->children[root->num_child] = insertNode("<term>");
        root->num_child++;
        term(root->children[root->num_child - 1], expressionEnd+1, end);
    }
    else {
        printf("\nSyntax Error - undefined operator use - expression");
        exit(EXIT_FAILURE);
    }
    
    return 0;
} 
    
int assignment(struct node* root, int ind) {

    if (searchVariable(tokens[ind]) == -1) {
        printf("\nSyntax Error - Variable not defined - assignment");
        exit(EXIT_FAILURE);
    }
    if (strcmp(tokens[ind + 1], "=") != 0) {
        printf("\nSyntax Error - Wrong assignment '=' missing - assignment");
        exit(EXIT_FAILURE);
    }

    root->children[root->num_child] = insertNode("<var>");
    root->num_child++;
    var(root->children[root->num_child - 1], ind, strlen(tokens[ind]));
    
    root->children[root->num_child] = insertNode("=");
    root->num_child++;  
    
    int assgn_end = ind;
    
    while(assgn_end < num_tokens && strcmp(tokens[assgn_end], ";") != 0 && strcmp(tokens[assgn_end], "{") != 0) {
        assgn_end++;
    }

    if(strcmp(tokens[assgn_end], "{") == 0) {
        assgn_end--;
    }
    
    root->children[root->num_child] = insertNode("<expression>");
    root->num_child++;  
    return expression(root->children[root->num_child - 1], ind+2, assgn_end) + 2;
}
    
int statement(struct node* root, int ind) {
    int statement_end = ind;

    if(strcmp(tokens[ind], "read") == 0) {
        root->children[root->num_child] = insertNode("<read>");
        root->num_child++;
        statement_end = read(root->children[root->num_child - 1], ind);
    }
    else if(strcmp(tokens[ind], "write") == 0) {
        root->children[root->num_child] = insertNode("<write>");
        root->num_child++;
        statement_end = write(root->children[root->num_child - 1], ind);
    }
    else if(strcmp(tokens[ind], "for") == 0) {
        root->children[root->num_child] = insertNode("<for_loop>");
        root->num_child++;
        statement_end = for_loop(root->children[root->num_child - 1], ind);
    }
    else if(strcmp(tokens[ind+1], "=") == 0) {
        root->children[root->num_child] = insertNode("<assignment>");
        root->num_child++;
        statement_end = assignment(root->children[root->num_child - 1], ind);
    }
    else {
        printf("\nSyntax Error - Wrong statement use - statement");
        exit(EXIT_FAILURE);
    }
    
    return statement_end + 1;
}
    
int statements(struct node* root, int ind, int end) {
    int finalEnd = end-1;

    if(strcmp(tokens[end], ";") != 0) {
        printf("\nSyntax Error - Semicolon missing - statements");
        exit(EXIT_FAILURE);
    }

    if (ind > end) {
        printf("\nSyntax Error - No proper statement - statements");
        exit(EXIT_FAILURE);
    }
    
    while(finalEnd > ind && strcmp(tokens[finalEnd], ";") != 0 && strcmp(tokens[finalEnd], "{") != 0) {
        finalEnd--;
    }

    if(strcmp(tokens[finalEnd], "{") == 0) {
        finalEnd++;
    }

    if(strcmp(tokens[end-1], "}") == 0) {
        finalEnd = end-1;

        while(finalEnd > ind && strcmp(tokens[finalEnd], "for") != 0) {
            finalEnd--;
        }
    }
    
    if(ind == finalEnd) {
        root->children[root->num_child] = insertNode("<statement>");
        root->num_child++;
        statement(root->children[root->num_child - 1], ind);
    
        root->children[root->num_child] = insertNode(";");
        root->num_child++;
    
        return finalEnd;
    }

    if(strcmp(tokens[end-1], "}") == 0) {
        finalEnd = end-1;

        while(finalEnd > ind && strcmp(tokens[finalEnd], "for") != 0) {
            finalEnd--;
        }

        finalEnd--;
    }
    
    root->children[root->num_child] = insertNode("<statements>");
    root->num_child++;
    statements(root->children[root->num_child - 1], ind, finalEnd);
    
    root->children[root->num_child] = insertNode("<statement>");
    root->num_child++;
    statement(root->children[root->num_child - 1], finalEnd+1);
    
    root->children[root->num_child] = insertNode(";");
    root->num_child++;
    
    return end;
}
    
int program(struct node* root, int ind) {
    int decl_end = ind;
    
    if(strcmp(tokens[ind], "int") == 0) {
        root->children[root->num_child] = insertNode("<declaration>");
        root->num_child++;
        decl_end = declaration(root->children[root->num_child - 1], ind);
        root->children[root->num_child] = insertNode(";");
        root->num_child++;
    
        if(decl_end < num_tokens-1) {
            root->children[root->num_child] = insertNode("<statements>");
            root->num_child++;
            decl_end = statements(root->children[root->num_child - 1], decl_end+1, num_tokens-1);
        }
    }
    else {
        root->children[root->num_child] = insertNode("<statements>");
        root->num_child++;
        decl_end = statements(root->children[root->num_child - 1], ind, num_tokens-1);
    }

    return decl_end;
}


// Evaluation

char* letter_evaluate(struct node* root) {
    char *ch = "";
    
    ch = root->children[0]->value;
    
    return ch;
}
    
char* var_evaluate(struct node* root) {
    char *var_name = (char *) calloc(10, sizeof(char));

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<var>") == 0) {
            strcat(var_name, var_evaluate(root->children[i]));
        }
        else if(strcmp(root->children[i]->value, "<letter>") == 0) {
            strcat(var_name, letter_evaluate(root->children[i]));
        }
    }

    return var_name;
}
        
char* integer_evaluate(struct node* root) {
    char *digit = "";

    digit = root->children[0]->value;
    
    return digit;
}
    
char* integer_const_evaluate(struct node* root) {
    char *integer_value = (char*) calloc(1000, sizeof(char));

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<integer_const>") == 0) {
            strcat(integer_value, integer_const_evaluate(root->children[i]));
        }
        else if(strcmp(root->children[i]->value, "<integer>") == 0) {
            strcat(integer_value, integer_evaluate(root->children[i]));
        }
    }

    return integer_value;
}
    
int read_evaluate(struct node* root) {
    char *var_name = "";

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<var>") == 0) {
            var_name = var_evaluate(root->children[i]);
        }
    }

    int ind = searchVariable(var_name);

    if(ind != -1) {
        int num = 0;
        scanf("%d", &num);

        if(num < 0) {
            printf("\nRead -ve Integer - Not part of Grammar");
            exit(EXIT_FAILURE);
        }

        variables[ind].value = num;
    }
    
    return (ind != -1);
}
    
int write_evaluate(struct node* root) {
    int status = 1;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<var>") == 0) {
            char *var_name = "";
            var_name = var_evaluate(root->children[i]);

            int ind = searchVariable(var_name);

            if(ind != -1) {
                printf("%d\n", variables[ind].value);
            }   

            status &= (ind != -1);
        }
        else if(strcmp(root->children[i]->value, "<integer_const>") == 0) {
            char *integer_value = "";
            integer_value = integer_const_evaluate(root->children[i]);

            printf("%s\n", integer_value);

            status &= 1;
        }
    }

    return status;
}
    
int for_loop_evaluate(struct node* root) {
    int status = 1;

    int assignmentCount = 0;

    int assignmentInd = -1;
    int expressionInd = -1;
    int statementInd = -1;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<assignment>") == 0) {
            if(assignmentCount == 0) {
                status &= assignment_evaluate(root->children[i]);
                assignmentCount++;
            }
            else if(assignmentCount == 1) {
                assignmentInd = i;
                assignmentCount++;
            }
        }
        else if(strcmp(root->children[i]->value, "<expression>") == 0) {
            expressionInd = i;
        }
        else if(strcmp(root->children[i]->value, "<statements>") == 0) {
            statementInd = i;
        }
    }

    while(expression_evaluate(root->children[expressionInd]) != 0) {
        status &= statements_evaluate(root->children[statementInd]);
        status &= assignment_evaluate(root->children[assignmentInd]);
    }

    return status;
}
    
int operand_evaluate(struct node* root) {

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<expression>") == 0) {
            return expression_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<integer_const>") == 0) {
            return atoi(integer_const_evaluate(root->children[i]));
        }
        else if(strcmp(root->children[i]->value, "<var>") == 0) {
            char* var_name = "";
            var_name = var_evaluate(root->children[i]);

            int ind = searchVariable(var_name);

            if(ind != -1) {
                return variables[ind].value;
            }
        }
    }

    return -1;
}
    
int factor_evaluate(struct node* root) {
    char *operator = "";
    int factorValue = 0;
    int operandValue = 0;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<factor>") == 0) {
            factorValue = factor_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<operand>") == 0) {
            operandValue = operand_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "*") == 0 || strcmp(root->children[i]->value, "/") == 0) {
            operator = root->children[i]->value;
        }
    }
    
    if(strcmp(operator, "*") == 0) {
        return (factorValue * operandValue);
    }
    else if(strcmp(operator, "/") == 0) {
        if(operandValue == 0) {
            printf("\nException: Divide by 0");
            exit(EXIT_FAILURE);
        }

        return (factorValue / operandValue);
    }
    else if(strcmp(operator, "") == 0) {
        return operandValue;
    }

    return -1;
}
    
int term_evaluate(struct node* root) {
    char *operator = "";
    int termValue = 0;
    int factorValue = 0;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<term>") == 0) {
            termValue = term_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<factor>") == 0) {
            factorValue = factor_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "+") == 0 || strcmp(root->children[i]->value, "-") == 0) {
            operator = root->children[i]->value;
        }
    }
    
    if(strcmp(operator, "+") == 0) {
        return (termValue + factorValue);
    }
    else if(strcmp(operator, "-") == 0) {
        return (termValue - factorValue);
    }
    else if(strcmp(operator, "") == 0) {
        return factorValue;
    }

    return -1;
}
    
int expression_evaluate(struct node* root) {
    char *operator = "";
    int expressionValue = 0;
    int termValue = 0;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<expression>") == 0) {
            expressionValue = expression_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<term>") == 0) {
            termValue = term_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "==") == 0 || strcmp(root->children[i]->value, ">") == 0) {
            operator = root->children[i]->value;
        }
    }
    
    if(strcmp(operator, "==") == 0) {
        return (termValue == expressionValue);
    }
    else if(strcmp(operator, ">") == 0) {
        return (expressionValue > termValue);
    }
    else if(strcmp(operator, "") == 0) {
        return termValue;
    }

    return -1;
} 
    
int assignment_evaluate(struct node* root) {
    char *var_name = "";
    int value = 0;

    int status = 0;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<var>") == 0) {
            var_name = var_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<expression>") == 0) {
            value = expression_evaluate(root->children[i]);
        }
    }

    int ind = searchVariable(var_name);

    if(ind != -1) {
        variables[ind].value = value;
        status = 1;
    }

    return status;
}
    
int statement_evaluate(struct node* root) {
    int status = 1;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<read>") == 0) {
            status &= read_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<write>") == 0) {
            status &= write_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<for_loop>") == 0) {
            status &= for_loop_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<assignment>") == 0) {
            status &= assignment_evaluate(root->children[i]);
        }
    }

    return status;
}
    
int statements_evaluate(struct node* root) {
    int status = 1;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<statements>") == 0) {
            status &= statements_evaluate(root->children[i]);
        }
        else if(strcmp(root->children[i]->value, "<statement>") == 0) {
            status &= statement_evaluate(root->children[i]);
        }
    }

    return status;
}
    
int program_evaluate(struct node* root) {
    int status = 1;

    for(int i=0; i<root->num_child; i++) {
        if(strcmp(root->children[i]->value, "<statements>") == 0) {
            status &= statements_evaluate(root->children[i]);
        }
    }

    return status;
}