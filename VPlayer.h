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
#ifndef __VPLAYERHP__
#define __VPLAYERHP__
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "VStopwatch.h"
#include "VCharacter.h"
#include "VLObjects.h"
#include "VPoints.h"
#include "VServer.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"
class Cworld;

class Cplayer : public Ccharacter {
private:
    unsigned int socketID;
    bool islogging;
    char *password;
    char *account;
    bool isonline;
public:
    int awayposition;
    Clogger logger;
    Cplayer(void);

    virtual ~Cplayer(void) {
    };

    virtual int load(FILE *configurationfile, Cobjectslist& objects, Cworld& world);
    int loadskeleton(const char *configurationfile, Cobjectslist& Ojects, Cworld& world, unsigned int characterID);
    virtual int save(FILE *configurationfile, Cobjectslist& objects);

    int setaccount(char *account);
    int setpassword(char *password);
    int delaccount(void);
    int delpassword(void);

    inline char *getaccount(void) {
        return account;
    };

    inline char *getpassword(void) {
        return password;
    };

    int pvrecv(Cserver& server, char **message, unsigned int sec, unsigned int usec);
    int pvsend(Cserver& server, const char *format, ...);
    int opvsend(Cserver& server, const char *format, ...);
    int spvsend(Cserver& server, const char *format, ...);

    int setlogging(bool islogging);

    inline bool setonline(void) {
        isonline = true;
        return isonline;
    }

    inline bool unsetonline(void) {
        isonline = false;
        return isonline;
    }

    inline bool getonline(void) {
        return isonline;
    }

    inline void setsocketID(unsigned int socketID) {
        this->socketID = socketID;
    };

    inline unsigned int getsocketID(void) {
        return socketID;
    };

    void unload(void);
};
#endif
