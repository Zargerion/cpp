#include <iostream>

using namespace std;

template <typename ElemType>
class Queue
{
	struct QElem
	{
		ElemType inf;        // Now elem is brobably can every type.
		QElem *next = NULL;  // Next.
	};
	// Pointers to head and tail.
	QElem * head = NULL;
	QElem * tail = NULL;
public:
	// Prototype declaration of of funks (methods).
	void push(const ElemType&);
	bool pop(ElemType&);
	virtual ~Queue(); // Destroys objects of class after end of main(). Virtual is good for all destructors.
};

template <typename ElemType>
void Queue<ElemType>::push(const ElemType& val) // Queue<struct> if Queur from inner struct queue(struct). Syntaxis for class.
{	
	QElem *elem = new QElem();					// New memory in heap for elem and elem is a pointer on it.
	elem->inf = val;							// Here part of elem that maned inf is changing by val.
	if (head == NULL) { head = tail = elem; }   // If empty head so head and tail is equel elem;
	else { tail = tail->next = elem; }			// In every next element got inf of elem. And now is it tail. To end of Queue<struct> object.
}

template <typename ElemType>
bool Queue<ElemType>::pop(ElemType& val)
{
	if (!head) return false; // If no head so false;
	val = head->inf;         // Variable val changing in memory of heap by using & ?
	QElem *elem = head;      // Pointer elem now equel head. That's all working with heap by using pointers.
	head = head->next;		 // Head got next value.
	delete elem;			 // Previously head is detiled in heap.
	return true;			 // Function returned true.
}

template <typename ElemType>
Queue<ElemType>::~Queue()
{
	while (head)
	{
		QElem *elem = head;
		head = head->next;
		delete elem;
	}
}


int main()
{
	Queue<int> q;
	for (int i = 0; i < 10; i++)
	{
		cout << "push: " << i << endl;
		q.push(i);
	}
	cout << "--------" << endl;
	int a;
	while (q.pop(a))
	{
		cout << "pop: " << a << endl;
	}
	
	return 0;
}