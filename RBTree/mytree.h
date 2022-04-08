
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
void judge_delete_handle(RBTree **root, RBTree *t);
void BBB_handle(RBTree **root, RBTree *t);
void BRB_handle(RBTree **root, RBTree *t);
void BBR_handle(RBTree **root, RBTree *t);
RBTree *get_close_nephew(RBTree *t);
RBTree *get_remote_nephew(RBTree *t);
RBTree *get_sibling(RBTree *t);
void traversal(RBTree*);
void tree_init(RBTree**, int);
void tree_free(RBTree*);
void _rotation(RBTree**, RBTree*, RBTree*);
void _solve_red_collision(RBTree**, RBTree*);

#endif

