#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

//User-defined Stack
class operatorStack {
    private:
        char stack[10000];
        int stack_top = -1;

    public:

        string get() {
            string str = "";
            for (int i = 0; i <= stack_top; i++) {
                str += stack[i];
            }
            return str;
        }

        void push(char value) {
            stack[++stack_top] = value;
        }

        void pop() {
            if (is_empty()) {
                return;
            }
            --stack_top;
        }

        char top() {
            if (is_empty()) {
                return '\0';
            }
            return stack[stack_top];
        }

        bool is_empty() {
            return stack_top == -1;
        }

};

string prefix(string); //Convert to prefix
string postfix(string); //Convert to postfix
int prio(char); //Get operator priority
int stringLength(string str); //Get string length
string reverseString(string str); //Reverse string
bool isAlphanumeric(char ch); //Check if character is alphanumeric
bool validExpression(); //Check if expression is valid
void pressEnter();

operatorStack post_stack, prefix_stack;
string output_postfix = "";
string output_prefix = "";
string expression = "";

int main() {
    system("cls");
    while (true) {
        cout << "================================================" << endl;
        cout << setw(43) << "Infix to Prefix and Postfix Conversion" << endl;
        cout << "================================================" << endl;
        cout << " Enter expression: ";
        getline(cin, expression);
        if (!validExpression()) {
            cout << " Invalid infix expression!" << endl;
            cout << "================================================" << endl;
            pressEnter();
            system("cls");
            continue;
        }
        cout << "================================================";
        cout << setw(47) << "Prefix Output: " + prefix(expression);
        cout << "\n================================================";

        cout << setw(47) << "Postfix Output: " + postfix(expression);
        cout << "\n================================================" << endl << endl;
        break;
    }
    return 0;
}

//Determine operator priority
int prio(char oper) {
    switch (oper) {
    case '^' :
        return 3;
    case '*': case '/':
        return 2;
    case '+': case '-':
        return 1;
    default:
        return 0;
    }
}
string postfix(string exp) {
    cout << endl;
    cout << " " << setw(15) << left << "Expression" << setw(13) << left << "STACK" << setw(7) << left << "Postfix Expression" << endl;

    //Add '(' and ')' at the start and end of the expression
    exp = "(" + exp + ")";
    //Scan the expression from left to right
    for (int i = 0; i < stringLength(exp); i++) {
        //Display expressions
        cout << " " << setw(15) << left << exp[i];

        //Pushing operators to stack
        switch (exp[i]) {
        case '(':
            post_stack.push(exp[i]); //If left parenthesis is encountered, push it onto STACK
            break;
        case ')':
            while (true) { //Repeatedly pop from STACK and add each operator to POSTFIX
                if (post_stack.top() == '(') {
                    post_stack.pop();
                    break;
                }
                output_postfix += post_stack.top();
                post_stack.pop();
            }
            break;
        case '*': case '/': case '+': case '-': case '^': // If an operator is encountered
            if (prio(exp[i]) > prio(post_stack.top()) || post_stack.top() == '(') {
                post_stack.push(exp[i]); //Add operator to STACK if it has lower priority than top
            }
            else {  //Repeatedly pop from STACK and add it to the POSTFIX, if top has same or higher priority than the operator
                while (prio(exp[i]) <= prio(post_stack.top())) {
                    output_postfix += post_stack.top();
                    post_stack.pop();
                    if (post_stack.is_empty()) { break; }
                }
                post_stack.push(exp[i]); //Then add incoming operator to STACK
            }
            break;
        default:
            output_postfix += exp[i]; //If an operand is encountered add it to the POSTFIX
            break;
        }
        cout << setw(13) << left << post_stack.get();
        cout << setw(18) << right << output_postfix << endl;
    }

    cout << "================================================" << endl;
    return output_postfix;
}
string prefix(string exp) {
    cout << endl;
    cout << " " << setw(15) << left << "Expression" << setw(13) << left << "STACK" << setw(18) << right << "Prefix Expression" << endl;
    //Reverse the expression
    exp = reverseString(exp);
    //Add '(' and ')' at the start and end of the expression
    exp = ")" + exp + "(";
    //Scan the expression from left to right, since it is already reversed
    for (int i = 0; i < stringLength(exp); i++) {
        //Display expressions
        cout << " " << setw(15) << left << exp[i];
        //Pushing operators to stack
        switch (exp[i]) {
        case ')': //If right parenthesis is encountered, push it onto STACK
            prefix_stack.push(exp[i]);
            break;
        case '(': //If left parenthesis is encountered
            while (true) { //Repeatedly pop from STACK and add each operator to PREFIX
                if (prefix_stack.top() == ')') {
                    prefix_stack.pop();
                    break;
                }
                output_prefix += prefix_stack.top();
                prefix_stack.pop();
            }
            break;
        case '*': case '/': case '+': case '-': case '^': //If an operator is encountered
            if (prio(exp[i]) >= prio(prefix_stack.top()) || prefix_stack.top() == ')') {
                prefix_stack.push(exp[i]); //Add operator to STACK if it has lower priority than top
            }
            else {  //Repeatedly pop from STACK and add it to the PREFIX, if top has higher priority than the operator
                while (prio(exp[i]) < prio(prefix_stack.top())) {
                    output_prefix += prefix_stack.top();
                    prefix_stack.pop();
                    if (prefix_stack.is_empty()) { break; }
                }
                prefix_stack.push(exp[i]); //Then add incoming operator to STACK
            }
            break;
        default: //If an operand is encountered add it to the PREFIX
            output_prefix += exp[i];
            break;
        }
        cout << setw(13) << prefix_stack.get();
        cout << setw(18) << right << output_prefix << endl;
    }
    //Reverse the expression
    output_prefix = reverseString(output_prefix);
    cout << "================================================" << endl;
    return output_prefix;
}

