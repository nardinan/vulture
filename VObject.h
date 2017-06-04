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
#ifndef __VOBJECTHP__
#define __VOBJECTHP__
#include <stdlib.h>
#include <stdio.h>
#include "VDescriptions.h"
#include "VPoints.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"
class Cobjectslist;

class Cobject {
private:
    unsigned int objectID;
    objectslist *objectsroot;
    objectslist *objectsthis;
    sexdefinition sex;
public:
    Cpoints logics;
    Cdescriptions descriptions;

    Cobject(void);

    virtual ~Cobject(void) {
    };

    virtual int load(FILE *configurationfile);
    int loadskeleton(const char *configurationfile, unsigned int objectID);
    virtual int save(FILE *configurationfile);

    int copy(Cobject *object);

    int addobject(Cobject *object);
    int delobject(unsigned int objectID);
    int delobject(const char *information, unsigned int position);
    objectslist *getobject(unsigned int objectID);
    objectslist *getobject(const char *information, unsigned int position);
    void deldependences(void);

    objectslist *nextvalue(void);
    objectslist *backvalue(void);

    inline void resetvalue(void) {
        objectsthis = NULL;
    };

    inline unsigned int getID(void) {
        return objectID;
    };

    inline sexdefinition getsex(void) {
        return sex;
    };

    inline void setID(unsigned int objectID) {
        this->objectID = objectID;
    };

    inline void setsex(sexdefinition sex) {
        this->sex = sex;
    }

    void unload(void);
};
#endif
