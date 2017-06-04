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
#ifndef __PVGAMEMAPHP__
#define __PVGAMEMAPHP__
#define _PVMAPD 11
#include "PVCommon.h"
#include "Vulture.h"
extern struct shash directions[];

class Cgmap {
public:
    Cworld gamemap;

    inline Cgmap(void) {
    };

    Ctile *get(tileinfos position);
    Ctile *get(char *name, Czone *zone, int hour);
    Czone *get(char *name);

    inline char *getsimplename(Ctile *tile, int hour) {
        return tile->descriptions.completename(hour);
    }

    inline char *getdescription(Ctile *tile, int hour) {
        return tile->descriptions.largedescription(hour);
    }

    char *getlook(Ctile *tile, int hour);

    inline void unload(void) {
        gamemap.unload();
    };
};
#endif
