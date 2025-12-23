#include <iostream>
#include <vector>
#include <tuple>  // 用于存储路径三元组
using namespace std;

// 栈节点：存储(x,y,d)，d表示走到下一个节点的方向（1-4）
typedef struct Node {
	int x;
	int y;
	int d;
	Node *next;
} Node;   

// 队列节点（BFS用）
typedef struct QueueNode {
	int x, y;               
	struct QueueNode *next; 
} QueueNode;

// 链式队列结构
typedef struct {
	QueueNode *front; 
	QueueNode *rear;  
} LinkQueue;

// 前驱节点（BFS回溯路径用）
typedef struct {
	int preX;  // 前驱x坐标
	int preY;  // 前驱y坐标
	int dir;   // 从前驱到当前的方向（1-4）
} PreNode;

// ===================== 新增：全局变量（存储所有通路） =====================
typedef tuple<int, int, int> PathNode;  // 路径节点三元组(x,y,d)
vector<vector<PathNode>> allPaths;     // 存储所有通路的容器

// ===================== 全局变量与方向定义 =====================
vector<vector<int>> maze;  // 迷宫矩阵
int startX, startY;        // 起点坐标（0开始）
int endX, endY;            // 终点坐标（0开始）
int m, n;                  // 迷宫行数、列数

// 方向偏移量：d=1右、d=2下、d=3左、d=4上（对应索引0-3）
const int dirOffset[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// ===================== 队列操作 =====================
void InitQueue(LinkQueue &q) {
	q.front = q.rear = nullptr;
}

bool IsQueueEmpty(LinkQueue &q) {
	return q.front == nullptr;
}

bool EnQueue(LinkQueue &q, int x, int y) {
	QueueNode *newNode = new QueueNode;
	if (!newNode) return false; 
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

bool DeQueue(LinkQueue &q, int &x, int &y) {
	if (IsQueueEmpty(q)) return false;
	QueueNode *temp = q.front;
	x = temp->x;
	y = temp->y;
	q.front = q.front->next;
	if (q.front == nullptr) { 
		q.rear = nullptr;
	}
	delete temp;
	return true;
}

void DestroyQueue(LinkQueue &q) {
	int x, y;
	while (DeQueue(q, x, y));
}

// ===================== 栈操作 =====================
Node* initStack() {
	return nullptr;
}

bool isEmpty(Node* stack) {
	return stack == nullptr;
}

void push(Node* &stack, int x, int y, int d) {
	Node *newNode = new Node;
	newNode->x = x;
	newNode->y = y;
	newNode->d = d;
	newNode->next = stack;
	stack = newNode;
}

bool pop(Node* &stack, int &x, int &y, int &d) {
	if (isEmpty(stack)) return false;
	Node* temp = stack;
	x = temp->x;
	y = temp->y;
	d = temp->d;
	stack = stack->next;
	delete temp;
	return true;
}

bool GetTop(Node* stack, int &x, int &y, int &d) {
	if (isEmpty(stack)) return false;
	x = stack->x;
	y = stack->y;
	d = stack->d;
	return true;
}

void DestroyStack(Node* &stack) {
	int x, y, d;
	while (pop(stack, x, y, d));
}

// ===================== 路径打印 =====================
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
	cout << "迷宫通路（三元组(x,y,d)）：" << endl;
	for (int i = path.size() - 1; i > 0; --i) {
		cout << "(" << path[i]->x << "," << path[i]->y << "," << path[i]->d << ")";
		if (i > 1) cout << ", ";
	}
	cout << endl;
}

// ===================== DFS求任意通路 =====================
Node* dfs() {
	vector<vector<bool>> visited(m, vector<bool>(n, false));
	Node* stack = initStack();
	push(stack, startX, startY, 0);
	visited[startX][startY] = true;
	
	while (!isEmpty(stack)) {
		int curX, curY, curD;
		GetTop(stack, curX, curY, curD);
		
		if (curX == endX && curY == endY) {
			return stack;
		}
		
		bool find = false;
		for (int i = curD; i < 4; ++i) {
			int newX = curX + dirOffset[i][0];
			int newY = curY + dirOffset[i][1];
			int newD = i + 1;
			
			if (newX >= 0 && newX < m && newY >= 0 && newY < n 
				&& maze[newX][newY] == 0 && !visited[newX][newY]) {
				stack->d = i + 1;
				push(stack, newX, newY, 0);
				visited[newX][newY] = true;
				find = true;
				break;
			}
		}
		
		if (!find) {
			int x, y, d;
			pop(stack, x, y, d);
		}
	}
	return stack;
}

// ===================== BFS求最短通路 =====================
Node* bfs() {
	vector<vector<bool>> visited(m, vector<bool>(n, false));
	vector<vector<PreNode>> pre(m, vector<PreNode>(n));
	
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			pre[i][j].preX = -1;
			pre[i][j].preY = -1;
			pre[i][j].dir = 0;
		}
	}
	
	LinkQueue q;
	InitQueue(q);
	EnQueue(q, startX, startY);
	visited[startX][startY] = true;
	bool found = false;
	
	while (!IsQueueEmpty(q) && !found) {
		int curX, curY;
		DeQueue(q, curX, curY);
		
		for (int i = 0; i < 4; ++i) {
			int newX = curX + dirOffset[i][0];
			int newY = curY + dirOffset[i][1];
			int dir = i + 1;
			
			if (newX >= 0 && newX < m && newY >= 0 && newY < n 
				&& maze[newX][newY] == 0 && !visited[newX][newY]) {
				visited[newX][newY] = true;
				pre[newX][newY].preX = curX;
				pre[newX][newY].preY = curY;
				pre[newX][newY].dir = dir;
				EnQueue(q, newX, newY);
				
				if (newX == endX && newY == endY) {
					found = true;
					break;
				}
			}
		}
	}
	DestroyQueue(q);
	
	if (!found) {
		return initStack();
	}
	
	Node* stack = initStack();
	int curX = endX, curY = endY;
	while (curX != -1 && curY != -1) {
		push(stack, curX, curY, 0);
		PreNode pNode = pre[curX][curY];
		if (pNode.preX != -1 && pNode.preY != -1 && stack->next != nullptr) {
			stack->next->d = pNode.dir;
		}
		curX = pNode.preX;
		curY = pNode.preY;
	}
	return stack;
}

