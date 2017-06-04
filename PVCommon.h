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
#ifndef __PVCOMMONHP__
#define __PVCOMMONHP__
#include <stdarg.h>
#include <string.h>
#include "Vulture.h"
extern struct shash emoticons[];
extern char *getemoticon(char **target);
extern char *getstatus(Cpoints& points);
extern int getname(char *string, char **name);
extern int spvsend(Cserver& server, Ctile *tile, const char *format, ...);
#endif
