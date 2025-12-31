#include <iostream>
#include <string>
#include <sstream>
using namespace std;

typedef struct node
{
	int data;
	node*link;
} Node;

int main()
{
	Node *Head=new Node;
	Head->data=-1;
	Head->link=NULL;
	Node *p=Head;
	string s;
	getline(cin,s);
	stringstream ss(s);
	int num;
	while(ss>>num){
		Node *q=new node;
		q->data=num;
		q->link=NULL;
		p->link=q;
		p=q;
	}
	int maxNum=Head->link->data;
	Node *pre_max=Head;
	Node *maxNode=Head->link;
	p=Head->link;
	while(p->link!=NULL){
		Node*nextNode=p->link;
		if(nextNode->data>maxNum){
			maxNode=nextNode;
			pre_max=p;
			maxNum=maxNode->data;
		}
		p=p->link;
	}
	
	if(maxNode->link!=NULL){
		pre_max->link=maxNode->link;
		p=Head;
		while(p->link!=NULL){
			p=p->link;
		}
		p->link=maxNode;
		maxNode->link=NULL;
	}
	
	p=Head->link;
	while(p!=NULL){
		cout<<p->data<<' ';
		p=p->link;
	}
}

