/**
 * @file bson.h
 * @brief a set of useful functions to read BSON documents
 */

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

/**
 * @brief lookup a certain key in a BSON document.
 *
 * @details finds a pointer to a value in a BSON object using a certain property key.
 *
 * @param key the property key used to perform the lookup.
 * @param document a pointer to a BSON document/object.
 * @param found value type (e.g. double, string, int32, etc...).
 *
 * @return a pointer to the value fond inside the document/object.
 */
const void *bson_key_lookup(const char *key, const void *document, uint8_t *type);

/**
 * @brief obtains the C string pointer to the string which valuePtr points to.
 *
 * @details this function should be used to get the C string that valuePtr points to.
 * If valuePtr doesn't point to a C string a NULL value will be returned.
 * If len parameter is not NULL it will be updated with the lenght of the returned
 * string.
 * This function doesn't return a copy, so the validity of that pointer has the same
 * lifecycle of the BSON document.
 *
 * @param valuePtr the pointer to the value
 * @param len the optional pointer to the variable that it may contain string len.
 *
 * @return a pointer to the C string that valuePtr points to, NULL if valuePtr doesn't
 * point to a string
 */
const char *bson_value_to_string(const void *valuePtr, uint8_t *len);

/**
 * @brief obtains the C string pointer to the string which valuePtr points to.
 *
 * @details this function should be used to get the C string that valuePtr points to.
 * If valuePtr doesn't point to a valid C string the function will return an invalid value.
 * If len parameter is not NULL it will be updated with the lenght of the returned
 * string.
 * This function doesn't return a copy, so the validity of that pointer has the same
 * lifecycle of the BSON document.
 *
 * @param valuePtr the pointer to the value.
 * @param len the optional pointer to the variable that it may contain string len.
 *
 * @return a pointer to the C string that valuePtr points to.
 */
int32_t bson_value_to_int32(const void *valuePtr);

/*
 * @brief performs a quick validation of the BSON document pointed by document
 *
 * @details this function should be used to check if document is a valid BSON document.
 *
 * @param document the BSON document object that should be checked.
 * @param fileSize the size of the BSON document.
 * @returns 1 if document is likely a valid BSON document, 0 otherwise.
 */
int bson_check_validity(const void *document, unsigned int fileSize);

#endif
