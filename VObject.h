/* PSYCHO GAMES(C) STUDIOS - 2007/2008 www.psychogames.net
 * Project: Vulture(c) Engine
 * Author : Andrea "Psycho(c)" Nardinocchi
 * eMail  : psycho@psychogames.net
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
