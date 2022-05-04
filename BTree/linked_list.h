#include <stdlib.h>
#include "mytree.h"

typedef struct node {
    void *value;
    struct node *next;
} Node;

Node* new_node(void *value) {
    Node *node = (Node*) calloc(1, sizeof(Node));
    node->value = value;
    return node;
}

Node* append_node(Node *node, void *value) {
    Node *new_node = (Node*) calloc(1, sizeof(Node));
    new_node->value = value;
    node->next = new_node;
    return new_node;
}

void free_node(Node *node) {
    while (node) {
        Node *prev = node;
        node = node->next;
        free(prev);
    }
}