void dfsAll(int curX, int curY, vector<vector<bool>> &visited, vector<PathNode> &curPath) {
	// 到达终点：保存当前路径
	if (curX == endX && curY == endY) {
		allPaths.push_back(curPath);  // 复制当前路径到总容器
		return;
	}
	
	// 遍历四个方向（右、下、左、上）
	for (int i = 0; i < 4; ++i) {
		int newX = curX + dirOffset[i][0];
		int newY = curY + dirOffset[i][1];
		int dir = i + 1;  // 方向编码：1=右，2=下，3=左，4=上
		
		// 合法性检查：边界+非障碍+未访问
		if (newX >= 0 && newX < m && newY >= 0 && newY < n 
			&& maze[newX][newY] == 0 && !visited[newX][newY]) {
			// 标记已访问（避免重复走）
			visited[newX][newY] = true;
			// 记录路径：当前节点(curX,curY) → 新节点(newX,newY)的方向是dir
			curPath.push_back(make_tuple(curX, curY, dir));
			// 递归访问新节点
			dfsAll(newX, newY, visited, curPath);
			// 回溯：撤销路径记录+取消访问标记（关键！允许其他路径走该节点）
			curPath.pop_back();
			visited[newX][newY] = false;
		}
	}
}

// 封装函数：初始化并启动全路径搜索
void findAllPaths() {
	allPaths.clear();  // 清空历史路径
	vector<vector<bool>> visited(m, vector<bool>(n, false));  // 访问标记
	vector<PathNode> curPath;  // 临时存储当前路径
	visited[startX][startY] = true;  // 起点标记为已访问
	dfsAll(startX, startY, visited, curPath);  // 启动递归
}

// 新增：打印所有通路
void PrintAllPaths() {
	if (allPaths.empty()) {
		cout << "无任何通路！" << endl;
		return;
	}
	
	cout << "迷宫所有通路（共" << allPaths.size() << "条）：" << endl;
	for (int k = 0; k < allPaths.size(); ++k) {
		cout << "第" << k+1 << "条通路（三元组(x,y,d)）：" << endl;
		const auto &path = allPaths[k];
		// 打印路径主体（起点→终点的每一步）
		for (int i = 0; i < path.size(); ++i) {
			int x = get<0>(path[i]);
			int y = get<1>(path[i]);
			int d = get<2>(path[i]);
			cout << "(" << x << "," << y << "," << d << ")";
			if (i < path.size() - 1) cout << ", ";
		}
		// 补充终点（无后续方向，d=0）
		cout << "(" << endX << "," << endY << ",0)" << endl;
	}
}

int main() {
	cout << "请输入迷宫的行数m和列数n：";
	cin >> m >> n;

	maze.resize(m, vector<int>(n));
	cout << "请输入迷宫矩阵（每行输入" << n << "个数字，空格分隔）：" << endl;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			cin >> maze[i][j];
		}
	}
	
	// 3. 输入起点和终点坐标（0开始）
	cout << "请输入起点坐标(x,y)（0≤x<" << m << ", 0≤y<" << n << "）：";
	cin >> startX >> startY;
	cout << "请输入终点坐标(x,y)（0≤x<" << m << ", 0≤y<" << n << "）：";
	cin >> endX >> endY;
	
	// 合法性检查
	if (startX < 0 || startX >= m || startY < 0 || startY >= n ||
		endX < 0 || endX >= m || endY < 0 || endY >= n) {
		cout << "起点/终点坐标超出迷宫范围！" << endl;
		return 0;
	}
	if (maze[startX][startY] == 1 || maze[endX][endY] == 1) {
		cout << "起点或终点为障碍，无通路！" << endl;
		return 0;
	}
	
	// 4. DFS求任意通路
	cout << "\n=== DFS 任意通路 ===" << endl;
	Node* dfsPath = dfs();
	PrintPath(dfsPath);
	DestroyStack(dfsPath);
	
	// 5. BFS求最短通路
	cout << "\n=== BFS 最短通路 ===" << endl;
	Node* bfsPath = bfs();
	PrintPath(bfsPath);
	DestroyStack(bfsPath);
	
	// 6. 新增：求所有通路
	cout << "\n=== 所有通路 ===" << endl;
	findAllPaths();
	PrintAllPaths();
	
	return 0;
}



/*
10
10
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
0 1
8 9
*/
