#ifndef S23_CS20A_LlST_H
#define S23_CS20A_LlST_H

#include<iostream>
#include<assert.h>
// Linked DLList object that maintains both front and rear pointers
// and the size of the list.  Note that you have to keep the front,
// rear and size consistent with the intended state of the DLList 
// otherwise very bad things happen. 
template<typename Type>
class DLList {
public:

	DLList();

	DLList(const DLList<Type>& other);

	DLList<Type>& operator=(const DLList<Type>& other);

	~DLList();

	void	print() const;
	bool	empty() const;

	void	insert_front(const Type &value);
	void	insert_rear(const Type &value);
	void	insert(int idx, const Type &value);

	// Note that the user must front ensure the list is not empty
	// prior to calling these functions. 
	Type	peek_front() const;
	Type	peek_rear() const;
	Type	peek(int idx) const;

	int		count() const;
	int		count_value(const Type &value) const;

	int		find(const Type &value) const;

	bool	remove_front();
	bool	remove_rear();
	bool	remove_index(int idx);
	int		remove_value(const Type &value);

#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare the nodes for our DLList.
	// Will be implemented along with list
	// member functions
	class DLNode;

	// We'll have both front and rear points for 
	// Fast insertion/deletion from both ends.
	DLNode*	front;
	DLNode*	rear;

	// Keep track of number of nodes in the list
	int		size;
};


/* DLList Implementation
//
//  Since DLList is a template class (which is not an actual
//  class yet, not until we actually instantiate the list)
//  we need to keep the implementation together with
//  the definition.  There are ways to simulate having
//  separate "implementation/definition" with templates,
//  but they aren't necessary and can be confusing.
*/

/* DLNode definition
//		Already implemented, nothing to do here but to use it.
*/
template<typename Type>
class DLList<Type>::DLNode {
public:
	DLNode() :nxt(nullptr), prv(nullptr) {}
	DLNode(Type v, DLNode* p, DLNode* n) : val(v), nxt(n), prv(p) {}

	DLNode * next() const { return nxt; }
	void next(DLNode *n) { nxt = n; }

	DLNode * prev() const { return prv; }
	void prev(DLNode *p) { prv = p; }

	Type value() const { return val; }
	void value(const Type &v) { val = v; }

private:
	Type  val;
	DLNode * nxt;
	DLNode * prv;
};



/* DLList default constructor
//		Set front and rear pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
*/
template<typename Type>
DLList<Type>::DLList() :front(nullptr), rear(nullptr), size(0) {
	/*    COMPLETE    */
}


/* Copy constructor
*/
template<typename Type>
DLList<Type>::DLList(const DLList<Type>& other) {

	DLNode* temp = other.front;
	while (temp != nullptr) {
		insert_rear(temp->value());
		temp = temp->next();
	}

}
/* Overloaded assignment operator
*/
template<typename Type>
DLList<Type>& DLList<Type>::operator=(const DLList<Type>& other) {

	// Edge case - same object
	if (this == &other) { return *this; }

	while (front != nullptr) {
		remove_front();
	}

	DLNode* temp = other.front;
	while (temp != nullptr) {
		insert_rear(temp->value());
		temp = temp->next();
	}

	return *this;
}


/* DLList destructor
*/
template<typename Type>
DLList<Type>::~DLList() {

	while (front != nullptr) {
		remove_front();
	}

}

/* DLList print
*/
template<typename Type>
void DLList<Type>::print() const {

	if (front == nullptr) { return; }
	if (size == 1) { std::cout << front->value(); }
	else if (size == 2) {
		std::cout << front->value() << " ";
		std::cout << front->next()->value();
	}
	else {
		DLNode* iterator = front;
		std::cout << front->value() << " ";
		for (int i = 0; i < (size - 2); i++) {
			iterator = iterator->next();
			std::cout << iterator->value() << " ";
		}
		std::cout << rear->value();
	}

}

/* DLList empty
*/
template<typename Type>
bool DLList<Type>::empty() const {

	return (size == 0);
	
}


/* DLList insert_front
* I WORKED WITH RYAN ON THIS ONE
*/
template<typename Type>
void DLList<Type>::insert_front(const Type &value) {

	DLNode* newNode = new DLNode(value, nullptr, front);

	if (front == nullptr) {
		rear = newNode;
	}
	else
	{
		front->prev(newNode);
	}

	front = newNode;
	size++;

}

/* DLList insert_rear
* I WORKED WITH RYAN ON THIS
*/
template<typename Type>
void DLList<Type>::insert_rear(const Type &value) {

	DLNode* newNode = new DLNode(value, rear, nullptr);

	if (rear == nullptr) {
		front = newNode;
	}
	else
	{
		rear->next(newNode);
	}

	rear = newNode;
	size++;

}

