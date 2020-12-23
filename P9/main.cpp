//题目九  排课软件
//UTF-8
//add .vscode in file
//remember to put in.txt together with it
//Linux
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "MyVector.h"

using namespace std;

class Course;
class ClassTime;
class System;

template <typename val_type>
void log(const val_type &data, char end = '\n',ostream& os=cout)
{
    os << data << end;
}

template <typename type>
void Swap(type &a, type &b)
{
    type t = a;
    a = b, b = t;
}

bool cinFail()
{
    if (!cin.fail())
        return false;
    cin.clear();
    cin.ignore(10000, '\n');
    return true;
}

bool isChar(char ch) //是字符
{
    return ch >= 0 && ch <= 127;
}

void output(const string &str, int width, ostream& os=cout) //Linux中考虑中文的左对齐格式化输出,无endl
{
    int cnt = 0;
    for (int i = 0; i < str.size(); i++)
    {
        char ch = str[i];
        if (isChar(ch))
        {
            os << ch;
            cnt++;
        }
        else
        {
            int t = i;
            while (!isChar(str[t]))
            {
                t++;
            }
            t--;
            int len = t - i + 1;
            //在Linux的UTF-8编码中，中文用三个字符储存，但是输出只占两个字符的位置
            string s;
            s.assign(str, i, len);
            os << s;
            i = t;
            cnt += len / 3 * 2;
        }
    }
    for (; cnt < width; cnt++)
    {
        os << ' ';
    }
}

MyVector<string> cutString(const string &str)
{
    string tmp;
    MyVector<string> res;
    for (char ch : str)
    {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            if (!tmp.empty())
            {
                res.push_back(tmp);
                tmp.clear();
            }
        }
        else
        {
            tmp += ch;
        }
    }
    if (!tmp.empty())
    {
        res.push_back(tmp);
        tmp.clear();
    }
    return res;
}

bool isInt(const string &str)
{
    for (char ch : str)
    {
        if (ch < '0' || ch > '9')
            return false;
    }
    return true;
}

class Exception
{
public:
    //Fail需要注意位置，Bad直接交给全局退出程序就好了
    enum EType
    {
        Fail,
        Bad
    };

private:
    EType _type;
    string _data;

public:
    Exception(const string &str, EType type = Fail)
        : _type(type), _data(str)
    {
    }

    bool fail() const
    {
        return _type == Fail;
    }

    bool bad() const
    {
        return _type == Bad;
    }

    string data() const
    {
        return _data;
    }

    friend ostream &operator<<(ostream &os, const Exception exc)
    {
        os << exc._data;
        return os;
    }
};

class Course
{
    friend class System;
    friend class WorkData;
    friend class Term;

private:
    string _id;               //课程编号
    string _name;             //课程名称
    int _hours;               //学时数
    int _term;                //开课学期
    MyVector<string> _preCrs; //先修课程
    MyVector<int> _indexOfPre;

public:
    Course(){}; //默认构造

    Course(const Course &crs) //复制构造
        : _id(crs._id), _name(crs._name), _hours(crs._hours), _term(crs._term), _preCrs(crs._preCrs)
    {
    }

    ~Course(){}; //折构

    bool empty() const
    {
        return _id.empty();
    }

    void clear()
    {
        _id.clear();
        _name.clear();
        _preCrs.clear();
    }

    bool operator==(string str) const
    {
        return str == _id;
    }

    bool havePre() const
    {
        return !(_preCrs.empty());
    }

    friend ifstream &operator>>(ifstream &ifs, Course &crs) //从文件读取输入
    {
        string str;
        getline(ifs, str);
        if (str.empty())
            return ifs;
        MyVector<string> tmp = cutString(str);
        if (tmp.size() < 4)
            throw Exception("读取文件失败,课程信息不足", Exception::Bad);
        //编号
        crs._id = tmp[0];
        //开课学期
        crs._name = tmp[1];

        //学时
        if (!isInt(tmp[2]))
            throw Exception("读取文件失败,课程" + crs._id + "学时不为正整数(若正确，请检查课程名称是否含有空格)", Exception::Bad);
        crs._hours = stoi(tmp[2]);
        if (crs._hours == 0 || crs._hours > 50)
            throw Exception("课程" + crs._id + "学时大于50或为0", Exception::Bad);

        //开课学期
        if (!isInt(tmp[3]))
            throw Exception("读取文件失败,课程" + crs._id + "开课学期不为非负整数", Exception::Bad);
        crs._term = stoi(tmp[3]);
        if (crs._hours > 8)
            throw Exception("课程" + crs._id + "开课学期大于8或为0", Exception::Bad);

        //先修课程
        for (int i = 4; i < tmp.size(); i++)
        {
            if (crs._preCrs.find(tmp[i]) == crs._preCrs.end())
                crs._preCrs.push_back(tmp[i]);
            else
            {
                throw Exception("课程" + crs._id + "先修课程" + tmp[i] + "重复出现", Exception::Fail);
            }
        }
        return ifs;
    }

