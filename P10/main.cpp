//项目十  8种排序算法的比较案例
//UTF-8

#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include "MyVector.h"
#include <vector>

using namespace std;

vector<int> t;

typedef void (*Function)(MyVector<int> &);

string g_name;
long long int g_cnt;

void log(string str = string(), char end = '\n')
{
	cout << str << end;
}

template <typename val_type>
void input(val_type &val, string str = string())
{
	cout << str;
	cin >> val;
}

bool cinFail()
{
	if (cin.fail())
	{
		log("输入失败");
		cin.clear();
		cin.ignore(1000000, '\n');
		return true;
	}
	return false;
}

void Swap(int &a, int &b)
{
	int t = a;
	a = b, b = t;
	g_cnt++;
}

void showVec(MyVector<int> vec, string str = string())
{
	long long int cnt = 0;
	cout << str;
	for (int a : vec)
	{
		cout << left << setw(15) << a;
		if (cnt == 9)
			cout << endl;
		cnt++;
		cnt %= 10;
	}
	cout << endl;
}

//小到大有序
bool check(MyVector<int> vec)
{
	for (int i = 0; i < vec.size() - 1; i++)
		if (vec[i] > vec[i + 1])
			return false;
	return true;
}

void NormalSort(MyVector<int> &vec)
{
	g_name = "平凡排序";
	for (int i = vec.size() - 1; i > 0; i--)
	{
		for (int j = 0; j < vec.size() - 1; j++)
		{
			if (vec[j] > vec[j + 1])
			{
				Swap(vec[j], vec[j + 1]);
			}
		}
	}
}

//将最大数不断上浮至最末
void BubbleSort(MyVector<int> &vec)
{
	g_name = "冒泡排序";
	for (int i = vec.size() - 1; i > 0; i--)
	{
		for (int j = 0; j < i; j++)
		{

			if (vec[j] > vec[j + 1])
			{
				Swap(vec[j], vec[j + 1]);
			}
		}
	}
}

//不断找到最小元并将其置于前方
void SelectionSort(MyVector<int> &vec)
{
	g_name = "选择排序";
	int tag;
	for (int i = 0; i < vec.size() - 1; i++)
	{
		tag = i;
		for (int j = i + 1; j < vec.size(); j++)
		{
			if (vec[j] < vec[tag])
			{
				tag = j;
			}
		}
		if (tag != i)
			Swap(vec[i], vec[tag]);
	}
}

//前几个数有序，将后续数字插入到有序数组的对应位置
void InsertionSort(MyVector<int> &vec)
{
	g_name = "插入排序";
	for (int i = 1; i < vec.size(); i++)
	{
		for (int j = i; j > 0; j--)
		{
			if (vec[j] < vec[j - 1])
			{
				Swap(vec[j], vec[j - 1]);
			}
			else
				break;
		}
	}
}

//含gap的插入排序
void shellInsert(MyVector<int> &vec, const int &gap)
{
	for (int i = gap; i < vec.size(); i++)
	{
		int j = i;
		while (j >= gap && vec[j] < vec[j - gap])
		{
			Swap(vec[j], vec[j - gap]);
			j -= gap;
		}
	}
}

//按间隔分为几个子序列，对子序列执行插入排序，并将间隔不断减小至1
void ShellSort(MyVector<int> &vec)
{
	g_name = "希尔排序";
	int gap = vec.size() / 2;
	while (gap)
	{
		shellInsert(vec, gap);
		gap /= 2;
	}
}

//从l到r，取l，将其分成左边均比l小右边均比l大两块
int Partition(MyVector<int> &vec, const int &left, const int &right)
{
	int pivot = left;
	int t = vec[pivot];
	for (int i = left + 1; i <= right; i++)
	{
		if (vec[i] < t && ++pivot != i)
		{
			Swap(vec[i], vec[pivot]);
		}
	}
	Swap(vec[left], vec[pivot]);
	return pivot;
}

//从left到right递归排序
void qs(MyVector<int> &vec, const int &left, const int &right)
{
	if (left < right)
	{
		//分块
		int pivot = Partition(vec, left, right);
		qs(vec, left, pivot - 1);
		qs(vec, pivot + 1, right);
	}
}

void QuickSort(MyVector<int> &vec)
{
	g_name = "快速排序";
	qs(vec, 0, vec.size() - 1);
}

