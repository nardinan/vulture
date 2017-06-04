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
#ifndef __PVMOVEMENTHP__
#define __PVMOVEMENTHP__
#include <string.h>
#include "PVEnvironment.h"
#include "PVulture.h"
#include "PVCommon.h"

class PVmovement {
public:
    int movement(void);
    int movement(Cplayer *player, char *direction, bool nocheck);
    int movement(Cmob *mob, char *direction, bool nocheck);
    bool checkmovement(Ccharacter *character, char *direction);
    int bounce(void);
    int bounce(Cplayer *player);
    int bounce(Cmob *mob);
    int bounce(char *message);
    int shift(void);
    int shift(Cplayer *player, char *message);
    int shift(Cmob *mob, char *message);
    int follow(void);
};
extern PVmovement movementcommands;
extern int runmovement(void);
#endif
