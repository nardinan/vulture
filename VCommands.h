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
#ifndef __VCOMMANDSHP__
#define __VCOMMANDSHP__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct commandslist {
    char *command;
    char *syntax;
    commandfunction *function;
    unsigned int length;
    struct commandslist *next;
    struct commandslist *back;
} commandslist;

class Ccommands {
private:
    commandslist *commandsroot;
public:
    int elements;
    Ccommands(void);

    inline ~Ccommands(void) {
    };

    int addcommand(const char *command, const char *syntax, commandfunction *function, int length);
    int delcommand(const char *command);
    commandfunction *getcommand(const char *command);

    int runcommand(const char *command);
    void unload(void);
};
#endif
