#ifndef _STACK_H
#define _STACK_H

#include <iostream>

//Stack is a template class with one generic type.
template <class T>
class Stack
{
public:
  Stack();
  Stack(const Stack & rhs );
  ~Stack();

  bool isEmpty() const;
  bool isFull() const;
  void makeEmpty();

  void pop();
  void push(const T & t);
  T topAndPop();
  T & top() const;

  const Stack & operator = (const Stack & rhs);

private:
  struct ListNode{
    T element;
    ListNode * next;

    ListNode(const T & theElement, ListNode * n = NULL)
      : element(theElement) , next(n){}
  };

  ListNode * topOfStack;
  
};

#endif
