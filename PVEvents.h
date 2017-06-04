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
#ifndef __PVEVENTHP__
#define __PVEVENTHP__
#define _AIMOVEMENT 10
#include "PVEnvironment.h"
#include "PVMovement.h"
#include "PVulture.h"
#include "PVCommon.h"

class PVintelligence {
public:
    int fight(Cmob *mob);
    int move(Cmob *mob);
    int messages(Cmob *mob);
    int emoticons(Cmob *mob);
    int meet(Cmob *mob, Cplayer *player);
    int recall(void);
};

class PVpersonalevents {
public:
    int damageArms(Cplayer *player);
    int damageLife(Cplayer *player);
    int blood(Cplayer *player);
    int equip(Cplayer *player);
};

class PVenvironmentsevents {
public:
    int weather(void);
    int stamina(void);
    int messages(void);
    int sunevent(void);
    int kicklist(void);
};
extern PVintelligence intelligenceevents;
extern PVpersonalevents personalevents;
extern PVenvironmentsevents environmentevents;
extern int runenvironmentevents(void);
extern int runintelligenceevents(void);
extern int runintelligence(void);
#endif
