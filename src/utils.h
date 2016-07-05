/**
 * @file utils.h
 * @brief miscelaneous useful functions.
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

#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef ENABLE_DEBUG_MSGS
  #if ENABLE_DEBUG_MSGS == 1
    #define DEBUG_MSG printf
  #else
    #define DEBUG_MSG(...)
  #endif
#else
  #define DEBUG_MSG(...)
#endif

/**
 * @brief checks if a string ends with a certain suffix pointed by end parameter.
 *
 * @details return 1 if string end matches a certain suffix specified by end parameter.
 *
 * @param string that should be checked.
 * @param suffix that should be matched.
 * @return 1 if strings ends with end, 0 otherwise.
 */
int string_ends_with(const char *string, const char *end);

/**
 * @brief maps a file to memory.
 *
 * @details open a file given its path and it maps it to memory using mmap.
 * Opened file descriptor and file size will be reported to the caller.
 *
 * @param name file path.
 * @param flags optional flags that they will be passed to mmap.
 * @param fileFD it will contain the opened file descritor, if not NULL.
 * @param fileSize it will contain opened file size, if not NULL.
 *
 * @return a pointer to the newly allocated memory mapped area.
 */
void *map_file(const char *name, int flags, int *fileFD, unsigned int *fileSize);

const char *file_name(const char *path);

#endif
