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
#ifndef __VTILEHP__
#define __VTILEHP__
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "VDescriptions.h"
#include "VLCharacters.h"
#include "VStopwatch.h"
#include "VLObjects.h"
#include "VPoints.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct archeslist {
    Ctile *tile;
    basicinfos *information;
    bool door;
    bool close;
    bool lock;
    unsigned int key;
    unsigned int length;
    struct archeslist *next;
    struct archeslist *back;
} archeslist;

class Ctile {
private:
    unsigned int tileID;
    unsigned int zoneID;
    archeslist *archesroot;
    archeslist *archesthis;
    mobslist *mobsroot;
    playerslist *playersroot;
    objectslist *objectsroot;
public:
    Cpoints logics;
    Cdescriptions descriptions;

    Ctile(void);

    virtual ~Ctile(void) {
    };

    virtual int load(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters);
    int loadobjects(FILE *configurationfile, Cobjectslist& objects);
    virtual int save(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters);
    int saveobjects(FILE *configurationfile, Cobjectslist& objects);
    int savearches(FILE *configurationfile);

    inline int addarch(basicinfos *information, Ctile *tile) {
        return addarch(information, tile, false, false, false, 0, 1);
    }
    int addarch(basicinfos *information, Ctile *tile, bool door, bool close, bool lock, unsigned int key, unsigned int length);
    int delarch(const char *information);
    archeslist *getarch(const char *information);

    inline archeslist *getarchesroot(void) {
        return archesroot;
    };

    archeslist *nextvalue(void);
    archeslist *backvalue(void);

    inline void resetvalue(void) {
        archesthis = NULL;
    };

    int addplayer(Cplayer *player);
    int delplayer(unsigned int characterID);
    int delplayer(const char *completename, unsigned int position);
    int delplayers(void);
    int getplayers(void);
    playerslist *getplayer(unsigned int characterID);
    playerslist *getplayer(const char *completename);
    playerslist *getplayer(const char *completename, unsigned int position);

    inline playerslist *getplayersroot(void) {
        return playersroot;
    };

    inline bool players(void) {
        return (playersroot) ? true : false;
    };

    int addmob(Cmob *mob);
    int delmob(unsigned int characterID);
    int delmob(const char *completename, unsigned int position);
    int delmobs(void);
    mobslist *getmob(unsigned int characterID);
    mobslist *getmob(const char *completename, unsigned int position);

    inline mobslist *getmobsroot(void) {
        return mobsroot;
    };

    int addobject(Cobject *object);
    int delobject(unsigned int objectID);
    int delobject(const char *completename, unsigned int position);
    objectslist *getobject(unsigned int objectID);
    objectslist *getobject(const char *completename, unsigned int position);

    inline objectslist *getobjectsroot(void) {
        return objectsroot;
    };

    int cpvsend(unsigned int distance, Ctile *node);
    int sppvsend(Cserver& server, unsigned int distance, Ctile *node, const char *format, va_list args);
    int pvsend(Cserver& server, unsigned int distance, bool clean, const char *format, ...);
    int pvsend(Cserver& server, const char *format, ...);
    int spspvsend(Cserver& server, unsigned int distance, Ctile *node, const char *format, va_list args);
    int spvsend(Cserver& server, unsigned int distance, bool clean, const char *format, ...);
    int spvsend(Cserver& server, const char *format, ...);

    char *randomize(void);

    inline unsigned int getID(void) {
        return tileID;
    };

    inline unsigned int getzoneID(void) {
        return zoneID;
    };

    inline void setID(unsigned int tileID) {
        this->tileID = tileID;
    };

    inline void setzoneID(unsigned int zoneID) {
        this->zoneID = zoneID;
    };

    void unload(void);
};
#endif
