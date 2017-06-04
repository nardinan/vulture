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
#ifndef __VTOOLSHP__
#define __VTOOLSHP__
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

class Cstrings {
public:
    int vsscanf(const char *string, char divider, const char *format, ...);
    int voccurrence(const char *string, const char *word);
    int vpush(char **string, const char *word);
    char *vpop(char **string);
    char *vpop(char **string, unsigned int length);
    char *vget(char *string, const char bchar, const char fchar);
    bool vreplace(char **string, const char *old, const char *replace);
    bool vsreplace(char **string, const char *old, const char *replace);
};
extern Cstrings strings;

class Callocate {
public:
    int vstrlen(const char *format, va_list arguments);
    int vtelnet(char **string, const char *format, ...);
    int vclient(char **string, const char *format, ...);
    int vadd(char **string, const char *format, ...);
    char *vsalloc(const char *format, ...);
    char *vstring(unsigned int value);
};
extern Callocate allocate;

class Ccompare {
public:
    int vcmpcase(const char *string, const char *compare, int length);
    int vcmpcase(const char *string, const char *compare);
    int vcompare(const char *string, const char *compare);
    int vstep(const char *string, const char *compare);
};
extern Ccompare compare;

class Cfunny {
public:
    int vrandom(char **string, unsigned int level);
    char *vdigit(unsigned int value);
    char *vbar(unsigned int value, unsigned int maximum);
};
extern Cfunny funny;

class Cnumbers {
public:
    int vdiv(int divider, int dividend);
    int vmax(int valuea, int valueb, int valuec);
    int vmin(int valuea, int valueb, int valuec);

    inline int vmax(int valuea, int valueb) {
        return (valuea > valueb) ? valuea : valueb;
    };

    inline int vmin(int valuea, int valueb) {
        return (valuea < valueb) ? valuea : valueb;
    };
};
extern Cnumbers numbers;
#endif
