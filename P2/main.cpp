//题目二	两个有序链表序列的交集
//UTF-8
//MyList已加入
//已完成
//10.12
#include <iostream>
#include "MyList.h"

using namespace std;

void cinFail()
{
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(100000000000,'\n');
		cout<<"cin失败,程序退出"<<endl;
		exit(0);
	}
}

//输入
void input(MyList<long long> &list)
{
	long long num;
	list.clear();
	while (true)
	{
		cin >> num;
		cinFail();
		if (num == -1)
			break;
		if(num<-1)
		{
			cout<<"检测非-1的负数，程序退出"<<endl;
			exit(0);
		}
		long long tail=*(--list.end());
		if(num<tail)
		{
			cout<<"检测到降序，程序退出"<<endl;
			exit(0);
		}
		list.push_back(num);
	}
}

//复杂度m+n
MyList<long long> intersection(MyList<long long> l1, MyList<long long> l2)
{
	MyList<long long> result;
	auto it1 = l1.begin(), it2 = l2.begin();
	while (it1 != l1.end() && it2 != l2.end())
	{
		if (*it1 < *it2)
			it1++;
		else if (*it1 > *it2)
			it2++;
		else
		{
			result.push_back(*it1);
			it1++, it2++;
		}
	}
	return result;
}

int main()
{
	MyList<long long> l1, l2, result;
	input(l1);
	input(l2);
	result = intersection(l1, l2);
	if (result.size() != 0)
	{
		for (auto it = result.begin();;)
		{
			cout << *it;
			if (++it != result.end())
				cout << ' ';
			else
				break;
		}
	}
	else
		cout << "NULL";
	return 0;
}