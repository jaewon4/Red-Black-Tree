#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);
// // 추가함수 -> rbtree.c로 이동
// void rotate_right(rbtree *t, node_t *x);
// void rotate_left(rbtree *t, node_t *x);
// void insertion_fixup(rbtree *t, node_t *z);
// void postorder_delete_all_node(rbtree* t, node_t* targetNode);
// void rbtree_inorder(const rbtree *t, node_t *root, int *cnt, key_t *arr, int n);
// void rbtree_transplant(rbtree *t, node_t *u, node_t *v);
// node_t *rbtree_successor(const rbtree *t, node_t *root);
// void rbtree_erase_fixup(rbtree* t, node_t* x);
#endif  // _RBTREE_H_
