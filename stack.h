#pragma once

class stack_empty_error
{
private:
	std::string errorText;

public:
	stack_empty_error(const std::string& err)
	{
		errorText = err;
	}

	std::string what() const
	{
		return errorText;
	}
};

template<typename T>
class Stack
{
private:
	void BufReconstruct()
	{
		T* new_buf = nullptr;
		int new_depth;

		if (!_depth)
		{
			new_depth = 1;
			new_buf = alloc.allocate(new_depth);
		}
		else
		{
			new_buf = alloc.allocate(_depth * 2);
			new_depth = _depth * 2;
		}

		for (int i = 0; i < _size; i++)
		{
			std::allocator_traits<std::allocator<T>>::construct(alloc, new_buf + i, *(_buf + i));

			std::allocator_traits<std::allocator<T>>::destroy(alloc, _buf + i);
		}

		alloc.deallocate(_buf, _depth);

		_buf = new_buf;
		_depth = new_depth;
	}

	void BufConstruct(const T* other, int size)
	{
		for (int i = 0; i < size; i++)
		{
			std::allocator_traits<std::allocator<T>>::construct(alloc, _buf + i, *(other + i));
		}
	}

	void BufDestroy()
	{
		for (int i = 0; i < _size; i++)
		{
			std::allocator_traits<std::allocator<T>>::destroy(alloc, _buf + i);
		}
	}

	void ToZero()
	{
		_depth = 0;
		_size = 0;
		_buf = nullptr;
	}

	void Move(Stack<T>& other)
	{
		_depth = other._depth;
		_size = other._size;
		_buf = other._buf;

		other.ToZero();
	}

	void Add(const T& value)
	{
		std::allocator_traits<std::allocator<T>>::construct(alloc, _buf + _size, value);

		_size++;
	}

public:
	Stack() noexcept : _depth(4), _buf(alloc.allocate(_depth)) {};

	Stack(const int& depth) noexcept : _depth(depth), _buf(alloc.allocate(_depth)) {}

	Stack(const Stack<T>& other) noexcept : _size(other._size), _depth(other._depth), _buf(alloc.allocate(other._depth))
	{
		this->BufConstruct(other._buf, other._size);
	}

	Stack(Stack<T>&& other) noexcept : _size(other._size), _depth(other._depth), _buf(other._buf)
	{
		other.ToZero();
	}

	~Stack() noexcept
	{
		if (_buf) 
		{
			this->BufDestroy();

			alloc.deallocate(_buf, _depth);
		}
	}

	Stack<T>& operator= (const Stack<T>& other)
	{
		if (*this == other)
		{
			return *this;
		}

		if (!_buf)
		{
			_depth = other._depth;
			_size = other._size;
			this->BufConstruct(other._buf, other._size);

			return *this;
		}
		else
		{
			this->BufDestroy();

			this->BufConstruct(other._buf, other._size);

			_depth = other._depth;
			_size = other._size;

			return *this;
		}
		
	}

	Stack<T>& operator= (Stack<T>&& other)
	{
		if (*this == other)
		{
			return *this;
		}

		if (!_buf)
		{
			this->Move(other);

			return *this;
		}
		else
		{
			this->BufDestroy();

			this->Move(other);

			return *this;
		}
	}

	bool operator==(const Stack<T>& other) const
	{
		if (_depth == other._depth && _size == other._size)
		{
			for (int i = 0; i < _size; i++)
			{
				if (_buf[i] != other._buf[i])
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsEmpty() const
	{		
		return _size == 0;
	}

	void Push(const T& value) 
	{
		if (_size == _depth) 
		{	
			this->BufReconstruct();

			this->Add(value);
		}
		else 
		{
			this->Add(value);
		}
	}

	void Pop() 
	{
		if (!_size) 
		{
			throw stack_empty_error("Stack is empty!\n");
		}
		else 
		{
			std::allocator_traits<std::allocator<T>>::destroy(alloc, _buf + _size - 1);

			_size--;
		}
	}

	T& Top() const
	{
		if (!_size)
		{
			throw stack_empty_error("Stack is empty!\n");
		}
		else
		{
			return *(_buf + _size - 1);
		}
	}

	const T& CTop() const
	{
		if (!_size)
		{
			throw stack_empty_error("Stack is empty!\n");
		}
		else
		{
			return *(_buf + _size - 1);
		}
	}

	int Depth() const
	{
		return _depth;
	}

	int Size() const	
	{
		return _size;
	}

private:
	int _depth;
	int _size = 0;
	T* _buf = nullptr;

	std::allocator<T> alloc;
};
