//题目八 电网建设造价模拟系统
//图
//最小生成树,Prim算法
//已完成

#include<string>
#include<iostream>
#include"MyVector.h"

using namespace std;

class Graph;
class Side;
class MST;
class Guide;

void log(string str = string(), char end = '\n')
{
	cout << str << end;
}

template<typename val_type>
void input(val_type& val, string str = string())
{
	cout << str;
	cin >> val;
}

//输入失败
bool cinFail()
{
	if (cin.fail())
	{
		log("输入失败");
		cin.clear();
		cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
		return true;
	}
	return false;
}

//边
class Side
{
private:
	string _start;
	string _end;
	int _val;
public:
	Side() {}
	Side(string start, string end, int val)
		: _start(start)
		, _end(end)
		, _val(val)
	{}
	friend ostream& operator<<(ostream& os, Side side)
	{
		os << side._start << "-<" << side._val << ">->" << side._end << endl;
		return os;
	}
};

//最小生成树
class MST
{
	friend class Graph;
private:
	MyVector<Side> _sides;
public:
	MST() {}
	friend ostream& operator<<(ostream& os, MST mst)
	{
		for (Side side : mst._sides)
		{
			os << side << endl;
		}
		return os;
	}
	void clear() { _sides.clear(); }
	void push_back(Side side) { _sides.push_back(side); }
	bool empty() { return _sides.empty(); }
};

//图
class Graph
{
	friend class Guide;
private:
	int _size;
	MyVector<string> _points;
	MyVector<MyVector<int>> _sides;
	MST _mst;
public:
	Graph(int size = 0)
		:_size(size)
	{
		MyVector<int> t;
		t.resize(size);
		_sides.resize(_size, t);
	}
	void clear()
	{
		_size = 0;
		_sides.clear();
		_points.clear();
	}
	void resize(int size)
	{
		clear();
		_size = size;
		MyVector<int> t;
		t.resize(size);
		_sides.resize(_size,t);
	}
	void addSide(string pa, string pb, int val)
	{
		int la, lb;
		auto it = _points.find(pa);
		if (it != _points.end())
			la = _points.find(pa).getPos();
		else
		{
			log(pa + "不存在");
			return;
		}
		it = _points.find(pb);
		if (it != _points.end())
			lb = _points.find(pb).getPos();
		else
		{
			log(pb + "不存在");
			return;
		}
		if (_sides[la][lb] != 0)
		{
			log("已存在"+pa+' '+pb+' '+to_string(_sides[la][lb]));
			log("已覆盖为"+pa + ' ' + pb + ' ' + to_string(val));
		}
		_sides[la][lb] = _sides[lb][la] = val;
	}
	bool findMST(string pbegin)
	{
		_mst.clear();
		int current, next;
		auto it = _points.find(pbegin);
		if (it != _points.end())
			current = next = it.getPos();
		else
		{
			log(pbegin + "不存在");
			return false;
		}
		MyVector<int> lowcost, nearvex;
		lowcost = _sides[current];
		nearvex.resize(_size, 0);
		nearvex[current] = -1;
		int cnt = 0;
		while (cnt != _size - 1)
		{
			int min = 1000000;
			int tag = -1;
			for (int i = 0; i < _size; i++)
			{
				if (nearvex[i] == -1 || lowcost[i] == 0)
					continue;
				if (_sides[next][i] < lowcost[i])
				{
					lowcost[i] = _sides[next][i];
					nearvex[i] = next;
				}
				if (lowcost[i] < min)
				{
					min = lowcost[i];
					tag = i;
				}
			}
			if (tag == -1)
			{
				_mst.clear();
				log("不存在最小生成树,请用B指令添加边至最小生成树存在");
				return false;
			}
			else
			{
				next = tag;
				Side res(_points[nearvex[tag]], _points[tag], min);
				nearvex[tag] = -1;
				_mst.push_back(res);
			}
			cnt++;
		}
		return true;
	}
	MST getMST()
	{
		return _mst;
	}
	void showPoints()
	{
		for (auto point : _points)
		{
			log(point, ' ');
		}
		cout << endl;
	}

};

class Guide
{
private:
	Graph _graph;
public:
	Guide() {}
	void guider()
	{
		log();
		log("**          电网造价模拟系统        **");
		log("======================================");
		log("**          A 创建电网顶点          **");
		log("**          B 添加电网的边          **");
		log("**          C 构造最小生成树        **");
		log("**          D 显示最小生成树        **");
		log("**          E 退出程序              **");
		log("======================================");
		log();
	}
	bool loop()
	{
		char ch;
		guider();
		input(ch, "请选择操作:");
		if (cinFail())
			return true;
		switch (ch)
		{
		case'A':
			funA();
			break;
		case'B':
			funB();
			break;
		case'C':
			funC();
			break;
		case'D':
			funD();
			break;
		case'E':
			return false;
		default:
			log("指令不存在");
			break;
		}
		return true;
	}
	//创建电网顶点
	void funA()
	{
		int n;
		input(n, "请输入顶点的个数:");
		if (cinFail())
			return;
		if (n <= 0)
		{
			log("顶点数需为正数");
			return;
		}
		_graph.resize(n);
		log("请依次输入各顶点的名称:");
		for (int i = 0; i < n; i++)
		{
			string str;
			cin >> str;
			_graph._points.push_back(str);
			if (cinFail())
				return;
		}
		log("顶点创建成功,当前顶点为:");
		_graph.showPoints();
	}
	//添加边
	void funB()
	{
		if (_graph._points.empty())
		{
			log("暂无顶点,请先通过指令A输入顶点");
			return;
		}
		while (true)
		{
			log("请输入两个顶点及边(? ? 0结束):", '\0');
			string s1, s2;
			int length;
			cin >> s1 >> s2 >> length;
			if (cinFail())
				continue;
			if (s1 == "?" && s2 == "?" && length == 0)
				break;
			if (length <= 0)
			{
				log("边长需为正");
				continue;
			}
			_graph.addSide(s1,s2,length);
		}
		log("输入结束");
	}
	//构造树
	void funC()
	{
		if (_graph._points.empty())
		{
			log("暂无顶点,请先通过指令A输入顶点");
			return;
		}
		string p;
		input(p, "请输入起始顶点:");
		if (cinFail())
			return;
		if (!_graph.findMST(p))
			log("生成失败");
		else
			log("Prim最小生成树生成成功!");
	}
	//显示树
	void funD()
	{
		if (_graph.getMST().empty())
			log("生成树不存在,请尝试用C指令生成");
		else
			cout<<_graph.getMST();
	}
};


int main()
{
	Guide guider;
	while (guider.loop()) {}
	return 0;
}