    friend ostream &operator<<(ostream &os, Course &crs)
    {
        output(crs._id, 10);
        output(crs._name, 20);
        output(to_string(crs._hours), 8);
        output(to_string(crs._term), 14);
        for (string str : crs._preCrs)
        {
            output(str, 10);
        }
        os << endl;
        return os;
    }
};

class Term
{
    friend class WorkData;

private:
    bool _done = false;                   //安排妥当
    MyVector<Course> _courses;            //课程
    MyVector<MyVector<Course>> _schedule; //课程表,存的是序号

public:
    Term() {}
    Term(MyVector<Course> &crss)
        : _courses(crss)
    {
    }

    bool done()
    {
        return _done;
    }

    void push_back(Course &crs)
    {
        _courses.push_back(crs);
    }

    void clear()
    {
        _courses.clear();
        _done = false;
    }

    void showCrs()
    {
        for (Course crs : _courses)
        {
            cout << crs;
        }
    }

    int available_3(int day)
    {
        if (_schedule[2][day].empty()) //3-5
            if (_schedule[3][day].empty())
                if (_schedule[4][day].empty())
                    return 2;
        if (_schedule[7][day].empty()) //8-10
            if (_schedule[8][day].empty())
                if (_schedule[9][day].empty())
                    return 7;
        return -1;
    }

    int attributeCourse_3(Course crs, int &hours, int& prefer)
    {
        for (int day = prefer; day < 5; day++, day %= 5)
        {
            int avi = available_3(day);
            if (avi != -1) //可以安排
            {
                prefer = (prefer + 3) % 5;
                _schedule[avi][day] = _schedule[avi + 1][day] = _schedule[avi + 2][day] = crs;
                hours -= 3;
                return day;
            }
        }
        return -1;
    }

    int available_2(int day)
    {
        if (_schedule[0][day].empty()) //1-2
            if (_schedule[1][day].empty())
                return 0;
        if (_schedule[2][day].empty()) //3-4
            if (_schedule[3][day].empty())
                return 2;
        if (_schedule[3][day].empty()) //4-5
            if (_schedule[4][day].empty())
                return 3;

        if (_schedule[5][day].empty()) //6-7
            if (_schedule[6][day].empty())
                return 5;
        if (_schedule[7][day].empty()) //8-9
            if (_schedule[8][day].empty())
                return 7;
        if (_schedule[8][day].empty()) //9-10
            if (_schedule[9][day].empty())
                return 8;
        return -1;
    }

    int attributeCourse_2(Course crs, int &hours, int& prefer)
    {
        for (int day = prefer; day < 5; day++, day %= 5)
        {
            int avi = available_2(day);
            if (avi != -1) //可以安排
            {
                prefer = (prefer + 3) % 5;
                _schedule[avi][day] = _schedule[avi+1][day] = crs;
                hours -= 2;
                return day;
            }
        }
        return -1;
    }

    int available_1(int day)
    {
        for (int i = 0; i < 10; i++)
            if (_schedule[i][day].empty())
                return i;
        return -1;
    }

    int attributeCourse_1(Course crs, int &hours, int &prefer)
    {
        for (int day = prefer; day < 5; day++, day %= 5)
        {
            int avi = available_1(day);
            if (avi != -1) //可以安排
            {
                prefer = (prefer + 3) % 5;
                _schedule[avi][day] = crs;
                hours -= 1;
                return day;
            }
        }
        return -1;
    }

