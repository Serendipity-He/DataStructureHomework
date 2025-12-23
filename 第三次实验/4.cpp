#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>  // 用于 malloc/free
using namespace std;

// 方向偏移量：d=1右、d=2下、d=3左、d=4上（对应行、列偏移）
const int dx[] = {0, 1, 0, -1};  // 行偏移（d=1/2/3/4 对应索引0/1/2/3）
const int dy[] = {1, 0, -1, 0};  // 列偏移
const int dir[] = {1, 2, 3, 4};  // 方向编号

// 1. 三元组结构体：(i,j,d) 表示坐标(i,j)，下一步方向d
struct Triple {
    int i, j, d;
    Triple(int i_ = 0, int j_ = 0, int d_ = 0) : i(i_), j(j_), d(d_) {}
};

// 2. 链表栈节点结构
struct StackNode {
    Triple data;       // 数据域：三元组
    StackNode* next;   // 指针域：指向下一节点
    StackNode(Triple t) : data(t), next(nullptr) {}
};

// 3. 链表栈结构（栈顶指针）
struct LinkStack {
    StackNode* top;    // 栈顶指针
    LinkStack() : top(nullptr) {}
};

// 4. 前驱节点结构体（用于BFS求最短路径）
struct PreNode {
    int i, j;          // 坐标
    int pre_i, pre_j;  // 前驱坐标
    int d;             // 到当前节点的方向
    PreNode() : i(0), j(0), pre_i(-1), pre_j(-1), d(0) {}
};

/*
1 0 1 1 1 1 1 1 1 1
1 0 0 1 0 0 0 1 0 1
1 0 0 1 0 0 0 1 0 1
1 0 0 0 0 1 1 0 0 1
1 0 1 1 1 0 0 0 0 1
1 0 0 0 1 0 0 0 0 1
1 0 1 0 0 0 1 0 0 1
1 0 1 1 1 0 1 1 0 1
1 1 0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1 1 1
*/

// const int maze[10][10] = {
//     {1,0,1,1,1,1,1,1,1,1},
//     {1,0,0,1,0,0,0,1,0,1},
//     {1,0,0,1,0,0,0,1,0,1},
//     {1,0,0,0,0,1,1,0,0,1},
//     {1,0,1,1,1,0,0,0,0,1},
//     {1,0,0,0,1,0,0,0,0,1},
//     {1,0,1,0,0,0,1,0,0,1},
//     {1,0,1,1,1,0,1,1,0,1},
//     {1,1,0,0,0,0,0,0,0,0},
//     {1,1,1,1,1,1,1,1,1,1}
// };
// 栈初始化（已在构造函数完成，此处仅作兼容）
void InitStack(LinkStack& S) {
    S.top = nullptr;
}

// 判断栈空
bool StackEmpty(LinkStack& S) {
    return S.top == nullptr;
}

// 入栈
bool Push(LinkStack& S, Triple t) {
    StackNode* p = new StackNode(t);  // 新建节点
    if (!p) return false;             // 内存分配失败
    p->next = S.top;                  // 新节点指向原栈顶
    S.top = p;                        // 栈顶指向新节点
    return true;
}

// 出栈（返回栈顶元素）
bool Pop(LinkStack& S, Triple& t) {
    if (StackEmpty(S)) return false;
    StackNode* p = S.top;             // 暂存栈顶节点
    t = p->data;                      // 取出数据
    S.top = p->next;                  // 栈顶后移
    delete p;                         // 释放节点内存
    return true;
}

// 取栈顶元素（不弹出）
bool GetTop(LinkStack& S, Triple& t) {
    if (StackEmpty(S)) return false;
    t = S.top->data;
    return true;
}

// 销毁栈
void DestroyStack(LinkStack& S) {
    Triple t;
    while (!StackEmpty(S)) {
        Pop(S, t);
    }
}

