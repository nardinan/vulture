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
#ifndef __VDATAHP__
#define __VDATAHP__ 
#define _DEFAULT_NAME_LEN 25
#define _DEFAULT_FILE_LEN 15
#define _DEFAULT_PASS_LEN 15
#define _DEFAULT_PYTH_LEN 64
#define _DEFAULT_MAXD_LEN 1024
#define _DEFAULT_MIND_LEN 125
#define _DEFAULT_AWAY_LEN 64
#define _DEFAULT_NOTE_LEN 125
#define _DEFAULT_LOGS_LEN 128
#define _DEFAULT_LOG_ERROR "./errors.log"
#define _DEFAULT_LOG_INFOS "./infos.log"
#define CSTRSIZE(x) x, (sizeof(x)-1)
#define LSTRSIZE(x) x, (strlen(x))
#include <string.h>
class Cdatabank;
class Cpoints;
class Cobject;
class Cmob;
class Cplayer;
class Ctile;
class Czone;

enum sexdefinition {
    MALE, FEMALE, NOSEX
};

enum characterdefinition {
    MOB, PLAYER, NOBODY
};
typedef int (commandfunction) (void);

typedef struct shash {
    char representation[5];
    char string[50];
} shash;

typedef struct objectslist {
    Cobject *object;
    struct objectslist *next;
    struct objectslist *back;
} objectslist;

typedef struct playerslist {
    Cplayer *player;
    struct playerslist *next;
    struct playerslist *back;
} playerslist;

typedef struct mobslist {
    Cmob *mob;
    struct mobslist *next;
    struct mobslist *back;
} mobslist;

typedef struct tileinfos {
    unsigned int tileID;
    unsigned int zoneID;
} tileinfos;
extern Cdatabank Vexcategory;
extern unsigned int Vflevelup;
extern unsigned int Vltime;
extern unsigned int Vport;
extern unsigned int Vflog;
extern unsigned int Vplog;
extern unsigned int Vfile;
extern unsigned int Vpass;

extern unsigned int NobjectID;
extern unsigned int NplayerID;
extern unsigned int NmobID;
extern unsigned int NtileID;
extern unsigned int NzoneID;

extern int getvalue(const char *category, const char *value, Cpoints& points, int defaultvalue);
extern int sgetvalue(const char *category, const char *value, Cpoints& points, int defaultvalue);
extern int setvalue(const char *category, const char *value, Cpoints& points, int modificator, int maxvalue);
extern int setvalue(const char *category, const char *value, Cpoints& points, int modificator);
#endif
