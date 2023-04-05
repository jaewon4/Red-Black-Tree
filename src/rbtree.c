#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree*)calloc(1, sizeof(rbtree));
    node_t *newNode = (node_t*)calloc(1, sizeof(node_t));
    newNode -> color = RBTREE_BLACK;
    p->root = p->nil = newNode;
    return p;
}

void postorder_delete_all_node(rbtree* t, node_t* targetNode){
    // 후위순회로 메모리할당을 해제해준다. 전위 x, 중위 x
    if (targetNode != t->nil){
        postorder_delete_all_node(t, targetNode->left);
        postorder_delete_all_node(t, targetNode->right);
        free(targetNode);
        targetNode = NULL;
    }
}

void delete_rbtree(rbtree *t) {
    postorder_delete_all_node(t, t->root);
    // 마지막으로 nil과 rbtree메모리 할당 해제
    free(t->nil);
    t->nil = NULL;
    free(t);
    t = NULL;
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
    // RB Tree 내 해당 key가 있는지 탐색하여 있으면 node pointer 반환; 없으면 NIL 반환
    node_t *x = t->root;
    while (x != t->nil){
        if (key == x->key)
            return x;
        else if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return NULL;
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

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  // u : 삭제할 노드, v : 대체할 노드
  // u가 루트노드인지 확인
  if (u->parent == t->nil)
    // 루트 노드면 t->root가 v를 가리키게 만든다.
    t->root = v;
  // u가 윗노드의 왼쪽에 붙어있었다면
  else if (u == u->parent->left)
    // 부모노드의 왼쪽을 v로 바꿔 붙인다.
    u->parent->left = v;
  else
    // 부모노드의 오른쪽을 v로 바꿔 붙인다.
    u->parent->right = v;
  // 노드를 바꿔 붙인 이후에는 가리키고 있는 부모노드도 바꿔줘야한다.
  // 즉 v의 부모노드를 갱신해준다.
  v->parent = u->parent;
}

node_t *rbtree_successor(const rbtree *t, node_t *root){
    // root는 오른쪽 노드의 주소가 들어오므로 최소값을 찾아주면 된다.
    while(root->left != t->nil)
        root = root->left;
    return root;
}

void rbtree_erase_fixup(rbtree* t, node_t* x){
   // 트리에서 노드를 제거한 후 발생할 수 있는 레드-블랙 트리 속성 위반을 수정하는 데 사용됩니다
    // w : 형재노드, x : 본인노드(더블블랙) 
    while((x != t->root) && (x->color == RBTREE_BLACK)){
        // CASE 1 ~ 4 : LEFT CASE
        // 부모노드의 왼쪽에 붙어있는 경우
        if (x == x->parent->left){
            // 형제노드 w선언
            node_t* w = x->parent->right;
            // CASE 1 : x의 형제 w가 적색인 경우
            if (w->color == RBTREE_RED){
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_left(t, x->parent);
                w = x->parent->right;
            }
            // CASE 2 : x의 형제 w는 흑색이고 w의 두 자식이 모두 흑색인 경우
            if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)){
                w->color = RBTREE_RED;
                // x의 부모가 더블블랙이나 레드엔블랙이 되므로
                // 더블요소를 가리키는 x를 바꾸어준다. 만약 부모가 레드였다면 x가 레드를 가지면서 
                // while문을 나가고 블랙으로 바꾸어준다 즉, 레드엔블랙인 요소를 처리하는 로진인것이다.
                x = x->parent;
            }
            else{
                // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
                if (w->right->color == RBTREE_BLACK){
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_right(t, w);
                    w = x->parent->right;
                }
                // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                rotate_left(t, x->parent);
                x = t->root;
            }
        }
        // 부모노드의 오른쪽에 붙어있는 경우
        // CASE 5 ~ 8 : RIGHT CASE
        else{
            node_t *w = x->parent->left;
            // CASE 5 : x의 형제 w가 적색인 경우
            if (w->color == RBTREE_RED){
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotate_right(t, x->parent);
                w = x->parent->left;
            }
            // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
            if ((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK)){
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else{
                // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
                if (w->left->color == RBTREE_BLACK){
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotate_left(t, w);
                    w = x->parent->left;
                }
                // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rotate_right(t, x->parent);
                x = t->root;
            }
        }
    }
    // 레드엔 블랙이면 블랙으로 바꿔주는 부분이다.
    x -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
      // p : 삭제할 노드
    node_t *x;
    // y에 삭제할 노드의 주소를 담는다.
    node_t *y = p;
    // 삭제할 노드의 색을 담아둔다.
    color_t y_original_color = p->color;
    // 삭제할 노드의 왼쪽 자식이 nil이면 오른쪽 노드를 삭제할 노드로 이동(자식노드가 없는경우 또한 포함)
    if (p->left == t->nil){
        x = p->right;
        rbtree_transplant(t, p, p->right);
    }
    // 삭제할 노드의 오른쪽 자식이 nil이면 왼쪽 노드를 삭제할 노드로 이동
    else if (p->right == t->nil){
        x = p->left;
        rbtree_transplant(t, p, p->left);
    }
    // 양쪽 모두 자식노드가 있는 경우
    else {
        // 삭제할 노드를 succesor로 바꿔준다. 오른쪽 최소값
        y = rbtree_successor(t, p->right);
        y_original_color = y->color;
        // x는 y의 오른쪽 서브트리를 가리킴
        x = y->right;
        // 조건의 의미는 y가 삭제할 노드 p의 직계자식인지 확인
        if (y->parent == p)
            // 직계자식이면 y의 오른쪽 하위 트리가 p에 직접 연결되어야함 
            x->parent = y;
        // y가 삭제할 노드 p의 직계자식이 아니면 else
        else{
            // 삭제할 노드 y를 대체할 노드(y->right)으로 교체하여 
            // y의 오른쪽 하위 트리를 노드의 오른쪽 하위 트리로 만듭니다
            rbtree_transplant(t, y, y->right);
            // p->right는 삭제할 노드(p)의 오른쪽 자식을 후속 노드(y)의 오른쪽 자식에 할당합니다. 
            // 이 단계는 삭제할 노드(p)의 오른쪽 하위 트리가 후속 노드(y)에 올바르게 연결되도록 합니다.
            y->right = p->right;                                            
            // y는 계승자(y)의 오른쪽 자식의 부모를 y로 설정하여 y가 새로운 부모임을 나타냅니다.
            // 이 단계는 계승자(y)가 삭제할 노드(p)를 대체하므로 오른쪽 자식이 y를 부모로 가져야 하기 때문에 필요합니다.
            y->right->parent = y;
        }
        // 삭제할 노드 p를 대체할 노드(y)으로 교체 
        rbtree_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    // 삭제되는 색이 블랙이면 케이스를 나눠서 처리해준다.
    if (y_original_color == RBTREE_BLACK)
        // 처리하는 함수 
        rbtree_erase_fixup(t, x);
    free(p);
    return 0;
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  
  return 0;
}

void rbtree_inorder(const rbtree *t, node_t *root, int *cnt, key_t *arr, int n){

  return;
}