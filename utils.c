#include "utils.h"

#include <string.h>

int string_ends_with(const char *string, const char *end)
{
    int stringlen = strlen(string);
    int endlen = strlen(end);

    if (stringlen < endlen) {
        return 0;
    }

    return !strcmp(string + (stringlen - endlen), end);
}