    void attribute()
    {
        _schedule = MyVector<MyVector<Course>>(10, MyVector<Course>(5)); //第几节课，天
        for (Course crs : _courses)
        {
            int lst = 0;
            while (crs._hours != 0)
            {
                if (crs._hours>= 5 || crs._hours == 3)
                    if (attributeCourse_3(crs, crs._hours, lst) != -1)
                        continue;
                if (crs._hours > 1)
                    if (attributeCourse_2(crs, crs._hours, lst) != -1)
                        continue;
                if (attributeCourse_1(crs, crs._hours, lst) != -1)
                    continue;
                throw Exception("课程" + crs._id + "找不到合适的上课时间", Exception::Bad);
            }
        }
    }

    void showSchedule(ostream& os=cout)
    {
        os<<setw(10)<<' ';
        for(int i=0;i<5;i++)
        {
            output("周"+to_string(i+1),6,os);
        }
        os<<endl;
        for(int i=0;i<10;i++)
        {
            output("第"+to_string(i+1)+"节课",10,os);
            for(int day=0;day<5;day++)
                output(_schedule[i][day]._id,6,os);
            os<<endl;
            if(i==1||i==4||i==6||i==9)
                os<<endl;
        }
    }

};

class WorkData //状态量
{
    friend class System;

private:
    int _cantPre = 0; //havePre中可安排的数量
    bool _allDone = false;
    //定长状态量
    MyVector<bool> _isSet;     //已经安排
    MyVector<int> _needed;     //优先级
    MyVector<bool> _available; //先修课程已上完
    int _ncrs[8];              //已经安排的课程数
    //变长状态量
    MyVector<int> _indexLast;   //不与先修相关的课程
    MyVector<int> _indexHIsPre; //与先修相关的课程的课的序号表
    Term _term[8];              //每学期的课程
public:
    WorkData() //默认构造
    {
    }
    WorkData(WorkData &wd) //复制构造
        : _cantPre(wd._cantPre), _allDone(wd._allDone), _isSet(wd._isSet), _needed(wd._needed), _available(wd._available), _indexHIsPre(wd._indexHIsPre)
    {
        for (int i = 0; i < 8; i++)
        {
            _ncrs[i] = wd._ncrs[i];
            _term[i] = wd._term[i];
        }
    }

    WorkData(WorkData &&wd) //复制构造
        : _cantPre(wd._cantPre), _allDone(wd._allDone), _isSet(wd._isSet), _needed(wd._needed), _available(wd._available), _indexHIsPre(wd._indexHIsPre)
    {
        for (int i = 0; i < 8; i++)
        {
            _ncrs[i] = wd._ncrs[i];
            _term[i] = wd._term[i];
        }
    }

    WorkData &operator=(WorkData &wd)
    {
        _cantPre = wd._cantPre;
        _allDone = wd._allDone;
        _isSet = wd._isSet;
        _needed = wd._needed;
        _available = wd._available;
        _indexHIsPre = wd._indexHIsPre;
        for (int i = 0; i < 8; i++)
        {
            _ncrs[i] = wd._ncrs[i];
            _term[i] = wd._term[i];
        }
        return *this;
    }

    WorkData &operator=(WorkData &&wd)
    {
        _cantPre = wd._cantPre;
        _allDone = wd._allDone;
        _isSet = wd._isSet;
        _needed = wd._needed;
        _available = wd._available;
        _indexHIsPre = wd._indexHIsPre;
        for (int i = 0; i < 8; i++)
        {
            _ncrs[i] = wd._ncrs[i];
            _term[i] = wd._term[i];
        }
        return *this;
    }

    void clearTerm()
    {
        for (int i = 0; i < 8; i++)
        {
            _term[i].clear();
        }
    }

    void initialize(); //初始化

    void updateNeeded(int index); //给index的所有先修课程++

    bool isAvailable(int index);

    void updateAvailable(); //更新available

    void updateUnset(); //设置needed并给unset排序

    void update();

    bool allDone()
    {
        int res = true;
        for (int i = 0; i < 8; i++)
        {
            if (!res)
                break;
            res = res && _term[i].done();
        }
        return res;
    }

    void attribute(int index, int term); //给学期安排课程
};

class System
{
private:
    WorkData _wd;
    Term _term[8];                  //结果
    void readFile(string fileName); //从文件读取所有课程

    void checkPre(int index); //确认先修课程是否存在并更新index

    void initializeIndex(); //对序号表进行初始化

