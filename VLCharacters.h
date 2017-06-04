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
#ifndef __VLCHARACTERSHP__
#define __VLCHARACTERSHP__
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "VCharacter.h"
#include "VStopwatch.h"
#include "VLObjects.h"
#include "VServer.h"
#include "VPoints.h"
#include "VPlayer.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VMob.h"
#include "VLog.h"

class Ccharacterslist {
public:
    playerslist *playersroot;
    mobslist *mobsroot;

    inline Ccharacterslist(void) {
        playersroot = NULL;
        mobsroot = NULL;
    };

    virtual ~Ccharacterslist(void) {
    };

    virtual int loadmobs(FILE *configurationfile, Cobjectslist& objects, Cworld& world);
    virtual int saveplayer(unsigned int characterID, const char *path, Cobjectslist& objects);
    virtual int saveplayer(const char *path, Cobjectslist& objects);
    virtual int savemobs(FILE *configurationfile, Cobjectslist& objects);

    int addplayer(Cplayer *player);
    int addplayer(FILE *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int socketID);
    int addplayer(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int socketID, unsigned int characterID);
    int delplayer(unsigned int characterID);
    playerslist *getplayer(unsigned int characterID);
    playerslist *getplayer(const char *completename, unsigned int position);
    playerslist *getplayer(const char *completename);
    playerslist *getaccount(const char *account);

    int pvsend(Cserver& server, const char *format, ...);
    int spvsend(Cserver& server, const char *format, ...);

    int addmob(Cmob *mob);
    int addmob(FILE *configurationfile, Cobjectslist& objects, Cworld& world);
    int addmob(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int characterID);
    int delmob(unsigned int characterID);
    mobslist *getmob(unsigned int characterID);
    mobslist *getmob(const char *completename, unsigned int position);
    mobslist *getmob(const char *completename);

    void unload(void);
};
#endif
