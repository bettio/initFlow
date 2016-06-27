#ifndef _BSON_H_
#define _BSON_H_

#include <stdint.h>

#define TYPE_DOUBLE 0x01
#define TYPE_STRING 0x02
#define TYPE_INT32 0x10

const void *bson_key_lookup(const char *key, const void *document, uint8_t *type);
const char *bson_value_to_string(const void *valuePtr, uint8_t *len);
int32_t bson_value_to_int32(const void *valuePtr);
void *map_file(const char *name, int flags, int *fileFD, unsigned int *fileSize);
int bson_check_validity(const void *document, unsigned int fileSize);

#endif
