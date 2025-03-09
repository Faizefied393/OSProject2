#include <stdio.h>
#include "alloc.h"

void test_strategy(allocation_strategy strategy, const char *name) {
    set_allocation_strategy(strategy);
    printf("Testing %s strategy\n", name);

    int *arr = (int *)tumalloc(5 * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < 5; i++) arr[i] = i + 1;
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    tufree(arr);
}

int main() {
    test_strategy(FIRST_FIT, "First Fit");
    test_strategy(BEST_FIT, "Best Fit");
    test_strategy(WORST_FIT, "Worst Fit");
    test_strategy(NEXT_FIT, "Next Fit");
    return 0;
}
