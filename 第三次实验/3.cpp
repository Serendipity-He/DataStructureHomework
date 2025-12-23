#include <iostream>
#include <vector>
using namespace std;

// 链表栈节点结构（存储路径）
typedef struct StackNode {
    int x;          // 行坐标
    int y;          // 列坐标
    int d;          // 走到下一个节点的方向（1=右，2=下，3=左，4=上）
    StackNode *next;
} Node;   

// ==================== 自定义链表队列（替代STL queue） ====================
// 队列节点结构
typedef struct QueueNode {
    int x, y;               // 当前坐标
    struct QueueNode *next; // 下一个节点
} QueueNode;

// 队列管理结构（简化操作，记录头尾）
typedef struct {
    QueueNode *front; // 队头
    QueueNode *rear;  // 队尾
} LinkQueue;

// 初始化队列
void InitQueue(LinkQueue &q) {
    q.front = q.rear = nullptr;
}

// 判断队列是否为空
bool IsQueueEmpty(LinkQueue &q) {
    return q.front == nullptr;
}

// 入队（尾插）
bool EnQueue(LinkQueue &q, int x, int y) {
    QueueNode *newNode = new QueueNode;
    if (!newNode) return false; // 内存分配失败
    newNode->x = x;
    newNode->y = y;
    newNode->next = nullptr;
    if (IsQueueEmpty(q)) {
        q.front = q.rear = newNode;
    } else {
        q.rear->next = newNode;
        q.rear = newNode;
    }
    return true;
}

// 出队（头删）
bool DeQueue(LinkQueue &q, int &x, int &y) {
    if (IsQueueEmpty(q)) return false;
    QueueNode *temp = q.front;
    x = temp->x;
    y = temp->y;
    q.front = q.front->next;
    if (q.front == nullptr) { // 队列为空时，尾指针置空
        q.rear = nullptr;
    }
    delete temp;
    return true;
}

// 销毁队列
void DestroyQueue(LinkQueue &q) {
    int x, y;
    while (DeQueue(q, x, y));
}

// ==================== 前驱信息结构体（替代tuple） ====================
typedef struct {
    int preX;  // 前驱x坐标
    int preY;  // 前驱y坐标
    int dir;   // 从前驱到当前的方向（1-4）
} PreNode;

// 迷宫全局变量（动态输入）
vector<vector<int>> maze;
int startX = 0;    // 入口x坐标
int startY = 1;    // 入口y坐标
int endX = 8;      // 出口x坐标
int endY = 9;      // 出口y坐标
int m = 10;        // 迷宫行数
int n = 10;        // 迷宫列数

