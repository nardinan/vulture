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
#ifndef __VWORLDHP__
#define __VWORLDHP__
#include <stdarg.h>
#include <string.h>
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
#include "VZone.h"
#include "VLog.h"

typedef struct zoneslist {
    Czone *zone;
    struct zoneslist *next;
    struct zoneslist *back;
} zoneslist;

class Cworld {
private:
    zoneslist *zonesroot;
    zoneslist *zonesthis;
public:
    Cdescriptions descriptions;

    inline Cworld(void) {
        zonesroot = NULL;
        zonesthis = NULL;
    };

    virtual ~Cworld(void) {
    };

    virtual int load(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters);
    int loadarches(FILE *configurationfile);
    virtual int save(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters);
    int savearches(FILE *configurationfile);

    int addzone(Czone *zone);
    int delzone(unsigned int zoneID);
    zoneslist *getzone(unsigned int zoneID);
    tileslist *gettile(tileinfos Position);

    inline zoneslist *getzonesroot(void) {
        return zonesroot;
    };

    inline tileslist *gettilesroot(void) {
        return zonesroot->zone->gettilesroot();
    };
    zoneslist *getzonelist(unsigned int zoneID);

    int delcharacters(void);

    zoneslist *nextvalue(void);
    zoneslist *backvalue(void);

    inline void resetvalue(void) {
        zonesthis = NULL;
    };

    int addarch(basicinfos *information, tileinfos from, tileinfos to, bool door, bool close, bool lock, unsigned int key, unsigned int length);
    int delarch(tileinfos tile, char *completename);
    int pvsend(Cserver& server, const char *format, ...);

    void unload(void);
};
#endif