// 打印栈中路径（从栈底到栈顶，即入口到出口）
void PrintPath(LinkStack& S) {
    if (StackEmpty(S)) {
        cout << "路径为空！" << endl;
        return;
    }
    // 先将栈元素存入数组（反转顺序）
    vector<Triple> path;
    StackNode* p = S.top;
    while (p) {
        path.push_back(p->data);
        p = p->next;
    }
    // 逆序打印（入口→出口）
    for (int i = path.size() - 1; i >= 0; --i) {
        cout << "(" << path[i].i << "," << path[i].j << "," << path[i].d << ")";
        if (i > 0) cout << " → ";
    }
    cout << endl;
}

// 辅助函数：判断坐标(i,j)是否合法（在迷宫范围内、非障碍、未访问）
bool IsValid(int i, int j, int m, int n, vector<vector<int>>& maze, vector<vector<bool>>& visited) {
    return (i >= 1 && i <= m && j >= 1 && j <= n)  // 坐标在合法范围
        && (maze[i][j] == 0)                       // 非障碍
        && (!visited[i][j]);                       // 未访问
}

// ------------- 1. 求一条通路（DFS + 链表栈）-------------
bool FindOnePath(vector<vector<int>>& maze, int m, int n, LinkStack& path) {
    if (maze[1][1] == 1 || maze[m][n] == 1) {  // 入口/出口是障碍
        return false;
    }
    vector<vector<bool>> visited(m + 1, vector<bool>(n + 1, false));  // 访问标记
    Push(path, Triple(1, 1, 0));  // 入口入栈（初始方向0，无实际意义）
    visited[1][1] = true;

    while (!StackEmpty(path)) {
        Triple cur;
        GetTop(path, cur);  // 取栈顶节点
        if (cur.i == m && cur.j == n) {  // 到达出口
            return true;
        }

        bool find = false;
        // 尝试4个方向（从当前方向的下一个方向开始，避免重复）
        for (int k = cur.d; k < 4; ++k) {
            int ni = cur.i + dx[k];
            int nj = cur.j + dy[k];
            int nd = dir[k];  // 方向编号（1-4）
            if (IsValid(ni, nj, m, n, maze, visited)) {
                // 更新栈顶节点的方向（标记已尝试到k+1）
                path.top->data.d = k + 1;
                // 新节点入栈
                Push(path, Triple(ni, nj, 0));
                visited[ni][nj] = true;
                find = true;
                break;
            }
        }
        if (!find) {  // 所有方向都走不通，回溯
            Pop(path, cur);
        }
    }
    return false;  // 无通路
}

// ------------- 2. 求最短通路（BFS）-------------
bool FindShortestPath(vector<vector<int>>& maze, int m, int n, vector<Triple>& shortPath) {
    if (maze[1][1] == 1 || maze[m][n] == 1) {
        return false;
    }
    vector<vector<bool>> visited(m + 1, vector<bool>(n + 1, false));
    vector<vector<PreNode>> pre(m + 1, vector<PreNode>(n + 1));  // 前驱数组
    queue<pair<int, int>> q;  // BFS队列（存储坐标）

    // 初始化
    q.push({1, 1});
    visited[1][1] = true;

    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop();

        if (i == m && j == n) {  // 到达出口，回溯路径
            int ci = m, cj = n;
            while (ci != 1 || cj != 1) {
                PreNode p = pre[ci][cj];
                shortPath.push_back(Triple(p.i, p.j, p.d));
                ci = p.i;
                cj = p.j;
            }
            // 反转路径（入口→出口）
            reverse(shortPath.begin(), shortPath.end());
            // 补充出口的方向（无实际意义，设为0）
            shortPath.push_back(Triple(m, n, 0));
            return true;
        }

        // 遍历4个方向
        for (int k = 0; k < 4; ++k) {
            int ni = i + dx[k];
            int nj = j + dy[k];
            int nd = dir[k];
            if (IsValid(ni, nj, m, n, maze, visited)) {
                visited[ni][nj] = true;
                q.push({ni, nj});
                // 记录前驱
                pre[ni][nj].i = i;
                pre[ni][nj].j = j;
                pre[ni][nj].d = nd;
            }
        }
    }
    return false;
}

