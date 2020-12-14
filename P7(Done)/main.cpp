//题目七  修理牧场
//霍夫曼树
//MyVector已加入
//已完成
#include <iostream>
#include "MyVector.h"
using namespace std;
//1 1 1 1 2 3 4 5
//2 2 2 3 4 5     2+2
//4 5 4 5      4+5
//8  10    8+10
//18    18
//2+2+4+6+8+10+18=49

bool bigger(int i, int j) { return i > j; }

void cinFail()
{
	if (cin.fail())
	{
		cout << "输入错误" << endl;
		exit(0);
	}
}

void swap(int &a, int &b)
{
	int t = a;
	a = b, b = t;
}

//大到小
void sort(MyVector<int> &vec)
{
	for (int i = 0; i < vec.size() - 1; i++)
	{
		for (int j = vec.size() - 1; j > i; j--)
		{
			if (vec[j] > vec[j - 1])
				swap(vec[j], vec[j - 1]);
		}
	}
}

class Wood
{
private:
	MyVector<int> _wood;
	int _price;
	//对最后一个数排序，大到小
	void sortBack()
	{
		int l = _wood.size();
		for (int i = l - 1; i > 0; i--)
		{
			if (_wood[i] > _wood[i - 1])
			{
				int tmp = _wood[i];
				_wood[i] = _wood[i - 1];
				_wood[i - 1] = tmp;
			}
		}
	}

public:
	Wood()
	{
	}
	void input()
	{
		int n, num;
		cin >> n;
		cinFail();
		if (n <= 0)
		{
			cout << "需为正整数" << endl;
			exit(0);
		}
		for (int i = 0; i < n; i++)
		{
			cin >> num;
			cinFail();
			if (num <= 0)
			{
				cout << "需为正整数" << endl;
				exit(0);
			}
			_wood.push_back(num);
		}
		//排序小到大
		sort(_wood);
	}
	//多少钱
	void countPrice()
	{
		_price = 0;
		for (int i = _wood.size() - 1; i > 0; i--)
		{
			_wood[i - 1] += _wood[i];
			_price += _wood[i - 1];
			_wood.pop_back();
			sortBack();
		}
	}
	int getPrice()
	{
		return _price;
	}
};

int main()
{
	Wood wood;
	wood.input();
	wood.countPrice();
	cout << wood.getPrice();
	return 0;
}