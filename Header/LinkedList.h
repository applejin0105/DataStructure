#pragma once

static const int MAX_CAPACITY = 1024 * 1024 * 10;
static const int DEFAULT_CAPACITY = 1024 * 1024 * 4;

template<typename T>
class Array
{
public:
	Array();
	~Array();

	Array(const Array& other);
	Array& operator=(const Array& other);

	int Capacity() const;
	bool IsFull() const;
	bool IsEmpty() const;

	bool Get(int index, T& out) const;
	void Set(int index, const T& value);

	bool Add(const T& value);
	bool Insert(int index, const T& value);

	bool Remove(const T& value);
	bool RemoveAt(int index);

	bool IndexOf(const T& value, int& out) const;
	bool Contains(const T& value) const;

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
Array<T>::Array(const Array& other) : size(other.size), capacity(other.capacity)
{
	array = new T[capacity];
	for (int i = 0; i < size; i++)
	{
		array[i] = other.array[i];
	}
}

template<typename T>
Array<T>::~Array()
{
	delete[] array;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other)
{
	if (this == &other)
		return *this;

	delete[] array;

	size = other.size;
	capacity = other.capacity;
	array = new T[capacity];

	for (int i = 0; i < size; i++)
	{
		array[i] = other.array[i];
	}

	return *this;
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
bool Array<T>::Get(int index, T& out) const
{
	if (index < 0 || index >= size)
		return false;
	out = array[index];
	return true;
}

template<typename T>
void Array<T>::Set(int index, const T& value)
{
	if (index < 0 || index >= size)
		return;
	array[index] = value;
}

template<typename T>
bool Array<T>::Add(const T& value)
{
	if (IsFull())
	{
		int maxCapacity = MAX_CAPACITY / sizeof(T);
		if (capacity < maxCapacity)
		{
			int nextCapacity = capacity * 2;
			if (nextCapacity > maxCapacity)
				nextCapacity = maxCapacity;

			Resize(nextCapacity);
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
bool Array<T>::Insert(int index, const T& value)
{
	if (index < 0 || index > size)
	{
		return false;
	}

	if (IsFull())
	{
		int maxCapacity = MAX_CAPACITY / sizeof(T);
		if (capacity < maxCapacity)
		{
			int nextCapacity = capacity * 2;
			if (nextCapacity > maxCapacity)
				nextCapacity = maxCapacity;

			Resize(nextCapacity);
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
bool Array<T>::Remove(const T& value)
{
	if (IsEmpty())
	{
		return false;
	}

	int index;
	if (IndexOf(value, index))
	{
		return RemoveAt(index);
	}
	return false;
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

	if (size <= capacity / 4 && capacity > (DEFAULT_CAPACITY / sizeof(T)))
	{
		Resize(capacity / 2);
	}
	return true;
}

template<typename T>
bool Array<T>::IndexOf(const T& value, int& out) const
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
bool Array<T>::Contains(const T& value) const
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
