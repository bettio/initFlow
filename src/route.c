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

#include "route.h"

#include "bson.h"
#include "utils.h"

#include <fcntl.h>
#include <sys/mount.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Route
{
    Unit parent_instance;
    const char *destination;
    const char *gateway;
    const char *genmask;
    const char *interface;
} Route;

Unit *route_new(const char *route_path)
{
    Route *new_route = malloc(sizeof(Route));
    if (!new_route) {
        return NULL;
    }

    unsigned int size;
    int fd;
    void *doc = map_file(route_path, O_RDONLY | O_CLOEXEC, &fd, &size);

    if (!doc && (fd >= 0)) {
        free(new_route);
        return NULL;
    }

    unit_constructor((Unit *) new_route, route_path, doc);
    new_route->parent_instance.type = UNIT_TYPE_ROUTE;

    uint8_t type;
    const void *value = bson_key_lookup("destination", doc, &type);
    new_route->destination = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("gateway", doc, &type);
    new_route->gateway = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("genmask", doc, &type);
    new_route->genmask = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("interface", doc, &type);
    new_route->interface = strdup(bson_value_to_string(value, NULL));

    munmap(doc, size);
    close(fd);

    return (Unit *) new_route;
}

int route_start(Unit *u)
{
    printf("Configuring network route: %s\n", u->name);

    Route *route_unit = (Route *) u;

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "init: %s: failed to open socket\n", u->name);
        return -1;
    }

    struct rtentry route;
    struct sockaddr_in sai;

    memset(&route, 0, sizeof(route));
    memset(&sai, 0, sizeof(sai));

    if (!inet_pton(0, route_unit->destination, &sai.sin_addr.s_addr)) {
        fprintf(stderr, "init: %s invalid destination address\n", u->name);
        return -1;
    }
    memcpy(&route.rt_dst, &sai, sizeof(struct sockaddr));
    if (!inet_pton(0, route_unit->gateway, &sai.sin_addr.s_addr)) {
        fprintf(stderr, "init: %s invalid gateway address\n", u->name);
        return -1;
    }
    memcpy(&route.rt_gateway, &sai, sizeof(struct sockaddr));
    if (!inet_pton(0, route_unit->genmask, &sai.sin_addr.s_addr)) {
        fprintf(stderr, "init: %s invalid genmask\n", u->name);
        return -1;
    }
    memcpy(&route.rt_genmask, &sai, sizeof(struct sockaddr));

    route.rt_dev = strdup(route_unit->interface);
    if (!route.rt_dev) {
        fprintf(stderr, "init: failed to allocate string copy");
        return -1;
    }
    int ioctl_ret = ioctl(sockfd, SIOCADDRT, &route);

    free(route.rt_dev);

    unit_set_status(u, UNIT_STATUS_RUNNING);

    return ioctl_ret;
}
