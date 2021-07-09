#include <iostream>
#include "Stack.h"
using namespace std;

//Constructor
template<class T>
Stack<T>::Stack(){
    topOfStack = NULL;
}

template<class T>
Stack<T>::~Stack(){
    makeEmpty();
}

template <class T>
Stack<T>::Stack( const Stack<T> & rhs ){
    topOfStack = NULL;
    *this = rhs; // deep copy
}

template<class T>
const Stack<T> & Stack<T>:: operator = (const Stack<T> & rhs){
    if(this != rhs){
        makeEmpty();
        if(rhs.isEmpty())
            return *this;


        ListNode * rhsPtr = rhs.topOfStack;
        ListNode * currPtr = new ListNode(rhsPtr -> element);

        for(rhsPtr = rhsPtr-> next; rhsPtr != NULL; rhsPtr->next ){
            currPtr-> next = new ListNode(rhsPtr-> elenent);
            currPtr = currPtr->next;
        }
    } 
    return *this;  

} 

template<class T>
bool Stack<T>::isEmpty() const{
    return topOfStack == NULL;
}

template<class T>
T & Stack<T>::top() const {
    if(isEmpty())
        throw exception();
    return topOfStack-> element;
}

template<class T>
void Stack<T>::pop(){
     if(isEmpty())
        //throw UnderFlow();
        return;
    ListNode * temp = topOfStack;
    topOfStack = topOfStack -> next;
    delete temp;

}

template<class T>
void Stack<T>::push(const T & elem){
    topOfStack = new ListNode(elem, topOfStack);
}

template<class T>
T Stack<T>::topAndPop(){
    T topElem = top();
    pop();
    return topElem;
}

template <class T>
void Stack<T>::makeEmpty( ){
    while (!isEmpty())
        pop();
}