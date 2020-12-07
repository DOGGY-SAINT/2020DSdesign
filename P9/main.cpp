#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Class;
class TmpClass;

class ClassTime//上课时间
{
private:
    int _day;//周几
    int _begin;//第几节课开始
    int _end;//到第几节课
public:
    ClassTime();
    friend ostream& operator<<(ostream&,ClassTime);
};

class Class
{
private:
string _id;//课程编号
string _name;//课程名称
int _hours;//学时数
int _term;//开课学期
vector<string> _preClasses;//先修课程
vector<ClassTime> _classTime;//具体上课时间
public:
Class(); //默认构造
Class(Class&);//复制构造
~Class();//折构
friend istream& operator>>(istream&,Class);//输入
friend ostream& operator<<(ostream&,Class);//输出
};

class TmpClass:public Class
{


};



int main()
{   

    return 0;
}