// ------------- 3. 求所有通路（DFS + 回溯）-------------
void DFS_AllPaths(vector<vector<int>>& maze, int m, int n, int i, int j,
                  vector<vector<bool>>& visited, vector<Triple>& curPath,
                  vector<vector<Triple>>& allPaths) {
    if (i == m && j == n) {  // 到达出口，记录路径
        allPaths.push_back(curPath);
        return;
    }
    // 遍历4个方向
    for (int k = 0; k < 4; ++k) {
        int ni = i + dx[k];
        int nj = j + dy[k];
        int nd = dir[k];
        if (IsValid(ni, nj, m, n, maze, visited)) {
            visited[ni][nj] = true;
            curPath.push_back(Triple(ni, nj, nd));
            DFS_AllPaths(maze, m, n, ni, nj, visited, curPath, allPaths);
            // 回溯
            curPath.pop_back();
            visited[ni][nj] = false;
        }
    }
}

vector<vector<Triple>> FindAllPaths(vector<vector<int>>& maze, int m, int n) {
    vector<vector<Triple>> allPaths;
    if (maze[1][1] == 1 || maze[m][n] == 1) {
        return allPaths;
    }
    vector<vector<bool>> visited(m + 1, vector<bool>(n + 1, false));
    vector<Triple> curPath;
    // 入口入栈
    visited[1][1] = true;
    curPath.push_back(Triple(1, 1, 0));
    DFS_AllPaths(maze, m, n, 1, 1, visited, curPath, allPaths);
    return allPaths;
}

// 辅助函数：打印所有通路
void PrintAllPaths(vector<vector<Triple>>& allPaths) {
    if (allPaths.empty()) {
        cout << "无任何通路！" << endl;
        return;
    }
    cout << "共找到 " << allPaths.size() << " 条通路：" << endl;
    for (int i = 0; i < allPaths.size(); ++i) {
        cout << "通路 " << i + 1 << "：";
        for (int j = 0; j < allPaths[i].size(); ++j) {
            cout << "(" << allPaths[i][j].i << "," << allPaths[i][j].j << "," << allPaths[i][j].d << ")";
            if (j != allPaths[i].size() - 1) cout << " → ";
        }
        cout << endl;
    }
}

// 辅助函数：打印最短通路
void PrintShortPath(vector<Triple>& shortPath) {
    if (shortPath.empty()) {
        cout << "无最短通路！" << endl;
        return;
    }
    cout << "最短通路：";
    for (int i = 0; i < shortPath.size(); ++i) {
        cout << "(" << shortPath[i].i << "," << shortPath[i].j << "," << shortPath[i].d << ")";
        if (i != shortPath.size() - 1) cout << " → ";
    }
    cout << endl;
}

int main() {
    int m,n;
    cin>>m>>n;
    vector<vector<int>> maze(n,vector<int>(m));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin>>maze[i][j];
        }
    }

    // ========== 1. 求一条通路 ==========
    cout << "----- 一条通路 -----" << endl;
    LinkStack onePath;
    InitStack(onePath);
    if (FindOnePath(maze, m, n, onePath)) {
        PrintPath(onePath);
    } else {
        cout << "无通路！" << endl;
    }
    DestroyStack(onePath);

    // ========== 2. 求最短通路 ==========
    cout << "\n----- 最短通路 -----" << endl;
    vector<Triple> shortPath;
    if (FindShortestPath(maze, m, n, shortPath)) {
        PrintShortPath(shortPath);
    } else {
        cout << "无最短通路！" << endl;
    }

    // ========== 3. 求所有通路 ==========
    cout << "\n----- 所有通路 -----" << endl;
    vector<vector<Triple>> allPaths = FindAllPaths(maze, m, n);
    PrintAllPaths(allPaths);

    return 0;
}