//从i到end的最大数上浮到i
void filterDown(MyVector<int> &vec, const int &i, const int &end)
{
	int current = i, child = 2 * i + 1;
	while (child <= end)
	{
		if (child + 1 <= end && vec[child] < vec[child + 1])
			child += 1;
		if (vec[child] > vec[current])
		{
			Swap(vec[child], vec[current]);
			current = child;
			child = 2 * child + 1;
		}
		else
			break;
	}
}

//不断最大堆并不断将最大值换至末尾
void HeapSort(MyVector<int> &vec)
{
	g_name = "堆排序  ";
	//生成堆
	for (int i = (vec.size() - 2) / 2; i >= 0; i--)
		filterDown(vec, i, vec.size() - 1);
	//上浮并不断排到最后
	for (int i = vec.size() - 1; i > 0; i--)
	{
		Swap(vec[0], vec[i]);
		filterDown(vec, 0, i - 1);
	}
}

//init中l到m,m+1到n两个子串归并并且存入mergedl到n中
void merge(MyVector<int> &initvec, MyVector<int> &mergedvec, const int l, const int m, const int n)
{
	int i = l, j = m + 1, k = l;
	while (i <= m || j <= n)
	{
		if (j > n || (i <= m && initvec[i] <= initvec[j]))
		{
			mergedvec[k++] = initvec[i++];
			g_cnt++;
		}
		else if (j <= n)
		{
			mergedvec[k++] = initvec[j++];
			g_cnt++;
		}
	}
}

//对长度为len，排子序列并归并
void mergeRecrusion(MyVector<int> &initvec, MyVector<int> &mergedvec, const int &len)
{
	int i = 0, mid = i + len, end = i + 2 * len;
	if (end > initvec.size())
		end = initvec.size();
	while (mid < initvec.size())
	{
		merge(initvec, mergedvec, i, mid - 1, end - 1);
		i += 2 * len;
		mid = i + len;
		end = i + 2 * len;
		if (end > initvec.size())
			end = initvec.size();
	}
	//剩下的元素小于等于len
	for (int j = i; j < initvec.size(); j++)
	{
		mergedvec[j] = initvec[j];
		g_cnt++;
	}
}

void MergeSort(MyVector<int> &vec)
{
	g_name = "归并排序";
	MyVector<int> tmp;
	tmp.resize(vec.size());
	int len = 1;
	while (len < vec.size())
	{
		mergeRecrusion(vec, tmp, len);
		len *= 2;
		mergeRecrusion(tmp, vec, len);
		len *= 2;
	}
}

int pow(int a, int b)
{
	int res = 1;
	while (b != 0)
	{
		if ((b & 1) != 0)
		{
			res *= a;
		}
		b >>= 1;
		a *= a;
	}
	return res;
}

class Node
{
public:
	int _data;
	Node *_next = nullptr;
	Node *_prev = nullptr;
	Node(int data = 0)
		: _data(data)
	{
	}
};

//带头节点双向不循环链表
class List
{
private:
	Node *_head;
	Node *_tail;
	int _size;

public:
	List()
	{
		_size = 0;
		_head = new Node();
		_tail = _head;
	}
	~List()
	{
		Node *current = _tail;
		while (current->_prev != nullptr)
		{
			current = current->_prev;
			delete current->_next;
		}
		delete current;
	}
	void show()
	{
		Node *current = _head;
		while (current->_next != nullptr)
		{
			current = current->_next;
			cout << current->_data << ' ';
		}
		cout << endl;
	}
	int size()
	{
		return _size;
	}
	void clear()
	{
		_size = 0;
		Node *current = _tail;
		while (_tail->_prev != nullptr)
		{
			current = _tail->_prev;
			delete current->_next;
		}
		current->_next = nullptr;
	}
	int back()
	{
		if (_size == 0)
			return 0;
		return _tail->_data;
	}
	int front()
	{
		if (_size == 0)
			return 0;
		return _head->_next->_data;
	}
	int pop_front()
	{
		if (_size == 0)
			return 0;
		int t = front();
		if (_size == 1)
		{
			delete _head->_next;
			_head->_next = nullptr;
			_tail = _head;
		}
		else
		{
			_head->_next = _head->_next->_next;
			delete _head->_next->_prev;
			_head->_next->_prev = _head;
		}
		_size--;
		return t;
	}
	void push_back(int num)
	{
		Node *t = new Node(num);
		_tail->_next = t;
		t->_prev = _tail;
		_tail = t;
		_size++;
	}
	List &operator+=(List &list)
	{
		if (list.size() != 0)
		{
			_size += list.size();
			_tail->_next = list._head->_next;
			_tail->_next->_prev = _tail;
			_tail = list._tail;
			list._size = 0;
			list._head->_next = nullptr;
			list._tail = list._head;
		}
		return *this;
	}
};

