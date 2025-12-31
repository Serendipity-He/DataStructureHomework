#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

typedef struct tree
{
	int data;
	tree *lchild;
	tree *rchild;
};
typedef struct tree *treeNode;
typedef tree *btree;

void bsinsert(treeNode &s,btree& t){
	if(t==NULL){
		t=s;
	}
	else if(s->data < t->data){
		bsinsert(s,t->lchild);
	}
	else{
		bsinsert(s,t->rchild);
	}
}

void sortTree(int m,vector<int>r,treeNode &p)
{
	treeNode q=new tree;
	q->data=r[0];
	q->rchild=NULL;
	q->lchild=NULL;
	p=q;
	for(int i=1;i<m;i++){
		treeNode q=new tree;
		q->data=r[i];
		q->rchild=NULL;
		q->lchild=NULL;
		bsinsert(q,p);	
	}
}

void inOrder(treeNode t)
{
	if(t!=NULL){
		inOrder(t->lchild);
		cout<<t->data<<' ';
		inOrder(t->rchild);
	}
}

int main()
{
    int n=0;
	cin>>n;
	vector<int>a(n);
	for (int i = 0; i < n; i++)
	{
		cin>>a[i];
	}
	
	treeNode p=NULL;
	sortTree(n,a,p);
	inOrder(p);
	
}



