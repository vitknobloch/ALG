#ifndef __DUALAVL_H__
#define __DUALAVL_H__

typedef struct node
{
    int keys[3];
    int keyc;
    int depth;
    struct node *left;
    struct node *right;
    struct node *parent;

} node_t;

typedef struct
{
    int nodes;
    int leafc[4];
} stats_t;

void avl_insert(int val, node_t *node);
void avl_delete(int val, node_t *node);
node_t *create_node(node_t *parent);
stats_t get_stats(node_t *node);
void free_tree(node_t *node);
node_t *get_sibling(node_t *node);

inline int is_leaf(node_t *node)
{
    return !(node->left || node->right);
}

inline int is_root(node_t *node)
{
    return !node->parent;
}

inline int get_depth(node_t *node)
{
    if (!node)
        return -1;
    return node->depth;
}

inline stats_t add_stats(const stats_t stat1, const stats_t stat2)
{
    stats_t ret;
    ret.nodes = stat1.nodes + stat2.nodes;
    for (int i = 0; i < 4; ++i)
        ret.leafc[i] = stat1.leafc[i] + stat2.leafc[i];
    return ret;
}

#endif