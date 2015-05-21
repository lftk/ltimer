#include "timer.h"
#include "rbtree.h"

#include <stdlib.h>
#include <time.h>

static unsigned long timer_get_time();
static void timer_delete_nodes(rbtree_node_t *node, rbtree_node_t *sentinel);
static rbtree_node_t * timer_find_node(rbtree_node_t *node, rbtree_node_t *sentinel, unsigned long id);

struct timer {
    rbtree_t tree;
    rbtree_node_t sentinel;
};

typedef struct timer_node {
    rbtree_node_t node;
    unsigned long id;
} timer_node_t;

timer_t *
timer_create()
{
    timer_t *timer;
    
    timer = (timer_t *)malloc(sizeof(*timer));
    if (timer) {
        rbtree_init(&timer->tree, &timer->sentinel, rbtree_insert_value);
    }

    return timer;
}

void
timer_destroy(timer_t *timer)
{
    if (timer) {
        timer_delete_all(timer);
        free(timer);
    }
}

void
timer_expire(timer_t *timer, timer_execute_ptr execute, void *userp)
{
    unsigned long id;
    timer_node_t *timer_node;
    rbtree_node_t *root, *sentinel, *node;

    sentinel = &timer->sentinel;

    while (1) {
        root = timer->tree.root;
        if (root == sentinel) {
            break;
        }

        node = rbtree_min(root, sentinel);

        if (timer_get_time() < node->key) {
            break;
        }

        timer_node = (timer_node_t *)node;
        id = timer_node->id;

        rbtree_delete(&timer->tree, node);
        free(timer_node);

        execute(id, userp);
    }
}

void
timer_add(timer_t *timer, unsigned long id, unsigned long elapse)
{
    timer_node_t *timer_node;

    timer_node = (timer_node_t *)malloc(sizeof(*timer_node));
    if (timer_node) {
        timer_node->id = id;
        timer_node->node.key = timer_get_time() + elapse;
        rbtree_insert(&timer->tree, &timer_node->node);
    }
}

void 
timer_delete(timer_t *timer, unsigned long id)
{
    rbtree_node_t *node;

    node = timer_find_node(timer->tree.root, &timer->sentinel, id);;
    if (node != &timer->sentinel) {
        rbtree_delete(&timer->tree, node);
    }
}

void
timer_delete_all(timer_t *timer) 
{
    timer_delete_nodes(timer->tree.root, &timer->sentinel);
}

static unsigned long
timer_get_time()
{
    return (unsigned long)clock();
}

static void 
timer_delete_nodes(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    if (node != sentinel) {
        timer_delete_nodes(node->left, sentinel);
        timer_delete_nodes(node->right, sentinel);
        free((timer_node_t *)node);
    }
}

static rbtree_node_t * 
timer_find_node(rbtree_node_t *node, rbtree_node_t *sentinel, unsigned long id)
{
    if (node != sentinel) {
        if (((timer_node_t *)node)->id == id) {
            return node;
        } else {
            rbtree_node_t *left, *right;

            left = timer_find_node(node->left, sentinel, id);
            if (left != sentinel) {
                return left;
            }

            right = timer_find_node(node->right, sentinel, id);
            if (right != sentinel) {
                return right;
            }
        }
    }

    return sentinel;
}
