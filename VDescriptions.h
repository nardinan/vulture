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
#ifndef __VDESCRIPTIONSHP__
#define __VDESCRIPTIONSHP__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct basicinfos {
    char completename[_DEFAULT_NAME_LEN];
    char smalldescription[_DEFAULT_MIND_LEN];
    char largedescription[_DEFAULT_MAXD_LEN];
} basicinfos;

typedef struct informationlist {
    unsigned int informationID;
    basicinfos *information;
    struct informationlist *next;
    struct informationlist *back;
} informationlist;

class Cdescriptions {
private:
    informationlist *informationroot;
public:

    inline Cdescriptions(void) {
        informationroot = NULL;
    };

    virtual ~Cdescriptions(void) {
    };

    int load(FILE *configurationfile, const char *blockname);

    inline int load(FILE *configurationfile) {
        return load(configurationfile, "DESCRIPTIONS");
    };
    int save(FILE *configurationfile, const char *blockname);

    inline int save(FILE *configurationfile) {
        return save(configurationfile, "DESCRIPTIONS");
    };

    int copy(Cdescriptions &descriptions);

    int adddescription(unsigned int informationID, const char *completename, const char *smalldescription, const char *largedescription);
    int deldescription(unsigned int informationID);
    informationlist *getdescription(unsigned int informationID);

    inline informationlist *getdescriptionsroot(void) {
        return informationroot;
    };

    char *completename(unsigned int informationID);
    char *smalldescription(unsigned int informationID);
    char *largedescription(unsigned int informationID);

    void unload(void);
};
#endif
