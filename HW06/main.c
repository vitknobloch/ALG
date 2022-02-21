#include <stdio.h>

#include "dualAVL.h"

void print_tree(node_t *root, int depth);
void test();

extern node_t *root;

int main(int argc, char *argv[])
{
    root = create_node(NULL);
    int ops, start, diff, max;
    char opchar;
    if (scanf("%d", &ops) != 1)
        return 100;
    for (int i = 0; i < ops; ++i)
    {
        if (scanf(" %c %d %d %d", &opchar, &start, &diff, &max) != 4)
            return 101;

        if (opchar == 'I')
            for (int key = start; key <= max; key += diff)
                avl_insert(key, root);
        else if (opchar == 'D')
            for (int key = start; key <= max; key += diff)
                avl_delete(key, root);

        else
            return 102;
    }

    stats_t stats = get_stats(root);
    free_tree(root);
    printf("%d %d %d %d\n", stats.nodes, stats.leafc[1], stats.leafc[2], stats.leafc[3]);

    return 0;
}

void print_tree(node_t *root, int depth)
{
    if (!root)
        return;
    print_tree(root->left, depth + 1);
    for (int i = 0; i < depth; ++i)
    {
        printf("\t");
    }
    printf("%d ", root->depth);
    for (int i = 0; i < root->keyc; ++i)
    {
        printf("%d ", root->keys[i]);
    }
    printf("\n");
    print_tree(root->right, depth + 1);
}
/*
void test()
{
    root = create_node(NULL);
    avl_insert(1, root);
    avl_insert(2, root);
    avl_insert(3, root);
    avl_insert(4, root);
    avl_insert(5, root);
    avl_insert(6, root);
    avl_insert(7, root);
    avl_insert(8, root);
    avl_insert(9, root);
    print_tree(root, 0);
    avl_insert(10, root);
    print_tree(root, 0);

    avl_delete(36, root);
    avl_delete(71, root);
    avl_delete(52, root);
    print_tree(root, 0);
}*/
