#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree*)calloc(1, sizeof(rbtree));
    node_t *newNode = (node_t*)calloc(1, sizeof(node_t));
    newNode -> color = RBTREE_BLACK;
    p->root = p->nil = newNode;
    return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // 트리의 노드 생성
    node_t *targetNode = (node_t*)calloc(1, sizeof(node_t));
    // 노드에 key값 할당
    targetNode->key = key;
    // 트리의 nil인 노드 parent_of_targetNode선언
    node_t *parent_of_targetNode = t->nil;
    // 트리의 루트노드 x선언
    node_t *x = t->root;

    // 어느자리에 새로운 노드가 삽입될지 결정되는 while문
    // 트리의 루트노드가 nil이 아니라면 즉 부모가 있으면(부모가 nil이면 루트노드이다.)
    while(x != t->nil){
        // x의 값을 parent_of_targetNode에 임시저장
        parent_of_targetNode = x;
        // 루트(부모)노드의 값과 넣을 노드의 값을 비교
        if (targetNode->key < x->key) 
            x = x->left;
        else
            x = x->right;
    }
    // targetNode의 부모 노드를 parent_of_targetNode로 변경
    targetNode->parent = parent_of_targetNode;

    // nill노드를 끊고 새로운 노드를 붙여넣는 조건문
    // parent_of_targetNode가 nil이라면
    if (parent_of_targetNode == t->nil)
        // 타겟노드의 부모가 nil이므로 타겟노드는 루트노드임을 나타낸다.
        t->root = targetNode;
    // 타겟노드의 값보다 부모의 값이 크다면
    else if (targetNode->key < parent_of_targetNode->key)
        parent_of_targetNode->left = targetNode;
    else
        parent_of_targetNode->right = targetNode;

    // 새로운 노드에 nil노드를 초기화
    targetNode->left = t->nil;
    targetNode->right = t->nil;
    targetNode->color = RBTREE_RED;
    insertion_fixup(t, targetNode);
    return t->root;
}

void insertion_fixup(rbtree *t, node_t *z)
{
    // z의 부모 노드가 붉은 노드인 경우
    while (z->parent->color == RBTREE_RED)
    {
        // z의 부모 노드가 왼쪽 자식 노드인 경우
        if (z->parent == z->parent->parent->left)
        {
            // z의 오른쫀 삼촌 노드 y 선언
            node_t *y = z->parent->parent->right;

            // 삼촌 노드 y가 붉은 노드인 경우
            if (y->color == RBTREE_RED)
            {
                // z의 부모 노드를 검은 노드로 변환
                z->parent->color = RBTREE_BLACK;
                // z의 삼촌 노드 y를 검은 노드로 변환
                y->color = RBTREE_BLACK;
                // z의 조부모 노드를 붉은 노드로 변환
                z->parent->parent->color = RBTREE_RED;
                // z를 z의 조부모 노드로 변경
                z = z->parent->parent;
            }
            // 삼촌 노드 y가 검은 노드인 경우
            else
            {
                // z가 부모 노드의 오른쪽 자식인 경우
                if (z == z->parent->right)
                {
                    // z에 z의 부모 노드 저장
                    z = z->parent;
                    // 레프트 로테이션 실행
                    rotate_left(t, z);
                }
                // z의 부모 노드를 검은 노드로 변환
                z->parent->color = RBTREE_BLACK;
                // z의 조부모 노드를 붉은 노드로 변환
                z->parent->parent->color = RBTREE_RED;
                // 라이트 로테이션 실행
                rotate_right(t, z->parent->parent);
            }
        }
        // z의 부모 노드가 오른쪽 자식 노드인 경우
        else
        {
            // z의 왼쪽 삼촌 노드 y 선언
            node_t *y = z->parent->parent->left;

            if (y->color == RBTREE_RED)
            {
                // z의 부모 노드를 검은 노드로 변환
                z->parent->color = RBTREE_BLACK;
                // z의 삼촌 노드 y를 검은 노드로 변환
                y->color = RBTREE_BLACK;
                // z의 조부모 노드를 붉은 노드로 변환
                z->parent->parent->color = RBTREE_RED;
                // z를 z의 조부모 노드로 변경
                z = z->parent->parent;
            }
            else
            {
                // z가 부모 노드의 왼쪽 자식 노드인 경우
                if (z == z->parent->left)
                {
                    // z에 z의 부모 노드 저장
                    z = z->parent;
                    // 라이트 로테이션
                    rotate_right(t, z);
                }
                // z의 부모 노드를 검은 노드로 변환
                z->parent->color = RBTREE_BLACK;
                // z의 조부모 노드를 붉으 노드로 변환
                z->parent->parent->color = RBTREE_RED;
                // 레프트 로테이션 실행
                rotate_left(t, z->parent->parent);
            }
        }
    }
    // 트리의 루트 노드롤 검은 노드로 변환
    t->root->color = RBTREE_BLACK;
}

void rotate_left(rbtree *t, node_t *x){
    // x의 오른쪽 자식 노드 y 선언
    node_t *y = x->right;

    // y의 왼쪽 자식 노드를 x의 오른쪽 자식 노드로 변경
    x->right = y->left;

    // y의 왼쪽 자식 노드가 nil 노드가 아니면 y의 왼쪽 자식 노드의 부모 노드를 x로 변경
    if (y->left != t->nil)
        y->left->parent = x;

    // y의 부모 노드를 x의 부모 노드로 변경
    y->parent = x->parent;

    // x의 부모 노드가 nil 노드이면 트리의 루트 노드를 y로 변경
    if (x->parent == t->nil)
        t->root = y;
    // x가 부모 노드의 왼쪽 자식 노드이면 x의 부모 노드의 왼쪽 자식 노드를 y로 변경
    else if (x == x->parent->left)
        x->parent->left = y;
    // x가 부모 노드의 오른쪽 자식 노드이면 x의 부모 노드의 오른쪽 자식 노드를 y로 변경
    else
        x->parent->right = y;

    // y의 왼쪽 자식 노드를 x로 변경
    y->left = x;
    // x의 부모 노드를 y로 변경
    x->parent = y;
}

void rotate_right(rbtree *t, node_t *x){
    // x의 왼쪽 자식 노드 y 선언
    node_t *y = x->left;
    // y의 오른쪽 자식 노드를 x의 왼쪽 자식 노드로 변경
    x->left = y->right;
    // y의 오른쪽 자식 노드가 nil 노드가 아니면 y의 오른쪽 자식 노드의 부모 노드를 x로 변경
    if (y->right != t->nil)
        y->right->parent = x;
    // y의 부모 노드를 x의 부모 노드로 변경
    y->parent = x->parent;
    // x의 부모 노드가 nil 노드이면 트리의 루트 노드를 y로 변경
    if (x->parent == t->nil)
        t->root = y;
    // x가 x의 부모 노드의 왼쪽 자식 노드이면 x의 부모 노드의 왼쪽 자식 노드를 y로 변경
    else if (x == x->parent->left)
        x->parent->left = y;
    // x가 x의 부모 노드의 오른쪽 자식 노드이면 x의 부모 노드의 오른쪽 자식 노드를 y로 변경
    else
        x->parent->right = y;
    // y의 오른쪽 자식 노드를 x로 변경
    y->right = x;
    // x의 부모 노드를 y로 변경
    x->parent = y;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}


node_t *rbtree_min(const rbtree *t) {
    node_t* min = t->root;
    while (min->left != t->nil){
        min = min->left;
    }
    return min;
}

node_t *rbtree_max(const rbtree *t) {
    node_t* max = t->root;
    while (max->right != t->nil){
        max = max->right;
    }
    return max;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
