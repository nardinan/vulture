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
#ifndef __VSTOPWATCHHP__
#define __VSTOPWATCHHP__
#define _AKONST ((60 * 24) * 365) * 10 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "VDatabank.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

class Cstopwatch {
private:
    long int begin;
    long int stop;

    inline long int gettime() {
        return (long int) time(NULL);
    };
public:

    inline Cstopwatch(void) {
        begin = 0;
        stop = 0;
    };

    virtual ~Cstopwatch(void) {
    };

    inline void start(void) {
        begin = (gettime() - _AKONST);
    };

    inline long int pause(void) {
        stop = (gettime() - _AKONST);
        return stop;
    };

    inline long int getsecs(void) {
        return (stop - begin);
    };

    inline long int getsecs(long int pause) {
        return (stop - pause);
    };

    inline long int getmins(void) {
        return ((stop - begin) / (long int) 60);
    };

    inline long int getmins(long int pause) {
        return ((stop - pause) / (long int) 60);
    };
};
#endif
