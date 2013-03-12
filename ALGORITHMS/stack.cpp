#include "stack.hpp"

   //destructor for stack class
Stack<ItemType>::~Stack()
{
    clear();
}

//constructor for stacks
Stack<ItemType>::Stack()
{
    top=NULL;
    counter=0;
}


class Stack
{
public:






   //method to push elements to the stack
   template <class ItemType>
   void push(const ItemType item)
   {
       Node <ItemType> *tmp = new Node<ItemType>;
       tmp->data=item;
       if(isEmpty())
       {
            top=NULL;

       }
        else
        {
        tmp->next=top;
        top=tmp;
       }
        counter++;
   }

   //method to remove elements from the stack
   template <class ItemType>
   ItemType pop()
   {
       assert(!IsEmpty());
       ItemType item = top->data;
        StackNode<ItemType> *tmp = top;
        top=top->next;
        delete tmp;
        counter--;
        return tmp;
   }


   //delete all the elements of the stack
   template <class ItemType>
   void clear()
   {
        assert(!IsEmpty());
        Node<ItemType> *tmp;
        //loop through all the nodes of stack from the top
        while(top != NULL)
        {
           tmp = top;
           top = top->next;
           delete tmp;
        }


    }

   //check if the stack is empty
   template <class ItemType>
   bool IsEmpty() const
    {
            return (top==NULL);
   }


   //return the number of elements of the stack
   template <class ItemType>
   int count() const
   {
        return counter;

   }

private:
   //pointer to the top the stack
   Node<ItemType> *top;

   //counter for number of items in the stack
   int counter;
};





