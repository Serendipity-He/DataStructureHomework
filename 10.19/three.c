#include "stdio.h"
#define N 100

typedef struct Stack {
    int data[N];
    int top;
} Stack;


void initStack(Stack *s) {
    s->top = 0;  // 空栈
}

void pushStack(Stack *s, int x) {
    if (s->top == N) {
        printf("溢出\n");
        return;
    }
    s->top++;
    s->data[s->top] = x;
}

/* 出栈：指针传递，修改原栈，返回弹出元素 */
int popStack(Stack *s) {
    if (s->top == 0) {
        printf("下溢\n");
        return -1;  // 下溢时返回无效值
    }
    int val = s->data[s->top];  // 保存栈顶元素
    s->top--;  // 栈顶下移
    return val;
}

/* 判断栈是否为空 */
int isEmpty(Stack *s) {  // 指针传递，检查原栈状态
    return (s->top == 0) ? 1 : 0;
}

/* 获取栈顶元素（不弹出）：仅返回值，不修改栈顶 */
int topStack(Stack *s) {
    if (isEmpty(s)) {
        printf("空\n");
        return -1;  // 空栈返回无效值
    }
    return s->data[s->top];  // 直接返回栈顶元素（top指向的位置）
}

int main() {
    int n;
    Stack stack;
    int inputIndex, outputIndex, popCount;
    int i;
    int input[N], output[N];

    scanf("%d", &n);

    /* 读取输入序列 */
    for (i = 0; i < n; i++) {
        scanf("%d", &input[i]);
    }

    /* 读取输出序列 */
    for (i = 0; i < n; i++) {
        scanf("%d", &output[i]);
    }

    initStack(&stack);  // 传递栈的地址
    inputIndex = 0;
    outputIndex = 0;
    popCount = 0;

    while (outputIndex < n) {
        /* 栈顶匹配当前输出元素，出栈 */
        if (!isEmpty(&stack) && topStack(&stack) == output[outputIndex]) {
            popStack(&stack);
            outputIndex++;
            popCount++;
        }
            /* 输入序列还有元素，继续入栈 */
        else if (inputIndex < n) {
            pushStack(&stack, input[inputIndex]);
            inputIndex++;
        }
            /* 无法匹配且无元素可入栈：序列不合法 */
        else {
            printf("0\n");
            return 0;  // 直接退出
        }
    }

    /* 所有输出元素匹配完成，输出出栈次数 */
    printf("%d\n", popCount);
    return 0;
}