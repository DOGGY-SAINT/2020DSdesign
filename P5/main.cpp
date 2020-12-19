//题目五  银行业务
//MyVector已加入
//已完成

#include <iostream>
#include "MyVector.h"

using namespace std;

bool cinFail()
{
	bool fail = cin.fail();
	if (fail)
	{
		cout << ("输入失败") << endl;
		cin.clear();
	}
	return fail;
}

class Bank
{
private:
	MyVector<int> WA, WB, output;

public:
	Bank()
	{
	}
	void input()
	{
		int n, tmp;
		cin >> n;
		if (cinFail())
			exit(0);
		for (int i = 0; i < n; i++)
		{
			cin >> tmp;
			if (cinFail())
				exit(0);
			if (tmp % 2)
				WA.push_back(tmp);
			else
				WB.push_back(tmp);
		}
	}
	void setOutput()
	{
		int i = 0;
		for (auto itA = WA.begin(), itB = WB.begin(); itA != WA.end() || itB != WB.end(); i++)
		{
			if (itA != WA.end())
			{
				output.push_back(*itA);
				itA++;
			}
			if (itB != WB.end() && i % 2)
			{
				output.push_back(*itB);
				itB++;
			}
		}
	}
	void showOutput()
	{
		for (auto it = output.begin();;)
		{
			cout << *it;
			if (++it != output.end())
				cout << ' ';
			else
				break;
		}
	}
};

int main()
{
	Bank bank;
	bank.input();
	bank.setOutput();
	bank.showOutput();
	return 0;
}