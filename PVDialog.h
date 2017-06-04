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
#ifndef __PVSOCIALIZEHP__
#define __PVSOCIALIZEHP__
#include <string.h>
#include "PVulture.h"
#include "PVCommon.h"

class PVdialog {
public:
    int say(void);
    int say(Cplayer *player, char *emoticon, char *text);
    int say(Cmob *mob, char *emoticon, char *text);
    int say(char *emoticon, char *text);
    int whisper(void);
    int whisper(Cplayer *player, char *emoticon, char *text);
    int whisper(Cmob *mob, char *emoticon, char *text);
    int announce(void);
    int think(void);
    int listen(void);
};
extern PVdialog dialogcommands;
extern int dialog(void);
#endif
