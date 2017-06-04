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
#ifndef __VLOGHP__
#define __VLOGHP__
#define LOG_ERROR(a, b...) virtuallogger(_DEFAULT_LOG_ERROR, __FILE__, __FUNCTION__, a, ##b)
#define LOG_INFOS(a, b...) virtuallogger(_DEFAULT_LOG_INFOS, __FILE__, __FUNCTION__, a, ##b)
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
extern void virtuallogger(const char *output, const char *file, const char *function, const char *format, ...);

class Clogger {
private:
    bool isB;
    bool isU;
    bool vdecode;
    FILE *logger;
public:

    inline Clogger(void) {
        logger = NULL;
        vdecode = true;
        isB = false;
        isU = false;
    };

    inline Clogger(const char *configurationfile) {
        logger = NULL;
        vdecode = true;
        isB = false;
        isU = false;
        load(configurationfile);
    };

    inline Clogger(FILE *configurationfile) {
        logger = NULL;
        vdecode = true;
        isB = false;
        isU = false;
        load(configurationfile);
    };

    inline ~Clogger(void) {
        unload();
    };

    int load(void);
    int load(const char *configurationfile);

    inline void load(FILE *configurationfile) {
        if (!logger) logger = configurationfile;
    };

    inline void unload(void) {
        if (logger) {
            fprintf(logger, "%s%s</font></body></html>", (isB) ? "</b>" : "", (isU) ? "</u>" : "");
            fclose(logger);
            logger = NULL;
        }
    };

    inline bool islogging(void) {
        return (logger) ? true : false;
    };

    int log(const char *subject, const char *format, ...);

    inline void setvdecode(bool vdecode) {
        this->vdecode = vdecode;
    };
};
#endif
