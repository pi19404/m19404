#ifndef STACK_HPP
#define STACK_HPP


//node for the linked list
template <class ItemType>
class Node
{
   ItemType data;
   Node <ItemType> next;
};

//class that implements stack data structure
class Stack
{
public:

    //constructor for stack class
    Stack();

    //destructor for stack class
    ~Stack();

};

#endif // STACK_HPP
