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

#include "utils.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

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

void *map_file(const char *name, int flags, int *fileFD, unsigned int *fileSize)
{
    int fd = open(name, flags);
    if (fileFD) {
        *fileFD = fd;
    }
    if (fd < 0) {
        return NULL;
    }

    struct stat fileStats;
    fstat(fd, &fileStats);

    if (fileSize) {
        *fileSize = fileStats.st_size;
    }

    return mmap(NULL, fileStats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
}


const char *file_name(const char *path)
{
   const char *found_sep = strrchr(path, '/');
   if (found_sep) {
       return found_sep + 1;
   } else {
       return path;
   }
}
