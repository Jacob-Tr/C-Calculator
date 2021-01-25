#include <iostream>
#include <stdlib.h> // C
#include <stdio.h> // C
#include <ctype.h> // C
using namespace std;

#define MAX_OPERATION 100

#define OPER_NULL 0
#define OPER_ADDSUB 1
#define OPER_MULDIV 2
#define OPER_EXP 3
#define OPER_UNRY 4
#define OPER_BRKT 5
#define OPER_EQL 6

char oper[MAX_OPERATION];
int operation_index = 0;

int user_input = -0;
int var = -0;

void ToString(const double value, char* string) {sprintf(string, "%f", value);}

int IsOperator(const char ch)
{
    if(ch == '=') return OPER_EQL;
    if(ch == '(' || ch == ')') return OPER_BRKT;
    if(ch == '√' || ch == '!' || ch == 's' || ch == 'c' || ch == 't' || ch == 'S' || ch == 'C' || ch == 'T') return OPER_UNRY;
    if(ch == '^') return OPER_EXP;
    if(ch == '*' || ch == '/') return OPER_MULDIV;
    if(ch == '+' || ch == '-') return OPER_ADDSUB;
    return OPER_NULL;
}

int IsNumeric(char* string)
{
   // if(isdigit(string)) return 1;
    for(int i = 0; i < strlen(string); i++) 
    {
        if(string[i] == '.' || isdigit(string[i])) continue;
        else if(i == 0 && string[i] == '-') continue;
        return 0;
    }
    
    return 1;
}

int IsCharNumeric(const char ch)
{
    if(ch == '.' || isdigit(ch) || ch == '-') return 1;
    return 0;
}

void CPPFunction();

int CountOpenBrackets(const char* string)
{
    int brackets = 0;
    for(int i = 0; i < strlen(string); i++) if(string[i] == '(') brackets++;

    return brackets;
}

int CountCloseBrackets(const char* string)
{
    int brackets = 0;
    for(int i = 0; i < strlen(string); i++) if(string[i] == ')') brackets++;
    
    return brackets;
}

int IsNegativeDeclarator(const char* string, int index)
{
    if(string[index] != '-') return 0;
    if(index <= strlen(string)) return 0;
    if(IsCharNumeric(string[index-1])) return 0;
    
    return 1;
}

int IsPossibleOperation(const char* string)
{
    for(int i = 0; i < strlen(string); i++)
    {
        if(IsOperator(string[i]) || IsCharNumeric(string[i])) continue;
        return 0;
    }
    return 1;
}

int IsValidOperation(const char* string)
{
    int open_brackets = CountOpenBrackets(string), close_brackets = CountCloseBrackets(string), brackets = 0;
    
    if(open_brackets != close_brackets) return 0;
    if(!strlen(string) > 2) return 0;

    for(int i = 0; i < strlen(string); i++)
    {
        int is_oper = IsOperator(string[i]), subs;
        if(is_oper == OPER_ADDSUB && string[i] == '-') {if(++subs == 3) return 0;}
        else if(subs > 0) subs = 0;
        
        if(is_oper != OPER_NULL)
        {
            if(is_oper == OPER_BRKT)
            {
                if(string[i] == '(') 
                {
                    if(i == strlen(string)) return 0;
                    brackets++;
                }
                else brackets--;
            }
            else
            {
                if(i == strlen(string)) return 0;
                if(is_oper == OPER_UNRY && i > 0 && (IsCharNumeric(string[i - 1]) || string[i - 1] != '(')) return 0;
                if(!IsCharNumeric(string[i+1]) && string[i+1] != '(' && string[i+1] != '-') return 0;
            }
            if(brackets < 0) return 0;
        }
        else if(!IsCharNumeric(string[i])) return 0;
    }

    if(brackets != 0) return 0;
    return 1;
}

void FindInnermostBracket(char* string, int* opening, int* closing)
{
    int nested = -0, highest_nested = -0, high_nest_pos = -0;
    
    for(int i = 0; i < strlen(string); i++)
    {
        int is_oper = IsOperator(string[i]);
        if(is_oper == OPER_BRKT)
        {
            if(string[i] == '(')
            {
                if(++nested > highest_nested)
                {
                    high_nest_pos = i;
                    highest_nested = nested;
                }
            }
            else nested--;
            continue;
        }
    }
    
    int closing_bracket_pos = -0;
    
    if(high_nest_pos != -0)
    {
        for(int i = high_nest_pos; i < strlen(string); i++)
        {
            if(string[i] == ')')
            {
                closing_bracket_pos = i;
                break;
            }
        }
    }
    
    *opening = high_nest_pos;
    *closing = closing_bracket_pos;
    return;
}

