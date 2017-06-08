/* PSYCHO GAMES(C) STUDIOS - 2007 www.psychogames.net
 * Project: Vulture(c)
 * Author : Andrea Nardinocchi
 * eMail  : andrea@nardinan.it
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __VSERVERHP__
#define __VSERVERHP__
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"
#ifdef _WIN32
#include <winsock.h>
#include <io.h>
#else
#define closesocket(s) close(s)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#endif

typedef struct socketslist {
    unsigned int socketID;
    int socket;
    char torcv[_DEFAULT_MAXD_LEN];
    struct socketslist *next;
    struct socketslist *back;
} socketslist;

typedef struct serverinfos {
    char *completename;
    char *largedescription;
    char *serverURL;
    char *adminname;
    char *adminmail;
    unsigned int port;
} serverinfos;

class Cserver {
private:
    int serversocket;
    unsigned int socketID;
    socketslist *socketsroot;
    sockaddr_in socketaddress;
#ifdef _WIN32
    WSADATA datasocket;
#endif
public:
    serverinfos information;

    Cserver(void);
    Cserver(FILE *configurationfile);

    inline Cserver(serverinfos information) {
        socketsroot = NULL;
        initializing(information);
    };

    virtual ~Cserver(void) {
    };

    int load(FILE *configurationfile);
    int save(FILE *configurationfile);
    int initializing(serverinfos information);
    int initializing(void);

    int search(unsigned int Sec, unsigned int USec);
    socketslist *getsocket(unsigned int socketID);

    int pvsend(unsigned int socketID, const char *format, ...);
    int pvrecv(unsigned int socketID, char **Message, unsigned int Sec, unsigned int USec);

    int unload(unsigned int socketID);
    void unload(void);
};
#endif
