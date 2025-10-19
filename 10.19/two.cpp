//
// Created by 何明阳 on 2025/10/19.
//

#include "iostream"
#define N 10001

using namespace std;

int main()
{
    int n;
    cin>>n;
    int front=0;
    int rear=0;
    int size=n+1;

    int top=0;
    int S[N];
    int Q[N];

    for (int i = 0; i < n; ++i) {
        rear=(rear+1)%size;
        cin>>Q[rear];
    }

    for (int i = 0; i < n; ++i) {
        top++;
        S[top]=Q[front+1];
        front=(front+1)%size;
    }

    for (int i = 0; i < n; ++i) {
        cout<<S[top]<<' ';
        top--;
    }
}