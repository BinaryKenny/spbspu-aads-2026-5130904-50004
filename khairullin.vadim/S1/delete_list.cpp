#include "delete_list.hpp"

template<class T>
void khairullin::delete_LL(List<T> * fake)
{
  List<T> * head = fake->cut_fake(fake);
  while(head)
  {
    head->val = head->val->cut_fake(head->val);
    if (head->val)
    {
      head->val->clear(head->val);
    }
    head = head->cut(head);
  }
}

template<class T>
void khairullin::delete_List(List<T> * fake)
{
  List<T> * head = fake->next;
  fake->cut_fake(fake);
  if (head)
  {
    head->clear(head);
  }
}
