
#ifndef MYTREE
#define MYTREE

typedef struct rbtree {
    struct rbtree* left;
    struct rbtree* right;
    struct rbtree* parent;
    int black_num;
    int data;
    int is_root;
} RBTree;

void insert(RBTree**, RBTree*, int);
void delete(RBTree**, RBTree*, int);
void after_delete(RBTree**, RBTree*);
void traversal(RBTree*);
void tree_init(RBTree**, int);
void tree_free(RBTree*);
void _rotation(RBTree**, RBTree*, RBTree*);
void _solve_red_collision(RBTree**, RBTree*);

#endif

