
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
				tail->next = new Node<T>(X, Y, val, 0 ,0);
				tail = tail->next;
			}
			else {
				head = tail = new Node<T>(X, Y, val, 0, 0);
			}
	}
	void add(T R, T G, T B,T X,T Y) {
		if (head) {
			tail->next = new Node<T>(R, G, B, X, Y);
			tail = tail->next;
		}
		else {
			head = tail = new Node<T>(R, G, B, X, Y);
		}
	}
	void remove(T val){
		Node<T> *p = head; 
		while (p->param3 != val ){
			p = p->next;
		}
		if (p->param3 == val){
			if (p == head){
				head = p->next;
			}
			else{
				head->next = p->next;
			}

			delete p;
		}
	}
	void removeFirst(){
		Node<T> *p = head;
		if (p == head){
			head = p->next;
		}
		else{
			head->next = p->next;
		}
		delete p;
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
	int getAverageWithID(int whichValue = 0){
		Node<T> *p = head;
		int i = 0;
		int value = 0;
		while (p) {
			switch (whichValue){
				case 1:
					value += p->param1;
					break;
				case 2:
					value += p->param2;
					break;
				case 4:
					value += p->param4;
					break;
				case 5:
					value += p->param5;
					break;
				default:
				case 3:
					value += p->param3;
					break;
				}
			i++;
			p = p->next;
		}

		return (value/i);
	}

	int getValueWithID(int n, int whichValue = 0){
		Node<T> *p = head;
		int i = 0;
		int value = -1;
		while (p) {
			if (i == n){
				switch (whichValue){
					case 1:
						value = p->param1;
						break;
					case 2:
						value = p->param2;
						break;
					case 4:
						value = p->param4;
						break;
					case 5:
						value = p->param5;
						break;
					default:
					case 3:
						value = p->param3;
						break;
				}
			}
			i++;
			p = p->next;
		}
		return value;
	}
	int getDataXBasedOnNumberInLine(int n){
		Node<T> *p = head;
		int i = 0;
		int value = -1;
		while (p) {
			i++;
			if (i == n){
				value = p->param1;
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
				value = p->param2;
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
				value = p->param3;
			}
			p = p->next;
		}
		return value;
	}
	void setValueBasedOnNumberInLine(int n, int value){
		Node<T> *p = head;
		int i = 0;
		while (p) {
			
			if (i == n){
				p->param3 = value;
			}
			i++;
			p = p->next;
		}
	}
	void setValueWithID(int n, int value, int parameter = 0){
		Node<T> *p = head;
		int i = 0;
		while (p) {
			if (i == n){
				switch (parameter){
					case 1:
						p->param1 = value;
						break;
					case 2:
						p->param2 = value;
						break;
					case 4:
						p->param5 = value;
						break;
					case 5:
						p->param5 = value;
						break;
					default:
					case 3:
						p->param3 = value;
						break;
				}
			}
			i++;
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
	void sort(int Links, int Rechts, int parameter = 0){
		int L = Links;
		int R = Rechts;
		int inhoudMidden = getValueWithID((L + R) / 2,parameter);
		//	cout << "DEBUG" << inhoudMidden << " L " << Links << " R " << Rechts <<  endl;
		do
		{
			while (getValueWithID(L) < inhoudMidden,parameter){
				L++;
			}
			while (inhoudMidden < getValueWithID(R,parameter)){
				R--;
			}

			if (L <= R){
				int inhoudLinks = getValueWithID(L,parameter);
				setValueWithID(L, getValueWithID(R, parameter),parameter);
				setValueWithID(R, inhoudLinks, parameter);
				L++;
				R--;
			}
		} while (L <= R);

		if (Links < R){
			sort(Links, R, parameter);
		}
		if (L < Rechts){
			sort(L, Rechts, parameter);
		}
	}
	
	void print() { //FOR DEBUGGING PURPOSES
		if (head) {
			Node<T> *p = head;

			while (p) {
				cout << "R " << p->param1 << " G " << p->param2 << " B " << p->param3 << " X " << p->param4 << " Y " << p->param5 << endl;
				p = p->next;
			}
		}
	}

};

#endif//_LINKEDLIST_H