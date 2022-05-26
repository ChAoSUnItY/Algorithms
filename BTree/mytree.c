
#include "mytree.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RIGHT_SIBLING(n) n->parent->key_len > n->parent_child ? n->parent->children[n->parent_child + 1] : NULL
#define LEFT_SIBLING(n) n->parent_child > 0 ? n->parent->children[n->parent_child - 1] : NULL
#define LAST_KEY(n) n->keys[n->key_len - 1]
#define LAST_CHILD(n) n->children[n->children_len - 1]
#define FIRST_KEY(n) n->keys[0]
#define FIRST_CHILD(n) n->children[0]
#define SWAP(a, b, typ) typ tmp = a; a = b; b = tmp;
#define MAX_KEY(t) t->key_len == t->degree * 2 - 1
#define MID(d) d - 1

void _traversal(Node *);
int binary_search_insert_index(int*, int, int, int);

void key_spare(BTree *t, int index) {
    for (int k = t->key_len; k > index; k--)
         t->keys[k] = t->keys[k-1];
    t->key_len++;
}

void key_spare_multiple(BTree *t, int index, int count) {
    for (int k = t->key_len + count - 1; k > index + count - 1; k--)
        t->keys[k] = t->keys[k - count];
    t->key_len += count;
}

void key_unspare(BTree *t, int index) {
    for (int k = index; k < t->key_len -1; k++)
        t->keys[k] = t->keys[k+1];
    t->key_len--;
}

void child_spare(BTree *t, int index) {
    for (int k = t->children_len; k > index; k--)
         t->children[k] = t->children[k-1];
    t->children_len++;
}

void child_spare_multiple(BTree *t, int index, int count) {
    for (int k = t->key_len + count - 1; k > index + count - 1; k--)
        t->children[k] = t->children[k - count];
    t->children_len += count;
}

void child_unspare(BTree *t, int index) {
    for (int k = index; k < t->children_len -1; k++)
        t->children[k] = t->children[k+1];
    t->children_len--;
}

void insert(BTree **root, BTree *t, int data)
{
    if (t->key_len == 0) {
        t->keys[t->key_len++] = data;
        return;
    }
    int pos = binary_search_insert_index(t->keys, 0, t->key_len-1, data);

    if (t->is_leaf) {
        // append
        key_spare(t, pos);
        t->keys[pos] = data;
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

int binary_search_insert_index(int *arr, int start_pos, int end_pos, int data) {
    if (start_pos >= end_pos) return arr[start_pos] > data ? start_pos : start_pos + 1;
    
    int middle_pos = (end_pos + start_pos) / 2, middle_val = arr[middle_pos];
    
    if (middle_val > data) {
        return binary_search_insert_index(arr, start_pos, middle_pos - 1, data);
    } else if (middle_val < data) {
        return binary_search_insert_index(arr, middle_pos + 1, end_pos, data);
    } else {
        return middle_pos + 1;
    }
}

BTree* find_right_most(BTree **root, BTree *t)
{
    if (!t) {
        printf("Got NULL node when searching nearest node.");
        exit(-1);
    }

    if (!t->is_leaf) {
        // search down
        return find_right_most(root, t->children[t->children_len - 1]);
    } else {
        return t;
    }
}

void delete_node(BTree **root, BTree *t, int data)
{
    if (!t) return;

    int index = binary_search_insert_index(t->keys, 0, t->key_len, data);

    if (t->keys[index - 1] == data) {
        // deletion
        if (t->is_leaf) {
            // shift parent keys
            for (int k = index - 1; k < t->key_len - 1; k++)
                t->keys[k] = t->keys[k + 1];
            t->key_len--;
            resolve(root, t);
        } else {
            BTree *right_most_node = find_right_most(root, t->children[index - 1]);
            SWAP(t->keys[index - 1], LAST_KEY(right_most_node), int);
            right_most_node->key_len--;
            resolve(root, right_most_node);        
        }
    } else {
        if (t->is_leaf) {
            // No match
        } else {
            delete_node(root, t->children[index], data);
        }
    }
}

void resolve(BTree **root, BTree *t)
{
    if (t->key_len >= t->degree - 1)
        return;

    if (t->is_root) {
        borrow(root, t);
    } else {
        borrow(root, t);
    }
}

void borrow(BTree **root, BTree *t)
{
    BTree *to_be_move;

    if ((to_be_move = LEFT_SIBLING(t)) && to_be_move->key_len > t->degree - 1) {
        // move left last element
        int removed_data = LAST_KEY(to_be_move);
        to_be_move->key_len--;
        int append_data = t->parent->keys[to_be_move->parent_child];
        t->parent->keys[to_be_move->parent_child] = removed_data;
        key_spare(t, 0);
        t->keys[0] = append_data;
    } else if ((to_be_move = RIGHT_SIBLING(t)) && to_be_move->key_len > t->degree - 1) {
        // move right last element
        int removed_data = FIRST_KEY(to_be_move);
        key_unspare(to_be_move, 0);
        int append_data = t->parent->keys[t->parent_child];
        t->parent->keys[t->parent_child] = removed_data;
        t->keys[t->key_len++] = append_data;
    } else {
        // merge
        merge_child(root, t);
        resolve(root, t->parent);
    }
}

void merge_child(BTree **root, BTree *t)
{
    int borrowed_key;
    BTree *left, *right;

    if ((left = LEFT_SIBLING(t))) {
        borrowed_key = t->parent->keys[t->parent_child - 1];
        key_unspare(t->parent, t->parent_child - 1);
    
        right = t;
        key_spare_multiple(t, 0, left->key_len);

        for (int i = 0; i < left->key_len; i++)
            right->keys[i] = left->keys[i];
    
        for (int i = t->parent_child; i < t->parent->children_len; i++)
            t->parent->children[i]->parent_child--;
    
        child_unspare(t, left->parent_child);
        
        child_spare_multiple(right, 0, left->children_len);
        
        for (int i = 0; i < left->children_len; i++)
            right->children[i] = left->children[i];

        for (int i = 0; i < right->children_len; i++)
            right->children[i]->parent_child = i;

        tree_free(left, false);
    } else {
        borrowed_key = t->parent->keys[t->parent_child];
        key_unspare(t->parent, t->parent_child);
    
        left = t;
        right = RIGHT_SIBLING(t);

        for (int i = 0; i < left->key_len; i++)
            left->keys[t->key_len - 1 + i] = right->keys[i];

        for (int i = t->parent_child + 1; i <= t->children_len; i++)
            t->parent->children[i]->parent_child--;
        
        child_unspare(t, right->children_len);

        for (int i = 0; i < right->children_len; i++)
            left->children[left->children_len + i] = right->children[i];

        for (int i = 0; i < left->children_len; i++)
            left->children[i]->parent_child = i;
    
        tree_free(right, false);
    }
}

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

        if (node->next) {
            node = node->next;
            current_tree = node->value;
            current_tree->traversal_id = now_traversal_id++;
        } else break;
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

void tree_free(BTree *t, bool free_child)
{
    free(t->keys);
    
    if (free_child)
        for (int i = 0; i < t->degree; i++)
            tree_free(t->children[i], free_child);
    
    free(t->children);
    free(t);
};

void validate(BTree *root) {
    check_valid(root, true);
    printf("Validate pass\n");
}
