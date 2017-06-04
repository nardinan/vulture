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
#ifndef __VPOINTSHP__
#define __VPOINTSHP__
#include <string.h>
#include "VDescriptions.h"
#include "VDatabank.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct pointslist {
    char *completename;
    Cdatabank *values;
    struct pointslist *next;
    struct pointslist *back;
} pointslist;

class Cpoints {
private:
    pointslist *pointsroot;
    pointslist *pointsthis;
public:
    basicinfos *information;
    Cpoints(void);

    virtual ~Cpoints(void) {
    };

    int load(FILE *configurationfile, const char *blockname);

    inline int load(FILE *configurationfile) {
        return load(configurationfile, "POINTS");
    };
    int loadcategory(FILE *configurationfile, const char *category);
    int save(FILE *configurationfile, const char *blockname);

    inline int save(FILE *configurationfile) {
        return save(configurationfile, "POINTS");
    };
    int savecategory(FILE *configurationfile, const char *category);

    int copy(Cpoints &points);

    int addcategory(const char *category);
    int delcategory(const char *category);
    int hascategory(const char *category);
    pointslist *getcategory(const char *category);

    int addvalue(const char *category, const char *key, int value);
    int setvalue(const char *category, const char *key, int value);
    int delvalue(const char *category, const char *key, int value);
    int delvalue(const char *category, const char *key);
    int delvalue(const char *category, int value);
    int hasvalue(const char *category, const char *key, int value);
    int hasvalue(const char *category, const char *key);
    int hasvalue(const char *category, int value);
    int getvalue(const char *category, const char *key);
    char *getvalue(const char *category, int value);

    pointslist *nextvalue(void);
    pointslist *backvalue(void);

    inline void resetvalue(void) {
        pointsthis = NULL;
    };

    bool equal(Cpoints* element);
    void unload(void);
};
#endif
