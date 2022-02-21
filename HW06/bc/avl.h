#ifndef __AVL_H__
#define __AVL_H__

typedef struct node
{
    int val;
    int depth;
    struct node *left;
    struct node *right;
    struct node *parent;

} node_t;

void avl_insert(node_t *avl, int val);
void avl_delete(node_t *avl, int val);
node_t *avl_find(int val);

#endif