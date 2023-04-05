#include "rbtree.h"
#include <stdio.h>
void inorderTraversalPrint(node_t *root) {
    if (root->left == NULL && root->right == NULL) return;
    inorderTraversalPrint(root->left);
    printf("%d ", root->key);
    inorderTraversalPrint(root->right);
}
int main(int argc, char *argv[]) {
    rbtree *tree = new_rbtree();
    node_t *t = rbtree_insert(tree, 30);
     t = rbtree_insert(tree, 50);
    // t = rbtree_insert(tree, 10);
     t = rbtree_insert(tree, 98);
    // t = rbtree_insert(tree, 20);
    // t = rbtree_insert(tree, 5);
     t = rbtree_insert(tree, 65);
    inorderTraversalPrint(t);
    delete_rbtree(tree);
    return 0;
}

// #include "rbtree.h"

// int main(int argc, char *argv[]) {

// }