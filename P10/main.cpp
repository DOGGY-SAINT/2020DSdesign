//项目十  8种排序算法的比较案例

#include<Windows.h>
#include<cstdlib>
#include<ctime>
#include<string>
#include<iostream>
#include<iomanip>
#include"MyVector.h"

using namespace std;

typedef void (*Function)(MyVector<int>&);

string g_name;
long long int g_cnt;


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

bool cinFail()
{
	if (cin.fail())
	{
		log("输入失败");
		cin.clear();
		cin.ignore(LLONG_MAX, '\n');
		return true;
	}
	return false;
}


//2.88s
void Swap(int& a, int& b)
{
	int t = a;
	a = b, b = t;
	g_cnt++;
}

void showVec(MyVector<int> vec,string str=string())
{
	long long int cnt = 0;
	cout << str;
	for (int a : vec)
	{
		cout << left << setw(7) << a;
		if (cnt == 9)
			cout << endl;
		cnt++;
		cnt %= 10;
	}
}

//小到大有序
bool check(MyVector<int> vec)
{
	for (int i = 0; i < vec.size() - 1; i++)
		if (vec[i] > vec[i + 1])
			return false;
	return true;
}

void NormalSort(MyVector<int>& vec)
{
	for (int i = vec.size() - 1; i > 0; i--)
	{
		for (int j = 0; j < vec.size() - 1; j++)
		{
			if (vec[j] > vec[j + 1])
			{
				Swap(vec[j], vec[j + 1]);
				/*t = vec[j];
				vec[j] = vec[j+1];
				vec[j + 1] = t;
				cnt++;*/
			}
		}
	}
	g_name = "平凡排序";
}

//将最大数不断上浮至最末
void BubbleSort(MyVector<int>& vec)
{
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
	g_name = "冒泡排序";
}

//不断找到最小元并将其置于前方
void SelectionSort(MyVector<int>& vec)
{
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
	g_name = "选择排序";
}

//前几个数有序，将后续数字插入到有序数组的对应位置
void InsertionSort(MyVector<int>& vec)
{
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
	g_name = "插入排序";
}

//含gap的插入排序
void shellInsert(MyVector<int>& vec, const int& gap)
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
void ShellSort(MyVector<int>& vec)
{
	int gap = vec.size() / 2;
	while (gap)
	{
		shellInsert(vec, gap);
		gap /= 2;
	}
	g_name = "希尔排序";
}

//从l到r，取l，将其分成左边均比l小右边均比l大两块
int Partition(MyVector<int>& vec, const int& left, const int& right)
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
void qs(MyVector<int>& vec, const int& left, const int& right)
{
	if (left < right)
	{
		//分块
		int pivot = Partition(vec, left, right);
		qs(vec, left, pivot - 1);
		qs(vec, pivot + 1, right);
	}
}

void QuickSort(MyVector<int>& vec)
{
	qs(vec, 0, vec.size() - 1);
	g_name = "快速排序";
}

//从i到end的最大数上浮到i
void filterDown(MyVector<int>& vec, const int& i, const int& end)
{
	int current = i, child = 2 * i + 1;
	while (child <= end)
	{
		if (child + 1 <= end && vec[child] < vec[child + 1])
			child += 1;
		if (vec[child] > vec[current])
		{
			Swap(vec[child], vec[current]);
			current = child; child = 2 * child + 1;
		}
		else
			break;
	}
}

//不断最大堆并不断将最大值换至末尾
void HeapSort(MyVector<int>& vec)
{
	//生成堆
	for (int i = (vec.size() - 2) / 2; i >= 0; i--)
		filterDown(vec, i, vec.size() - 1);
	//上浮并不断排到最后
	for (int i = vec.size() - 1; i > 0; i--)
	{
		Swap(vec[0], vec[i]);
		filterDown(vec, 0, i - 1);
	}
	g_name = "堆排序  ";
}

//init中l到m,m+1到n两个子串归并并且存入mergedl到n中
void merge(MyVector<int>& initvec, MyVector<int>& mergedvec, const int l, const int m, const int n)
{
	int i = l, j = m+1, k = l;
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
void mergeRecrusion(MyVector<int>& initvec, MyVector<int>& mergedvec, const int& len)
{
	int i = 0, mid = i + len, end = i + 2 * len;
	if (end > initvec.size())
		end = initvec.size();
	while (mid < initvec.size())
	{
		merge(initvec, mergedvec, i, mid-1, end - 1);
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

void MergeSort(MyVector<int>& vec)
{
	MyVector<int> tmp;
	tmp.resize(vec.size());
	int len = 1;
	while (len < vec.size())
	{
		mergeRecrusion(vec, tmp, len);
		len *= 2;
		mergeRecrusion(tmp,vec,len);
		len *= 2;
	}
	g_name = "归并排序";
}

int pow(int a,int b)
{
	int res = 1;
	for (int i = 0; i < b; i++)
	{
		res *= a;
	}
	return res;
}

//第key位排序，如果下一次均为0则不继续
bool sortKey(MyVector<int>& vec,int key)
{
	bool _continue = false;
	MyVector<MyVector<int>> buckets;
	buckets.resize(10);
	//入桶
	for (int t : vec)
	{
		int front = t / pow(10, key - 1);
		if (front/10 != 0)
			_continue = true;
		int data = front % 10;
		if (data != 0)
			_continue = true;
		buckets[data].push_back(t);
		g_cnt++;
	}
	//复刻
	int i = 0;
	for (MyVector<int>& bucket : buckets)
	{
		for (int t : bucket)
		{
			vec[i] = t;
			g_cnt++;
			i++;
		}
	}
	return _continue;
}


void RadixSort(MyVector<int>& vec)
{
	int i = 1;
	while (sortKey(vec,i))
	{
		i++;
	}
	g_name = "基数排序";
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
		else if (n >= 10 && n <= 100000)
		{
			initialize(n);
			log("生成随机数组成功");
		}
		else
		{
			log("n需大于等于一百小于等于十万");
		}
	}
	void sort(void p(MyVector<int>&))
	{
		if (_vec.empty())
		{
			log("数组为空，请重随");
			return;
		}
		MyVector<int> copy = _vec;
		g_cnt = 0;
		LARGE_INTEGER begin, end, gap;
		QueryPerformanceFrequency(&gap);
		QueryPerformanceCounter(&begin);
		p(copy);
		QueryPerformanceCounter(&end);
		double t = double(end.QuadPart - begin.QuadPart) / gap.QuadPart;
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
		if (cin.fail())
		{
			log("输入失败");
			cin.clear();
			cin.ignore(10000, '/n');
		}
		else if (n == 1)
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
{ BubbleSort,SelectionSort,InsertionSort,ShellSort,QuickSort,HeapSort,MergeSort,RadixSort };

int main()
{
	Guide guider;
	while (guider.loop())
	{
	}
	return 0;
}