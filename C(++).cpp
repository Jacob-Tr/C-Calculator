#include <iostream>
#include <stdlib.h> // C
#include <stdio.h> // C
#include <ctype.h> // C
using namespace std;

#define MAX_OPERATION 100

char oper[MAX_OPERATION];
int operation_index = 0;

int user_input = -0;
int var = -0;

void ToString(const double value, char* string) {sprintf(string, "%f", value);}

int IsOperator(const char ch)
{
    if(ch == '=') return 5;
    if(ch == '(' || ch == ')') return 4;
    if(ch == '^') return 3;
    if(ch == '*' || ch == '/') return 2;
    if(ch == '+' || ch == '-') return 1;
    return 0;
}

int IsNumeric(char* string)
{
   // if(isdigit(string)) return 1;
    for(int i = 0; i < strlen(string); i++) 
    {
        if(string[i] == '.' || isdigit(string[i])) continue;
        return 0;
    }
    
    return 1;
}

int IsCharNumeric(const char ch)
{
    if(ch == '.' || isdigit(ch)) return 1;
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
    if(!IsCharNumeric(string[0]) && string[0] != '(') return 0;
    if(!strlen(string) > 2) return 0;

    for(int i = 0; i < strlen(string); i++)
    {
        int is_oper = IsOperator(string[i]);
        if(is_oper != 0)
        {
            if(is_oper == 4)
            {
                if(i == strlen(string) && string[i] == '(') return 0;
                if(string[i] == '(') brackets++;
                else brackets--;
            }
            else
            {
                if(i == strlen(string)) return 0;
                if(!IsCharNumeric(string[i+1]) && string[i+1] != '(') return 0;
            }
            if(brackets < 0) return 0;
        }
        else if(!IsCharNumeric(string[i])) {return 0;}
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
        if(is_oper == 4)
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
        if(!IsCharNumeric(string[i])) 
        {
            *begin = i + 1;
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
        if((!IsCharNumeric(string[i])) || i == strlen(string)) 
        {
            *end = i - 1;
            if(strlen(digit) == 0) sprintf(digit, "%f", -0.0);
            break;
        }
        else if(!IsOperator(string[i])) sprintf(digit, "%s%c", digit, string[i]);
    }
    
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

double SolveOperation(char* string, char* answer)
{
    if(!IsValidOperation(string)) return -0.0;
    char* operation = string;
    int brackets = CountOpenBrackets(string);
    
    while(brackets > 0)
    {
        char* inner_operation;
        int opening, closing;
        FindInnermostBracket(string, &opening, &closing);
        
        for(int i = (opening + 1); i < closing; i++) inner_operation += string[i];
        SolveOperation(inner_operation, operation);
        brackets--;
    }
    
    for(int i = 0; i < strlen(operation); i++)
    {
        int is_oper = IsOperator(operation[i]);
        if(is_oper != 0)
        {
            printf("Test\n");
            int oper_begin, oper_end;
            double left_digit = GetForwardingDigit(operation, i, &oper_begin), right_digit = GetFollowingDigit(operation, i, &oper_end);
            double solution;
            if(is_oper == 3) solution = GetPower(left_digit, right_digit);
            if(is_oper == 2)
            {
                if(operation[i] == '*') solution = (left_digit * right_digit);
                else solution == (left_digit / right_digit);
            }
            if(is_oper == 1)
            {
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
                    char* soltn;
                    ToString(solution, soltn);
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
    if(IsOperator(user_input) == 5) printf("%c = %llf\n", oper, SolveOperation(oper));
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
    char* string = "(1+(2.3)^4)+5";
    //CPPFunction();
    int end;
    
    //double digit = IsCharNumeric(string[1]);
    
    SolveOperation(string);
    return 0;
}