//2020.12.1 11:22 remove ostream, rename some functions
#ifndef _MYLIST_H_
#define _MYLIST_H_

template<typename>
class MyListNode;
template<typename>
class MyList;
template<typename, typename>
class MyListIterator;

template<typename val_type>
class MyListNode
{
	using node = MyListNode;
	friend class MyList<val_type>;
	friend class MyListIterator<node, val_type>;
private:
	val_type _val;
	node* _prev;
	node* _next;
	//删除后续所有节点
	void deleteAll(node* head)
	{
		if (_next != head && _next)
			_next->deleteAll(head);
		delete this;
	}
public:
	MyListNode()
		:_val(-1)
		, _next(nullptr)
		, _prev(nullptr)
	{}
	MyListNode(const node& node)
		:_val(node._val)
		, _next(node._next)
		, _prev(node._prev)
	{}
	MyListNode(const val_type& val)
		:_val(val)
		, _next(nullptr)
		, _prev(nullptr)
	{}
	~MyListNode()
	{}
	node* prev()const
	{
		return _prev;
	}
	node* next()const
	{
		return _next;
	}
};

template<typename node_type, typename val_type>
class MyListIterator
{
	using iterator = MyListIterator;
	using node = node_type;
	using pointer = node_type*;
	using reference = val_type&;
	friend class MyList<val_type>;
private:
	pointer _ptr;
public:
	MyListIterator(pointer ptr = nullptr) :_ptr(ptr) {}
	bool operator==(const iterator& it)
	{
		return _ptr == it._ptr;
	}
	bool operator!=(const iterator& it)
	{
		return _ptr != it._ptr;
	}
	iterator& operator++()
	{
		_ptr = _ptr->_next;
		return *this;
	}
	iterator& operator++(int)
	{
		iterator tmp = *this;
		_ptr = _ptr->_next;
		return tmp;
	}
	iterator& operator--()
	{
		_ptr = _ptr->_prev;
		return *this;
	}
	iterator& operator--(int)
	{
		iterator tmp = *this;
		_ptr = _ptr->_prev;
		return tmp;
	}
	reference operator*()
	{
		return _ptr->_val;
	}
	pointer operator->()
	{
		return _ptr;
	}
};

template<typename val_type>
class MyList
{
	using list = MyList;
	using node = MyListNode<val_type>;
	using iterator = MyListIterator<node, val_type>;
private:
	int _length;
	node* _head;
	node* _tail;
public:
	//构造
	MyList()
		:_head(nullptr)
		, _tail(nullptr)
		, _length(0)
	{
		_head = new node;
		_head->_prev = _head;
		_head->_next = _head;
		_tail = _head;
	}
	MyList(MyList<val_type>& l2)
		:_head(nullptr)
		, _tail(nullptr)
		, _length(0)
	{
		_head = new node;
		_head->_prev = _head;
		_head->_next = _head;
		_tail = _head;
		for (auto val : l2)
		{
			push_back(val);
		}
	}
	//析构
	~MyList()
	{
		_head->deleteAll(_head);
	}
	iterator begin()
	{
		return iterator(_head->_next);
	}
	iterator end()
	{
		return iterator(_tail->_next);
	}
	//清空表，留头节点
	void clear()
	{
		while (_head->_next != _head)
			remove(_head->_next);
	}
	//长度
	int length()
	{
		return _length;
	}
	int size()
	{
		return _length;
	}
	void insert(iterator position, const val_type& val)
	{
		node* p = position._ptr;
		node* data = new node(val);
		data->_next = p->_next;
		data->_prev = p;
		p->_next = data;
		data->_next->_prev = data;
		if (p == _tail)
			_tail = data;
		_length++;
	}
	val_type remove(iterator position)
	{
		val_type data = *position;
		node* p = position._ptr;
		p->_prev->_next = p->_next;
		p->_next->_prev = p->_prev;
		if (p == _tail)
			_tail = p->_prev;
		delete p;
		_length--;
		return data;
	}
	void push_back(const val_type& val)
	{
		insert(--end(), val);
	}
	val_type pop_back()
	{
		return remove(--end());
	}
	list& operator= (list& l2)
	{
		clear();
		for (auto val : l2)
		{
			push_back(val);
		}
		return *this;
	}
	list& operator= (list&& l2)
	{
		clear();
		for (auto val : l2)
		{
			push_back(val);
		}
		return *this;
	}
};

#endif // !_MYLIST_H_
