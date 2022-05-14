
#include "mytree.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY(t) t->key_len == t->degree * 2 - 1
#define MID(d) d - 1

void _traversal(Node *);
int binary_search(int*, int, int, int);

void insert(BTree **root, BTree *t, int data)
{
    if (t->key_len == 0) {
        t->keys[t->key_len++] = data;
        return;
    }
    int pos = binary_search(t->keys, 0, t->key_len-1, data);

    if (t->is_leaf) {
        // append
        for (int k = t->key_len; k > pos; k--)
            t->keys[k] = t->keys[k-1];
        t->keys[pos] = data;
        t->key_len++;
    } else {
        // search children node
        insert(root, t->children[pos], data);
    }

    if (MAX_KEY(t)) {
        // Split
        split_child(root, t);
    }
};

void split_child(BTree **root, BTree *t) {
    if (t->is_root) {
        // Root split
        BTree *new_root, *right;
        tree_init(&new_root, t->degree);
        tree_init(&right, t->degree);
        new_root->keys[0] = t->keys[MID(t->degree)];
        new_root->key_len++;
        new_root->children[0] = t;
        new_root->children[1] = right;
        new_root->children_len = 2;
        new_root->is_leaf = false;
        *root = new_root;
        t->parent_child = 0;
        t->is_root = false;
        t->parent = new_root;
        for (int i = MID(t->degree) + 1, j = 0; i < t->key_len ; i++, j++)
            right->keys[j] = t->keys[i];
        t->key_len = t->degree - 1;
        right->parent_child = 1;
        right->is_root = false;
        right->parent = new_root;
        right->key_len = t->degree - 1;
    } else if (t->is_leaf) {
        // Leaf split
        BTree* right;
        tree_init(&right, t->degree);
        int mid_val = t->keys[MID(t->degree)];
        for (int i = MID(t->degree) + 1, j = 0; i < t->key_len ; i++, j++)
            right->keys[j] = t->keys[i];
        right->parent = t->parent;
        right->is_root = false;
        right->key_len = t->degree - 1;
        right->parent_child = t->parent_child + 1;
        // shift parent keys
        for (int k = t->key_len; k > t->parent_child; k--)
            t->parent->keys[k] = t->parent->keys[k-1];
        // move mid_val to parent->key[parent_child]
        t->parent->keys[t->parent_child] = mid_val;
        t->key_len = t->degree - 1;
        // shift parent_child (increment)
        for (int k = t->children_len; k > t->parent_child + 1; k--) {
            t->parent->children[k] = t->parent->children[k-1];
            t->parent->children[k]->parent_child++;
        }
        t->parent->children[t->parent_child + 1] = right;

        if (MAX_KEY(t->parent)) {
            // Split
            split_child(root, t->parent);
        }
    }
}

int binary_search(int *arr, int start_pos, int end_pos, int data) {
    if (start_pos >= end_pos) return arr[start_pos] > data ? start_pos : start_pos + 1;
    
    int middle_pos = (end_pos + start_pos) / 2, middle_val = arr[middle_pos];
    
    if (middle_val > data) {
        return binary_search(arr, start_pos, middle_pos - 1, data);
    } else if (middle_val < data) {
        return binary_search(arr, middle_pos + 1, end_pos, data);
    } else {
        return middle_pos + 1;
    }
}

void delete (BTree **root, BTree *t, int data)
{
};

void traversal(BTree *t)
{
    if (!t) return;

    Node *start = new_node(t), *current = start;

    _traversal(start);

    while (current) {
        print_tree(current->value);
        current = current->next;
    }

    free_node(start);
};

void _traversal(Node *node) {
    int now_traversal_id = 0;
    BTree *current_tree = node->value;
    current_tree->traversal_id = now_traversal_id++;
    Node *tail = node;

    while (current_tree) {
        for (int i = 0; i < current_tree->children_len; i++) {
            tail = append_node(tail, current_tree->children[i]);
        }

        node = node->next;
        current_tree = node->value;
        current_tree->traversal_id = now_traversal_id++;
    }
}

void print_tree(BTree* tree) {
    #define B2S(b) b ? "true" : "false"

    if (tree->parent) {
        printf("parent id: #[%d]", tree->parent->traversal_id);
    } else {
        printf("parent id: #[?]");
    }

    printf("id: #[%d], key len: %d, children len: %d, is root: %s, is leaf: %s, degree: %d, parent child: %d\n",
        tree->traversal_id,
        tree->key_len,
        tree->children_len,
        B2S(tree->is_root),
        B2S(tree->is_leaf),
        tree->degree,
        tree->parent_child);
    printf("id: %d -> key list: ", tree->traversal_id);
    for (int i = 0; i < tree->key_len; i++)
        printf("[%d] ", tree->keys[i]);
    printf("\nid: %d -> children list: ", tree->traversal_id);
    for (int i = 0; i < tree->children_len; i++)
        printf("#[%d] ", tree->children[i]->traversal_id);
    printf("\n=======================\n");

    #undef B2S
}

void tree_init(BTree **tp, int degree)
{
    *tp = calloc(1, sizeof(BTree));

    BTree *t = *tp;
    t->keys = calloc(2 * degree - 1, sizeof(int));
    t->children = calloc(2 * degree, sizeof(BTree*));
    t->is_root = t->is_leaf = true;
    t->degree = degree;
    t->parent_child = -1;
    t->traversal_id = -1;
};

void tree_free(BTree *t)
{
    free(t->keys);
    
    for (int i = 0; i < t->degree; i++)
        tree_free(t->children[i]);
    
    free(t->children);
    free(t);
};

void validate(BTree *root) {
    check_valid(root, true);
    printf("Validate pass\n");
}
