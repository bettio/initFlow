#include "unitmanager.h"

#include <stdlib.h>
#include <stdio.h>

unitmanager *unitmanager_init()
{
    return malloc(sizeof(unitmanager));
}

int unitmanager_loadservice(unitmanager *unitman, const char *unit_path)
{
    printf("loading unit: %s\n", unit_path);

    return 0;
}
