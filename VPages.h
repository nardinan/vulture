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
#ifndef __VPAGESHP__
#define __VPAGESHP__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct pageslist {
    char *completename;
    char *page;
    struct pageslist *next;
    struct pageslist *back;
} pageslist;

class Cpages {
private:
    pageslist *pagesroot;
public:
    unsigned int elements;

    Cpages(void);

    virtual ~Cpages(void) {
    };

    int load(FILE *configurationfile);
    int save(FILE *configurationfile);

    int addpage(const char *information, const char *page);
    int delpage(void);
    char *getpage(bool remove);

    void unload(void);
};
#endif
