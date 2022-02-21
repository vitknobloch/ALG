#include "bst.h"
#include "my_malloc.h"

#include <stdio.h>

void bst_insert(node_t *bst, int val)
{
    if (val < bst->val)
    {
        if (bst->left)
            bst_insert(bst->left, val);
        else
            bst->left = create_node(val);
    }
    else if (val > bst->val)
    {
        if (bst->right)
            bst_insert(bst->right, val);
        else
            bst->right = create_node(val);
    }
}

void bst_delete(node_t **bst_ptr, int val)
{
    node_t *bst = *bst_ptr;
    if (!bst)
        return;

    if (val < bst->val)
        bst_delete(&bst->left, val);
    else if (val > bst->val)
        bst_delete(&bst->right, val);
    else
        *bst_ptr = delete_node(bst);
}

void bst_int_insert(node_t *bst, int lo, int hi)
{
    if (lo > hi) //empty interval
        return;

    if (hi < bst->val) //whole interval to the left
    {
        if (bst->left)
            bst_int_insert(bst->left, lo, hi);
        else
        {
            bst->left = create_node(lo);
            bst_int_insert(bst->left, lo + 1, hi);
        }
    }
    else if (lo > bst->val) //whole interval to the right
    {
        if (bst->right)
            bst_int_insert(bst->right, lo, hi);
        else
        {
            bst->right = create_node(lo);
            bst_int_insert(bst->right, lo + 1, hi);
        }
    }
    else //inside interval
    {
        if (bst->left)
        {
            bst_int_insert(bst->left, lo, bst->val - 1);
        }
        else if (bst->val > lo)
        {
            bst->left = create_node(lo);
            bst_int_insert(bst->left, lo + 1, bst->val - 1);
        }

        if (bst->right)
        {
            bst_int_insert(bst->right, bst->val + 1, hi);
        }
        else if (bst->val < hi)
        {
            bst->right = create_node(bst->val + 1);
            bst_int_insert(bst->right, bst->val + 2, hi);
        }
    }
}

void bst_int_delete(node_t **bst_ptr, int lo, int hi)
{
    if (lo > hi) //empty interval
        return;
    node_t *bst = *bst_ptr;
    if (!bst) //empty tree
        return;

    if (hi < bst->val) //whole interval to the left
        bst_int_delete(&bst->left, lo, hi);
    else if (lo > bst->val) //whole interval to the right
        bst_int_delete(&bst->right, lo, hi);
    else //inside interval
    {
        bst_int_delete(&bst->left, lo, bst->val - 1);
        bst_int_delete(&bst->right, bst->val + 1, hi);
        *bst_ptr = delete_node(bst);
    }
}

node_t *create_node(int val)
{
    node_t *new = allocate_memory(sizeof(node_t));
    new->left = NULL;
    new->right = NULL;
    new->val = val;
    return new;
}

node_t *delete_node(node_t *node)
{
    node_t *ret;
    if (!node->left) //no left child, add right child as parents child
    {
        ret = node->right;
        deallocate_memory((void **)&node);
    }
    else if (!node->right) //no right child, add left child as parents child
    {
        ret = node->left;
        deallocate_memory((void **)&node);
    }
    else //both children
    {
        //find left-most node of right subtree
        node_t *smallest_larger = node->right;
        node_t *prev = NULL;
        while (smallest_larger->left)
        {
            prev = smallest_larger;
            smallest_larger = smallest_larger->left;
        }

        //swap values
        node->val = smallest_larger->val;
        //delete left-most node of right subtree
        if (prev)
            prev->left = delete_node(smallest_larger);
        else
            node->right = delete_node(smallest_larger);

        ret = node;
    }
    return ret;
}

bst_info_t bst_crawl(node_t *bst)
{
    bst_info_t info = {.depth = -1, .num_nodes = 0};
    if (!bst)
    {
        return info;
    }

    bst_info_t left = bst_crawl(bst->left);
    bst_info_t right = bst_crawl(bst->right);

    info.num_nodes = 1 + left.num_nodes + right.num_nodes;
    info.depth = 1 + (left.depth > right.depth ? left.depth : right.depth);

    return info;
}

bst_info_t bst_crawl_free(node_t *bst)
{
    bst_info_t info = {.depth = -1, .num_nodes = 0};
    if (!bst)
    {
        return info;
    }

    bst_info_t left = bst_crawl_free(bst->left);
    bst_info_t right = bst_crawl_free(bst->right);
    deallocate_memory((void **)&bst);

    info.num_nodes = 1 + left.num_nodes + right.num_nodes;
    info.depth = 1 + (left.depth > right.depth ? left.depth : right.depth);

    return info;
}

void print_bst_inorder(node_t *bst)
{
    if (!bst)
        return;

    print_bst_inorder(bst->left);
    printf("%d ", bst->val);
    print_bst_inorder(bst->right);
}