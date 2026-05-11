# DataStructure
자료구조 연습

> 참고서적
>  1. Doit C++ 완전 정복 - 조규남, 문종채
>  2. C++ Plus Data Structure - Nell Dale
>  3. Effective C++ - 스콧 마이어스
>  4. Effective Modern C++ - 스콧 마이어스

---

## 개요
C++를 사용하여 의존성을 최소로 낮추고, 각 자료구조를 배열 형식으로 직접 구현해보면서 자료구조를 연습하고, C++ 메모리 관리 구조와 객체지향 프로그래밍을 연습했습니다.
특히 C++ 기초를 다지기 위해서 #include를 사용하지 않고, 언제 어디서든 사용 가능하게 구현하는데 집중했습니다. 연산자 오퍼레이터를 적극적으로 사용하며, 모던 C++에서 개인적으로 편하다고 느꼈던 기능들을 추가하고자 노력했습니다.

---

### Array.h
모든 자료구조의 기본이 되는 배열입니다. C++의 동적 배열(Dynamic Array, 특히 std::vector)의 핵심적인 동작 원리를 이해하고 구현하고자 했습니다.
동적 배열인 만큼, 메모리 관리에 특히 생각을 많이했고 이렇게 도입한 것이

```
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
```

부분입니다. 동적 배열에서 흔히 발생하는 Flapping(잦은 메모리 재할당으로 인한 성능 저하)을 막기 위해 도입했습니다.
Vector에 대한 욕심으로 begin과 end까지 제공하여 C++에서 흔히 사용하는 for (auto& item : array)와 같은 모던 C++ 문법을 사용할 수 있게 설계했습니다.
모든 데이터 전달은 불필요한 복사를 막기 윟 const T&으로 통일, 내부 상태를 변경하지 않는 읽기 전용 함수들은 모두 const 함수로 시그니쳐를 통일했습니다.

---

### LinkedList.h
배열 다음으로 많이 구현해보는 연결 리스트입니다. 양방향 탐색이 가능한 Doubly Linked List로 설계하였고, 이를 적극적으로 사용하기 위해 prev 포인터를 도입, 탐색 및 삭제의 유연성을 확보했습니다.

```
	struct Node
	{
		T data;
		Node* next;
		Node* prev;

		Node(T& value) : data(value), prev(nullptr), next(nullptr) {}
	};
```

앞선 Array와 마찬가지로 Iterator 기반 클래스를 구현하여 편하게 내부 탐색이 가능하게 만들었습니다.
동적 할당된 노드들이 힙(Heap)메모리에 남아있는 것을 방지하기 위해 Celar() 메서드를 구현했고, 이를 소멸자에서 호출하는 방향으로 메모리 관리에도 신경썼습니다.
추후, 삭제하려는 노드가 Head 혹은 Tail일 경우 이전이나 다음 노드의 포인터 갱신 로직을 추가하여 견고하게 사용할 예정입니다.

---

### Stack.h
가장 많이 배우고, 사실상 가장 많이 사용하는 대표적인 자료구조 중 하나인 스택입니다.
노드 기반이 아닌 배열 기반으로 스택을 구현하여, 메모리 연속성을 확보, 이를 통해 CPU 캐시 히트율을 높이고 메모리 파편화를 방지했습니다. 이를 통해 Push Pop 연산을 위해 포인터 이동없이, 단순히 top 인덱스 증감만 이루어지게 하여, 시간 복잡도를 O(1)로 잡았습니다.

---

### Queue.h
스택과 함께 자료구조를 대표하는 큐입니다.
단순히 선형큐만 하려다가, 뭔가 프로그래머라면 좀 더 제대로 된거, 특히 당시 사용하던 유니티에서 원형 큐를 사용하여 작업할 것이 있어서 이참에 원형큐를 구현해보았습니다.
데이터를 빼고 넣을때 빈 공간 재활용을 위해, 그리고 앞뒤로 당겨야하는 기존 선형 큐의 단점을 모듈러 연산을 통해 해결했습니다.
또한, 동적으로 원형 큐를 사용하면서, 특히 복사 할 때 front와 rear가 꼬이는 것을 방지하기 위해 Resize 함수 내부에서 for 문과 % capacity를 활용, 물리적으로 쪼개져 있던 데이터를 새로운 배열에 논리적 순서대로 일자로 펴서 복사하게 만들었습니다.

