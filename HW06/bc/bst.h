#ifndef __BST_H__
#define __BST_H__

typedef struct node_t
{
    int val;              //key value
    struct node_t *left;  //left child
    struct node_t *right; //right child
} node_t;

typedef struct
{
    int num_nodes; //number of nodes in the tree/subtree
    int depth;     //depth of the tree/subtree
} bst_info_t;

/* Insert a single value in the BST (bst cannot be NULL)*/
void bst_insert(node_t *bst, int val);
/* Delete value from the BST 
 * Changes the pointer to new child (can set the pointer to NULL) */
void bst_delete(node_t **bst_ptr, int val);

/* Insert an interval of integers into the BST (bst cannot be NULL)*/
void bst_int_insert(node_t *bst, int lo, int hi);
/* Delete an interval of integer values from the BST 
 * Changes the pointer to new child (can set the pointer to NULL) */
void bst_int_delete(node_t **bst_ptr, int lo, int hi);

/* Allocates and returns pointer to new node with given key value */
node_t *create_node(int val);
/* Deletes given node and return pointer to new child formed from it's subtree
 * (Can return NULL) */
node_t *delete_node(node_t *node);

/* Crawls through the bst and counts it's depth and number of nodes */
bst_info_t bst_crawl(node_t *bst);
/* Crawls through the bst and counts it's depth and number of nodes,
 * while also deallocating remaining nodes from the tree */
bst_info_t bst_crawl_free(node_t *bst);
/* Prints key values from the BST in-order */
void print_bst_inorder(node_t *bst);

#endif