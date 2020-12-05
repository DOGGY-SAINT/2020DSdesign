//题目一  考试报名系统
//已完成
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

#define SYNTHESIZE(varType, varName, funName)                       \
protected:                                                          \
	varType varName;                                                \
                                                                    \
public:                                                             \
	inline void set##funName(varType newVar) { varName = newVar; }; \
	inline varType get##funName() const { return varName; };

//指令
#define ESC "exit"
#define INSERT "1"
#define REMOVE "2"
#define FIND "3"
#define CHANGE "4"
#define SHOW "5"

//格式化输出
void output5(ostream &os, string s1, string s2, string s3, string s4, string s5, int size = 8)
{
	os << setw(size) << setiosflags(ios::left) << s1;
	os << setw(size) << setiosflags(ios::left) << s2;
	os << setw(size) << setiosflags(ios::left) << s3;
	os << setw(size) << setiosflags(ios::left) << s4;
	os << setw(size) << setiosflags(ios::left) << s5;
}

class Student
{
	SYNTHESIZE(string, _id, ID);
	SYNTHESIZE(string, _name, Name);
	SYNTHESIZE(string, _gender, Gender);
	SYNTHESIZE(string, _age, Age);
	SYNTHESIZE(string, _subject, Subject);
public:
	//construct
	Student()
	{
	}
	Student(string id, string name, string gender, string age, string subject)
		: _id(id), _name(name), _gender(gender), _age(age), _subject(subject)
	{
	}
	Student(const Student &st)
		: _id(st._id), _name(st._name), _gender(st._gender), _age(st._age), _subject(st._subject)
	{
	}
	//destruct
	~Student()
	{
	}
	//三个样例
	static const Student example1;
	static const Student example2;
	static const Student example3;
	//input
	friend istream &operator>>(istream &is, Student &st)
	{
		is >> st._id >> st._name >> st._gender >> st._age >> st._subject;
		return is;
	}
	//output
	friend ostream &operator<<(ostream &os, const Student &st)
	{
		output5(os, st._id, st._name, st._gender, st._age, st._subject);
		return os;
	}
};

class Node
{
	SYNTHESIZE(Student, _data, Data);
	SYNTHESIZE(Node*, _next, Next);

public:
	Node() : _next(nullptr)
	{
	}
	Node(const Student &st)
		: _data(st), _next(nullptr)
	{
	}
	void deleteAll()
	{
		if (_next != nullptr)
			_next->deleteAll();
		delete this;
	}
};

class List
{
private:
	Node *_head;
	int _length;

public:
	//表格标题
	static const string title[5];
	//输出表头
	static void outputTitle()
	{
		output5(cout, title[0], title[1], title[2], title[3], title[4], 10);
		cout << endl;
	}
	//construct
	List() : _length(0)
	{
		_head = new Node;
	}
	//destruct
	~List()
	{
		_head->deleteAll();
	}
	//默认初始化
	void initialize()
	{
		insert(_length + 1, Student::example1);
		insert(_length + 1, Student::example2);
		insert(_length + 1, Student::example3);
	}
	//全部delete，清空表
	void clear()
	{
		if (_head->getNext())
			_head->getNext()->deleteAll();
		_head->setNext(nullptr);
	}
	//length
	int length()
	{
		return _length;
	}
	//单次输入num个学生
	bool input(int num)
	{
		Student tmp;
		for (int i = 0; i < num; i++)
		{
			if (!(cin >> tmp))
			{
				cout << "Wrong input";
				return false;
			}
			insert(_length + 1, tmp);
		}
		return true;
	}
	//插入学生
	void insert(int pos, const Student &st)
	{
		Node *thisNode = _head;
		Node *newNode = new Node(st);
		for (int i = 0; i < _length && i < pos; i++)
			thisNode = thisNode->getNext();
		newNode->setNext(thisNode->getNext());
		thisNode->setNext(newNode);
		_length++;
	}
	//delete删除
	bool remove(const string &id)
	{
		Node *thisNode = _head;
		Node *next=_head;
		while(thisNode->getNext()!=nullptr)
		{
			next = thisNode->getNext();
			if (next->getData().getID() == id)
			{
				cout << "你删除的考生信息是:" << next->getData() << endl;
				thisNode->setNext(next->getNext());
				delete next;
				_length--;
				return true;
			}
			else
				thisNode = next;
		}
		return false;
	}
	//查找
	bool find(const string &id)
	{
		Node *thisNode = _head;
		Node *next=thisNode;
		while(thisNode->getNext()!=nullptr)
		{
			next = thisNode->getNext();
			if (next->getData().getID() == id)
			{
				cout << "该考生为:" << next->getData() << endl;
				return true;
			}
			else
				thisNode = next;
		}
		return false;
	}
	//修改
	bool change(const string &id, const Student &st)
	{
		Node *thisNode = _head;
		Node *next;
		while(thisNode->getNext()!=nullptr)
		{
			next = thisNode->getNext();
			if (next->getData().getID() == id)
			{
				next->setData(st);
				return true;
			}
			else
				thisNode = next;
		}
		return false;
	}
	//展示
	void showAll()
	{
		if (!_length)
			return;
		cout << endl;
		List::outputTitle();
		Node *elem = _head;
		do
		{
			elem = elem->getNext();
			cout << elem->getData() << endl;
		} while (elem->getNext() != nullptr);
		cout << endl;
	}
};

