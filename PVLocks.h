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
#ifndef __PVLOCKSHP__
#define __PVLOCKSHP__
#include <string.h>
#include "PVEnvironment.h"
#include "PVulture.h"
#include "PVCommon.h"

class PVlock {
public:
    char *getopposite(char *direction);
    int close(char *direction, Cplayer *player);
    int close(Cobject *object, Cplayer *player);
    int open(char *direction, Cplayer *player);
    int open(Cobject *object, Cplayer *player);
    int lock(char *direction, Cplayer *player);
    int lock(Cobject *object, Cplayer *player);
    int unlock(char *direction, Cplayer *player);
    int unlock(Cobject *object, Cplayer *player);
};
extern PVlock lockcommands;
extern int runlock(void);
#endif
