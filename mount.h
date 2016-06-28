#ifndef _MOUNT_H_
#define _MOUNT_H_

#include "unitmanager.h"

unit *mount_new(const char *mount_new);
int mount_start(unit *u);

#endif
