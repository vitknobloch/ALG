#include "dualAVL.h"
#include "my_malloc.h"

void insert_sort(int *array, int len);
void leaf_insert(int val, node_t *node);
void leaf_delete(int val, node_t *node);
int get_val_index(int val, node_t *node);
node_t *get_max_node(node_t *node);
node_t *get_min_node(node_t *node);
int get_max_key(node_t *node);
int get_min_key(node_t *node);
void update_depth(node_t *node);
void rotate_left(node_t *node);
void rotate_right(node_t *node);
void identify_and_rotate(node_t *node);
void update_depth_no_propagate(node_t *node);

node_t *root;

void avl_insert(int val, node_t *node)
{
    // node contains inserted value
    for (int i = 0; i < node->keyc; ++i)
        if (node->keys[i] == val)
            return;

    if (is_leaf(node))
    {
        leaf_insert(val, node);
        return;
    }

    // node is inner node
    if (val < node->keys[0])
        avl_insert(val, node->left);
    else if (val > node->keys[1])
        avl_insert(val, node->right);
    else
    {
        if (get_depth(node->left) <= get_depth(node->right))
        {
            if (!node->left)
                node->left = create_node(node);
            avl_insert(node->keys[0], node->left);
            node->keys[0] = val;
        }
        else
        {
            if (!node->right)
                node->right = create_node(node);
            avl_insert(node->keys[1], node->right);
            node->keys[1] = val;
        }
    }
}

void avl_delete(int val, node_t *node)
{
    if (is_leaf(node))
        leaf_delete(val, node);
    else if (val < node->keys[0])
    {
        if (node->left)
            avl_delete(val, node->left);
    }
    else if (val > node->keys[1])
    {
        if (node->right)
            avl_delete(val, node->right);
    }
    else
    {
        int val_index = get_val_index(val, node);
        if (val_index == -1)
            return;

        if (get_depth(node->left) <= get_depth(node->right))
        {
            node_t *min_node = get_min_node(node->right);
            int min_key = get_min_key(min_node);
            node->keys[val_index] = min_key;
            if (node->keys[0] > node->keys[1])
            {
                int temp = node->keys[0];
                node->keys[0] = node->keys[1];
                node->keys[1] = temp;
            }
            avl_delete(min_key, min_node);
        }
        else
        {
            node_t *max_node = get_max_node(node->left);
            int max_key = get_max_key(max_node);
            node->keys[val_index] = max_key;
            if (node->keys[0] > node->keys[1])
            {
                int temp = node->keys[0];
                node->keys[0] = node->keys[1];
                node->keys[1] = temp;
            }
            avl_delete(max_key, max_node);
        }
    }
}

node_t *create_node(node_t *parent)
{
    node_t *new = allocate_memory(sizeof(node_t));
    new->depth = 0;
    new->parent = parent;
    new->keyc = 0;
    new->left = new->right = NULL;
    return new;
}

void leaf_insert(int val, node_t *node)
{
    // leaf has room
    if (node->keyc < 3)
    {
        node->keys[node->keyc++] = val;
        return;
    }

    // leaf has to split
    // sort
    int sorted[4] = {val, node->keys[0], node->keys[1], node->keys[2]};
    insert_sort(sorted, 4);
    // split
    node->left = create_node(node);
    node->right = create_node(node);
    avl_insert(sorted[0], node->left);
    avl_insert(sorted[3], node->right);
    node->keys[0] = sorted[1];
    node->keys[1] = sorted[2];
    node->keyc = 2;
    // update depth
    update_depth(node);
}

void leaf_delete(int val, node_t *node)
{
    int val_index = get_val_index(val, node);
    if (val_index == -1)
        return;

    if (node->keyc > 1)
    {
        for (int i = val_index; i < node->keyc - 1; ++i)
            node->keys[i] = node->keys[i + 1];
        node->keyc--;
        return;
    }

    node_t *sibling = get_sibling(node);
    if (!sibling) // root
    {
        node->keyc = 0;
    }
    else if (is_leaf(sibling))
    {
        if (sibling->keyc == 1)
        {
            node->parent->keys[node->parent->keyc++] = sibling->keys[0];
            node_t *parent = node->parent;
            deallocate_memory((void **)&node);
            deallocate_memory((void **)&sibling);
            parent->left = NULL;
            parent->right = NULL;
            update_depth(parent);
        }
        else
        {
            int keys[5] = {0};
            keys[0] = node->parent->keys[0];
            keys[1] = node->parent->keys[1];
            int idx = 2;
            for (int i = 0; i < sibling->keyc; ++i)
                keys[idx++] = sibling->keys[i];
            insert_sort(keys, 2 + sibling->keyc);
            sibling->keyc--;
            if (node->parent->left == node)
            {
                node->keys[0] = keys[0];
                node->parent->keys[0] = keys[1];
                node->parent->keys[1] = keys[2];
                for (int i = 3; i < idx; ++i)
                    sibling->keys[i - 3] = keys[i];
            }
            else
            {
                node->keys[0] = keys[--idx];
                node->parent->keys[1] = keys[--idx];
                node->parent->keys[0] = keys[--idx];
                for (int i = idx - 1; i >= 0; --i)
                    sibling->keys[i] = keys[i];
            }
        }
    }
    else
    {
        int key = node->parent->left == node ? node->parent->keys[0] : node->parent->keys[1];
        node->keys[0] = key;
        avl_delete(key, node->parent);
    }
}

