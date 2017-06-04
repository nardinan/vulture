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
#ifndef __VZONEHP__
#define __VZONEHP__
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
#include "VTile.h"
#include "VData.h"
#include "VLog.h"

typedef struct tileslist {
    Ctile *tile;
    struct tileslist *next;
    struct tileslist *back;
} tileslist;

class Czone {
private:
    unsigned int zoneID;
    tileslist *tilesroot;
    tileslist *tilesthis;
public:
    Cpoints logics;
    Cdescriptions descriptions;

    inline Czone(void) {
        tilesroot = NULL;
        tilesthis = NULL;
    };

    virtual ~Czone(void) {
    };

    virtual int load(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters);
    virtual int save(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters);
    int savearches(FILE *configurationfile);

    int addtile(Ctile *tile);
    int deltile(unsigned int TileID);
    tileslist *gettile(unsigned int tileID);

    inline tileslist *gettilesroot(void) {
        return tilesroot;
    };

    tileslist *nextvalue(void);
    tileslist *backvalue(void);

    inline void resetvalue(void) {
        tilesthis = NULL;
    };

    int delcharacters(void);

    inline int addarch(basicinfos *information, unsigned int from, unsigned int to) {
        return addarch(information, from, to, false, false, false, 0, 1);
    }
    int addarch(basicinfos *information, unsigned int from, unsigned int to, bool door, bool close, bool lock, unsigned int key, unsigned int length);
    int addarch(basicinfos *information, Ctile *from, Ctile *to, bool door, bool close, bool lock, unsigned int key, unsigned int length);
    int delarch(unsigned int tileID, const char *information);

    int pvsend(Cserver& server, const char *format, ...);
    int spvsend(Cserver& server, const char *format, ...);

    inline unsigned int getID(void) {
        return zoneID;
    };

    inline void setID(unsigned int zoneID) {
        this->zoneID = zoneID;
    };

    void unload(void);
};
#endif