double GetForwardingDigit(char* string, int loc, int* begin)
{
    char digit[32];
    
    for(int i = loc > 0 ? loc - 1 : -1; i >= 0; i--) 
    {
        if(i == -1)
        {
            sprintf(digit, "-0.0");
            break;
        }
        if((!IsCharNumeric(string[i]) && !IsNegativeDeclarator(string, i)) || i == 0) 
        {
            *begin = i == 0 ? i : i + 1;
            for(int ii = *begin; ii < loc; ii++) sprintf(digit, "%s%c", digit, string[ii]);
            if(strlen(digit) == 0) sprintf(digit, "%f", -0.0); 
            break;
        }
        continue;
    }
    
    return strtod(digit, NULL);
}

double GetFollowingDigit(char* string, int loc, int* end)
{
    char digit[32];
    
    for(int i = (loc + 1); i < strlen(string); i++)
    {
        if(!IsCharNumeric(string[i]) && (string[i] != '-' || IsOperator(string[i-1])) || i == strlen(string)) 
        {
            *end = i - 1;
            if(strlen(digit) == 0) sprintf(digit, "%f", -0.0);
            break;
        }
        else sprintf(digit, "%s%c", digit, string[i]);
    }
    
    printf("%s\n", digit);
    
    return strtod(digit, NULL);
}

void GetForwardingDigitAsCharArray(char* dest, char* string, int loc, int* end) {ToString(GetForwardingDigit(string, loc, end), dest);}
void GetFollowingDigitAsCharArray(char* dest, char* string, int loc, int* begin) {ToString(GetFollowingDigit(string, loc, begin), dest);}

int GetPower(const double value, int power)
{
    int num = value;
    for(int i = 1; i <= power; i++) num *= value;
    return num;
}

void RemoveChar(char* string, int index) 
{
    char* dest;
    for(int i = 0; i < strlen(string); i++) 
    {
        if(i != index) 
        sprintf(dest, "%s%c", dest, string[i]);
    }
    
    sprintf(string, "%s", dest);
}

double SolveOperation(char* string, char* operation)
{
    if(!IsValidOperation(string)) return -0.0;
    sprintf(operation, "%s", string);
    int brackets = CountOpenBrackets(string);
    
    if(brackets > 0)
    {
        char* inner_operation;
        int opening, closing;
        FindInnermostBracket(operation, &opening, &closing);
        
        for(int i = (opening + 1); i < closing; i++) sprintf(inner_operation, "%s%c", inner_operation, string[i]);
        RemoveChar(operation, opening);
        RemoveChar(operation, closing);
        
        SolveOperation(inner_operation, operation);
        brackets--;
    }
    
    printf("%s\n", operation);
    
    for(int i = 0; i < strlen(operation); i++)
    {
        int is_oper = IsOperator(operation[i]);
        if(is_oper != OPER_NULL)
        {
            printf("Test\n");
            int oper_begin, oper_end;
            double left_digit = GetForwardingDigit(operation, i, &oper_begin);
            double right_digit = GetFollowingDigit(operation, i, &oper_end);
            double solution;
            
            printf("%f:%f", left_digit, right_digit);
            if(is_oper == OPER_EXP) solution = GetPower(left_digit, right_digit);
            if(is_oper == OPER_MULDIV)
            {
                printf("Mul\n");
                if(operation[i] == '*') solution = (left_digit * right_digit);
                else solution == (left_digit / right_digit);
            }
            if(is_oper == OPER_ADDSUB)
            {
                printf("Add\n");
                if(operation[i] == '+') solution = (left_digit + right_digit);
                else solution = (left_digit - right_digit);
            }
            printf("Test: %f %d %f = %f\n", left_digit, is_oper, right_digit, solution);
            
            char* new_oper;
            for(int ii = 0; ii < strlen(operation); ii++)
            {
                if(ii < oper_begin || ii > oper_end) sprintf(new_oper, "%s%c", new_oper, operation[ii]);
                else
                {
                    printf("%s\n", operation);
                    char* soltn;
                    ToString(solution, soltn);
                    printf("%f\n", solution);
                    sprintf(new_oper, "%s%s", new_oper, soltn);
                    i = oper_end;
                }
            }
            printf("Test: %s", new_oper);
            //SolveOperation(new_oper);
        }
    }
    return 0.0;
}

void CFunction()
{
    printf("This is C.\n");
    char* solution;
    if(IsOperator(user_input) == OPER_EQL) printf("%c = %llf\n", oper, SolveOperation(oper, solution));
    else
    {
        var = user_input;
        printf("Input: %d\n", var);

        CPPFunction();
    }
    return;
}

void CPPFunction()
{
    cout << "This is C++.\n";
    cin >> user_input;
    
    if(!IsOperator(user_input) && !isdigit(user_input))
    {
        cout << "Variables not supported.";
        CPPFunction();
        return;
    }
    
    CFunction();
    return;
}

int main() 
{
    char* string = "(1+(-2.3)^4)+5";
    char* stringwobr = "5+5*5"; // String without brackets
    char answer[32];
    //CPPFunction();
    int end;
    
    //double digit = IsCharNumeric(string[1]);
    
    //printf(IsValidOperation(string) ? "True" : "False");
    
    double digit = GetForwardingDigit(stringwobr, 1, &end);
    //SolveOperation(stringwobr, answer);
    printf("%f", digit);
    return 0;
}