class Guide
{
private:
	List _list;
	bool _isBuilt;

public:
	//输入提示
	static const string toInput;
	//指令提示
	static const string orderGuide;
	Guide() : _isBuilt(0)
	{
	}
	//插入
	void insert()
	{
		int pos;
		cout << "请输入你要插入的考生的位置:";
		if(!(cin >> pos))
		{
			cin.clear();
			cin.ignore(100000000,'\n');
			cout << "非法位置" << endl;
			return;
		}
		if (pos > _list.length() + 1)
		{
			cout << "位置过大" << endl;
			return;
		}
		if (pos <= 0)
		{
			cout << "非法位置" << endl;
			return;
		}
		cout << toInput << endl;
		Student st;
		if (cin >> st)
		{
			_list.insert(pos, st);
			showAll();
		}
		else
		{
			cin.clear();
			cin.ignore(100000);
			cout << "输入错误" << endl;
			exit(0);
		}
	}
	//删除
	void remove()
	{
		string id;
		cout << "请输入要删除的考生的考号:";
		if (cin >> id)
			if (!(_list.remove(id)))
				cout << "未找到学生" << endl;
		showAll();
	}
	//查找
	void find()
	{
		string id;
		cout << "请输入要查找的考生的考号:";
		if (cin >> id)
			if (!(_list.find(id)))
				cout << "未找到学生" << endl;
		showAll();
	}
	//输入学号修改
	void change()
	{
		string id;
		cout << "请先输入要修改的考生的考号:";
		if (cin >> id)
			if (!(_list.find(id)))
				cout << "未找到学生" << endl;
			else
			{
				cout << "接下来" + Guide::toInput << endl;
				Student st;
				if (cin >> st)
					if (!(_list.change(id, st)))
						cout << "未找到学生" << endl;
			}
		showAll();
	}
	//执行指令
	bool runOrder(string order)
	{
		if (order == INSERT)
		{
			insert();
		}
		else if (order == REMOVE)
		{
			remove();
		}
		else if (order == FIND)
		{
			find();
		}
		else if (order == CHANGE)
		{
			change();
		}
		else if (order == SHOW)
		{
			showAll();
		}
		else if (order == ESC)
		{
			return false;
		}
		else
		{
			cout<<"命令不存在"<<endl;
		}
		return true;
	}
	//获取指令
	bool getOrder()
	{
		cout << orderGuide << endl;
		cout << "请选择您要进行的操作:";
		string order;
		cin >> order;
		return (runOrder(order));
	}
	//初始化
	bool initialize()
	{
		_isBuilt = true;
		int num;
		cout << "首先请建立考生信息系统" << endl;
		cout << "请输入考生人数(0为创建默认表格)：";
		if (cin >> num && num >= 0)
		{
			if (num == 0)
			{
				_list.initialize();
				showAll();
				return true;
			}
			else
			{
				cout << toInput << endl;
				bool res = _list.input(num);
				showAll();
				return res;
			}
		}
		_isBuilt = false;
		cout << "输入错误" << endl;
		cin.clear();
		cin.ignore(100000000,'\n');
		return true;
	}
	//循环体
	bool loop()
	{
		if (_isBuilt)
			return getOrder();
		else
			return initialize();
	}
	//展示表格
	void showAll()
	{
		if (_list.length())
			_list.showAll();
		else
			cout << endl
				 << "表为空" << endl
				 << endl;
	}
	//主循环
	void main()
	{
		while (loop())
		{
		}
	}
};

//常量
const Student Student::example1 = Student("1", "stu1", "女", "20", "软件设计师");
const Student Student::example2 = Student("2", "stu2", "男", "21", "软件开发师");
const Student Student::example3 = Student("3", "stu3", "男", "20", "软件设计师");
const string List::title[5] = {"考号", "姓名", "性别", "年龄", "报考类别"};
const string Guide::toInput = "请依次输入考生的考号,姓名,性别,年龄及报考类别!";
const string Guide::orderGuide = "1为插入,2为删除,3为查找,4为修改,5为统计,exit退出";

int main()
{
	Guide guide;
	guide.main();
	return 0;
}