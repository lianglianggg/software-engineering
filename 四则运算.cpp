/*
    实现自动生成四则运算的算式并判断用户计算的正误。
    计划分三步走：
    1.自动生成算式
    2.输入算式转换为逆波兰式
    3.计算算式结果
*/
#include<stdlib.h>
#include<iostream>
#include<string>
#include<sstream>
#include<stack>
#include<queue>
#include<map>
#include<fstream>
using namespace std;

//将中缀表达式转换为逆波兰式
queue<string> ConvertToRpn(string s,map<string,int>p,map<char,int>p_char)
{
    int length = s.length();
    string temp_s="";
    string temp_for_push;
    stack<string>sk1;
    queue<string>sk2;
    sk1.push("#");
    for (int i = 0; i < length;)
    {
        if (isdigit(s[i]))
        {//判断字符是否是0~9的数字
            while (isdigit(s[i]) || s[i] == '.')
            {
                temp_s = temp_s + s[i];
                i++;
            }
            sk2.push(temp_s);
            temp_s.clear();
        }
        else
        {
            if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'||s[i]=='^')
            {
                if (p_char[s[i]] >p[sk1.top()])
                {
                    temp_for_push.clear();
                    temp_for_push = temp_for_push + s[i];
                    sk1.push(temp_for_push);
                    i++;
                }
                else
                {
                    while (p_char[s[i]] <= p[sk1.top()])
                    {
                        sk2.push(sk1.top());
                        sk1.pop();
                    }
                    temp_for_push.clear();
                    temp_for_push = temp_for_push + s[i];
                    sk1.push(temp_for_push);
                    i++;
                }
            }
            else if (s[i] == '(')
            {
                temp_for_push.clear();
                temp_for_push = temp_for_push + s[i];
                sk1.push(temp_for_push);
                i++;
            }
            else if(s[i]==')')
            {
                while (sk1.top() != "(")
                {
                    sk2.push(sk1.top());
                    sk1.pop();
                }
                sk1.pop();
                i++;
            }
        }
        if (i == length)
        {
            while (sk1.size() != 1)
            {
                sk2.push(sk1.top());
                sk1.pop();
            }
        }
    }
    return sk2;
}

//生成随机小数
double GeneratNumber()
{
    double number;
    int temp;
    number = ((double)rand()) / ((double)(rand()/50));
    temp = number * 10;
    number = ((double)temp) / 10;
    number = number - (int)number + (int)number % 49;
    return number;
}

//生成随机整数
int GenerateInt()
{
    double int_number;
    int_number = rand() % 49;
    return int_number;
}

//计算逆波兰式中简单表达式
double Calculate(double n1, double n2, char c){
    double result = 0;
    if (c == '+'){
        result = n1 + n2;
    }
    else if (c == '-'){
        result = n2 - n1;
    }
    else if (c == '*'){
        result = n1*n2;
    }
    else if (c == '/'){
        result = n2 / n1;
    }
    return result;
}

//计算逆波兰式
double Operation(queue<string> q)
{
    stack<double> temp_for_digit;
    char temp_for_char;
    double temp_for_push = 0;
    double num1, num2;
    double temp_result = 0;
    int length = q.size();
    stringstream ss;
    while (q.size() != 0)
    {
        if (isdigit(q.front()[0]))
        {
            ss << q.front();
            ss >> temp_for_push;
            temp_for_digit.push(temp_for_push);
            q.pop();
            ss.clear();
        }
        else
        {
            temp_for_char = q.front()[0];
            q.pop();
            num1 = temp_for_digit.top();
            temp_for_digit.pop();
            num2 = temp_for_digit.top();
            temp_for_digit.pop();
            temp_result = Calculate(num1, num2, temp_for_char);
            temp_for_digit.push(temp_result);
        }
    }
    return temp_result;
}

//生成随机运算符
char GenerateOperator()
{
    char result;
    int which = rand() % 6;
    if (which == 0 || which == 4)
    {
        result = '+';
    }
    else if (which == 1 || which == 5)
    {
        result = '-';
    }
    else if (which == 2)
    {
        result = '*';
    }
    else if (which == 3)
    {
        result = '/';
    }
    return result;
}

//生成左括号
int GenerateLeftBracket()
{
    int result = 0;
    int whether_bracket = rand() % 7;
    if (whether_bracket ==1)
    {
        result = 1;
    }
    return result;
}

//生成右括号
int GenerateRightBracket()
{
    int result = 0;
    int whether_bracket = rand() % 7;
    if (whether_bracket <= 5)
    {
        result = 1;
    }
    return result;
}

//生成表达式
string GenerateExpression()
{
    string expression = "";
    string temp_string;
    int count_right_bracket = 0;
    int length = 3;
    int location_for_last_bracket = 0;
    length += 2*(rand() % 15);
    stringstream ss;
    double temp_num;
    int whether_int = 0;
    int whether_bracket = 0;
    for (int i = 0; i < length; i++)
    {
        whether_int = rand() % 5;
        if (i % 2 == 0)
        {
            if (whether_int <= 3)
            {//80%生成整数
                temp_num = GenerateInt();
            }
            else
            {
                temp_num = GeneratNumber();
            }
            ss << temp_num;
            ss >> temp_string;
            expression += temp_string;
            ss.clear();
            if (count_right_bracket&&i>=location_for_last_bracket+3)
            {
                if (GenerateRightBracket())
                {
                    count_right_bracket -= 1;
                    expression += ')';
                }
            }
        }
        else
        {
            expression += GenerateOperator();
            whether_bracket= GenerateLeftBracket();
            if (whether_bracket == 1)
            {
                expression += '(';
                count_right_bracket += whether_bracket;
                location_for_last_bracket = i;
            }
        }
    }
    while ((count_right_bracket--) != 0)
    {
        expression += ')';
    }
    return expression;
}

int main()
{
    map<string, int> priorites;
    priorites["+"] = 1;
    priorites["-"] = 1;
    priorites["*"] = 2;
    priorites["/"] = 2;
    priorites["^"] = 3;
    map<char, int> priorites_char;
    priorites_char['+'] = 1;
    priorites_char['-'] = 1;
    priorites_char['*'] = 2;
    priorites_char['/'] = 2;
    priorites_char['^'] = 3;
    string expression;
    queue<string> RPN;
    double result,your_answer=0.0;
    int count_expression;
    ofstream just_expression, answer;
    just_expression.open("expression.txt");
    answer.open("answer.txt");
    cout << "how many expressions do you want: " << endl;
    cin >> count_expression;
    for (int i = 0; i<count_expression; i++)
    {
        expression = GenerateExpression();
        RPN = ConvertToRpn(expression,priorites,priorites_char);//得到后缀表达式
        result = Operation(RPN);
        cout << i+1 << ".  " << expression << "your answer: ";
        cin >> your_answer;
        cout << endl;
        just_expression << i+1 << ".  " << expression << "  your answer: " << your_answer;
        answer << i+1 << ".  " << expression << " = " << result << endl;
        if(your_answer == result)
		{
        	cout << "your answer is right." << endl;
        	just_expression << "  √" << endl;
		}
		else
		{
		    cout << "your answer is wrong!" << endl;
		    just_expression << "  ×" << endl;
		}
        expression.clear();
        RPN = queue<string>();//清空当前队列
    }
    just_expression.close();
    answer.close();
    cout << "finished" << endl;
    system("pause");
    return 0;
}