int stringLength(string str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

string reverseString(string str) {
    int len = stringLength(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

bool isAlphanumeric(char ch) {
    return (ch >= 'A' && ch <= 'Z') ||
        (ch >= 'a' && ch <= 'z') ||
        (ch >= '0' && ch <= '9');
}

bool isOperator(char ch) {
    return (ch == '^' || ch == '*' || ch == '/' || ch == '+' || ch == '-');
}

bool validExpression() {
    int len = stringLength(expression);
    int operatorCount = 0, operandCount = 0;
    int leftParenthesis = 0, rightParenthesis = 0;
    string trimmed_exp = "";
    if (len == 1) return false;
    for (int i = 0; i < len; i++) {
        //Ignore whitespaces in expression
        if (expression[i] == ' ') {
            continue;
        }
        else if (isOperator(expression[i])) {
            //Returns invalid if operator is at the beginning of the expression
            if (i == 0) { return false; }
            ++operatorCount;
        }
        else if (expression[i] == '(' || expression[i] == ')') {
            if (expression[i] == '(') {
                if ((trimmed_exp != "" && isAlphanumeric(trimmed_exp[stringLength(trimmed_exp) - 1]))) return false;
                ++leftParenthesis;
            }
            if (expression[i] == ')') {
                if (leftParenthesis == 0 || (isOperator(trimmed_exp[stringLength(trimmed_exp) - 1])) || trimmed_exp[stringLength(trimmed_exp) - 1] == '(') return false;
                ++rightParenthesis;
            }
        }
        //Check If there is a non alphanumeric and non operator symbol or
        //If both current and previous symbol are alphanumeric (eg. 'a+bc+')
        else if (!isAlphanumeric(expression[i]) || (trimmed_exp != "" && isAlphanumeric(trimmed_exp[stringLength(trimmed_exp) - 1]))) {
            return false;
        }
        else {
            ++operandCount;
        }
        trimmed_exp += expression[i];
    }
    //Checks if parenthesis are unbalanced or
    //If the number of operators is not one less than the number of operands (eg. 'a+b+')
    if (leftParenthesis != rightParenthesis || operatorCount != operandCount - 1) {
        return false;
    }
    expression = trimmed_exp;
    return true;
}

void pressEnter() {
    cout << "  Press <Enter> to continue...";
    cin.get();
}