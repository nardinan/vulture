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
#ifndef __VCHARACTERHP__
#define __VCHARACTERHP__
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
#include "VTime.h"
#include "VData.h"
#include "VLog.h"
class Ctile;

typedef struct characterslist {
    unsigned int characterID;
    characterdefinition character;
    struct characterslist *next;
    struct characterslist *back;
} characterslist;

typedef struct definitionslist {
    unsigned int definitionID;
    char *completename;
    char *category;
    char *keyname;
    struct definitionslist *next;
    struct definitionslist *back;
} definitionslist;

class Ccharacter {
protected:
    unsigned int characterID;
    characterdefinition kind;
    objectslist *objectsroot;
    objectslist *objectsthis;
    characterslist *charactersroot;
    definitionslist *definitionsroot;
    datainfos *birth;
    sexdefinition sex;
public:
    Ctile *position;
    Cpoints logics;
    Cstopwatch stopwatch;
    Cdescriptions descriptions;

    Ccharacter(void);

    virtual ~Ccharacter(void) {
    };

    virtual int load(FILE *configurationfile) {
        return 0;
    };
    int loadobjects(FILE *configurationfile, Cobjectslist& objects);
    int loadcharacters(FILE *configurationfile);

    virtual int save(FILE *configurationfile) {
        return 0;
    };
    int saveobjects(FILE *configurationfile, Cobjectslist& objects);
    int savecharacters(FILE *configurationfile);
    int savedefinitions(FILE *configurationfile);
    int addobject(Cobject *object);
    int delobject(unsigned int objectID);
    int delobject(const char *information, unsigned int position);
    objectslist *getobject(unsigned int objectID);
    objectslist *getobject(const char *information, unsigned int position);

    int getage(datainfos& present);

    inline datainfos *getbirth(void) {
        return birth;
    };

    int addcharacter(unsigned int characterID, characterdefinition character);
    int delcharacter(unsigned int characterID, characterdefinition character);
    characterslist *getcharacter(unsigned int characterID, characterdefinition character);

    int adddefinition(const char *definition, const char *category, const char *keyname, unsigned int definitionID);
    int deldefinition(unsigned int definitionID);
    int deldefinition(const char *completename);
    definitionslist *getdefinition(unsigned int definitionID);
    definitionslist *getdefinition(const char *completename);

    objectslist *nextvalue(void);
    objectslist *backvalue(void);

    inline void resetvalue(void) {
        objectsthis = NULL;
    };

    inline int getID(void) {
        return characterID;
    };

    inline sexdefinition getsex(void) {
        return sex;
    };

    inline characterdefinition getkind(void) {
        return kind;
    }

    inline void setID(unsigned int characterID) {
        this->characterID = characterID;
    };

    inline void setsex(sexdefinition sex) {
        this->sex = sex;
    }

    void unload(void);
};
#endif
