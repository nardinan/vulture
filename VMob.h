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
#ifndef __VMOBHP__
#define __VMOBHP__
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

typedef struct argumentslist {
    char *completename;
    Cdatabank *keywords;
    char *response;
    struct argumentslist *next;
    struct argumentslist *back;
} argumentslist;

class Cmob : public Ccharacter {
private:
    argumentslist *argumentsroot;
public:
    Cpoints AI;
    Cmob(void);

    virtual ~Cmob(void) {
    };

    virtual int load(FILE *configurationfile, Cobjectslist& objects, Cworld& world);
    int loadskeleton(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int characterID);
    int loadarguments(FILE *configurationfile);
    virtual int save(FILE *configurationfile, Cobjectslist& objects);
    int savearguments(FILE *configurationfile);

    int addargument(const char *information);
    int addresponse(const char *information, const char *response);
    int delargument(const char *information);
    argumentslist *getargument(const char *information);

    void unload(void);
};
#endif