```
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
```

배열의 물리적 인덱스가 아닌 논리적 순서를 가진 iterator를 구현했습니다. 이를 통해 begin end 순회 시, 밖에서는 복잡하게 원형 큐에 접근할 필요 없이 물리적 형태를 캡슐화해서 숨겨 직관적인 탐색을 제공했습니다.

엔진에서 사용할 일이 있을지는 모르겠지만, 지금 유니티 멀티플레이에서 주로 공부했던 스레딩 시스템을 연동해서, mutex를 통해 락과 해제를 구현하는 것도 구현해보고 싶습니다.

---

### Binary Search Tree.h
개인적으로 좀 자랑하고 싶은 코드가 있다면, 이 코드랑 아래있는 헤시 테이블입니다.
모던 C++을 한창 공부하고 있던 중에 배운 콜백 함수를 사용하여 이를 인자로 받는 템플릿 기반의 TraverseInOrder를 구현했습니다.

```
	template<typename F>
	void TraverseInOrder(Node* node, F&& visit) const
	{
		if (node == nullptr) return;
		this->TraverseInOrder(node->left, visit);
		visit(node->key, node->value);
		this->TraverseInOrder(node->right, visit);
	}
```
함수 객체를 외부에서 주입하고, 트래 내의 데이터로 출력, 합계 계산, 리스트 변환 등 원하는 작업을 유연하게 수행할 수 있도록 설계햇습니다. 이 당시 함수형 프로그래밍을 처음 배우고 꽂혀서 AI에게 도움을 받아가며 공부하고, 구현했습니다.

세가지 기본 순회를 적극적으로 사용해보았습니다. 데이터 조회를 할때는 정렬된 순서를 보장하는 중위 순회(In-order)를, 트리 전체 메모리 해제를 위해서는 자식 먼저 지워야 하므로 후위 순회(Post-order)를 사용했습니다. 이를 통해 메모리 누수나 댕글링 포인터 문제를 차단했습니다.

Remove연산은 정석에 맞춰서 구현했습니다. 삭제할 노드의 자식이 2개일 때, 단순히 노드를 지우는게 아니라 우측 서브트리에서 가장 작은 값(FindMin)을 찾아 데이터를 교체, 기존 위치의 노드를 삭제하는 방식으로 정석대로 구현했습니다.

그리고 아무래도 BST인만큼 오름차순 데이터만 계속 넣으면 편향 트리가 발생하는데 이를 좀 해결해야 할 것 같습니다. 추후 RBT를 구현해보고 싶습니다.

이게 좀 부족한게 많긴 한데, 목표는 어떻게 돌아가는지 이해하고, 추후 더 나은 자료구조를 구현하는게 목표라, 지금의 BST는 여기까지만 구현하고 추후 RBT나 AVL Tree를 구현하고자 합니다.

---

### HashTable.h
해시 충돌이 너무 무서웠습니다. AI에게 어떻게 할지 물어봐도 해시 충돌은 거의 피할 수 없다고 해서 계속 늘리고, 늘리고 늘렸다가 결국 TABLE_SIZE를 1031, 소수로 설정하고 해시 함수에 Knuth 곱셈 해싱 함수를 적용해서 해싱 충돌을 피할 수 있는 수학적 알고리즘을 적용했습니다.

단일 키-값 쌍이 아닌, 하나의 키에 여러 데이터가 연결될 수 있게 구현했습니다. 이는 게임에서 사용할 수 있게끔, 공간 분할 알고리즘이라던가, 인벤토리 시스템 같은 것에 사용 가능하게끔 구현해보았습니다.

또한 operator++ 내부에서 현재 노드가 nullptr이 되면 다음 유효한 버킷을 찾을 때까지 인덱스를 자동으로 전진시키는 로직을 구현하여 사용자가 for(auto val : hashTable) 형태로 끊김 없이 던체 데이터를 순회할 수 있게 만들었습니다.

```
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

```

지금 문제가, string을 사용하면 컴파일 에러가 발생합니다. 키값이 정수형 키에만 최적화 되어 있어서 범용성을 높이기 위해서는 결국 std를 사용해야 할 것 같습니다.

---
