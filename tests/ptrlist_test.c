#include "../src/ptrlist.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    PtrList *l = ptr_list_new();
    assert(l != NULL);
    assert(ptr_list_count(l) == 0);

    assert(ptr_list_append(l, (void *) 1) == 0);
    assert(ptr_list_count(l) == 1);
    assert(ptr_list_at(l, 0) == (void *) 1);

    return EXIT_SUCCESS;
}
