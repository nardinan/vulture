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
#ifndef __PVOBJECTSHP__
#define __PVOBJECTSHP__
#include <string.h>
#include "PVulture.h"
#include "PVCommon.h"

class PVobject {
public:
    int duplicate(void);
    int destroy(void);
    int generate(void);
};

class PVaction {
public:
    int use(void);
    int drop(void);
    int put(void);
    int put(Cobject *container, Cobject *object);
    int replace(void);
    int take(void);
    int take(Cobject *object);
    int take(Cobject *container, Cobject *object);
    int give(void);
    int give(Cplayer *player, Cobject *object);
    int give(Cmob *mob, Cobject *object);
    int eat(char **message);
    int drink(char **message);
    int read(char **message);
    int write(char **message);
    int remove(char **message);
    int equip(char **message);
    int equip(Cplayer *player, Cobject *object);
    int equip(Cmob *mob, Cobject *object);
};
extern PVobject objects;
extern PVaction actions;
extern int runobjects(void);
#endif
