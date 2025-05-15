#pragma once
constexpr size_t SIZE_LIMIT = 2ULL * 1024 * 1024 * 1024;

template <typename K, typename V>
class HashTable
{
	struct HashNode
	{
		K key;
		V value;
		HashNode* next;
	};

public:
	class Iterator
	{
	public:
		Iterator(HashNode** t, int size, int index, HashNode* start)
			: table(t), tableSize(size), currentIndex(index), currentNode(start) {}

		V& operator*() const { return currentNode->value; }

		Iterator& operator++()
		{
			if (currentNode != nullptr)
			{
				currentNode = currentNode->next;
			}

			while (currentNode == nullptr && ++currentIndex < tableSize)
			{
				currentNode = table[currentIndex];
			}

			return *this;
		}

		bool operator==(const Iterator& other) const { return currentNode == other.currentNode; }
		bool operator!=(const Iterator& other) const { return !(*this == other); }

	private:
		HashNode** table;
		int tableSize;
		int currentIndex;
		HashNode* currentNode;
	};

	struct Range
	{
		Iterator begin;
		Iterator end;
	};

	Iterator begin()
	{
		for (int i = 0; i < TABLE_SIZE; ++i)
		{
			if (table[i] != nullptr)
			{
				return Iterator(table, TABLE_SIZE, i, table[i]);
			}
		}
	}
	Iterator end()
	{
		return Iterator(table, TABLE_SIZE, TABLE_SIZE, nullptr);
	}

	HashTable();
	~HashTable();

	bool Insert(K key, V value);
	bool Remove(K key);
	bool RemoveFront(K key);
	bool RemoveBack(K key);
	bool RemoveAt(K key, V value);
	bool Get(K key, V* outArray, int& outCount, int maxCount = -1);
	bool GetFront(K key, V& out);
	bool GetBack(K key, V& out);
	void Clear();
	int Size() const;
	bool IsEmpty() const;

	Range GetRange(const K& key);

private:

	HashNode** table;
	static const int TABLE_SIZE = 1031;
	int size;

	int HashFunction(K key) const;
};

template<typename K, typename V>
HashTable<K, V>::HashTable() : table(nullptr), size(0)
{
	table = new HashNode * [TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; ++i)
		table[i] = nullptr;
}

template<typename K, typename V>
HashTable<K, V>::~HashTable()
{
	if (table != nullptr)
	{
		for (int i = 0; i < TABLE_SIZE; ++i)
		{
			HashNode* node = table[i];
			if (node != nullptr)
			{
				HashNode* next = node->next;
				delete node;
				node = next;
			}
		}
		delete[] table;
	}
}

template<typename K, typename V>
bool HashTable<K, V>::Insert(K key, V value)
{
	int hashKey = HashFunction(key);
	HashNode* newNode = new HashNode{ key, value, nullptr };
	HashNode* node = table[hashKey];

	if (node == nullptr)
	{
		table[hashKey] = newNode;
	}
	else
	{
		while (node->next != nullptr)
		{
			node = node->next;
		}
		node->next = newNode;
	}
	++size;
	return true;
}

template<typename K, typename V>
bool HashTable<K, V>::Remove(K key)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];
	HashNode* prev = nullptr;
	bool found = false;

	while (node != nullptr)
	{
		if (node->key == key)
		{
			HashNode* toDeleteNode = node;
			if (prev == nullptr)
			{
				table[hashKey] = node->next;
				node = node->next;
			}
			else
			{
				prev->next = node->next;
				node = node->next;
			}
			delete toDeleteNode;
			--size;
			found = true;
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return found;
}

template<typename K, typename V>
bool HashTable<K, V>::RemoveFront(K key)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];

	if (node != nullptr)
	{
		if (node->next != nullptr)
		{
			HashNode* toDeleteNode = node;
			table[hashKey] = node->next;
			delete toDeleteNode;
		}
		else
		{
			HashNode* toDeleteNode = node;
			table[hashKey] = nullptr;
			delete toDeleteNode;
		}
		return true;
	}
	return false;
}

