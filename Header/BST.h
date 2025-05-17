#pragma once

template<typename K, typename V>
class BST
{
	struct Node
	{
		K key;
		V value;
		Node* left;
		Node* right;
		Node(K k, V v) : key(k), value(v), left(nullptr), right(nullptr) {}
	};

	Node* root;

public:
	BST() : root(nullptr) {}
	~BST();

	bool Insert(K key, V value);
	bool Remove(K key);
	bool Search(K key, V& out) const;
	void Clear();

	class Iterator
	{

	};


private:
	Node* Insert(Node* node, K key, V value);
	Node* Remove(Node* node, K key);
	Node* FindMin(Node* node);
	Node* Search(Node* node, K key);
	void DeleteTree(Node* node);
};

template<typename K, typename V>
BST<K, V>::~BST()
{
	DeleteTree(root);
}

template<typename K, typename V>
bool BST<K, V>::Insert(K key, V value)
{
	root = Insert(root, key, value);
	return true;
}

template<typename K, typename V>
bool BST<K, V>::Remove(K key)
{
	if (Remove(root, key) != nullptr) return true;
	return false;
}

template<typename K, typename V>
bool BST<K, V>::Search(K key, V& out) const
{
	Node* result = Search(root, key);
	if (out != nullptr)
	{
		out = result->value;
		return true;
	}
	return false;
}

template<typename K, typename V>
void BST<K, V>::Clear()
{
	if (root != nullptr)
	{
		DeleteTree(root);
		root = nullptr;
	}
}

template<typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::Insert(Node* node, K key, V value)
{
	if (node == nullptr)
		return new Node(key, value);

	if (key < node->key)
		node->left = Insert(node->left, key, value);
	else if (key > node->key)
		node->right = Insert(node->right, key, value);
	else
		node->value = value;

	return node;
}

template<typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::Remove(Node* node, K key)
{
	if (node == nullptr) return nullptr;

	if (key < node->key)
		node->left = Remove(node->left, key);
	else if (key > node->key)
		node->right = Remove(node->right, key);
	else
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			delete node;
			return nullptr;
		}
		else if (node->left == nullptr)
		{
			Node* temp = node->right;
			delete node;
			return temp;
		}
		else if (node->right == nullptr)
		{
			Node* temp = node->left;
			delete node;
			return temp;
		}
		else
		{
			Node* successor = FindMin(node->right);
			node->key = successor->key;
			node->value = successor->value;
			node->right = Remove(node->right, successor->key);
		}
	}

	return node;
}

template<typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::FindMin(Node* node)
{
	while (node->left != nullptr)
		node = node->left;
	return node;
}

template<typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::Search(Node* node, K key)
{
	if (node == nullptr)
		return nullptr;

	if (key < node->key)
		node->left = Search(node->left, key);
	else if (key > node->key)
		node->right = Search(node->right, key);
	else
		return node;
}

template<typename K, typename V>
void BST<K, V>::DeleteTree(Node* node)
{
	if (node == nullptr) return;
	DeleteTree(node->left);
	DeleteTree(node->right);
	delete node;
}
