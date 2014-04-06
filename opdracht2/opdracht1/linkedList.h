
#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include <iostream>
using namespace std;

#include "node.h"

template<typename T> class LinkedList {
private:
	Node<T> *head;
	Node<T> *tail;
public:
	LinkedList() : head(0), tail(0){}
	~LinkedList(){
		if (head) {
			Node<T> *p = head;
			Node<T> *q = 0;

			while (p) {
				q = p;
				p = p->next;
				delete q;
			}
		}
	}
	void add(T val){
		if (head) {
			tail->next = new Node<T>(val);
			tail = tail->next;
		}
		else {
			head = tail = new Node<T>(val);
		}
	}

	void add(T X, T Y, T val = 0) {
			if (head) {
				tail->next = new Node<T>(X, Y, val);
				tail = tail->next;
			}
			else {
				head = tail = new Node<T>(X, Y, val);
			}
	}
	void remove(T val){
		Node<T> *p = head; 
		while (p->value != val ){
			p = p->next;
		}
		if (p->value == val){
			if (p == head){
				head = p->next;
			}
			else{
				head->next = p->next;
			}

			delete p;
		}
	}

	int count(){
		int i = 0;
		Node<T> *p = head;
		while (p) {
			i++;
			p = p->next;
		}
		return i;

	}
	int getDataXBasedOnNumberInLine(int n){
		Node<T> *p = head;
		int i = 0;
		int value = -1;
		while (p) {
			i++;
			if (i == n){
				value = p->data_X;
			}
			p = p->next;
		}
		return value;
	}
	int getDataYBasedOnNumberInLine(int n){
		Node<T> *p = head;
		int i = 0;
		int value = -1;
		while (p) {
			i++;
			if (i == n){
				value = p->data_Y;
			}
			p = p->next;
		}
		return value;
	}
	int getValueBasedOnNumberInLine(int n){
		Node<T> *p = head;
		int i = 0;
		int value = -1;
		while (p) {
			i++;
			if (i == n){
				value = p->value;
			}
			p = p->next;
		}
		return value;
	}
	void setValueBasedOnNumberInLine(int n, int value){
		Node<T> *p = head;
		int i = 0;
		while (p) {
			i++;
			if (i == n){
				p->value = value;
			}
			p = p->next;
		}
	}
	
	void sort(int Links, int Rechts){
		int L = Links;
		int R = Rechts;
		int inhoudMidden = getValueBasedOnNumberInLine((L + R) / 2);
	//	cout << "DEBUG" << inhoudMidden << " L " << Links << " R " << Rechts <<  endl;
		do
		{
			while (getValueBasedOnNumberInLine(L) < inhoudMidden){
				L++;
			}
			while (inhoudMidden < getValueBasedOnNumberInLine(R)){
				R--;
			}

			if (L <= R){
				int inhoudLinks = getValueBasedOnNumberInLine(L);
				setValueBasedOnNumberInLine(L, getValueBasedOnNumberInLine(R));
				setValueBasedOnNumberInLine(R, inhoudLinks);
				L++;
				R--;
			}
		} while (L <= R);

		if (Links < R){
			sort(Links, R);
		}
		if (L < Rechts){
			sort(L, Rechts);
		}
	}
	
	void print() { //FOR DEBUGGING PURPOSES
		if (head) {
			Node<T> *p = head;

			while (p) {
			//	cout << "X " << p->data_X << "\tY " << p->data_Y << "\t" << endl;
				cout <<	" V " << p->value << " -> \n";
				p = p->next;
			}
		}
	}

};

#endif//_LINKEDLIST_H