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
#ifndef __VDATABANKHP__
#define __VDATABANKHP__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct datalist {
    char *completename;
    int value;
    struct datalist *next;
    struct datalist *back;
} datalist;

class Cdatabank {
private:
    datalist *dataroot;
    datalist *datathis;
    unsigned int elements;
public:

    inline Cdatabank(void) {
        dataroot = NULL;
        datathis = NULL;
        elements = 0;
    };

    virtual ~Cdatabank(void) {
    };
    int addvalue(const char *key, int value);
    int setvalue(const char *key, int value);
    int delvalue(const char *key, int value);
    int delvalue(const char *key);
    int delvalue(int value);
    int getvalue(const char *key);
    char *getvalue(int value);
    int hasvalue(const char *key, int value);
    int hasvalue(const char *key);
    int hasvalue(int value);

    int load(FILE *configurationfile, const char *blockname);

    inline int load(FILE *configurationfile) {
        return load(configurationfile, "DATABANK");
    };

    int save(FILE *configurationfile, const char *blockname);

    inline int save(FILE *configurationfile) {
        return save(configurationfile, "DATABANK");
    };

    datalist *nextvalue(void);
    datalist *backvalue(void);

    inline void resetvalue(void) {
        datathis = NULL;
    };

    inline datalist *getdataroot(void) {
        return dataroot;
    };

    inline unsigned int getelements(void) {
        return elements;
    };
    bool equal(Cdatabank *element);
    void unload(void);
};
#endif
