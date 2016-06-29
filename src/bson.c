/***************************************************************************
 *   Copyright 2016 by Davide Bettio <davide@uninstall.it>                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "bson.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t read_uint32(const void *u)
{
    const char *b = (const char *) u;
    /* TODO: fix endianess issues here */
    return (b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24));
}

const void *bson_key_lookup(const char *key, const void *document, uint8_t *type)
{
    const char *docBytes = (const char *) document;
    uint32_t docLen = read_uint32(document);

    /* TODO: it would be nice to check len validity here */

    unsigned int offset = 4;
    while (offset + 1 < docLen) {
       uint8_t elementType = (uint8_t) docBytes[offset];
       int keyLen = strnlen(docBytes + offset + 1, docLen - offset);

       if (!strncmp(key, docBytes + offset + 1, docLen - offset)) {
           if (type) {
               *type = elementType;
           }
           return (void *) (docBytes + offset + 1 + keyLen + 1);
       }

       /* offset <- type (uint8_t) + key (const char *) + '\0' (char) */
       offset += 1 + keyLen + 1;

       switch (elementType) {
           case TYPE_STRING: {
               uint32_t stringLen = read_uint32(docBytes + offset);
               offset += stringLen + 4;
           }
       }
    }

    return NULL;
}

const char *bson_value_to_string(const void *valuePtr, uint8_t *len)
{
    const char *valueBytes = (const char *) valuePtr;
    uint32_t stringLen = read_uint32(valueBytes);

    if (len) {
        *len = stringLen - 1;
    }

    return valueBytes + 4;
}

int32_t bson_value_to_int32(const void *valuePtr)
{
    return read_uint32(valuePtr);
}

int bson_check_validity(const void *document, unsigned int fileSize)
{
    const char *docBytes = (const char *) document;
    uint32_t docLen = read_uint32(document);
    int offset;

    if (fileSize < 4 + 1 + 2 + 1) {
        return 0;
    }

    if (docLen > fileSize) {
        return 0;
    }

    offset = 4;
    switch (docBytes[offset]) {
       case TYPE_STRING:
       break;

       default:
           return 0;
    }

    return 1;
}
