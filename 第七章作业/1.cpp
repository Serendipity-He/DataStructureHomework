#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct Node
{
    int x;
    Node* next;
};

int main()
{
    int pn,dn;
    cin>>pn>>dn;
    vector<int> p(pn);
    for (int i = 0; i < pn; i++)
    {
        cin>>p[i];
    }

}