//第key位排序，如果下一次均为0则不继续
bool sortKey(List &list, int key)
{
	bool _continue = false;
	List buckets[10];
	//入桶
	while (list.size() != 0)
	{
		int t = list.pop_front();
		int front = t / pow(10, key - 1);
		front = front < 0 ? 0 : front;
		if (front >= 10)
			_continue = true;
		int data = front % 10;
		buckets[data].push_back(t);
		g_cnt++;
	}
	for (int i = 0; i < 10; i++)
		list += buckets[i];
	return _continue;
}

void RadixSort(MyVector<int> &vec)
{
	g_name = "基数排序";
	int i = 1;
	List list;
	for (int num : vec)
		list.push_back(num);
	while (sortKey(list, i))
		i++;
	for (int &num : vec)
		num = list.pop_front();
}

class Guide
{
private:
	MyVector<int> _vec;
	static const Function fun[8];

public:
	Guide()
	{
		guider();
	}
	bool empty()
	{
		return _vec.empty();
	}
	void showSize()
	{
		log("数组元素数量:" + to_string(_vec.size()));
	}
	void initialize(int n)
	{
		_vec.clear();
		srand(time(0));
		for (int i = 0; i < n; i++)
			_vec.push_back(rand());
	}
	void reRand()
	{
		int n;
		input(n, "请输入要产生的随机数个数:");
		if (cinFail())
			return;
		else if (n >= 100 && n <= 100000)
		{
			initialize(n);
			log("生成随机数组成功");
		}
		else
		{
			log("n需大于等于一百小于等于十万");
		}
	}
	void sort(void p(MyVector<int> &))
	{
		if (_vec.empty())
		{
			log("数组为空，请重随");
			return;
		}
		MyVector<int> copy = _vec;
		g_cnt = 0;
		double tb, te, gap;
		tb = clock();
		p(copy);
		te = clock();
		double t = (te - tb) / CLOCKS_PER_SEC;
		if (check(copy))
		{
			log(g_name + " 耗时:" + to_string(t) + "s " + "交换次数:" + to_string(g_cnt));
		}
		else
			log(g_name + "排序失败");
	}
	void sortAll()
	{
		if (_vec.empty())
		{
			log("数组为空，请重随");
			return;
		}
		for (int i = 0; i < 8; i++)
			sort(fun[i]);
	}
	void test(int n)
	{
		initialize(n);
		sort(BubbleSort);
		sort(InsertionSort);
		sort(SelectionSort);
		sort(QuickSort);
	}
	void guider()
	{
		log();
		log("**              排序算法比较              **");
		log("============================================");
		log("**              1   重置随机数            **");
		log("**              2   冒泡排序              **");
		log("**              3   选择排序              **");
		log("**              4   直接插入排序          **");
		log("**              5   希尔排序              **");
		log("**              6   快速排序              **");
		log("**              7   堆排序                **");
		log("**              8   归并排序              **");
		log("**              9   基数排序              **");
		log("**              10  所有排序              **");
		log("**              11  退出程序              **");
		log("============================================");
		log();
	}
	bool loop()
	{
		log();
		int n;
		input(n, "请输入指令:");
		cinFail();
		if (n == 1)
		{
			reRand();
		}
		else if (n >= 2 && n <= 9)
		{
			if (_vec.empty())
			{
				log("数组为空，请重随");
				return true;
			}
			showSize();
			sort(fun[n - 2]);
		}
		else if (n == 10)
		{
			if (_vec.empty())
			{
				log("数组为空，请重随");
				return true;
			}
			showSize();
			sortAll();
		}
		else if (n == 11)
		{
			return false;
		}
		else
		{
			log("指令不存在");
		}
		return true;
	}
};

const Function Guide::fun[8] =
	{BubbleSort, SelectionSort, InsertionSort, ShellSort, QuickSort, HeapSort, MergeSort, RadixSort};

int main()
{
	Guide guider;
	while (guider.loop())
	{
	}
	return 0;
}