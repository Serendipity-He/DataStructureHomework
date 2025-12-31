#include <iostream>
#include <stdio.h>
using namespace std;

typedef struct Node{
    int id;
    int data;
    Node *next;
}*Pointer;
int main(){
    int n=3;
    Pointer head=new Node;
    head->next=NULL;
    Pointer p=head;
    while (n--)
    {
        int id;
        int value;
        cin>>id>>value;
        Pointer q=new Node;
        q->id=id;
        q->data=value;
        q->next=NULL;
        p->next=q;
        p=q;
    }
    p=head->next;
    while(p!=NULL)
    {
        printf("[num=%d,score=%d]\n",p->id,p->data);
        p=p->next;
    }
}


