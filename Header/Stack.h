#pragma once
static const int MAX_CAPACITY = 1024 * 1024 * 10;
static const int DEFAULT_CAPACITY = 1024 * 1024 * 4;

template<typename T>
class Stack
{
public:
	class Iterator
	{
	private:
		Stack<T>* stackRef;
		int index;

	public:
		Iterator(Stack<T>* s, int i) : stackRef(s), index(i) {}

		T& operator*()
		{
			return stackRef->stack[index];
		}

		Iterator& operator++()
		{
			index++;
			return *this;
		}

		bool operator==(const Iterator other) const { return index == other.index; }
		bool operator!=(const Iterator other) const { return index != other.index; }
	};

	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, top + 1); }

	Stack();
	~Stack();

	int Capacity() const;
	bool IsEmpty() const;
	bool IsFull() const;
	bool Push(T value);
	bool Pop(T& out);
	bool Peek(T& out) const;
	int Size() const;
	void Clear();

	T& operator[](int i);

private:
	T* stack;
	int top;
	int capacity;

	void Resize(int newCapacity);
};

template<typename T>
Stack<T>::Stack() : stack(nullptr), top(-1), capacity(0)
{
	capacity = DEFAULT_CAPACITY / sizeof(T);
	stack = new T[capacity];
}

template<typename T>
Stack<T>::~Stack()
{
	if(stack)
		delete[] stack;
	Clear();
}

template<typename T>
int Stack<T>::Capacity() const
{
	return capacity;
}

template<typename T>
bool Stack<T>::IsEmpty() const
{
	return (top == -1);
}

template<typename T>
bool Stack<T>::IsFull() const
{
	return ((top+1) == capacity);
}

template<typename T>
bool Stack<T>::Push(T value)
{

	if (IsFull())
	{
		if (capacity < (MAX_CAPACITY / sizeof(T)))
		{
			Resize(capacity * 2);
		}
		if (IsFull())
		{
			return false;
		}
	}
	top++;
	stack[top] = value;
	return true;
}

template<typename T>
bool Stack<T>::Pop(T& out)
{
	if (IsEmpty())
	{
		return false;
	}

	T popData;
	popData = stack[top--];

	if (Size() <= capacity / 4 && capacity > (DEFAULT_CAPACITY / sizeof(T)))
	{
		Resize(capacity / 2);
	}

	out = popData;
	return true;
}

template<typename T>
bool Stack<T>::Peek(T& out) const
{
	if (IsEmpty())
	{
		return false;
	}
	out = stack[top];
	return true;
}

template<typename T>
int Stack<T>::Size() const
{
	return (top + 1);
}

template<typename T>
void Stack<T>::Clear()
{
	delete[] stack;
	stack = nullptr;
	top = -1;
	capacity = 0;
}

template<typename T>
T& Stack<T>::operator[](int i)
{
	return stack[i];
}

template<typename T>
void Stack<T>::Resize(int newCapacity)
{
	if (capacity == newCapacity) return;

	T* newStack = new T[newCapacity];

	for (int i = 0; i <= top; i++)
	{
		newStack[i] = stack[i];
	}

	delete[] stack;

	stack = newStack;
	capacity = newCapacity;
}