    void resetNCrs()
    {
        for (int i = 0; i < 8; i++)
            _ncrs[i] = 0;
    }

    bool findBefore(string id, int index); //id可以在index或其先修里找到

    void simplifyPre(); //简化先修链条：我的先修课的先修课不必写在我的先修课中

    void showCrs(); //输出到n的各学期课程数

public:
    static MyVector<Course> _courses;      //所有课程
    static MyVector<int> _indexHavePre;    //有先修课程的课的序号表，方便处理
    static MyVector<int> _indexDueTerm[8]; //固定学期的课的序号表
    static int _ndue[8];                   //固定课程数
    static int _ncrs[8];                   //所需课程数

    System()
    {
        for (int i = 0; i < 8; i++)
            _ndue[i] = _ncrs[i] = 0;
    }

    void initialize(string fileName); //总体读文件初始化

    void inputCrs(); //输入各学期的课程数

    void clearTerm()
    {
        for (int i = 0; i < 8; i++)
            _term[i].clear();
    }

    WorkData distributePre(WorkData wd, int term, int it) //递归寻找Pre解决方案
    {
        //8个学期
        for (int t = it; t >= 0; t--)
        {
            int index = wd._indexHIsPre[t];
            WorkData tmp = wd, t2;
            if ((!tmp._available[index]) || tmp._isSet[index])
                continue;
            tmp.attribute(index, term);
            if (tmp._term[term].done())
            {
                tmp.update();
                if (tmp._allDone)
                    return tmp;
                t2 = distributePre(tmp, term + 1, tmp._indexHIsPre.size() - 1);
            }
            else
                t2 = distributePre(tmp, term, it);
            if (t2._allDone)
                return t2;
        }
        return wd;
    }

    void distributeLst(WorkData &wd) //递归寻找Pre解决方案
    {
        //8个学期
        int j = 0;
        for (int i = 0; i < 8; i++)
        {
            while (_ncrs[i] > wd._ncrs[i])
            {
                for (; j < wd._indexLast.size(); j++)
                {
                    wd.attribute(wd._indexLast[j], i);
                }
            }
        }
    }

    void distribute() //总体寻找
    {
        log("正在安排课程");
        _wd.initialize();
        _wd.clearTerm();
        clearTerm();
        for (int i = 0; i < 8; i++) //固定学期的课程
        {
            for (int t : _indexDueTerm[i])
                _wd.attribute(t, i);
        }
        int cnt = 0;
        _wd = distributePre(_wd, 0, _wd._indexHIsPre.size() - 1);
        if (_wd._allDone)
        {
            distributeLst(_wd);
        }
        for (int i = 0; i < 8; i++) //复制课程到term
        {
            _term[i] = _wd._term[i];
        }
    }

    bool showTermCrs()
    {
        if (!_wd.allDone())
            return false;
        for (int i = 0; i < 8; i++)
        {
            cout << endl;
            cout << _term[i].done();
            log("第" + to_string(i) + "学期");
            _term[i].showCrs();
        }
        return true;
    }

    bool allDone()
    {
        return _wd._allDone;
    }

    void setSchedule()
    {
        for(int i=0;i<8;i++)
        {
            _term[i].attribute();
        }
    }


    void showSchedule()
    {
        for(int i=0;i<8;i++)
        {
            cout<<endl;
            log("               第"+to_string(i+1)+"学期");
            _term[i].showSchedule();
        }
    }

    void writeSchedule(string fileName)
    {
        ofstream of;
        of.open(fileName,ios::out|ios::trunc);
        if(!of.is_open())
            throw Exception(fileName+"不存在，输出失败",Exception::Bad);
        for(int i=0;i<8;i++)
        {
            of<<endl;
            log("               第"+to_string(i+1)+"学期",'\n',of);
            _term[i].showSchedule(of);
        }
    }


    void showCourses(); //输出所有课程
};

MyVector<Course> System::_courses;
MyVector<int> System::_indexHavePre;
MyVector<int> System::_indexDueTerm[8];
int System::_ndue[8];
int System::_ncrs[8];

