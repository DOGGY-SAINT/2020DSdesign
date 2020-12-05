//题目四  算数表达式求解
//后缀表达式
//MyVector已加入
//已完成

#include<iostream>
#include<string>
#include"MyVector.h"
using namespace std;

//常量
const string c_number = "0123456789";
const string c_operator = "+-*/%^()= ";
const string c_priority = "^%*/+-()=";

int pow(int t1, int t2)
{
	int res = 1;
	for (int i = 0; i < t2; i++)
		res *= t1;
	return res;
}

//类型
enum Type
{
	NUM,
	OPERATOR,
	WRONGTYPE
};

//数据
class Data
{
public:
	Type _type;
	int _data;
	Data(Type type = WRONGTYPE, int data = 0) :_type(type), _data(data) {}
	bool operator==(const Data& data)const
	{
		return _type == data._type && _data == data._data;
	}
	bool operator!=(Data& data)const
	{
		return _type != data._type || _data != data._data;
	}
};

//处理


//优先级更大
bool bigger(char c1, char c2)
{
	for (int i = 0; i < c_priority.size(); i++)
	{
		if (c_priority[i] == c2)
			return false;
		else if (c_priority[i] == c1)
			return true;
	}
}

//数字判定
bool isNumber(char ch)
{
	for (char t : c_number)
		if (t == ch)
			return true;
	return false;
}

//操作符判定
bool isOperator(char ch)
{
	for (char t : c_operator)
		if (t == ch)
			return true;
	return false;
}

//数组输出
void show(MyVector<Data> vec, char end = '\n')
{
	for (Data data : vec)
	{
		if (data._type == NUM)
			cout << data._data;
		else if (data._type == OPERATOR)
			cout << char(data._data);
	}
	cout << end;
}

//类型判定
Type typeJudge(char ch)
{
	if (isNumber(ch))
		return NUM;
	if (isOperator(ch))
		return OPERATOR;
	return WRONGTYPE;
}

