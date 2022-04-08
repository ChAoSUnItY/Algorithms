
#include "mytree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tmp = 0;

void insert(RBTree **root, RBTree *t, int data)
{
    if (t->data == -1)
    {
        t->data = data;
        printf("%d\n", t->data);
        _solve_red_collision(root, t);
    }
    else if (t->data > data)
    {
        if (!t->left)
        {
            tree_init(&(t->left), 0);
            t->left->parent = t;
        }
        insert(root, t->left, data);
    }
    else
    {
        if (!t->right)
        {
            tree_init(&(t->right), 0);
            t->right->parent = t;
        }
        insert(root, t->right, data);
    }
};

void delete (RBTree **root, RBTree *t, int data)
{
#define swap(x, y) \
    tmp = x;       \
    x = y;         \
    y = tmp;

    if (t->data == -1)
    {
        return; // No data to be deleted
    }

    // binary search
    RBTree *matchedNode = t;
    while (matchedNode)
    {
        if (!matchedNode)
            return; // no match

        if (matchedNode->data > data)
        {
            matchedNode = matchedNode->left;
        }
        else if (matchedNode->data < data)
        {
            matchedNode = matchedNode->right;
        }
        else
            break;
    }

    // find most-center node
    RBTree *node = matchedNode;
    while (node)
    {
        if (node == matchedNode && node->right)
        {
            node = node->right;
        }
        else if (node->left)
        {
            node = node->left;
        }
        else
            break;
    }

    // swap nodes values
    swap(matchedNode->data, node->data);
    // swap(matchedNode->black_num, node->black_num);

    if (!node->parent)
    {
        t->data = -1;
        return;
    }

    // 1.
    if (node->black_num == 0)
    {
        if (node->parent->right == node)
        {
            node->parent->right = NULL;
        }
        else
        {
            node->parent->left = NULL;
        }
        tree_free(node);
        return;
        // 2.
    }
    else if (node->black_num == 1 && (node->right || node->left))
    {
        if (node->parent->right == node)
        {
            if (node->right)
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
                node->right->black_num = 1;
            }
            else
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
                node->left->black_num = 1;
            }
        }
        else
        {
            if (node->right)
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
                node->right->black_num = 1;
            }
            else
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
                node->left->black_num = 1;
            }
        }
        free(node);
        return;
    }

    judge_delete_handle(root, node);

    free(node);
}

void judge_delete_handle(RBTree **root, RBTree *t)
{
    RBTree *parent = t->parent, *sibling = get_sibling(t);

    if (!t->parent)
    {
        return;
    }
    else if (parent->black_num == 1 && sibling->black_num == 1)
    {
        BBB_handle(root, t);
    }
    else if (parent->black_num == 0 && sibling->black_num == 1)
    {
        BRB_handle(root, t);
    }
    else if (parent->black_num == 1 && sibling->black_num == 0)
    {
        BBR_handle(root, t);
    }
}

void BBB_handle(RBTree **root, RBTree *t)
{
    RBTree *parent = t->parent, *sibiling = get_sibling(t), *close_nephew = get_close_nephew(t), *remote_nephew = get_remote_nephew(t);
    if (close_nephew || remote_nephew)
    {
        if (!remote_nephew) {
            _rotation(root, close_nephew, sibiling);
            _rotation(root, close_nephew, parent);
            close_nephew->black_num = 1;
        } else {
            _rotation(root, sibiling, parent);
            remote_nephew->black_num = 1;
        }
    }
    else
    {
        sibiling->black_num = 0;
        judge_delete_handle(root, parent);
    }
}

void BRB_handle(RBTree **root, RBTree *t)
{
    RBTree *parent = t->parent, *sibiling = get_sibling(t), *close_nephew = get_close_nephew(t), *remote_nephew = get_remote_nephew(t);

    if (close_nephew || remote_nephew)
    {
        if (!remote_nephew) {
            _rotation(root, close_nephew, sibiling);
            _rotation(root, close_nephew, parent);
            close_nephew->black_num = 0;
            sibiling->black_num = 1;
            parent->black_num = 1;
        } else {
            _rotation(root, sibiling, parent);
            remote_nephew->black_num = 1;
            sibiling->black_num = 0;
            parent->black_num = 1;
        }
    }
    else
    {
        parent->black_num = 1;
        sibiling->black_num = 0;
    }
}

void BBR_handle(RBTree **root, RBTree *t)
{
    RBTree *parent = t->parent, *sibiling = get_sibling(t), *close_nephew = get_close_nephew(t), *remote_nephew = get_remote_nephew(t);

    _rotation(root, sibiling, parent);
    parent->black_num = 0;
    sibiling->black_num = 1;

    BRB_handle(root, t);
}

RBTree *get_close_nephew(RBTree *t)
{
    if (!t || !t->parent)
        return NULL;

    RBTree *parent = t->parent;
    RBTree *sibling = get_sibling(t);

    if (!sibling)
        return NULL;

    return parent->left == t ? sibling->left : sibling->right;
}