template<typename K, typename V>
bool HashTable<K, V>::RemoveBack(K key)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];
	HashNode* prev = nullptr;

	if (node == nullptr)
		return false;

	if (node->next == nullptr)
	{
		delete node;
		table[hashKey] = nullptr;
		return true;
	}

	while (node->next != nullptr)
	{
		prev = node;
		node = node->next;
	}

	prev->next = nullptr;
	delete node;
	return true;
}

template<typename K, typename V>
bool HashTable<K, V>::RemoveAt(K key, V value)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];
	HashNode* prev = nullptr;
	bool found = false;

	while (node != nullptr)
	{
		if (node->key == key && node->value == value)
		{
			HashNode* toDeleteNode = node;
			if (prev == nullptr)
			{
				table[hashKey] = node->next;
				node = node->next;
			}
			else
			{
				prev->next = node->next;
				node = node->next;
			}
			delete toDeleteNode;
			--size;
			found = true;
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
	return found;
}

template<typename K, typename V>
bool HashTable<K, V>::Get(K key, V* outArray, int& outCount, int maxCount)
{
	outCount = 0;
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];
	int effectiveMax;
	
	if (node != nullptr)
	{
		constexpr size_t maxSafeCount = (SIZE_LIMIT / sizeof(node->value));
		int min = (maxCount > (int)maxSafeCount) ? (int)maxSafeCount : maxCount;
		effectiveMax = (maxCount == -1) ? maxSafeCount : min;
	}


	while (node != nullptr)
	{
		if (node->key == key)
		{
			if (outCount < effectiveMax)
			{
				outArray[outCount++] = node->value;
			}
			else
			{
				break;
			}
		}
		node = node->next;
	}

	return (outCount > 0);
}

template<typename K, typename V>
bool HashTable<K, V>::GetFront(K key, V& out)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];

	if (node != nullptr)
	{
		out = node->value;
		return true;
	}
	return false;
}

template<typename K, typename V>
bool HashTable<K, V>::GetBack(K key, V& out)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];

	if (node == nullptr)
		return false;

	if (node->next == nullptr)
	{
		out = node->value;
		return true;
	}

	while (node->next != nullptr)
	{
		node = node->next;
	}

	out = node->value;
	return false;
}

template<typename K, typename V>
void HashTable<K, V>::Clear()
{
	for (int i = 0; i < TABLE_SIZE; ++i)
	{
		HashNode* node = table[i];
		while (node != nullptr)
		{
			HashNode* next = node->next;
			delete node;
			node = next;
		}
		table[i] = nullptr;
	}
	size = 0;
}

template<typename K, typename V>
int HashTable<K, V>::Size() const
{
	return size;
}

template<typename K, typename V>
bool HashTable<K, V>::IsEmpty() const
{
	return (size == 0);
}

/*C++ 템플릿에서 이름 해석(name lookup)과 정확한 범위 지정(qualified name) 때문에 생기는 현상
* Range가 HashTable<K, V>의 내부 타입이기 때문에, 템플릿 외부에서 정의시 반드시 전체 범위를 명시해줘야 한다. 
*/
template<typename K, typename V>
typename HashTable<K, V>::Range HashTable<K, V>::GetRange(const K& key)
{
	int hashKey = HashFunction(key);
	HashNode* node = table[hashKey];

	while (node && node->key != key)
		node = node->next;

	if (node == nullptr)
		return { end(), end() };

	HashNode* endNode = node;
	while (endNode && endNode->key == key)
		endNode = endNode->next;

	return { Iterator (table, TABLE_SIZE, hashKey, node),
			 Iterator(table, TABLE_SIZE, hashKey, endNode) };
}

template<typename K, typename V>
int HashTable<K, V>::HashFunction(K key) const
{
	// Knuth’s Multiplicative Hashing
	return (key * 2654435761) % TABLE_SIZE;
}
