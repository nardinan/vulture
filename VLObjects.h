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
#ifndef __VLOBJECTSHP__
#define __VLOBJECTSHP__
#include <stdlib.h>
#include <stdio.h>
#include "VPoints.h"
#include "VObject.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

class Cobjectslist {
public:
    objectslist *objectsroot;

    inline Cobjectslist(void) {
        objectsroot = NULL;
    };

    virtual ~Cobjectslist(void) {
    };

    virtual int load(FILE *configurationfile);
    int loaddependences(FILE *configurationfile);
    virtual int save(FILE *configurationfile);
    int savedependences(FILE *configurationfile);

    objectslist * addobject(FILE *configurationfile);
    int addobject(const char *configurationfile, unsigned int objectID);
    objectslist *addobject(Cobject *object);
    int delobject(unsigned int objectID);
    objectslist *getobject(unsigned int objectID);
    objectslist *getobject(const char *information, unsigned int position);

    void unload(void);
};
#endif
