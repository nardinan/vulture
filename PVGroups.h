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
#ifndef __PVGROUPSHP__
#define __PVGROUPSHP__
#include <string.h>
#include "PVEnvironment.h"
#include "PVCommon.h"
#include "PVulture.h"
#include "Vulture.h"

class PVgroup {
public:
    int build(char *string);
    int add(char *string);
    int rule(char *string);
    int rule(Cplayer *player, char *string);
    int rule(Cmob *mob, char *string);
    int exit(void);
    int kick(char *string);
    int kick(Cplayer *player);
    int kick(Cmob *mob);
    int transfer(char *string);
    int moderator(char *string);
    int moderator(Cplayer *player);
    int moderator(Cmob *mob);
    int bounce(char *string);
    int chat(char *string);
    int list(void);
    int destroy(void);
    int information(void);
};
extern PVgroup groupcommands;
extern int group(void);
#endif
