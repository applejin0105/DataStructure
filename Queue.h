#pragma once
static const int MAX_CAPACITY = 1024 * 1024 * 10;
static const int DEFAULT_CAPACITY = 1024 * 1024 * 4;

template<typename T>
class Queue
{
public:
	Queue();
	~Queue();

	int Capacity() const;
	bool IsEmpty() const;
	bool IsFull() const;
	bool Enqueue(T value);
	bool Dequeue(T& out);
	bool Front(T& out) const;
	int Size() const;

	T& operator[](int i);

	class Iterator
	{
	private:
		Queue<T>* queueRef;
		int index;

	public:
		Iterator(Queue<T>* q, int i) : queueRef(q), index(i) {}

		T& operator*()
		{
			int actualIndex = (queueRef->front + index) & queueRef->capacity;
			return queueRef->queue[actualIndex];
		}
		Iterator& operator++()
		{
			index++;
			return *this; 
		}

		bool operator==(const Iterator other) const { return index == other.index; }
		bool operator!=(const Iterator other) const { return index != other.index; }
	};

	Iterator begin() { return Iterator(queue+front); }
	Iterator end() { return Iterator(queue + rear); }

private:
	T* queue;
	int front;
	int rear;
	int capacity;
	int size;

	void Resize(int newCapacity);

};

template<typename T>
Queue<T>::Queue() : queue(nullptr), front(0), rear(-1), capacity(0), size(0)
{
	capacity = DEFAULT_CAPACITY / sizeof(T);
	queue = new T[capacity];
}

template<typename T>
Queue<T>::~Queue()
{
	if (queue)
		delete[] queue;
}

template<typename T>
inline int Queue<T>::Capacity() const
{
	return capacity;
}

template<typename T>
bool Queue<T>::IsEmpty() const
{
	return (size == 0);
}

template<typename T>
bool Queue<T>::IsFull() const
{
	return (size == capacity);
}

template<typename T>
bool Queue<T>::Enqueue(T value)
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
	rear = (rear + 1) % capacity;
	queue[rear] = value;
	size++;
	return true;
}

template<typename T>
bool Queue<T>::Dequeue(T& out)
{
	if (IsEmpty())
	{
		return false;
	}

	out = queue[front];
	front = (front + 1) % capacity;
	size--;

	// 단순히 size <= capacity / 2일 때 줄이면 너무 자주 Resize 발생, 성능저하.
	if (size <= capacity / 4 && capacity > (DEFAULT_CAPACITY / sizeof(T)))
	{
		Resize(capacity / 2);
	}

	return true;
}

template<typename T>
bool Queue<T>::Front(T& out) const
{
	if (IsEmpty())
	{
		return false;
	}
	out = queue[front];
	return true;
}

template<typename T>
int Queue<T>::Size() const
{
	return size;
}

template<typename T>
T& Queue<T>::operator[](int i)
{
	return queue[i];
}

template<typename T>
void Queue<T>::Resize(int newCapacity)
{
	if (capacity == newCapacity) return;

	T* newQueue = new T[newCapacity];

	for (int i = 0; i < size; i++)
	{
		newQueue[i] = queue[(front + i) % capacity];
	}

	delete[] queue;

	queue = newQueue;
	front = 0;
	rear = size - 1;
	capacity = newCapacity;
}