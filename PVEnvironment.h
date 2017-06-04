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
#ifndef __PVENVIRONMENTHP__
#define __PVENVIRONMENTHP__
#include <string.h>
#include "PVulture.h"
#include "PVCommon.h"

class PVenvironment {
public:
    int look(void);
    int lookenvironment(Cplayer *player);

    inline int lookenvironment(void) {
        return lookenvironment(infos.player);
    }
    int lookobject(char *name);
    int lookcharacter(char *name);
    int date(void);
    int action(void);
    int action(char *text);
    int action(Cplayer *player, char *text);
    int invoke(void);
    int kick(void);
    int kick(Cplayer *player);
    int kick(Cmob *mob);
    int peace(void);
    int relog(void);
    int list(void);
};
extern PVenvironment environmentcommands;
extern int environment(void);
#endif
