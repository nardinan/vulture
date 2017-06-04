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
#ifndef __PVFIGHTHP__
#define __PVFIGHTHP__
#define _AAMINDAM 30
#define _AAWEAPON "un attacco violento"
#include <string.h>
#include "PVulture.h"
#include "PVCommon.h"

struct PVinfos {
    char *weaponname;
    char *targetname;
    char *playername;
};

class PVfight {
public:
    int attack(void);
    int attack(Cplayer *player);
    int attack(Cmob *mob);
    int stop(void);
    int mode(void);
    int position(void);
};

class PVround {
public:
    PVinfos *getnames(Ccharacter *attacker, Ccharacter *target, Cdatabank& events);
    int messages(Ccharacter *attacker, Ccharacter *target, Cdatabank& events);
    int attack(Cpoints& attacker, Cpoints& target);
    int escape(Cpoints& attacker, Cpoints& target);
    int damage(Cpoints& attacker, int value);
    int disarm(Ccharacter *target, int value);
    int head(Ccharacter *target, int value);
    int legs(Ccharacter *target, int value);
};
extern PVfight fight;
extern PVround attack;
extern int round(Ccharacter *attacker, Ccharacter *target);
extern int escape(Ccharacter *escaper, Ccharacter *attacker);
extern int round(Ccharacter *attacker);
extern int escape(Ccharacter *escaper);
extern int runfight(void);
extern int runescape(void);
#endif
