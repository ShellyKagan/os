#include<stdio.h>
#include<stdlib.h>
#define SYSTEM_ERROR -2
#define INPUT_ERROR -3
struct node
{
    int data;
    struct node *next;
};
struct node *head;

/**
 * inserts a thread to the sleeping list.
 * @return 0 on success, SYSTEM_ERROR or INPUT_ERROR on failure.
 */
int insert_to_sleeping(int tid)
{
  struct node *ptr;
  int item;
  ptr = (struct node *) malloc(sizeof(struct node *));
  if(ptr == NULL)
  {
    return SYSTEM_ERROR;
  }
  else
  {
    ptr->data = tid;
    ptr->next = head;
    head = ptr;
    return 0;
  }
}

/**
 * returns the index of the thread in the sleeping list
 * @return the index on success, SYSTEM_ERROR or INPUT_ERROR on failure.
 */
int index_in_sleeping(int tid)
{
  struct node *ptr;
  int i=0;
  ptr = head;
  if(ptr == NULL)
  {
    return INPUT_ERROR;
  }
  else
  {
    while (ptr!=NULL)
    {
      if(ptr->data == tid)
      {
        return i+1;
      }
      i++;
      ptr = ptr -> next;
    }
    return INPUT_ERROR;
  }
}

void delete_fron_sleeping(int tid)
{
  struct node *ptr,*ptr1;
  int loc,i;
  loc = index_in_sleeping (tid);
  ptr=head;
  for(i=0;i<loc;i++)
  {
    ptr1 = ptr;
    ptr = ptr->next;

    if(ptr == NULL)
    {
      return INPUT_ERROR;
    }
  }
  ptr1 ->next = ptr ->next;
  free(ptr);
  return 0;
}

void display()
{
  struct node *ptr;
  ptr = head;
  if(ptr == NULL)
  {
    printf("sleeping list doesnt exists");
    fflush();
  }
  else
  {
    printf("\nprinting sleeping threads . . . . .\n");
    fflush();
    while (ptr!=NULL)
    {
      printf("\n%d",ptr->data);
      fflush();
      ptr = ptr -> next;
    }
  }
}

int get_sleepers_amount(){
  int len = 0;
  while (ptr!=NULL)
  {
    len++;
    ptr = ptr -> next;
  }
  return len;
}