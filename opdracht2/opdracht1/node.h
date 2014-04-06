#ifndef _NODE_H
#define _NODE_H

//@author Lars Veenendaal
//V0.1 03-04-14

template<typename T>class LinkedList;
template<typename T>class Node {
	friend class LinkedList<T>;
public:
	Node(T val = 0, Node<T> *next = 0) : value(val), next(next){}
	Node(T dataX, T dataY, T val, Node<T> *next = 0) : data_X(dataX), data_Y(dataY), value(val), next(next){}
private:
	T data_X;
	T data_Y;
	T value;
	Node<T> *next;
};

#endif//_NODE_H