// 方向偏移：0=右(d=1)、1=下(d=2)、2=左(d=3)、3=上(d=4)
int d[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// ==================== 链表栈操作函数（复用原有逻辑） ====================
Node* initStack() { return nullptr; }
bool isEmpty(Node* stack) { return stack == nullptr; }
void push(Node*& p, int x, int y, int dir) {
    Node *q = new Node;
    q->x = x; q->y = y; q->d = dir; q->next = p;
    p = q;
}
bool pop(Node* &stack, int &x, int &y, int &dir) {
    if (isEmpty(stack)) return false;
    Node* temp = stack;
    x = temp->x; y = temp->y; dir = temp->d;
    stack = stack->next; delete temp;
    return true;
}
bool GetTop(Node* stack, int &x, int &y, int &dir) {
    if (isEmpty(stack)) return false;
    x = stack->x; y = stack->y; dir = stack->d;
    return true;
}
void destroyStack(Node* &stack) {
    int x, y, dir;
    while (pop(stack, x, y, dir));
}
void PrintPath(Node* stack) {
    if (isEmpty(stack)) {
        cout << "无通路！" << endl;
        return;
    }
    vector<Node*> path;
    Node* cur = stack;
    while (cur != nullptr) {
        path.push_back(cur);
        cur = cur->next;
    }
    for (int i = path.size() - 1; i > 0; --i) {
        cout << "(" << path[i]->x << "," << path[i]->y << "," << path[i]->d << ")";
        if (i > 1) cout << ", ";
    }
    cout << endl;
}

// ==================== DFS 求任意通路（原有逻辑） ====================
Node* dfs() {
    vector<vector<bool>> flag(m, vector<bool>(n, false));
    Node* p = initStack();
    push(p, startX, startY, 0);
    flag[startX][startY] = true;
    while (!isEmpty(p)) {
        int tmpX, tmpY, tmpD;
        GetTop(p, tmpX, tmpY, tmpD);
        if (tmpX == endX && tmpY == endY) {
            return p;
        }

        bool find = false;
        for (int i = 0; i < 4; ++i) {
            int newX = tmpX + d[i][0];
            int newY = tmpY + d[i][1];
            int newD = i + 1;
            if (newX >= 0 && newX < m && newY >= 0 && newY < n 
                && maze[newX][newY] == 0 && !flag[newX][newY]) {
                p->d = newD;
                push(p, newX, newY, 0);
                flag[newX][newY] = true;
                find = true;
                break;
            }
        }
        if (!find) {
            pop(p, tmpX, tmpY, tmpD);
        }
    }
    return p;
}

// ==================== BFS 求最短通路（无queue/tuple） ====================
Node* bfs() {
    // 1. 初始化：访问标记 + 前驱记录（自定义PreNode结构体）
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    vector<vector<PreNode>> pre(m, vector<PreNode>(n));
    // 初始化前驱为-1（表示无前驱）
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            pre[i][j].preX = -1;
            pre[i][j].preY = -1;
            pre[i][j].dir = 0;
        }
    }

    // 2. 初始化自定义链表队列
    LinkQueue q;
    InitQueue(q);
    EnQueue(q, startX, startY);
    visited[startX][startY] = true;
    bool found = false;

    // 3. BFS核心循环（层序遍历）
    while (!IsQueueEmpty(q) && !found) {
        int curX, curY;
        DeQueue(q, curX, curY); // 出队当前节点

        // 遍历四个方向
        for (int i = 0; i < 4; ++i) {
            int newX = curX + d[i][0];
            int newY = curY + d[i][1];
            int dir = i + 1; // 方向编码（1-4）

            // 合法性检查
            if (newX >= 0 && newX < m && newY >= 0 && newY < n 
                && maze[newX][newY] == 0 && !visited[newX][newY]) {
                
                visited[newX][newY] = true;
                // 记录前驱信息（替代tuple）
                pre[newX][newY].preX = curX;
                pre[newX][newY].preY = curY;
                pre[newX][newY].dir = dir;
                EnQueue(q, newX, newY); // 新节点入队

                // 找到出口，终止搜索
                if (newX == endX && newY == endY) {
                    found = true;
                    break;
                }
            }
        }
    }

    // 销毁队列（释放内存）
    DestroyQueue(q);

    // 4. 未找到出口，返回空栈
    if (!found) {
        return initStack();
    }

    // 5. 回溯前驱，构建最短路径栈
    Node* pathStack = initStack();
    int curX = endX, curY = endY;
    while (curX != -1 && curY != -1) {
        PreNode pNode = pre[curX][curY];
        // 当前节点入栈（初始方向0）
        push(pathStack, curX, curY, 0);
        // 修正前驱节点的方向（前驱→当前的方向）
        if (pNode.preX != -1 && pNode.preY != -1 && pathStack->next != nullptr) {
            pathStack->next->d = pNode.dir;
        }
        // 跳转到前驱节点
        curX = pNode.preX;
        curY = pNode.preY;
    }

    return pathStack;
}

// ==================== 主函数 ====================
int main() {
    // 输入迷宫尺寸和布局
    cin >> m >> n;
    vector<vector<int>> a(m, vector<int>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> a[i][j];
        }
    }
    maze = a;

    // 测试DFS任意通路
    cout << "=== DFS 任意通路 ===" << endl;
    Node* dfsPath = dfs();
    PrintPath(dfsPath);
    destroyStack(dfsPath);

    // 测试BFS最短通路
    cout << "\n=== BFS 最短通路 ===" << endl;
    Node* bfsPath = bfs();
    PrintPath(bfsPath);
    destroyStack(bfsPath);

    return 0;
}