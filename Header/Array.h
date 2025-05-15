#pragma once

static const int MAX_CAPACITY = 1024 * 1024 * 10;
static const int DEFAULT_CAPACITY = 1024 * 1024 * 4;

template<typename T>
class Array
{
public:
	Array() : array(nullptr), size(0);
	~Array();

	int Capacity() const;
	bool IsFull() const;
	bool IsEmpty() const;

	bool Get(int index, T& out);
	void Set(int index, T value);

	bool Add(T value);
	bool Insert(int index, T value);

	bool Remove(T value);
	bool RemoveAt(int index);

	bool IndexOf(T value, int& out);
	bool Contains(const T& value);

	void Clear(bool resetMemory = false);

	class Iterator
	{
	public:
		Iterator(T* ptr) : m_ptr(ptr) {}

		T& operator*() { return *m_ptr; }
		Iterator& operator++() { ++m_ptr; return *this; }

		bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }
		bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }

	private:
		T* m_ptr;
	};

	Iterator begin() { return Iterator(array); }
	Iterator end() { return Iterator(array + size); }

private:
	T* array;
	int capacity;
	int size;

	void Resize(int newCapacity);

};

template<typename T>
Array<T>::Array() : array(nullptr), size(0)
{
	capacity = DEFAULT_CAPACITY / sizeof(T);
	array = new T[capacity];
}

template<typename T>
Array<T>::~Array()
{
	if(array)
		delete[] array;
}

template<typename T>
int Array<T>::Capacity() const
{
	return capacity;
}

template<typename T>
bool Array<T>::IsFull() const
{
	return (size == capacity);
}

template<typename T>
bool Array<T>::IsEmpty() const
{
	return (size == 0);
}

template<typename T>
bool Array<T>::Get(int index, T& out)
{
	if (index < 0 || index >= size)
		return false;
	out = array[index];
	return true;
}

template<typename T>
void Array<T>::Set(int index, T value)
{
	if (index < 0 || index >= size)
		return;
	array[index] = value;
}

template<typename T>
bool Array<T>::Add(T value)
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

	array[size] = value;
	size++;
	return true;
}

template<typename T>
bool Array<T>::Insert(int index, T value)
{
	if (index < 0 || index > size)
	{
		return false;
	}

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

	for (int i = size; i > index; i--)
	{
		array[i] = array[i - 1];
	}
	array[index] = value;
	size++;
	return true;
}

template<typename T>
bool Array<T>::Remove(T value)
{
	if (IsEmpty())
	{
		return false;
	}

	int index;
	if (IndexOf(value, index))
	{
		RemoveAt(index);
	}
	return true;
}

template<typename T>
bool Array<T>::RemoveAt(int index)
{
	if (IsEmpty() || index < 0 || (index >= size))
	{
		return false;
	}

	for (int i = index; i < size - 1; i++)
	{
		array[i] = array[i + 1];
	}

	size--;

	// 단순히 size <= capacity / 2일 때 줄이면 너무 자주 Resize 발생, 성능저하.
	if (size <= capacity / 4 && capacity > (DEFAULT_CAPACITY / sizeof(T)))
	{
		Resize(capacity / 2);
	}
	return true;
}

template<typename T>
bool Array<T>::IndexOf(T value, int& out)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i] == value)
		{
			out = i;
			return true;
		}
	}
	return false;
}

template<typename T>
bool Array<T>::Contains(const T& value)
{
	int unusedIndex = -1;
	return IndexOf(value, unusedIndex);
}

template<typename T>
void Array<T>::Clear(bool resetMemory)
{
	size = 0;

	if (resetMemory)
	{
		delete[] array;
		array = new T[DEFAULT_CAPACITY / sizeof(T)];
		capacity = DEFAULT_CAPACITY / sizeof(T);
	}
}

template<typename T>
void Array<T>::Resize(int newCapacity)
{
	if (capacity == newCapacity) return;

	T* newArray = new T[newCapacity];

	for (int i = 0; i < size; i++)
	{
		newArray[i] = array[i];
	}

	delete[] array;

	array = newArray;
	capacity = newCapacity;
}