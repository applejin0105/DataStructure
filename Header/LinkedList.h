#pragma once
static const int MAX_CAPACITY = 1024 * 1024 * 10;
static const int DEFAULT_CAPACITY = 1024 * 1024 * 4;

template<typename T>
class LinkedList
{
public:
	LinkedList() : head(nullptr), tail(nullptr), size(0);
	~LinkedList() { Clear(); }

	void AddFront(T value);
	void AddBack(T value);
	bool Insert(int index, T value);

	bool Remove(T value);
	bool RemoveAt(int index);

	bool Get(int index, T& out);
	bool Contains(T value);
	bool IndexOf(T value, int& out);
	void Clear();

	int Size() const;
	bool IsEmpty() const;

	class Iterator
	{
	private:
		Node* current;

	public:
		Iterator(Node* start) : current(start) {}

		T& operator*() { return current->data; }
		Iterator& operator++() { if (current) cuurrent = current->next; return *this; }
		
		bool operator==(const Iterator& other) cosnt { return current == other.current; }
		bool operator!=(const Iterator& other) const { return current != other.current; }
	};

	Iterator begin() { return Iterator(head); }
	Iterator end() { return Iterator(nullptr); }

private:
	struct Node
	{
		T data;
		Node* next;
		Node* prev;

		Node(T& value) : data(value), prev(nullptr), next(nullptr) {}
	};

	Node<T>* head;
	Node<T>* tail;
	int size;

	void Resize();
};

template<typename T>
LinkedList<T>::LinkedList()
{
	head = nullptr;
	tail = nullptr;
	size = 0;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
	Clear();
}

template<typename T>
void LinkedList<T>::AddFront(T value)
{
	// newNode 자체는 지역 포인터 변수 -> AddFront 함수가 끝나면 이 포인터만 사라진다.
	// newNode<T>(value)로 만든 노드 객체는 힙 메모리에 살아있다.
	Node<T>* newNode = new Node<T>(value);
	newNode->next = head;

	if(!IsEmpty())
		head->prev = newNode;

	head = newNode;
	size++;
}

template<typename T>
void LinkedList<T>::AddBack(T value)
{
	Node<T>* newNode = new Node<T>(value);

	if (IsEmpty())
	{
		head = newNode;
	}
	else
	{
		Node<T>* current = head;
		while (current->next != nullptr)
		{
			current = current->next;
		}
		current->next = newNode;
		newNode->prev = current;
	}
	size++;
}

template<typename T>
bool LinkedList<T>::Insert(int index, T value)
{
	if (index == 0)
	{
		AddFront(value);
		return true;
	}
	else if (index == size)
	{
		AddBack(value);
		return true;
	}
	else if (index > size)
	{
		return false;
	}

	Node<T>* currentNode = head;

	Node<T>* preNode = head;
	Node<T>* subNode;

	Node<T>* newNode = new Node<T>(value);

	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}
	
	subNode = preNode->next;
	preNode->next = newNode;
	newNode->prev = preNode;
	newNode->next = subNode;
	subNode->prev = newNode;

	return true;
}

template<typename T>
bool LinkedList<T>::Remove(T value)
{
	if (IsEmpty())
	{
		return false;
	}

	Node<T>* currentNode = head;

	while (currentNode->data != value)
	{
		if (currentNode->next == nullptr)
		{
			return false;
		}
		currentNode = currentNode->next;
	}

	Node<T>* preNode = currentNode->prev;
	Node<T>* subNode = currentNode->next;

	preNode->next = subNode;
	subNode->prev = preNode;

	size--;

	return true;
}

template<typename T>
bool LinkedList<T>::RemoveAt(int index)
{
	if (IsEmpty() || index >= Size())
	{
		return false;
	}

	Node<T>* currentNode = head;

	for (int i = 0; i < index; i++)
	{
		currentNode = currentNode->next;
	}

	Node<T>* preNode = currentNode->prev;
	Node<T>* subNode = currentNode->next;

	preNode->next = subNode;
	subNode->prev = preNode;

	size--;

	return false;
}

template<typename T>
bool LinkedList<T>::Get(int index, T& out)
{
	if (index > Size())
		return false;

	Node<T>* currentNode = head;

	for (int i = 0; i < index; i++)
	{
		currentNode = currentNode->next;
	}
	out = currentNode->data;
	return true;
}

template<typename T>
bool LinkedList<T>::Contains(T value)
{
	Node<T>* currentNode = head;

	while (currentNode->data != value)
	{
		if (currentNode->next == nullptr)
		{
			return false;
		}
		currentNode = currentNode->next;
	}
	return true;
}

template<typename T>
bool LinkedList<T>::IndexOf(T value, int& out)
{
	Node<T>* currentNode = head;
	int index = 0;

	while (currentNode->data != value)
	{
		if (currentNode->next == nullptr)
		{
			return false;
		}
		currentNode = currentNode->next;
		index++;
	}
	out = index;
	return true;
}

template<typename T>
void LinkedList<T>::Clear()
{
	Node<T>* current = head;

	while (current != nullptr)
	{
		Node<T>* next = current->next;
		delete current;
		current = next;
	}
	head = nullptr;
	tail = nullptr;
	size = 0;
}

template<typename T>
int LinkedList<T>::Size() const
{
	return size;
}

template<typename T>
bool LinkedList<T>::IsEmpty() const
{
	return (head == nullptr);
}

template<typename T>
void LinkedList<T>::Resize()
{
	return -1;
}