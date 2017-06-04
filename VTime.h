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
#ifndef __VTIMEHP__
#define __VTIMEHP__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "VDescriptions.h"
#include "VDatabank.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct datainfos {
    unsigned int day;
    unsigned int month;
    unsigned int year;
} datainfos;

typedef struct timeinfos {
    unsigned int hour;
    unsigned int minutes;
} timeinfos;

class Ctime {
private:
    Cdatabank mname;
    Cdatabank msize;
    Cdatabank dname;
    Cdatabank dsize;

    long int seed;

    inline long int gettime(void) {
        return (long int) time(NULL);
    };
public:
    basicinfos *information;
    datainfos pdate;
    timeinfos ptime;
    Ctime(void);

    virtual ~Ctime(void) {
    };

    int load(FILE *configurationfile);
    int loadmonths(FILE *configurationfile);
    int loaddays(FILE *configurationfile);
    virtual int save(FILE *configurationfile);
    int savemonths(FILE *configurationfile);
    int savedays(FILE *configurationfile);

    inline void start(void) {
        seed = gettime();
    };
    int update(void);

    datalist *getday(void);
    datalist *getmonth(void);

    void unload(void);
};
#endif