//计算器
class Calculator
{
private:
	bool _wrong = false;
	string _input;
	MyVector<Data> _infix;
	MyVector<Data>_suffix;
	double _result;
public:
	Calculator(string str = "") :_input(str) {}
	bool wrong()
	{
		return _wrong;
	}
	//初始化
	void clear()
	{
		_wrong = false;
		_input.clear();
		_infix.clear();
		_suffix.clear();
		_result = 0;
	}
	//计算
	double cal(int t1, int t2, char ope)
	{
		switch (ope)
		{
		case '+':
			t1 += t2;
			break;
		case '-':
			t1 -= t2;
			break;
		case '*':
			t1 *= t2;
			break;
		case '/':
			t1 /= t2;
			break;
		case '^':
			t1 = pow(t1, t2);
			break;
		case '%':
			t1 %= t2;
			break;
		default:
			_wrong = true;
			throw string("运算符错误");
			break;
		}
		return t1;
	}
	//输入
	void input()
	{
		getline(cin, _input);
		if (cin.fail())
		{
			cin.clear();
			_wrong = true;
			throw string("输入错误");
		}
	}
	//处理
	void include(MyVector<Data>& res, MyVector<char>& tmp, char input, char begin, char end)
	{
		if (input == begin)
		{
			while (tmp.size() && tmp.back() != end)
			{
				res.push_back(Data(OPERATOR, tmp.back()));
				tmp.pop_back();
			}
			if (tmp.empty())
			{
				_wrong = true;
				throw string("未找到" + end);
			}
			else
				tmp.pop_back();
		}
	}
	//计算中缀并验证
	void calInfix()
	{
		string int_tmp;
		int bracket = 0;
		bool haveLeftNum = false;
		bool isAddingNum = false;
		for (int i = 0; i < _input.size(); i++)
		{
			char ch = _input[i];
			if (i == _input.size() - 1)
				if (ch == '=')
				{
					if (bracket != 0)
					{
						_wrong = true;
						throw string("括号不匹配");
					}
					_infix.push_back(Data(OPERATOR, '='));
					return;
				}
				else
				{
					_wrong = true;
					throw string("缺少等于号");
					return;
				}
			Type type = typeJudge(ch);
			if (type == WRONGTYPE)
			{
				_wrong = true;
				throw string("输入了非法字符");
				return;
			}
			else if (isAddingNum || type == NUM)
			{
				if (type == NUM)
					haveLeftNum = true;
				isAddingNum = true;
				int_tmp += ch;
				if (i == _input.size() - 1 || typeJudge(_input[i + 1]) != NUM)
				{
					isAddingNum = false;
					int t = stoi(int_tmp);
					int_tmp.clear();
					Data data(NUM, t);
					_infix.push_back(data);
				}
			}
			switch (ch)
			{
			case '+':
			case '-':
			{
				if (haveLeftNum == false)
				{
					/*isAddingNum = true;
					int_tmp += ch;*/
					_input.insert(i, "(0");
					i--;
					int cnt = 0, j = i + 4;
					if (_input[j] != '(')
					{
						char thisch = _input[j];
						while (isNumber(thisch))
						{
							j++;
							thisch = _input[j];
						}
					}
					else
					{
						cnt = 1;
						j++;
						while (cnt != 0)
						{
							if (j < _input.size())
							{
								if (_input[j] == '(')
									cnt++;
								else if (_input[j] == ')')
									cnt--;
							}
							j++;
						}
					}
					//string的插入是在该位置前插入
					_input.insert(j, ")");
					break;
				}
			}
			case '*':
			case '/':
			case '%':
			case '^':
			case ')':
			case '(':
				haveLeftNum = false;
				if (ch == '(')
					bracket++;
				else if (ch == ')')
				{
					haveLeftNum = true;
					bracket--;
				}
				
				_infix.push_back(Data(OPERATOR, ch));
				break;
			case '=':
				_wrong = true;
				throw string("等于号位置错误");
				return;
			case ' ':
				;
			}
		}
	}
	//计算后缀
	void calSuffix()
	{
		MyVector<char> tmp;
		MyVector<Data> res;
		tmp.push_back('=');
		for (Data data : _infix)
		{
			if (data._type == NUM)
				res.push_back(data);
			else if (data._type == OPERATOR)
			{
				char ch = data._data;
				include(res, tmp, ch, '=', '=');
				include(res, tmp, ch, ')', '(');
				if (ch == '=' || ch == ')')
					continue;
				if (ch == '(')
					tmp.push_back('(');
				else
				{
					while (true)
					{
						if (tmp.empty())
						{
							_wrong = true;
							throw string("表达式错误");
							return;
						}
						if (!bigger(tmp.back(), ch))
							break;
						res.push_back(Data(OPERATOR, tmp.back()));
						tmp.pop_back();
					}
					tmp.push_back(ch);
				}
			}
		}
		_suffix = res;
	}
	//展示中缀
	void showInfix()
	{
		show(_infix);
	}
	//展示后缀
	void showSuffix()
	{
		show(_suffix);
	}
	//根据后缀表达式计算结果
	void calResult()
	{
		MyVector<double> tmp;
		for (Data data : _suffix)
		{
			if (data._type == NUM)
				tmp.push_back(data._data);
			else
			{
				int t1, t2;
				if (tmp.empty())
				{
					_wrong = true;
					throw string("表达式错误");
					return;
				}
				t1 = tmp.back();
				tmp.pop_back();
				if (tmp.empty())
				{
					_wrong = true;
					throw string("表达式错误");
					return;
				}
				t2 = tmp.back();
				tmp.pop_back();
				double res = cal(t2, t1, data._data);
				tmp.push_back(res);
			}
		}
		_result = tmp.back();
		if (tmp.empty())
		{
			_wrong = true;
			throw string("表达式错误");
			return;
		}
		tmp.pop_back();
	}
	//结果
	int result()
	{
		return _result;
	}
	//一键计算
	bool calAll()
	{
		if (!_wrong)
		{
			calInfix();
			/*showInfix();*/
		}
		if (!_wrong)
		{
			calSuffix();
			/*showSuffix();*/
		}
		if (!_wrong)
			calResult();
		return !_wrong;
	}
	//主循环
	void loop()
	{
		while (true)
		{
			try
			{
				clear();
				cout << "输入表达式(exit退出):" << endl;
				input();
				if (_input == "exit")
					break;
				if (!_wrong)
					calAll();
				if (!_wrong)
					cout << "结果是：" << fixed << _result << endl;
			}
			catch (string str)
			{
				cout << str << endl;
			}
		}
	}

};

//main
int main()
{
	Calculator cal;
	cal.loop();
	return 0;
}