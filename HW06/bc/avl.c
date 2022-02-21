#include "my_malloc.h"
#include "avl.h"

void create_node(node_t **new_ptr, int val, node_t *parent);

void rotate_right(node_t *node);
void rotate_left(node_t *node);
int update_depth(node_t *node);

void avl_insert(node_t *avl, int val)
{
    if (val < avl->val)
    {
        if (avl->left)
            avl_insert(avl->left, val);
        else
            create_node(&avl->right, val, avl);
    }
    else if (val > avl->val)
    {
        if (avl->right)
            avl_insert(avl->right, val);
        else
            create_node(&avl->right, val, avl);
    }
}
void avl_delete(node_t *avl, int val);
node_t *avl_find(int val);

void create_node(node_t **new_ptr, int val, node_t *parent)
{
    node_t *ret = allocate_memory(sizeof(node_t));
    ret->val = val;
    ret->parent = parent;
    ret->left = NULL, ret->right = NULL;
    ret->depth = 0;
    *new_ptr = ret;
}

void rotate_right(node_t *node)
{
    node_t *temp = node->left;
    node->left = node->left->right;
    temp->right = node;
    update_depth(node);
}

void rotate_left(node_t *node)
{
}

int update_depth(node_t *node)
{
    const int left = node->left ? node->left->depth : -1;
    const int right = node->right ? node->right->depth : -1;
    const int max = left > right ? left : right;
    if (node->depth == max + 1)
        return 0;

    node->depth = max + 1;
    return 1;
}
