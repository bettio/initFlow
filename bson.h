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