/* DLList insert
* I WORKED ON THIS WITH RYAN
*/
template<typename Type>
void DLList<Type>::insert(int idx, const Type& value) {

	if (front == nullptr || idx <= 0) {
		insert_front(value);
	}
	else if (idx >= size) {
		insert_rear(value);
	} else {
		DLNode* temp = front;
		for (int i = 0; i < idx; i++) {
			temp = temp->next();
		}
		DLNode* newNode = new DLNode(value, temp->prev(), temp);
		temp->prev()->next(newNode);
		temp->prev(newNode);
		size++;
	}

}

/*  DLList peek_front
*/
template<typename Type>
Type DLList<Type>::peek_front() const {
	// Force check that the front is pointing to a DLNode
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(front != nullptr);

	return front->value();

}

/* DLList peek_rear
*/
template<typename Type>
Type DLList<Type>::peek_rear() const {

	// Force check that the rear is pointing to a DLNode
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(rear != nullptr);

	return rear->value();

}

/* DLList peek
*/
template<typename Type>
Type DLList<Type>::peek(int idx) const {

	// Force index to be correct before getting the Type
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(idx >= 0 && idx < size);

	if (idx == 0) { return front->value(); }
	if (idx == (size - 1)) { return rear->value(); }
	DLNode* iterator = front;
	for (int i = 0; i < idx; i++) {
		iterator = iterator->next();
	}
	return iterator->value();
}

/* DLList count
*/
template<typename Type>
int DLList<Type>::count() const {
	return size;
}

/* DLList count_value
*/
template<typename Type>
int DLList<Type>::count_value(const Type &value) const {
	int count = 0;
	if (front == nullptr) { return count; }
	else if (size == 1) {
		if (front->value() == value) { count++; }
		return count;
	}
	else {
		DLNode* iterator = front;
		while (iterator != nullptr) {
			if (iterator->value() == value) { count++; }
			iterator = iterator->next();
		}
		return count;
	}
}
/* DLList find
* I WORKED ON THIS WITH RYAN
*/
template<typename Type>
int DLList<Type>::find(const Type &value) const {
	if (front == nullptr) { return -1; }

	DLNode* temp = front;
	int count = 0;

	while (temp != nullptr) {
		if (temp->value() == value) { return count; }
		temp = temp->next();
		count++;
	}

	return -1;
}

/* DLList remove_front
*/
template<typename Type>
bool DLList<Type>::remove_front() {

	if (front == nullptr) { return false; }
	else if (size == 1) {
		delete front;
		size--;
		front = nullptr;
		rear = nullptr;
		return true;
	}
	else {
		DLNode* temp = front->next();
		front->next()->prev(nullptr);
		delete front;
		front = temp;
		size--;
		return true;
	}

}
/* DLList remove_rear
*/
template<typename Type>
bool DLList<Type>::remove_rear() {

	if (rear == nullptr) { return false; }
	else if (size == 1) {
		delete rear;
		size--;
		front = nullptr;
		rear = nullptr;
		return true;
	}
	else {
		DLNode* temp = rear->prev();
		rear->prev()->next(nullptr);
		delete rear;
		rear = temp;
		size--;
		return true;
	}
}

/* DLList remove_index
*/
template<typename Type>
bool DLList<Type>::remove_index(int idx) {

	if (idx < 0 || idx >= size) { return false; }
	else if (front == nullptr) { return false; }
	else if (size == 1) {
		delete front;
		size--;
		front = nullptr;
		rear = nullptr;
		return true;
	}
	else if (idx == 0) {
		DLNode* temp = front->next();
		front->next()->prev(nullptr);
		delete front;
		front = temp;
		size--;
		return true;
	}
	else if (idx == (size - 1)) {
		DLNode* temp = rear->prev();
		rear->prev()->next(nullptr);
		delete rear;
		rear = temp;
		size--;
		return true;
	}
	else {
		DLNode* temp = front;
		for (int i = 0; i < idx; i++) {
			temp = temp->next();
		}
		temp->prev()->next(temp->next());
		temp->next()->prev(temp->prev());
		delete temp;
		size--;
		return true;
	}
}

/* DLList remove_index on value
*/
template<typename Type>
int DLList<Type>::remove_value(const Type &value) {

	if (front == nullptr) { return -1; }
	else if (size == 1) {
		if (front->value() == value) {
			delete front;
			size--;
			front = nullptr;
			rear = nullptr;
			return 0;
		}
		return -1;
	}
	else if (front->value() == value) {
		DLNode* temp = front->next();
		front->next()->prev(nullptr);
		delete front;
		front = temp;
		size--;
		return 0;
	}
	else {
		DLNode* temp = front;
		for (int i = 0; i < size; i++) {
			temp = temp->next();
			if (temp->value() == value) {
				if (i == (size - 2)) {
					DLNode* temp2 = rear->prev();
					rear->prev()->next(nullptr);
					delete rear;
					rear = temp2;
					size--;
					return (i + 1);
				}
				else {
					temp->prev()->next(temp->next());
					temp->next()->prev(temp->prev());
					delete temp;
					size--;
					return (i + 1);
				}
			}
		}
		return -1;
	}


}

#endif







































































































// _X_XMXMXI