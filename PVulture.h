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
#ifndef __PVULTUREHP__
#define __PVULTUREHP__
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include "Vulture.h"
#include "PVGCharacters.h"
#include "PVGObjects.h"
#include "PVGMap.h"
#define _VERSION  "1.4b"
#define _ASTAMINA 5
#define _AADELAY  2
#define _PVFILES  "./adventure/"
#define _APAGE   10
#define _ALINE   100

enum emode {
    NORMAL = 0, BERSERK, DEFENSE, AGGRESSIVE
};

enum etarget {
    TORSO = 0, ARMS, LEGS, HEAD
};

enum ekind {
    USER = 0, ADMIN, MODERATOR
};

struct PVvulture {
    Cgcharacters characters;
    Cstopwatch stopwatch;
    Cgobjects objects;
    Clogger logger;
    Cserver server;
    Cpages pages;
    Ctime time;
    Cgmap map;
    Ccommands usrcommands;
    Ccommands modcommands;
    Ccommands admcommands;
};

struct sinformation {
    Cplayer *player;
    Cmob *mob;
    char *message;
};
extern PVvulture pvulture;
extern sinformation infos;
extern bool shell;
extern bool gstop;
extern char *logo;
extern char *subtitle;
extern char *message;
extern char *sshell;
extern int timeclimate;
extern int timemessage;
extern int timetimeout;
extern int timeattack;
extern int timeblood;
extern int timestun;
extern int pointstun;
extern int pointstamina;
extern int logintries;
extern long int timeloop;
extern Cdatabank diary;
extern Cdatabank groups;
extern Cdatabank players;
extern Cdatabank events;
extern Cdatabank kickid;

extern int loadgame(FILE *configurationfile);
extern int savegame(FILE *configurationfile);
extern bool getkind(Cplayer *player, ekind kind);
#endif