RBTree *get_remote_nephew(RBTree *t)
{
    if (!t || !t->parent)
        return NULL;

    RBTree *parent = t->parent;
    RBTree *sibling = get_sibling(t);

    if (!sibling)
        return NULL;

    return parent->left == t ? sibling->right : sibling->left;
}

RBTree *get_sibling(RBTree *t)
{
    if (!t || !t->parent)
        return NULL;

    RBTree *parent = t->parent;

    if (parent->left == t)
    {
        return parent->right;
    }
    else
    {
        return parent->left;
    }
}



void _solve_red_collision(RBTree **root, RBTree *t)
{
    if (t->is_root)
    {
        t->black_num = 1;
        return;
    }
    RBTree *parent = t->parent;
    if (t->black_num || parent->black_num)
    {
        return;
    }
    RBTree *grand_parent = parent->parent;
    if (t == parent->left && parent == grand_parent->left)
    {
        if (grand_parent->right && !grand_parent->right->black_num)
        { // R/R/B\R
            parent->black_num += 1;
            grand_parent->black_num = 0;
            grand_parent->right->black_num += 1;
            _solve_red_collision(root, grand_parent);
        }
        else
        { // R/R/B\B
            _rotation(root, parent, grand_parent);
            parent->black_num += 1;
            grand_parent->black_num = 0;
        }
    }
    else if (t == parent->left && parent == grand_parent->right)
    {
        if (grand_parent->left && !grand_parent->left->black_num)
        { // R/R\B/R
            parent->black_num += 1;
            grand_parent->black_num = 0;
            grand_parent->left->black_num += 1;
            _solve_red_collision(root, grand_parent);
        }
        else
        { // R/R\B/B
            _rotation(root, t, parent);
            _rotation(root, t, grand_parent);
            t->black_num += 1;
            grand_parent->black_num = 0;
        }
    }
    else if (t == parent->right && parent == grand_parent->left)
    {
        if (grand_parent->right && !grand_parent->right->black_num)
        { // R\R/B\R
            parent->black_num += 1;
            grand_parent->black_num = 0;
            grand_parent->right->black_num += 1;
            _solve_red_collision(root, grand_parent);
        }
        else
        { // R\R/B\B
            _rotation(root, t, parent);
            _rotation(root, t, grand_parent);
            t->black_num += 1;
            grand_parent->black_num = 0;
        }
    }
    else if (t == parent->right && parent == grand_parent->right)
    {
        if (grand_parent->left && !grand_parent->left->black_num)
        { // R\R\B/R
            parent->black_num += 1;
            grand_parent->black_num = 0;
            grand_parent->left->black_num += 1;
            _solve_red_collision(root, grand_parent);
        }
        else
        { // R\R\B/B
            _rotation(root, parent, grand_parent);
            parent->black_num += 1;
            grand_parent->black_num = 0;
        }
    }
}

void _rotation(RBTree **root, RBTree *up, RBTree *down)
{
    /*printf("%d %d\n", up->data, down->data);*/
    if (up == down->left)
    {
        up->parent = down->parent;
        if (!up->parent)
        {
        }
        else if (up->parent->left == down)
        {
            up->parent->left = up;
        }
        else
        {
            up->parent->right = up;
        }
        down->parent = up;
        down->left = up->right;
        if (up->right)
        {
            up->right->parent = down;
        }
        up->right = down;
        up->is_root = down->is_root;
        if (up->is_root)
        {
            (*root) = up;
        }
        down->is_root = 0;
    }
    else if (up == down->right)
    {
        up->parent = down->parent;
        if (!up->parent)
        {
        }
        else if (up->parent->left == down)
        {
            up->parent->left = up;
        }
        else
        {
            up->parent->right = up;
        }
        down->parent = up;
        down->right = up->left;
        if (up->left)
        {
            up->left->parent = down;
        }
        up->left = down;
        up->is_root = down->is_root;
        if (up->is_root)
        {
            (*root) = up;
        }
        down->is_root = 0;
    }
    else
    {
        printf("lalala\n");
        exit(-1);
    }
};

void traversal(RBTree *t)
{
    if (!t)
    {
        return;
    }
    traversal(t->left);
    printf("%d %d %d %d %d\n",
           t->parent ? t->parent->data : -1,
           t->data,
           t->black_num,
           t->left ? t->left->data : -1,
           t->right ? t->right->data : -1);
    traversal(t->right);
};

void tree_init(RBTree **tp, int is_root)
{
    *tp = malloc(sizeof(RBTree));
    RBTree *t = *tp;
    t->is_root = is_root;
    t->parent = t->right = t->left = NULL;
    t->data = -1;
    if (is_root)
    {
        t->black_num = 1;
    }
    else
    {
        t->black_num = 0;
    }
};

void tree_free(RBTree *t)
{
    if (!t)
    {
        return;
    }
    tree_free(t->left);
    tree_free(t->right);
    free(t);
};
