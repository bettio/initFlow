/**
 * @file ptrlist.h
 * @brief pointers list realated functions and definitions.
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

#ifndef _PTRLIST_H_
#define _PTRLIST_H_

struct _PtrList typedef PtrList;

/**
 * @brief instantiate a new PtrList object.
 *
 */
PtrList *ptr_list_new();

/**
 * @brief return pointer at index i.
 *
 * @description gets the point that is located at list index.
 * index must be valid otherwise the application may crash.
 *
 * @param list the list
 * @param index list index.
 * @return pointer at list index.
 */
void *ptr_list_at(const PtrList *list, int index);

/**
 * @brief set pointer at index i.
 *
 * @description set list item at the specified index.
 *
 * @param list the list that should be modified.
 * @param index list index.
 * @param value that should be set.
 */
void ptr_list_set(PtrList *list, int index, void *value);

/**
 * @brief append a pointer to the end of the PtrList.
 *
 * @description insert a pointer to the end of the list and return 0 if the operation
 * has been successfull, otherwise -1.
 *
 * @param list the list that we should append the pointer to it.
 * @param ptr the pointer that should be appended.
 * @return 0 on success, otherwise -1.
 */
int ptr_list_append(PtrList *list, void *ptr);

/**
 * @brief take a pointer from the end of the PtrList.
 *
 * @description remove last pointer from the end of the list and return it.
 *
 * @param list the list that we should remove the pointer from it.
 * @return the pointer that has been removed
 */
void *ptr_list_take_last(PtrList *list);

/**
 * @brief return the number of pointers.
 *
 * @description return the number of pointers inside the list.
 *
 * @param list the list that we should get the size from it.
 * @return the number of elements.
 */
int ptr_list_count(const PtrList *list);

#endif
