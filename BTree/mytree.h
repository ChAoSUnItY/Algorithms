#ifndef MYTREE
#define MYTREE

#include <stdbool.h>

typedef struct btree {
    int key_len;
    int *keys;
    int children_len;
    struct btree **children;
    bool is_root, is_leaf;
    int degree;
    struct btree *parent;
    int parent_child;
    int traversal_id;
} BTree;


void insert(BTree**, BTree*, int);
void split_child(BTree**, BTree*);
void delete(BTree**, BTree*, int);
void traversal(BTree*);
void print_tree(BTree*);
void tree_init(BTree**, int);
void tree_free(BTree*);
void validate(BTree*);

#endif

