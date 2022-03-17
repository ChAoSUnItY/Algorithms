
#include "myrandom.h"
#include "mytree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 10000
#define DEFAULT_LENGTH 16
#define DEFAULT_RANGE 1000

int main(int argc, char* argv[]) {
    int* random_list;
    int length = DEFAULT_LENGTH;
    int range = DEFAULT_RANGE;
    RBTree* t;
    tree_init(&t, 1);

    for (int i = 1; i < argc ; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            length = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-r") == 0) {
            range = atoi(argv[++i]);
        }
    }


    int true_length = 0;
    /*int a[10] = {122,288,268,458,647,363,328,272,284,702};*/
#ifdef DEBUG1
    int a[10] = {1,5,2,8,9,7,6,3,4,10};
    random_list = &a;
    true_length = 10;
#else
    true_length = random_list_gen(&random_list, (length > MAX_LENGTH) ? MAX_LENGTH : length, 0, range);
#endif

    for (int i = 0; i < true_length; i++) {
    // for (int i = 0; i < 10; i++) {
        // insert(&t, t, a[i]);
        insert(&t, t, random_list[i]);
        /*printf("==================\n");*/
        /*traversal(t);*/
        /*printf("==================\n");*/
    }

    traversal(t);

    delete(&t, t, 4);

    tree_free(t);
}


