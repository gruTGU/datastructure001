#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char data;
    struct TreeNode *lchild;
    struct TreeNode *rchild;
} TreeNode;

/* ===================== 建树：前序 + # 为空 ===================== */
/*
   输入示例：ABD##E##CF###  （# 表示空）
   读取方式：从字符串中按下标递增读取
*/
TreeNode* buildFromPreorder(const char *pre, int *idx) {
    char c = pre[*idx];
    if (c == '\0') return NULL;          // 防御：字符串结束
    (*idx)++;

    if (c == '#') return NULL;

    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = c;
    node->lchild = buildFromPreorder(pre, idx);
    node->rchild = buildFromPreorder(pre, idx);
    return node;
}

/* ===================== (1) 树高：递归 ===================== */
int treeHeight(TreeNode *root) {
    if (!root) return 0;
    int hl = treeHeight(root->lchild);
    int hr = treeHeight(root->rchild);
    return (hl > hr ? hl : hr) + 1;
}

/* ===================== (3) 后序遍历：递归 ===================== */
void postorder(TreeNode *root) {
    if (!root) return;
    postorder(root->lchild);
    postorder(root->rchild);
    printf("%c", root->data);
}

/* ===================== (2) 根到任意结点路径：DFS ===================== */
/*
   用 DFS 回溯找路径：
   - path[] 保存当前路径
   - 找到目标就返回 1，并打印路径
*/
int dfsFindPath(TreeNode *root, char target, char path[], int depth) {
    if (!root) return 0;

    path[depth] = root->data;

    if (root->data == target) {
        // 打印 root -> target 的路径
        for (int i = 0; i <= depth; i++) {
            if (i) printf("->");
            printf("%c", path[i]);
        }
        printf("\n");
        return 1;
    }

    if (dfsFindPath(root->lchild, target, path, depth + 1)) return 1;
    if (dfsFindPath(root->rchild, target, path, depth + 1)) return 1;

    return 0;
}

/* ===================== (4) 结点所处高度：BFS（层序） ===================== */
/*
   这里“所处高度”按 BFS 典型含义：结点在树中的层数 level
   - 根 level=1
   - 根的孩子 level=2
   BFS：队列中同时记录结点及其 level
*/

typedef struct QNode {
    TreeNode *node;
    int level;
} QNode;

typedef struct Queue {
    QNode *arr;
    int front, rear, cap;
} Queue;

Queue* createQueue(int cap) {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->arr = (QNode*)malloc(sizeof(QNode) * cap);
    q->front = 0;
    q->rear = 0;
    q->cap = cap;
    return q;
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

void enqueue(Queue *q, TreeNode *node, int level) {
    if (q->rear >= q->cap) {
        // 简单扩容
        q->cap *= 2;
        q->arr = (QNode*)realloc(q->arr, sizeof(QNode) * q->cap);
    }
    q->arr[q->rear].node = node;
    q->arr[q->rear].level = level;
    q->rear++;
}

QNode dequeue(Queue *q) {
    return q->arr[q->front++];
}

int bfsGetNodeLevel(TreeNode *root, char target) {
    if (!root) return -1;

    Queue *q = createQueue(64);
    enqueue(q, root, 1);

    while (!isEmpty(q)) {
        QNode cur = dequeue(q);
        TreeNode *p = cur.node;

        if (p->data == target) {
            free(q->arr);
            free(q);
            return cur.level;
        }

        if (p->lchild) enqueue(q, p->lchild, cur.level + 1);
        if (p->rchild) enqueue(q, p->rchild, cur.level + 1);
    }

    free(q->arr);
    free(q);
    return -1;  // 没找到
}

/* ===================== 释放树 ===================== */
void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->lchild);
    freeTree(root->rchild);
    free(root);
}

/* ===================== 主程序 ===================== */
int main() {
    char preorderStr[2048];

    printf("请输入前序序列（#表示空），例如：ABD##E##CF###\n");
    printf("输入：");
    if (scanf("%2047s", preorderStr) != 1) {
        printf("读取失败。\n");
        return 1;
    }

    int idx = 0;
    TreeNode *root = buildFromPreorder(preorderStr, &idx);

    // (1) 树高
    int h = treeHeight(root);
    printf("\n(1) 树的高度 = %d\n", h);
    if (h < 5) {
        printf("提示：你输入的树高度小于 5，不满足题目“高度不低于5”的要求，请换一组前序测试。\n");
    }

    // (3) 后序
    printf("\n(3) 后序遍历序列：");
    postorder(root);
    printf("\n");

    // (2) 路径查询（DFS）
    printf("\n(2) 查询根到任意结点路径（DFS）。输入目标结点字符（输入0结束）：\n");
    while (1) {
        char target;
        printf("目标结点 = ");
        scanf(" %c", &target);
        if (target == '0') break;

        char path[1024];
        if (!dfsFindPath(root, target, path, 0)) {
            printf("未找到结点 %c\n", target);
        }
    }

    // (4) 结点所处高度（BFS）
    printf("\n(4) 计算任意结点所处高度（BFS层序，根为1层）。输入目标结点字符（输入0结束）：\n");
    while (1) {
        char target;
        printf("目标结点 = ");
        scanf(" %c", &target);
        if (target == '0') break;

        int level = bfsGetNodeLevel(root, target);
        if (level == -1) {
            printf("未找到结点 %c\n", target);
        } else {
            printf("结点 %c 所处高度(层数) = %d\n", target, level);
        }
    }

    freeTree(root);
    return 0;
}