void update_depth_no_propagate(node_t *node)
{
    int left_depth = get_depth(node->left);
    int right_depth = get_depth(node->right);
    node->depth = left_depth > right_depth ? left_depth + 1 : right_depth + 1;
}

void update_depth(node_t *node)
{
    if (!node)
        return;
    int previous = node->depth;
    int left_depth = get_depth(node->left);
    int right_depth = get_depth(node->right);
    int diff = left_depth > right_depth ? left_depth - right_depth : right_depth - left_depth;
    if (diff > 1)
    {
        identify_and_rotate(node);
    }
    else
    {
        node->depth = left_depth > right_depth ? left_depth + 1 : right_depth + 1;
        if (previous != node->depth)
            update_depth(node->parent);
    }
}

void identify_and_rotate(node_t *node)
{
    if (get_depth(node->left) > get_depth(node->right))
    {
        if (get_depth(node->left->right) > get_depth(node->left->left))
            rotate_left(node->left);
        rotate_right(node);
    }
    else
    {
        if (get_depth(node->right->left) > get_depth(node->right->right))
            rotate_right(node->right);
        rotate_left(node);
    }
    update_depth(node->parent->parent);
}

void rotate_left(node_t *node)
{
    node_t *new_root = node->right;
    node->right = new_root->left;
    if (node->right)
        node->right->parent = node;
    new_root->left = node;
    if (node->parent)
    {
        if (node->parent->left == node)
            node->parent->left = new_root;
        else
            node->parent->right = new_root;
    }
    else
        root = new_root;
    new_root->parent = node->parent;
    node->parent = new_root;

    update_depth_no_propagate(node);
    update_depth_no_propagate(new_root);
}

void rotate_right(node_t *node)
{
    node_t *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    if (node->left)
        node->left->parent = node;
    if (node->parent)
    {
        if (node->parent->left == node)
            node->parent->left = new_root;
        else
            node->parent->right = new_root;
    }
    else
        root = new_root;
    new_root->parent = node->parent;
    node->parent = new_root;

    update_depth_no_propagate(node);
    update_depth_no_propagate(new_root);
}

void insert_sort(int *array, int len)
{
    for (int i = 1; i < len; ++i)
    {
        for (int j = i; j > 0; --j)
        {
            if (array[j] < array[j - 1])
            {
                int temp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = temp;
            }
            else
                break;
        }
    }
}

int get_val_index(int val, node_t *node)
{
    int val_index = -1;
    for (int i = 0; i < node->keyc; ++i)
        if (node->keys[i] == val)
            val_index = i;
    return val_index;
}

node_t *get_max_node(node_t *node)
{
    while (node->right)
        node = node->right;

    return node;
}

node_t *get_min_node(node_t *node)
{
    while (node->left)
        node = node->left;

    return node;
}

int get_max_key(node_t *node)
{
    int max_key = node->keys[0];
    for (int i = 1; i < node->keyc; ++i)
        if (node->keys[i] > max_key)
            max_key = node->keys[i];
    return max_key;
}

int get_min_key(node_t *node)
{
    int min_key = node->keys[0];
    for (int i = 1; i < node->keyc; ++i)
        if (node->keys[i] < min_key)
            min_key = node->keys[i];
    return min_key;
}

node_t *get_sibling(node_t *node)
{
    if (!node->parent)
        return NULL;

    return node->parent->left == node ? node->parent->right : node->parent->left;
}

stats_t get_stats(node_t *node)
{
    stats_t ret = {.nodes = 1, .leafc = {0}};
    if (is_leaf(node))
    {
        ret.leafc[node->keyc]++;
        return ret;
    }

    if (node->left)
        ret = add_stats(ret, get_stats(node->left));
    if (node->right)
        ret = add_stats(ret, get_stats(node->right));
    return ret;
}

void free_tree(node_t *node)
{
    if (node)
    {
        free_tree(node->left);
        free_tree(node->right);
        deallocate_memory((void **)&node);
    }
}
