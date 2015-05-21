#ifndef RBTREE_H
#define RBTREE_H

typedef struct rbtree_node rbtree_node_t;

struct rbtree_node {
    unsigned color;  
    unsigned long key;
    rbtree_node_t *parent;
    rbtree_node_t *left, *right;
};

typedef void (* rbtree_insert_ptr)(rbtree_node_t *temp, rbtree_node_t *node, rbtree_node_t *sentinel);
    
typedef struct rbtree {
    rbtree_node_t *root;
    rbtree_node_t *sentinel;
    rbtree_insert_ptr insert;
} rbtree_t;

#define rbtree_init(tree, s, i)     \
{                                   \
    rbtree_sentinel_init(s);        \
    (tree)->root = s;               \
    (tree)->sentinel = s;           \
    (tree)->insert = i;             \
}

void rbtree_insert(rbtree_t *tree, rbtree_node_t *node);
void rbtree_delete(rbtree_t *tree, rbtree_node_t *node);
    
#define rbtree_red(node)                ((node)->color = 1)
#define rbtree_black(node)              ((node)->color = 0)
#define rbtree_is_red(node)             ((node)->color)
#define rbtree_is_black(node)           (!rbtree_is_red(node))
#define rbtree_copy_color(n1, n2)       ((n1)->color = (n2)->color)

#define rbtree_sentinel_init(node)      rbtree_black(node)

void rbtree_insert_value(rbtree_node_t *temp, rbtree_node_t *node, rbtree_node_t *sentinel);

static rbtree_node_t *
rbtree_min(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

#endif /* RBTREE_H */
