#include <stdio.h>

#include "bst.h"

int main(int argc, char *argv[])
{
    node_t *bst = NULL;
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        int lo, hi;
        char op;

        scanf(" %c %d %d", &op, &lo, &hi);
        if (op == 'i') //INSERT
        {
            if (!bst) //empty BST
            {
                bst = create_node(lo);
                bst_int_insert(bst, lo + 1, hi);
            }
            else
                bst_int_insert(bst, lo, hi);
        }
        else if (op == 'd') //DELETE
        {
            bst_int_delete(&bst, lo, hi);
        }
    }

    bst_info_t info = bst_crawl_free(bst);
    printf("%d %d\r\n", info.num_nodes, info.depth);

    return 0;
}
