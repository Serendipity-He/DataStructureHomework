#include <iostream>
#include <string>
#include <sstream>
using namespace std;

typedef struct Node {
    int data;
    Node *next;
}*Pointer;

int main() {
    string s;
    getline(cin, s);
    stringstream ss(s);
    int k;
    ss >> k;

    // 1. 显式处理k<=0的非法情况
    if (k <= 0) {
        cout << "Not Found";
        return 0;
    }

    // 构建链表（头节点为哨兵节点，不存储有效数据）
    Pointer head = new Node;
    head->data = -1;
    head->next = NULL;
    Pointer p = head; // 指向当前链表尾节点
    int num;
    int cnt = 0; // 统计有效节点数量
    while (ss >> num) {
        Pointer q = new Node;
        q->data = num;
        q->next = NULL;
        p->next = q;
        p = q;
        cnt++;
    }

    // 2. 计算正数第n个节点（倒数第k个 = 正数第cnt-k+1个）
    int n = cnt - k + 1;
    if (n <= 0) { // 节点数量不足k个
        cout << "Not Found";
        // 释放链表内存（避免泄漏）
        p = head;
        while (p != NULL) {
            Pointer temp = p;
            p = p->next;
            delete temp;
        }
        return 0;
    }

    // 3. 遍历找到第n个节点
    p = head;
    do {
        p = p->next;
        n--;
    } while (n > 0);

    // 4. 输出目标节点的值（核心修复点）
    cout << p->data;

    // 释放链表内存（避免泄漏）
    p = head;
    while (p != NULL) {
        Pointer temp = p;
        p = p->next;
        delete temp;
    }

    return 0; // main函数规范返回0
}
