#ifndef _NODE_H
#define _NODE_H

//@author Lars Veenendaal
//V0.1 03-04-14

template<typename T>class LinkedList;
template<typename T>class Node {
	friend class LinkedList<T>;
public:
	Node(T val = 0, Node<T> *next = 0) : param3(param3), next(next){}
	Node(T param1, T param2, T param3, Node<T> *next = 0) : param1(param1), param2(param2), param3(param3), next(next){}
	Node(T param1, T param2, T param3, T param4, T param5, Node<T> *next = 0) : param1(param1), param2(param2), param3(param3), param4(param4), param5(param5), next(next){}
private:
	T param1;
	T param2;
	T param3;
	T param4;
	T param5;
	Node<T> *next;
};

#endif//_NODE_H

