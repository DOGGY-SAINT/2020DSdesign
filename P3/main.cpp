//题目三  勇闯迷宫游戏
//dfs
//MyVector已加入
//已完成
#include <iostream>
#include "MyVector.h"

using namespace std;

#define WALL '#'
#define EMPTY '0'
#define ENTRANCE '1'
#define EXIT '2'
#define SIZE 7
#define OPPOSITE(x) 3 - x

const char defaultmap1[SIZE][SIZE] =
	{{'#', '#', '#', '#', '#', '#', '#'},
	 {'#', '1', '#', '0', '0', '0', '#'},
	 {'#', '0', '#', '0', '#', '#', '#'},
	 {'#', '0', '0', '0', '#', '#', '#'},
	 {'#', '0', '#', '0', '0', '0', '#'},
	 {'#', '0', '#', '0', '#', '2', '#'},
	 {'#', '#', '#', '#', '#', '#', '#'}};

const char defaultmap2[SIZE][SIZE] =
	{{'#', '#', '#', '#', '#', '#', '#'},
	 {'#', '1', '#', '0', '0', '0', '#'},
	 {'#', '0', '#', '0', '#', '#', '#'},
	 {'#', '0', '0', '0', '#', '#', '#'},
	 {'#', '0', '#', '0', '0', '#', '#'},
	 {'#', '0', '#', '0', '#', '2', '#'},
	 {'#', '#', '#', '#', '#', '#', '#'}};


class Position
{
	friend class Solution;
	friend class Map;
private:
	int _x, _y;
public:
	Position()
	{
	}
	Position(int x, int y) : _x(x), _y(y)
	{
	}
	bool isInside()
	{
		return _x >= 0 && _y >= 0 && _x < SIZE && _y < SIZE;
	}
	Position operator+(const Position &p2) const
	{
		return Position(p2._x + _x, p2._y + _y);
	}
	bool operator==(const Position &p2) const
	{
		return _x == p2._x && _y == p2._y;
	}
	friend ostream &operator<<(ostream &os, const Position &pos)
	{
		os << "<" << pos._x << "," << pos._y << ">";
		return os;
	}
};
//左，上，下，右
const Position Direction[4] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

class Solution
{
private:
	MyVector<Position> _solution;
	bool _arrive;

public:
	Solution()
	{
	}
	void setArrive(bool arrive)
	{
		_arrive = arrive;
	}
	bool arrive() const
	{
		return _arrive;
	}
	bool empty()
	{
		return _solution.empty();
	}
	void clear()
	{
		_solution.clear();
		_arrive=false;
	}
	Solution operator+(const Position &pos) const
	{
		Solution res = *this;
		res._solution.push_back(pos);
		return res;
	}
	Solution operator+=(const Position &pos)
	{
		_solution.push_back(pos);
		return *this;
	}
	Solution operator--()
	{
		_solution.pop_back();
		return *this;
	}
	friend ostream &operator<<(ostream &os, Solution &solution)
	{
		for (auto it = solution._solution.begin();;)
		{
			os << *it;
			if (++it != solution._solution.end())
				os << " ---> ";
			else
				break;
		}
		return os;
	}
};

class Map
{
private:
	char _map[SIZE][SIZE];
	Position _entrance, _exit;
	Solution _solution;
public:
	Map()
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				_map[i][j] = defaultmap1[i][j];
		_entrance = Position(1, 1), _exit = Position(5, 5);
	}
	Map(const char map[SIZE][SIZE])
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
			{
				if (map[i][j] == ENTRANCE)
					_entrance = Position(i, j);
				else if (map[i][j] == EXIT)
					_exit = Position(i, j);
				_map[i][j] = map[i][j];
			}
	}
	//设置地图
	void setMap(const char map[SIZE][SIZE])
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
			{
				if (map[i][j] == ENTRANCE)
					_entrance = Position(i, j);
				else if (map[i][j] == EXIT)
					_exit = Position(i, j);
				_map[i][j] = map[i][j];
			}
	}
	//展示地图
	void showMap()
	{
		cout <<endl<< "地图(起点为1终点为2):" << endl;
		cout << ' ' << "    ";
		for (int i = 0; i < SIZE; i++)
		{
			cout << i << "列  ";
		}
		cout << endl;
		for (int i = 0; i < SIZE; i++)
		{
			cout << i << "行  ";
			for (int j = 0; j < SIZE; j++)
				cout << _map[i][j] << "    ";
			cout << endl;
		}
	}
	//寻路
	bool findSolution()
	{
		_solution.clear();
		_solution = findNextStep(_entrance, _solution, -1);
		if (_solution.arrive())
			return true;
		return false;
	}
	char operator[](Position pos) const
	{
		return _map[pos._x][pos._y];
	}
	//递归
	Solution findNextStep(Position pos, Solution solution, int lastdir) const
	{
		solution += pos;
		if (pos == _exit)
		{
			solution.setArrive(true);
			return solution;
		}
		Solution newSolution = solution;
		for (int dir = 0; dir < 4; dir++)
		{
			if (dir == OPPOSITE(lastdir))
				continue;
			Position nextPos = pos + Direction[dir];
			if (nextPos.isInside())
				if ((*this)[nextPos] == EMPTY || (*this)[nextPos] == EXIT)
				{
					newSolution = findNextStep(nextPos, solution, dir);
					if (newSolution.arrive())
						return newSolution;
				}
		}
		return newSolution;
	}
	void showSolution()
	{
		if (_solution.arrive())
		{
			cout << "路径:" << endl;
			cout << _solution << endl;
		}
		else
		{
			cout << "路径不存在" << endl;
		}
	}
};


int main()
{
	Map map;
	map.showMap();
	map.findSolution();
	map.showSolution();
	map.setMap(defaultmap2);
	map.showMap();
	map.findSolution();
	map.showSolution();
	return 0;
}