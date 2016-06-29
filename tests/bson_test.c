#include "../src/bson.h"
#include "../src/utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int fd;
    unsigned int size;
    void *doc = map_file("bsonfile0.bson", O_RDONLY | O_CLOEXEC, &fd, &size);
    assert(fd > 0);
    assert(doc != NULL);

    assert(bson_check_validity(doc, size) == 1);

    uint8_t slen;
    uint8_t type;
    const void *value = bson_key_lookup("interface", doc, &type);
    assert(type == TYPE_STRING);
    assert(value != NULL);
    assert(strcmp(bson_value_to_string(value, &slen), "eth0") == 0);
    assert(slen == 4);

    value = bson_key_lookup("doesnotexist", doc, &type);
    assert(value == NULL);

    return EXIT_SUCCESS;
}
