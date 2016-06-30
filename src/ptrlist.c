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

#include "ptrlist.h"

#include <stdlib.h>

struct _PtrList
{
    void **array;
    int capacity;
    int count;
};

PtrList *ptr_list_new()
{
    PtrList *list = malloc(sizeof(PtrList));
    if (!list) {
        return NULL;
    }

    list->count = 0;
    list->capacity = 8;
    list->array = malloc(sizeof(void *) * list->capacity);
    if (!list->array) {
        free(list);
        return NULL;
    }

    return list;
}

void *ptr_list_at(const PtrList *list, int index)
{
    return list->array[index];
}

void ptr_list_set(PtrList *list, int index, void *value)
{
    list->array[index] = value;
}

int ptr_list_append(PtrList *list, void *ptr)
{
    int count = list->count;

    if (count == list->capacity) {
        void *new_array = realloc(list->array, sizeof(void *) * list->capacity * 2);
        if (!new_array) {
            return -1;
        }
        list->array = new_array;
        list->capacity *= 2;
    }

    list->array[count] = ptr;
    list->count++;

    return 0;
}

void *ptr_list_take_last(PtrList *list)
{
    list->count--;

    return list->array[list->count];
}

int ptr_list_count(const PtrList *list)
{
    return list->count;
}
