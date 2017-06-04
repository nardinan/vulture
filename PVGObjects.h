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
#ifndef __PVGAMEOBJECTSHP__
#define __PVGAMEOBJECTSHP__
#include "PVCommon.h"
#include "Vulture.h"
extern struct shash objepositions[];

class Cgobjects {
public:
    Cobjectslist gameobjects;

    inline Cgobjects(void) {
    };

    Cobject *get(char *name, Cplayer *player);
    Cobject *get(char *name, Cobject *object);
    Cobject *get(char *name, Ctile *tile);
    Cobject *get(char *name, Cmob *mob);

    Cobject *getweapon(char *position, Ccharacter *character);
    Cobject *getweapon(char *position, Cplayer *player);
    Cobject *getweapon(char *position, Cmob *mob);
    Cobject *getobject(char *position, Cplayer *player);
    Cobject *getobject(char *position, Cmob *mob);

    int getweight(Cplayer *player);
    int getweight(Cobject *object);
    int getweight(Ctile *tile);
    int getweight(Cmob *mob);

    inline int remove(Cobject *object) {
        return gameobjects.delobject(object->getID());
    }

    inline Cobject *build(Cobject *object) {
        return gameobjects.addobject(object)->object;
    }
    Cobject *build(char *name);
    Cobject *duplicate(Cobject *object);

    char *getposition(Cobject *object);
    char *getcompletename(Cobject *object);
    char *getsimplename(Cobject *object);
    char *getadjective(Cobject *object);
    char *getdescription(Cobject *object);

    char *getinventory(Cplayer *player);
    char *getinventory(Cmob *mob);

    char *getlook(Cplayer *player);
    char *getlook(Cobject *object);
    char *getlook(Ctile *tile);
    char *getlook(Cmob *mob);

    inline void unload(void) {
        gameobjects.unload();
    }
};
#endif
