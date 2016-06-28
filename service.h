#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "unitmanager.h"

unit *service_new(const char *service_path);
int service_start(unit *u);

#endif
