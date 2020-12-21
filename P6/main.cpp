//题目六  家谱管理系统
//UTF-8
//已完成
#include<iostream>
#include<string>
#include"MyVector.h"
using namespace std;

class MemberNode;
class FamilyTree;
class Guide;

void log(string str, char end = '\n')
{
	cout << str << end;
}

class MemberNode
{
	friend class FamilyTree;
private:
	string _name;
	MyVector<MemberNode*> _children;
public:
	MemberNode(string name = string())
		:_name(name)
		, _children()
	{}
	string name()
	{
		return _name;
	}
	void setName(string name)
	{
		_name = name;
	}
	MyVector<MemberNode*>getChildren()
	{
		return _children;
	}
	MemberNode* getChild(string name)
	{
		if (!(_children.empty()))
		{
			for (MemberNode* t : _children)
			{
				if (t->name() == name)
					return t;
			}
		}
		return nullptr;
	}
	void addChild(string name)
	{
		MemberNode* mb = new MemberNode(name);
		_children.push_back(mb);
	}
	void show(char gap = ' ')
	{
		cout << _name << gap;
	}
	void showChildren()
	{
		log(_name + "的第一代子孙是:", '\0');
		if (!(_children.empty()))
		{
			for (auto mb : _children)
			{
				mb->show();
			}
		}
		log("");
	}
	void showAll()
	{
		show();
		if (!(_children.empty()))
		{
			for (auto mb : _children)
			{
				mb->showAll();
			}
		}
	}
	void deleteAll()
	{
		if (!(_children.empty()))
		{
			for (auto mb : _children)
			{
				mb->deleteAll();
			}
		}
		delete this;
	}
	void deleteChildren()
	{
		if (!(_children.empty()))
		{
			for (auto mb : _children)
			{
				mb->deleteChildren();
				delete mb;
			}
		}
		_children.clear();
	}
	MemberNode* find(string name)
	{
		if (name == _name)
			return this;
		if (!(_children.empty()))
		{
			for (auto mb : _children)
			{
				auto res=mb->find(name);
				if (res)
					return res;
			}
		}
		return nullptr;
	}
};

class FamilyTree
{
private:
	MemberNode* _root;
public:
	FamilyTree()
		:_root(nullptr)
	{}
	~FamilyTree()
	{
		_root->deleteAll();
	}
	void create(string name)
	{
		MemberNode* mb = new MemberNode(name);
		_root = mb;
	}
	MemberNode* getRoot()
	{
		return _root;
	}
	void show()
	{
		if (_root)
			_root->showAll();
	}
	void clear()
	{
		if (_root)
			_root->deleteAll();
		_root = nullptr;
	}
	MemberNode* find(string name)
	{
		if (_root)
			return _root->find(name);
		return nullptr;
	}
};

class Guide
{
private:
	FamilyTree _tree;
public:
	Guide()
	{
		setAncestor();
	}
	MemberNode* find()
	{
		string name;
		cin >> name;
		MemberNode* target = _tree.find(name);
		if (!target)
		{
			log("成员不存在");
		}
		return target;
	}
	void setAncestor()
	{
		log("首先建立一个家庭!");
		log("请输入祖先的姓名:", '\0');
		string str;
		cin >> str;
		_tree.create(str);
		log("此家谱的祖先是:" + str);
	}
	void showGuider()
	{
		log("");
		log("**      家谱管理系统      **");
		log("============================");
		log("**   请选择要执行的操作:  **");
		log("**    A 完善家谱          **");
		log("**    B 添加家庭成员      **");
		log("**    C 解散局部家庭      **");
		log("**    D 更改家庭成员姓名  **");
		log("**    E 退出程序          **");
		log("============================");
		log("");
		log("请选择要执行的操作:", '\0');
	}
	bool loop()
	{
		showGuider();
		char ch;
		do
		{
			ch = getchar();
		} while (ch != '\n');
		ch = getchar();
		switch (ch)
		{
		case'A':
			addChildren();
			break;
		case'B':
			addChild();
			break;
		case'C':
			deleteChildren();
			break;
		case'D':
			changeName();
			break;
		case'E':
			return false;
			break;
		default:
			log("错误指令");
		}
		return true;
	}
	void addChildren()
	{
		log("请输入要建立家庭的人的姓名:", '\0');
		MemberNode* target = find();
		if (!target)
		{
			return;
		}
		if (!(target->getChildren().empty()))
		{
			log("已有家庭,欲添加子女请用B");
			return;
		}
		log("请输入" + target->name() + "的儿女人数:", '\0');
		int n;
		if (!(cin >> n))
		{
			cin.clear();
			log("输入错误");
			return;
		}
		log("请依次输入"+ target->name() +"的儿女的姓名:", '\0');
		for (int i = 0; i < n; i++)
		{
			string cname;
			cin >> cname;
			target->addChild(cname);
		}
		target->showChildren();
	}
	void addChild()
	{
		log("请输入要添加子女的人的姓名:", '\0');
		MemberNode* target = find();
		if (!target)
		{
			return;
		}
		log("请输入" + target->name() + "要添加的儿女的姓名:", '\0');
		string cname;
		cin >> cname;
		target->addChild(cname);
		target->showChildren();
	}
	void deleteChildren()
	{
		log("请输入要解散家庭的人的姓名:", '\0');
		MemberNode* target = find();
		if (!target)
		{
			return;
		}
		log("要解散家庭的人是" + target->name());
		target->showChildren();
		target->deleteChildren();
	}
	void changeName()
	{
		log("请输入要更改姓名的人的目前姓名:", '\0');
		MemberNode* target = find();
		if (!target)
		{
			return;
		}
		log("请输入更改后的姓名:", '\0');
		string name;
		cin >> name;
		log(target->name() + "已更名为" + name);
		target->setName(name);
	}
};

int main()
{
	Guide guide;
	while (guide.loop())
	{}
	return 0;
}