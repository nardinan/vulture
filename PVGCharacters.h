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
#ifndef __PVGAMECHARACTERSHP__
#define __PVGAMECHARACTERSHP__
#include "PVCommon.h"
#include "Vulture.h"
extern struct shash chacharas[];

class Cgcharacters {
public:
    Ccharacterslist gamecharacters;

    inline Cgcharacters(void) {
    };

    Cplayer *getplayer(char *name, Ctile *tile);
    Cplayer *getplayer(char *name);
    Cmob *getmob(char *name, Ctile *tile);
    Cmob *getmob(char *name);

    int addobject(Cplayer *player, Cobject *object);
    int delobject(Cplayer *player, Cobject *object);

    char *getsimplename(Ccharacter *character);
    char *getsimplename(Cplayer *player);
    char *getsimplename(Cmob *mob);
    char *getdescription(Cplayer *player);
    char *getdescription(Cmob *mob);
    char *gettargetname(Cplayer *target, Cplayer *pvsender);
    char *gettargetname(Cmob *target, Cplayer *pvsender);
    char *getlookstring(Cplayer *player, Cplayer *pvsender);
    char *getlookstring(Cmob *mob, Cplayer *pvsender);

    char *getappearance(Cpoints& points, bool male, unsigned int age);
    char *getage(unsigned int age, bool male);
    char *getcharism(Cpoints& points, bool male);
    char *getstrength(Cpoints& points, bool male);
    char *getIntelligence(Cpoints& points);

    inline char *getappearance(Cplayer *player, datainfos& present) {
        return getappearance(player->logics, (player->getsex() != MALE) ? false : true, player->getage(present));
    }

    inline char *getappearance(Cmob *mob, datainfos& present) {
        return getappearance(mob->logics, (mob->getsex() != MALE) ? false : true, mob->getage(present));
    }

    char *getlook(Cplayer *player, Cplayer *pvsender, datainfos& present);
    char *getlook(Cmob *mob, Cplayer *pvsender, datainfos& present);
    char *getlook(Ctile *tile, Cplayer *player);

    bool infight(Ctile *tile, Ccharacter *character);
    bool infight(Ctile *tile, Cplayer *player);
    bool infight(Ctile *tile, Cmob *mob);

    char *getability(Cplayer *player, const char *name);
    char *getability(Cmob *mob, const char *name);

    int setbonus(Cplayer *player, Cobject *object);
    int setmalus(Cplayer *player, Cobject *object);
    int setbonus(Cmob *mob, Cobject *object);
    int setmalus(Cmob *mob, Cobject *object);

    inline void unload(void) {
        gamecharacters.unload();
    }
};
#endif
