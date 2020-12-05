//version:2020.11.14 16:58
#ifndef _MYVECTOR_H_
#define _MYVECTOR_H_



template<typename>
class MyVector;
template<typename>
class MyVectorIterator;

#define MYVECTORTYPE(typeName,valName,funName)			\
private:typeName valName;								\
public:void set##funName (typeName val){valName=val;}	\
public:typeName get##funName ()const{return valName;}

#define DEFAULT_CAPACITY 4

template<typename val_type>
class MyVectorIterator
{
	using iterator = MyVectorIterator;
	using pointer = val_type*;
	using reference = val_type&;
	friend class MyVector<val_type>;
private:
	pointer _ptr;
	int _pos;
public:
	MyVectorIterator(pointer ptr = nullptr, int pos = -1) :_ptr(ptr), _pos(pos)
	{}
	~MyVectorIterator()
	{}
	int getPos()
	{
		return _pos;
	}
	bool operator==(const iterator& it)const
	{
		return _ptr == it._ptr;
	}
	bool operator!=(const iterator& it)const
	{
		return _ptr != it._ptr;
	}
	iterator& operator++()
	{
		_ptr++;
		_pos++;
		return *this;
	}
	iterator& operator++(int)
	{
		iterator tmp = *this;
		_ptr++;
		_pos++;
		return tmp;
	}
	iterator& operator--()
	{
		_ptr--;
		_pos--;
		return *this;
	}
	iterator& operator--(int)
	{
		iterator tmp = *this;
		_ptr--;
		_pos--;
		return tmp;
	}
	reference operator*()
	{
		return *_ptr;
	}
	pointer operator->()
	{
		return _ptr;
	}
};

template<typename val_type>
class MyVector
{
	using vector = MyVector<val_type>;
	using reference = val_type&;
	using iterator = MyVectorIterator<val_type>;
private:
	int _size;
	int _capacity;
	val_type* _data;
	void enlarge(int newcapacity)
	{
		if (_size > newcapacity)
			return;
		val_type* newdata = new val_type[newcapacity];
		for (int i = 0; i < _size; i++)
		{
				newdata[i] = _data[i];
		}
		delete[] _data;
		_data = newdata;
		_capacity = newcapacity;
	}
public:
	MyVector(int capacity = DEFAULT_CAPACITY)
		:_size(0)
		, _capacity(capacity)
	{
		_data = new val_type[capacity];
	}
	MyVector(const vector& vec)
		:_size(vec._size)
		,_capacity(vec._capacity)
	{
		_data = new val_type[_capacity];
		for (int i = 0; i < _size; i++)
			_data[i] = vec._data[i];
	}
	~MyVector()
	{
		delete[] _data;
	}
	bool empty()
	{
		return _size == 0;
	}
	int size()
	{
		return _size;
	}
	int length()
	{
		return _size;
	}
	int capacity()
	{
		return _capacity;
	}
	void resize(int newsize,val_type val=val_type())
	{
		if (newsize > _capacity)
			enlarge(newsize * 2);
		for (int i = _size; i < newsize; i++)
		{
				_data[i] = val;
		}
		_size = newsize;
	}
	void insert(iterator pos, const val_type& val)
	{
		if (_capacity - _size <= 1)
			enlarge(_capacity * 2);
		for(int i=_size;i>pos._pos;i--)
			_data[i] = _data[i-1];
		_data[pos._pos] = val;
		_size++;
	}
	val_type erase(iterator pos)
	{
		val_type tmp = _data[pos._pos];
		_size--;
		for (int i = pos._pos; i < _size; i++)
			_data[i] = _data[i + 1];
		return tmp;
	}
	void push_back(val_type val)
	{
		if (_capacity - _size <= 1)
			enlarge(_capacity * 2);
		_data[_size]=val;
		_size++;
	}
	val_type pop_back()
	{
		return erase(iterator(&_data[_size - 1], _size - 1));
	}
	vector& operator=(vector& vec)
	{
		delete[]_data;
		_size = vec._size;
		_capacity = vec._capacity;
		_data = new val_type[_capacity];
		for (int i = 0; i < _size; i++)
			_data[i] = vec._data[i];
		return *this;
	}
	vector& operator=(vector&& vec)
	{
		delete[]_data;
		_size = vec._size;
		_capacity = vec._capacity;
		_data = new val_type[_capacity];
		for (int i = 0; i < _size; i++)
			_data[i] = vec._data[i];
		return *this;
	}

	iterator find(val_type val)
	{
		auto it = begin();
		for (; it != end(); it++)
		{
			if (*it == val)
				break;
		}
		return it;
	}
	reference at(int pos)
	{
		return _data[pos];
	}
	reference operator[](int pos)
	{
		if (pos < 0 || pos >= _size)
			throw "oversides";
		return _data[pos];
	}

	reference front()
	{
		if (_size)
			return _data[0];
	}
	reference back()
	{
		if (_size)
			return _data[_size-1];
	}

	iterator begin()
	{
		return iterator(&_data[0], 0);
	}
	iterator end()
	{
		return iterator(&_data[_size], _size);
	}

	void clear()
	{
		val_type* newdata = new val_type[DEFAULT_CAPACITY];
		delete[] _data;
		_data = newdata;
		_capacity = DEFAULT_CAPACITY;
		_size = 0;
	}
};

#endif // !_MYVECTOR_H_