void WorkData::initialize() //准备安排课程
{
    _cantPre = 0;
    _allDone = false;
    _indexHIsPre.clear();
    _isSet.clear();
    _needed.clear();
    _available.clear();
    _indexLast.clear();
    _isSet.resize(System::_courses.size(), false);
    _needed.resize(System::_courses.size(), 0);
    _available.resize(System::_courses.size(), false);
    //初始化_available和HisPre
    for (int i = 0; i < _available.size(); i++)
    {
        Course &crs = System::_courses[i];
        if (crs._term == 0 && crs.havePre())
            _indexHIsPre.push_back(i);
        if (!crs.havePre())
            _available[i] = true;
    }
    //将没有先修但为其他先修的丢入其中
    for (int i = 0; i < _indexHIsPre.size(); i++)
    {
        Course &crs = System::_courses[_indexHIsPre[i]];
        for (int t : crs._indexOfPre)
        {
            auto it = _indexHIsPre.find(t);
            if (it == _indexHIsPre.end())
                _indexHIsPre.push_back(t);
        }
    }
    //剩余均丢入Last中
    for (int i = 0; i < System::_courses.size(); i++)
    {
        Course &crs = System::_courses[i];
        if (crs._term == 0 && !crs.havePre())
        {
            auto it = _indexHIsPre.find(i);
            if (it == _indexHIsPre.end())
                _indexLast.push_back(i);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        _ncrs[i] = 0;
    }
    update();
}

void WorkData::updateNeeded(int index) //给index的所有先修课程++
{
    Course &crs = System::_courses[index];
    if (crs.havePre())
    {
        for (int i : crs._indexOfPre)
        {
            _needed[i]++;
            updateNeeded(i);
        }
    }
}

bool WorkData::isAvailable(int index)
{
    bool res = true;
    Course &crs = System::_courses[index];
    if (crs.havePre())
        for (int i : crs._indexOfPre)
        {
            if (!res)
                break;
            res = res && _isSet[i];
        }
    return res;
}

void WorkData::updateAvailable() //更新available
{
    _cantPre = 0;
    for (int i : System::_indexHavePre)
    {
        if (isAvailable(i))
            _available[i] = true;
        else
            _cantPre++;
    }
}

void WorkData::updateUnset() //更新unset设置needed并给unset排序
{
    //更新unset
    for (int i = _indexHIsPre.size() - 1; i >= 0; i--)
    {
        if (_isSet[_indexHIsPre[i]])
            _indexHIsPre.erase(_indexHIsPre.iter(i));
    }
    if (_indexHIsPre.empty())
    {
        _allDone = true;
        return;
    }
    //清空
    for (int i = 0; i < _needed.size(); i++)
    {
        _needed[i] = 0;
    }
    //更新needed
    for (int i : _indexHIsPre)
    {
        updateNeeded(i);
    }
    //冒泡排序，优先级大的放后面
    for (int i = _indexHIsPre.size() - 1; i > 0; i--)
    {
        for (int j = 1; j <= i; j++)
        {
            if (_needed[_indexHIsPre[j]] < _needed[_indexHIsPre[j - 1]])
                Swap(_indexHIsPre[j], _indexHIsPre[j - 1]);
        }
    }
}

void WorkData::update()
{
    updateUnset();
    updateAvailable();
}

void WorkData::attribute(int index, int term) //给学期安排课程
{
    Course &crs = System::_courses[index];
    _term[term].push_back(crs);
    _isSet[index] = true;
    _ncrs[term]++;
    if (_ncrs[term] - System::_ndue[term] == _indexHIsPre.size())
    {
        for (int i = 0; i < 8; i++)
        {
            _term[term]._done = true;
        }
        _allDone = true;
        return;
    }
    if (_ncrs[term] == System::_ncrs[term])
        _term[term]._done = true;
    _allDone = true;
    for (int i = 0; i < 8; i++)
    {
        if (_allDone)
            _allDone = _allDone && _term[i]._done;
    }
}

void System::readFile(string fileName) //从文件读取所有课程
{
    ifstream in;
    in.open(fileName);
    if (!in.is_open())
        throw Exception("未找到文件" + fileName, Exception::Bad);
    string t;
    getline(in, t);
    while (!in.eof())
    {
        try
        {
            Course tmp;
            in >> tmp;
            if (!tmp.empty())
                _courses.push_back(tmp);
        }
        catch (Exception exc)
        {
            log(exc.data());
            if (exc.bad())
                exit(0);
        }
    }
}

void System::checkPre(int index) //确认先修课程是否存在并更新index
{
    Course &crs = _courses[index];
    for (string str : crs._preCrs)
    {
        auto it = _courses.find(str);
        if (it == _courses.end())
            throw Exception(crs._id + "课程的先修课程" + str + "未找到", Exception::Bad);
        else
        {
            crs._indexOfPre.push_back(it.getPos());
        }
    }
}

void System::initializeIndex() //对序号表进行初始化
{
    for (int i = 0; i < _courses.size(); i++)
    {
        Course &crs = _courses[i];
        if (crs._term != 0)
        {
            _indexDueTerm[crs._term - 1].push_back(i);
            _ndue[crs._term - 1]++;
        }
        else
        {
            if (crs.havePre())
            {
                checkPre(i);
                _indexHavePre.push_back(i);
            }
        }
    }
}

bool System::findBefore(string id, int index) //id可以在index或其先修里找到
{
    Course &crs = _courses[index];
    if (id == crs._id)
        return true;
    bool res = false;
    if (crs.havePre())
    {
        for (int i : crs._indexOfPre)
        {
            res = res || findBefore(id, i);
            if (res == true)
                return true;
        }
    }
    return false;
}

void System::simplifyPre() //简化先修链条：我的先修课的先修课不必写在我的先修课中
{
    for (Course &crs : _courses)
    {
        for (int i = 0; i < crs._preCrs.size(); i++)
        {
            for (int j = 0; j < crs._preCrs.size(); j++)
            {
                if (j == i)
                    continue;
                if (findBefore(crs._preCrs[i], j))
                {
                    crs._preCrs.erase(crs._preCrs.iter(i));
                    crs._indexOfPre.erase(crs._indexOfPre.iter(i));
                    i--;
                    break;
                }
            }
        }
    }
}

void System::initialize(string fileName) //总体读文件初始化
{
    readFile("in.txt");
    initializeIndex();
    simplifyPre();
}

void System::inputCrs() //输入各学期的课程数
{
    resetNCrs();
    int size = _courses.size();
    int cnt = 0; //已经输入的课程总数
    int tmp;     //输入缓存
    cout << endl;
    log("当前课程总数为:" + to_string(cnt));
    showCrs();
    for (int i = 0; i < 8; i++)
    {
        int max = size;
        for (int j = 7; j > i; j--)
            max -= _ndue[j];
        max -= cnt;
        int min = (i == 7 ? max : _ndue[i]);
        cout << "请输入第" << i + 1 << "学期的课程数(最小" << min << "最大" << max << "):";
        cin >> tmp;
        if (cinFail())
        {
            log("输入失败，请重试");
            i--;
            continue;
        }
        if (tmp > max || tmp < min)
        {
            log("输入失败,请输入" + to_string(min) + "到" + to_string(max) + "之间的整数");
            i--;
            continue;
        }
        _ncrs[i] = tmp;
        cnt += _ncrs[i];
        cout << endl;
        log("输入成功,当前课程总数为:" + to_string(cnt));
        showCrs();
        log("课程数已输入完毕");
    }
}

void System::showCrs() //输出到n的各学期课程数
{
    cout << "各学期的课程数分别为:";
    for (int i = 0; i < 8; i++)
        cout << left << setw(3) << _ncrs[i];
    cout << endl;
}

void System::showCourses() //输出所有课程
{
    output("课程编号", 10);
    output("课程名称", 20);
    output("学时数", 8);
    output("指定开课学期", 14);
    log("先修课程");
    for (Course crs : _courses)
    {
        cout << crs;
    }
}

int main()
{
    try
    {
        System system;
        system.initialize("in.txt");
        log("输入成功，课程列表如下(课程列表已经过处理，删去了冗余的先修课程)");
        system.showCourses();
        while (true)
        {
            system.inputCrs();
            system.distribute();
            if (system.allDone())
                break;
            else
            {
                log("不存在所求的安排，请重新安排并合理安排课程数");
            }
        }
        system.setSchedule();
        log("已经成功生成课表，课表如下:");
        system.showSchedule();
        system.writeSchedule("out.txt");
        log("课表已经成功输出至out.txt,请查看");
    }
    catch (Exception exc) //未经捕获的异常都是bad，直接结束
    {
        cout << exc << endl;
        exit(0);
    }
    return 0;
}