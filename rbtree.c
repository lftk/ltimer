#include "rbtree.h"

#include <stdlib.h>

static void rbtree_left_rotate(rbtree_node_t **root, 
    rbtree_node_t *sentinel, rbtree_node_t *node);
static void rbtree_right_rotate(rbtree_node_t **root, 
    rbtree_node_t *sentinel, rbtree_node_t *node);

void 
rbtree_insert(rbtree_t *tree, rbtree_node_t *node) 
{
    rbtree_node_t **root, *temp, *sentinel;
  
    root = &tree->root;
    sentinel = tree->sentinel;
  
    if (*root == sentinel) {
        node->parent = NULL;    
        node->left = sentinel;
        node->right = sentinel;
        rbtree_black(node);
        *root = node;

        return;
    }
  
    tree->insert(*root, node, sentinel);
  
    while (node != *root && rbtree_is_red(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            if (rbtree_is_red(temp)) {
                rbtree_black(temp);
                rbtree_black(node->parent);
                rbtree_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rbtree_left_rotate(root, sentinel, node);
                }

                rbtree_black(node->parent);
                rbtree_red(node->parent->parent);
                rbtree_right_rotate(root, sentinel, node->parent->parent);
            }
        } else {
            temp = node->parent->parent->left;

            if (rbtree_is_red(temp)) {
                rbtree_black(temp);
                rbtree_black(node->parent);
                rbtree_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbtree_right_rotate(root, sentinel, node);
                }

                rbtree_black(node->parent);
                rbtree_red(node->parent->parent);
                rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    rbtree_black(*root);
}

void 
rbtree_delete(rbtree_t *tree, rbtree_node_t *node) 
{
    short red;
    rbtree_node_t **root, *sentinel, *subst, *temp, *w;

    root = &tree->root;
    sentinel = tree->sentinel;

    if (node->left == sentinel) {
        temp = node->right;
        subst = node;
    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;
    } else {
        subst = rbtree_min(node->right, sentinel);
        
        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }

    if (subst == *root) {
        *root = temp;
        rbtree_black(temp);

        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        return;
    }

    red = rbtree_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;
    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {
        temp->parent = subst->parent;
    } else {
        if (subst->parent == node) {
            temp->parent = subst;
        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        rbtree_copy_color(subst, node);

        if (node == *root) {
            *root = subst;
        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        } else {
            subst->right->parent = subst;
        }
    }

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if (red) {
        return;
    }

    while (temp != *root && rbtree_is_black(temp)) {
        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (rbtree_is_red(w)) {
                rbtree_black(w);
                rbtree_red(temp->parent);
                rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            if (rbtree_is_black(w->left) && rbtree_is_black(w->right)) {
                rbtree_red(w);
                temp = temp->parent;
            } else {
                if (rbtree_is_black(w->right)) {
                    rbtree_black(w->left);
                    rbtree_red(w);
                    rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                rbtree_copy_color(w, temp->parent);
                rbtree_black(temp->parent);
                rbtree_black(w->right);
                rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        } else {
            w = temp->parent->left;

            if (rbtree_is_red(w)) {
                rbtree_black(w);
                rbtree_red(temp->parent);
                rbtree_right_rotate(root, sentinel, temp->parent);
                temp = temp->parent->left;
            }

            if (rbtree_is_black(w->left) && rbtree_is_black(w->right)) {
                rbtree_red(w);
                temp = temp->parent;
            } else {
                if (rbtree_is_black(w->left)) {
                    rbtree_black(w->right);
                    rbtree_red(w);
                    rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                rbtree_copy_color(w, temp->parent);
                rbtree_black(temp->parent);
                rbtree_black(w->left);
                rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    rbtree_black(temp);
}

void 
rbtree_insert_value(rbtree_node_t *temp, rbtree_node_t *node, rbtree_node_t *sentinel)
{
    rbtree_node_t **p;

    while (1) {
        p = (node->key < temp->key) ? &temp->left : &temp->right;
        
        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    rbtree_red(node);
}

static void
rbtree_left_rotate(rbtree_node_t **root, rbtree_node_t *sentinel, rbtree_node_t *node)
{
    rbtree_node_t *temp;

    temp = node->right;
    node->right = temp->left;

    if (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->left) {
        node->parent->left = temp;
    } else {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

static void
rbtree_right_rotate(rbtree_node_t **root, rbtree_node_t *sentinel, rbtree_node_t *node)
{
    rbtree_node_t *temp;

    temp = node->left;
    node->left = temp->right;

    if (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;
    } else if (node == node->parent->right) {
        node->parent->right = temp;
    } else {
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
}
