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

#include "interface.h"

#include "bson.h"
#include "utils.h"

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

typedef struct Interface
{
    Unit parent_instance;
    const char *network_interface;
    const char *ipv4_address;
} Interface;

Unit *interface_new(const char *interface_path)
{
    Interface *new_interface = malloc(sizeof(Interface));
    if (!new_interface) {
        return NULL;
    }

    unit_constructor((Unit *) new_interface, interface_path);
    new_interface->parent_instance.type = UNIT_TYPE_INTERFACE;

    unsigned int size;
    int fd;
    void *doc = map_file(interface_path, O_RDONLY | O_CLOEXEC, &fd, &size);

    if (!doc && (fd >= 0)) {
        free(new_interface);
        return NULL;
    }

    uint8_t type;
    const void *value = bson_key_lookup("interface", doc, &type);
    new_interface->network_interface = strdup(bson_value_to_string(value, NULL));

    value = bson_key_lookup("ipv4_address", doc, &type);
    new_interface->ipv4_address = strdup(bson_value_to_string(value, NULL));

    return (Unit *) new_interface;
}

int interface_start(Unit *u)
{
    Interface *iface = (Interface *) u;

    printf("Configuring network interface: %s", iface->network_interface);

    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "init: %s: failed to open socket\n", u->name);
        return -1;
    }
    struct ifreq ifr;
    struct sockaddr_in sai;

    memset(&ifr, 0, sizeof(ifr));
    memset(&sai, 0, sizeof(sai));

    strncpy(ifr.ifr_name, iface->network_interface, IFNAMSIZ);

    if (iface->ipv4_address) {
        if (!inet_pton(0, iface->ipv4_address, &sai.sin_addr.s_addr)) {
            printf("init: %s invalid IPv4 interface address\n", u->name);
            unit_set_status(u, UNIT_STATUS_FAILED);
            return -1;
        }
        sai.sin_family = PF_INET;
        sai.sin_port = 0;
        memcpy(&ifr.ifr_addr, &sai, sizeof(struct sockaddr));

        if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0) {
            unit_set_status(u, UNIT_STATUS_FAILED);
            perror("init: cannot configure interface: ");
            return -1;
        }
        if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
            unit_set_status(u, UNIT_STATUS_FAILED);
            perror("init: cannot configure interface: ");
            return -1;
        }

        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) {
            unit_set_status(u, UNIT_STATUS_FAILED);
            perror("init: cannot configure interface: ");
            return -1;
        }
    }
    unit_set_status(u, UNIT_STATUS_RUNNING);
    printf(" [ OK ]\n");

    return 0;
}
