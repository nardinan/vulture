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
#ifndef __PVPERSONALHP__
#define __PVPERSONALHP__
#include <string.h>
#include "PVEnvironment.h"
#include "PVGroups.h"
#include "PVulture.h"
#include "PVCommon.h"
#include "PVEvents.h"

class PVpersonal {
public:
    int login(void);
    int logout(bool message);
    int status(void);
    int status(Cplayer *player);
    int status(Cmob *mob);
    int position(void);
    int inventory(void);
    int points(void);
    int points(Cplayer *player, char *message);
    int points(Cmob *mob, char *message);
    int password(void);
    int appearance(void);
    int emoticon(void);
    int meet(void);
    int meet(Cplayer *player);
    int meet(Cmob *mob);
    int hide(void);
};
extern PVpersonal personalcommands;
extern int personal(void);
#endif
