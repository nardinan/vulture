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
#include "VLCharacters.h"

int Ccharacterslist::loadmobs(FILE *configurationfile, Cobjectslist& objects, Cworld& world) {
    char string[_DEFAULT_MAXD_LEN];
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/MOBS]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[MOBS]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("[MOB]")) == 0) {
                        if (addmob(configurationfile, objects, world) < 0) LOG_ERROR("Unable to load MOB");
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cobjectslist] %s (unknown command)", string);
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Ccharacterslist::saveplayer(unsigned int characterID, const char *path, Cobjectslist& objects) {
    playerslist *player = NULL;
    FILE *configurationfile = NULL;
    char *directory = NULL;
    if ((player = getplayer(characterID))) {
        if (compare.vcmpcase(player->player->descriptions.completename(0), CSTRSIZE("###")) != 0) {
            if ((directory = (char *) pvmalloc(strlen(path) + strlen(player->player->getaccount()) + strlen(".dp") + 1))) {
                snprintf(directory, (strlen(path) + strlen(player->player->getaccount()) + strlen(".dp") + 1), "%s%s.dp", path, player->player->getaccount());
                if ((configurationfile = fopen(directory, "w"))) {
                    if (player->player->save(configurationfile, objects) > 0) LOG_ERROR("Unable to save ACCOUNT %s of Player %d", player->player->getaccount(), characterID);
                    fclose(configurationfile);
                } else return 1;
            } else return 1;
            if (directory) {
                pvfree(directory);
                directory = NULL;
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Ccharacterslist::saveplayer(const char *path, Cobjectslist& objects) {
    playerslist *player = playersroot;
    while (player) {
        if (saveplayer(player->player->getID(), path, objects) > 0) LOG_ERROR("Unable to save some ACCOUNTS");
        player = player->next;
    }
    return 0;
}

int Ccharacterslist::savemobs(FILE *configurationfile, Cobjectslist& objects) {
    mobslist *list = mobsroot;
    if (configurationfile) {
        fprintf(configurationfile, "[MOBS]\n");
        while (list) {
            if (compare.vcmpcase(list->mob->descriptions.completename(0), CSTRSIZE("###")) != 0) {
                if (list->mob->save(configurationfile, objects) > 0) LOG_ERROR("Unable to save MOB %d", list->mob->getID());
                else fputc('\n', configurationfile);
            }
            list = list->next;
        }
        fprintf(configurationfile, "[/MOBS]");
    } else return 1;
    return 0;
}

int Ccharacterslist::addplayer(Cplayer *player) {
    playerslist *list = playersroot;
    unsigned int playerID;
    if (player) {
        if (playersroot) {
            while (list->next) list = list->next;
            if ((list->next = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                list->next->player = player;
                list->next->back = list;
                list->next->next = NULL;
                playerID = list->next->player->getID();
            } else return 1;
        } else {
            if ((playersroot = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                playersroot->player = player;
                playersroot->next = NULL;
                playersroot->back = NULL;
                playerID = playersroot->player->getID();
            } else return 1;
        }
    } else return 1;
    return playerID;


}

int Ccharacterslist::addplayer(FILE *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int socketID) {
    playerslist *list = playersroot;
    unsigned int playerID;
    if (configurationfile) {
        if (playersroot) {
            while (list->next) list = list->next;
            if ((list->next = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                list->next->player = new (Cplayer);
                list->next->back = list;
                list->next->next = NULL;
                list->next->player->setsocketID(socketID);
                if (list->next->player->load(configurationfile, objects, world) > 0) return 1;
                playerID = list->next->player->getID();
            } else return 1;
        } else {
            if ((playersroot = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                playersroot->player = new (Cplayer);
                playersroot->next = NULL;
                playersroot->back = NULL;
                playersroot->player->setsocketID(socketID);
                if (playersroot->player->load(configurationfile, objects, world) > 0) return 1;
                playerID = playersroot->player->getID();
            } else return 1;
        }
    } else return 1;
    return playerID;
}

int Ccharacterslist::addplayer(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int socketID, unsigned int characterID) {
    playerslist *list = playersroot;
    unsigned int playerID;
    if (configurationfile) {
        if (playersroot) {
            while (list->next) list = list->next;
            if ((list->next = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                list->next->player = new (Cplayer);
                list->next->back = list;
                list->next->next = NULL;
                list->next->player->setsocketID(socketID);
                if (list->next->player->loadskeleton(configurationfile, objects, world, characterID) > 0) return 1;
                playerID = list->next->player->getID();
            } else return 1;
        } else {
            if ((playersroot = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                playersroot->player = new (Cplayer);
                playersroot->next = NULL;
                playersroot->back = NULL;
                playersroot->player->setsocketID(socketID);
                if (playersroot->player->loadskeleton(configurationfile, objects, world, characterID) > 0) return 1;
                playerID = playersroot->player->getID();
            } else return 1;
        }
    } else return 1;
    return playerID;
}

int Ccharacterslist::delplayer(unsigned int characterID) {
    playerslist *list = playersroot;
    if ((list = getplayer(characterID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else playersroot = list->next;
        list->player->unload();
        delete(list->player);
        pvfree(list);
    } else return 1;
    return 0;
}

playerslist *Ccharacterslist::getplayer(unsigned int characterID) {
    playerslist *list = playersroot;
    if (playersroot) {
        while (((unsigned int) list->player->getID() != characterID) && (list->next)) list = list->next;
        if ((unsigned int) list->player->getID() != characterID) return NULL;
    } else return NULL;
    return list;
}

playerslist *Ccharacterslist::getplayer(const char *completename, unsigned int position) {
    playerslist *list = playersroot;
    if (playersroot) {
        while ((compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

playerslist *Ccharacterslist::getplayer(const char *completename) {
    playerslist *list = playersroot;
    int position;
    if (playersroot) {
        if (list->player->logics.hasvalue("SYSTEM", "Position") != 0) {
            if (list->player->logics.hascategory("SYSTEM") != 0)
                if (list->player->logics.addcategory("SYSTEM") > 0) return NULL;
            if (list->player->logics.addvalue("SYSTEM", "Position", 0) > 0) return NULL;
        }
        position = list->player->logics.getvalue("SYSTEM", "Position");
        while ((compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) {
            list = list->next;
            if (list->player->logics.hasvalue("SYSTEM", "Position") != 0) {
                if (list->player->logics.hascategory("SYSTEM") != 0)
                    if (list->player->logics.addcategory("SYSTEM") > 0) return NULL;
                if (list->player->logics.addvalue("SYSTEM", "Position", 0) > 0) return NULL;
            }
            position = list->player->logics.getvalue("SYSTEM", "Position");
        }
        if (compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

playerslist *Ccharacterslist::getaccount(const char *account) {
    playerslist *list = playersroot;
    char *backupaccount = NULL;
    if (playersroot) {
        backupaccount = list->player->getaccount();
        while ((compare.vcmpcase(backupaccount, LSTRSIZE(account)) != 0) && (list->next)) {
            backupaccount = list->player->getaccount();
            list = list->next;
        }
        if (compare.vcmpcase(backupaccount, LSTRSIZE(account)) != 0) return NULL;
    } else return NULL;
    return list;
}

int Ccharacterslist::pvsend(Cserver& server, const char *format, ...) {
    va_list args;
    playerslist *list = playersroot;
    int length = 0;
    char *message = NULL;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((message = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(message, length, format, args);
            va_end(args);
            while (list) {
                if (list->player->pvsend(server, message) > 0) return 1;
                list = list->next;
            }
        } else return 1;
        if (message) {
            pvfree(message);
            message = NULL;
        }
    }
    return 0;
}

int Ccharacterslist::spvsend(Cserver& server, const char *format, ...) {
    va_list args;
    playerslist *list = playersroot;
    int length = 0;
    char *message = NULL;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((message = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(message, length, format, args);
            va_end(args);
            while (list) {
                if (list->player->spvsend(server, message) > 0) return 1;
                list = list->next;
            }
        } else return 1;
        if (message) {
            pvfree(message);
            message = NULL;
        }
    }
    return 0;
}

int Ccharacterslist::addmob(Cmob *mob) {
    mobslist *list = mobsroot;
    unsigned int mobID;
    if (mob) {
        if (mobsroot) {
            while (list->next) list = list->next;
            if ((list->next = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                list->next->mob = mob;
                list->next->back = list;
                list->next->next = NULL;
                mobID = list->next->mob->getID();
            } else return 1;
        } else {
            if ((mobsroot = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                mobsroot->mob = mob;
                mobsroot->next = NULL;
                mobsroot->back = NULL;
                mobID = mobsroot->mob->getID();
            } else return 1;
        }
    } else return 1;
    return mobID;
}

int Ccharacterslist::addmob(FILE *configurationfile, Cobjectslist& objects, Cworld& world) {
    mobslist *list = mobsroot;
    unsigned int mobID;
    if (configurationfile) {
        if (mobsroot) {
            while (list->next) list = list->next;
            if ((list->next = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                list->next->mob = new (Cmob);
                list->next->back = list;
                list->next->next = NULL;
                if (list->next->mob->load(configurationfile, objects, world) > 0) return 1;
                mobID = list->next->mob->getID();
            } else return 1;
        } else {
            if ((mobsroot = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                mobsroot->mob = new (Cmob);
                mobsroot->next = NULL;
                mobsroot->back = NULL;
                if (mobsroot->mob->load(configurationfile, objects, world) > 0) return 1;
                mobID = mobsroot->mob->getID();
            } else return 1;
        }
    } else return 1;
    return mobID;
}

int Ccharacterslist::addmob(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int characterID) {
    mobslist *list = mobsroot;
    if (configurationfile) {
        if (mobsroot) {
            while (list->next) list = list->next;
            if ((list->next = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                list->next->mob = new (Cmob);
                list->next->back = list;
                list->next->next = NULL;
                if (list->next->mob->loadskeleton(configurationfile, objects, world, characterID) > 0) return 1;
            } else return 1;
        } else {
            if ((mobsroot = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                mobsroot->mob = new (Cmob);
                mobsroot->next = NULL;
                mobsroot->back = NULL;
                if (mobsroot->mob->loadskeleton(configurationfile, objects, world, characterID) > 0) return 1;
            } else return 1;
        }
    } else return 1;
    return 0;
}

int Ccharacterslist::delmob(unsigned int characterID) {
    mobslist *list = mobsroot;
    if ((list = getmob(characterID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else mobsroot = list->next;
        list->mob->unload();
        delete(list->mob);
        pvfree(list);
    } else return 1;
    return 0;
}

mobslist *Ccharacterslist::getmob(unsigned int characterID) {
    mobslist *list = mobsroot;
    if (mobsroot) {
        while (((unsigned int) list->mob->getID() != characterID) && (list->next)) list = list->next;
        if ((unsigned int) list->mob->getID() != characterID) return NULL;
    } else return NULL;
    return list;
}

mobslist *Ccharacterslist::getmob(const char *completename, unsigned int position) {
    mobslist *list = mobsroot;
    if (mobsroot) {
        while ((compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

mobslist *Ccharacterslist::getmob(const char *completename) {
    mobslist *list = mobsroot;
    int position;
    if (mobsroot) {
        if (list->mob->logics.hasvalue("SYSTEM", "Position") != 0) {
            if (list->mob->logics.hascategory("SYSTEM") != 0)
                if (list->mob->logics.addcategory("SYSTEM") > 0) return NULL;
            if (list->mob->logics.addvalue("SYSTEM", "Position", 0) > 0) return NULL;
        }
        position = list->mob->logics.getvalue("SYSTEM", "Position");
        while ((compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) {
            list = list->next;
            if (list->mob->logics.hasvalue("SYSTEM", "Position") != 0) {
                if (list->mob->logics.hascategory("SYSTEM") != 0)
                    if (list->mob->logics.addcategory("SYSTEM") > 0) return NULL;
                if (list->mob->logics.addvalue("SYSTEM", "Position", 0) > 0) return NULL;
            }
            position = list->mob->logics.getvalue("SYSTEM", "Position");
        }
        if (compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

void Ccharacterslist::unload(void) {
    while (playersroot) {
        if (delplayer(playersroot->player->getID()) > 0) {
            LOG_ERROR("Unable to unload Player %d", playersroot->player->getID());
            playersroot = playersroot->next;
        }
    }
    while (mobsroot) {
        if (delmob(mobsroot->mob->getID()) > 0) {
            LOG_ERROR("Unable to unload Mob %d", mobsroot->mob->getID());
            mobsroot = mobsroot->next;
        }
    }
}
