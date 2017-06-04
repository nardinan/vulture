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
#include "PVMovement.h"

int PVmovement::movement(void) {
    if (infos.player->logics.hascategory("FOLLOW") == 0)
        if (infos.player->logics.delcategory("FOLLOW") > 0) LOG_ERROR("Unable to delete FOLLOW Category");
    return movement(infos.player, infos.message, false);
}

int PVmovement::movement(Cplayer *player, char *direction, bool nocheck) {
    archeslist *archeslist = NULL;
    mobslist *mobslist = NULL, *mobbackup = NULL;
    playerslist *playerslist = NULL, *playerbackup = NULL;
    char *buffer = NULL;
    if ((nocheck) || (checkmovement((Ccharacter *) player, direction))) {
        if ((archeslist = player->position->getarch(direction)) && (getvalue("STATUS", "Walkable", archeslist->tile->logics, 0) == 1)) {
            if (!archeslist->close) {
                if (setvalue("STATS", "SPoints", player->logics, (pointstamina*-1) - archeslist->length) > 0) LOG_ERROR("Unable to set SYSTEM->SPoints Logic");
                if (player->position->getplayer(player->getID())) {
                    if (player->position->delplayer(player->getID()) > 0) return 1;
                }
                mobslist = player->position->getmobsroot();
                playerslist = player->position->getplayersroot();
                if (player->logics.hasvalue("STATUS", "Hide") != 0) {
                    if (spvsend(pvulture.server, player->position, (buffer = allocate.vsalloc("[n][yellow]$name prosegue a %s[n]", direction)), (Ccharacter *) player) > 0) return 1;
                    if (spvsend(pvulture.server, archeslist->tile, "[reset][n][yellow]vedi arrivare $name[n]", (Ccharacter *) player) > 0) return 1;
                    if (buffer) {
                        pvfree(buffer);
                        buffer = NULL;
                    }
                    if (player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (archeslist->tile->spvsend(pvulture.server, sshell) > 0) return 1;
                }
                if (archeslist->tile->addplayer(player) > 0) return 1;
                if (environmentcommands.lookenvironment(player) > 0) return 1;
                if (player->spvsend(pvulture.server, sshell) > 0) return 1;
                while (playerslist) {
                    playerbackup = playerslist;
                    playerslist = playerslist->next;
                    if ((playerbackup->player->logics.hascategory("FOLLOW") == 0) &&
                            (playerbackup->player->logics.hasvalue("FOLLOW", "Player", player->getID()) == 0)) {
                        if (playerbackup->player->pvsend(pvulture.server, "[reset][n][yellow]segui %s ...[n]", buffer = pvulture.characters.gettargetname(player, playerbackup->player)) > 0) return 1;
                        if (buffer) {
                            pvfree(buffer);
                            buffer = NULL;
                        }
                        if (movement(playerbackup->player, direction, false) > 0) return 1;
                    }
                }
                while (mobslist) {
                    mobbackup = mobslist;
                    mobslist = mobslist->next;
                    if ((mobbackup->mob->logics.hascategory("FOLLOW") == 0) &&
                            (mobbackup->mob->logics.hasvalue("FOLLOW", "Player", player->getID()) == 0)) {
                        if (movement(mobbackup->mob, direction, false) > 0) return 1;
                    }
                }
                shell = false;
            } else if (player->pvsend(pvulture.server, "[reset]e' chiuso[n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]non puoi andare in quella direzione![n]") > 0) return 1;
    }
    return 0;
}

int PVmovement::movement(Cmob *mob, char *direction, bool nocheck) {
    archeslist *archeslist = NULL;
    mobslist *mobslist = NULL, *mobbackup = NULL;
    playerslist *playerslist = NULL, *playerbackup = NULL;
    char *buffer = NULL;
    if ((nocheck) || (checkmovement((Ccharacter *) mob, direction))) {
        if ((archeslist = mob->position->getarch(direction)) && (getvalue("STATUS", "Walkable", archeslist->tile->logics, 0) == 1)) {
            if (!archeslist->close) {
                if (setvalue("STATS", "SPoints", mob->logics, (pointstamina*-1) - archeslist->length) > 0) LOG_ERROR("Unable to set SYSTEM->SPoints Logic");
                if (mob->position->delmob(mob->getID()) > 0) return 1;
                mobslist = mob->position->getmobsroot();
                playerslist = mob->position->getplayersroot();
                if (mob->logics.hasvalue("STATUS", "Hide") != 0) {
                    if (spvsend(pvulture.server, mob->position, (buffer = allocate.vsalloc("[n][yellow]$name prosegue a %s[n]", direction)), (Ccharacter *) mob) > 0) return 1;
                    if (spvsend(pvulture.server, archeslist->tile, "[reset][n][yellow]vedi arrivare $name[n]", (Ccharacter *) mob) > 0) return 1;
                    if (buffer) {
                        pvfree(buffer);
                        buffer = NULL;
                    }
                    if (mob->position->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (archeslist->tile->spvsend(pvulture.server, sshell) > 0) return 1;
                }
                if (archeslist->tile->addmob(mob) > 0) return 1;
                while (playerslist) {
                    playerbackup = playerslist;
                    playerslist = playerslist->next;
                    if ((playerbackup->player->logics.hascategory("FOLLOW") == 0) &&
                            (playerbackup->player->logics.hasvalue("FOLLOW", "Mob", mob->getID()) == 0)) {
                        if (playerbackup->player->pvsend(pvulture.server, "[reset][n][yellow]segui %s ...[n]", buffer = pvulture.characters.gettargetname(mob, playerbackup->player)) > 0) return 1;
                        if (buffer) {
                            pvfree(buffer);
                            buffer = NULL;
                        }
                        if (movement(playerbackup->player, direction, false) > 0) return 1;
                    }
                }
                while (mobslist) {
                    mobbackup = mobslist;
                    mobslist = mobslist->next;
                    if ((mobbackup->mob->logics.hascategory("FOLLOW") == 0) &&
                            (mobbackup->mob->logics.hasvalue("FOLLOW", "Mob", mob->getID()) == 0)) {
                        if (movement(mobbackup->mob, direction, false) > 0) return 1;
                    }
                }
            }
        }
    }
    return 0;
}

bool PVmovement::checkmovement(Ccharacter *character, char *direction) {
    Cplayer *backup = NULL;
    bool allow = false;
    char player[_DEFAULT_MAXD_LEN];
    if (getvalue("STATS", "SPoints", character->logics, 0) <= 0) {
        if (character->getsex() != MALE) strncpy(player, "[reset]sei tremendamente esausta![n]", _DEFAULT_MAXD_LEN);
        else strncpy(player, "[reset]sei tremendamente esausto![n]", _DEFAULT_MAXD_LEN);
    } else if (getvalue("STATS", "Legs", character->logics, 0) <= 0) strncpy(player, "[reset]hai le gambe decisamente a pezzi![n]", _DEFAULT_MAXD_LEN);
    else if ((character->logics.hascategory("FIGHT") == 0) ||
            (pvulture.characters.infight(character->position, character))) strncpy(player, "[reset]sei in combattimento! puoi solo provare a fuggire![n]", _DEFAULT_MAXD_LEN);
    else if ((character->logics.hasvalue("STATUS", "Seated") == 0) || (character->logics.hasvalue("STATUS", "Stretched") == 0)) strncpy(player, "[reset]prima e' il caso di alzarsi in piedi![n]", _DEFAULT_MAXD_LEN);
    else allow = true;
    if (!allow) {
        if (character->logics.hascategory("FOLLOW") == 0)
            if (character->logics.delcategory("FOLLOW") > 0) LOG_ERROR("Unable to delete FOLLOW Category");
        if (character->getkind() != MOB) {
            backup = (Cplayer *) character;
            if (backup->pvsend(pvulture.server, player) > 0) throw "Unable to pvsend message";
        }
    }
    return allow;
}

int PVmovement::bounce(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if (!(infos.player->position->getplayer(command))) {
                if (strchr(message, '.')) {
                    value = bounce(message);
                } else if ((player = pvulture.characters.getplayer(message)) && (player->position)) {
                    value = bounce(player);
                } else if ((mob = pvulture.characters.getmob(message)) && (mob->position)) {
                    value = bounce(mob);
                } else if (infos.player->pvsend(pvulture.server, "[reset]non c'e' nessuno con quel nome in questo mondo![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare dove vuoi muoverti[n]") > 0) return 1;
    }
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    return value;
}

int PVmovement::bounce(Cplayer *player) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if ((infos.player->position->getID() != player->position->getID()) || (infos.player->position->getzoneID() != player->position->getzoneID())) {
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (infos.player->pvsend(pvulture.server, "[reset][green]ti muovi presso %s[n]", charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
            if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name scompare in una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) return 1;
                if (spvsend(pvulture.server, player->position, "[reset][n][yellow]$name appare da una nuvola di fumo[n]", infos.player) > 0) return 1;
                if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                if (player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            }
            if (player->position->addplayer(infos.player) > 0) return 1;
            if (environmentcommands.lookenvironment() > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;
    return 0;
}

int PVmovement::bounce(Cmob *mob) {
    char *charactername = NULL;
    if ((infos.player->position->getID() != mob->position->getID()) || (infos.player->position->getzoneID() != mob->position->getzoneID())) {
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
        }
        if (infos.player->pvsend(pvulture.server, "[reset][green]ti muovi presso %s[n]", charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
        if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
            if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name scompare in una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) return 1;
            if (spvsend(pvulture.server, mob->position, "[reset][n][yellow]$name appare da una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) return 1;
            if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            if (mob->position->spvsend(pvulture.server, sshell) > 0) return 1;
        }
        if (mob->position->addplayer(infos.player) > 0) return 1;
        if (environmentcommands.lookenvironment() > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) return 1;
    return 0;
}

int PVmovement::bounce(char *message) {
    Ctile *tile = NULL;
    tileinfos position;
    if (strings.vsscanf(message, '.', "dd", &position.zoneID, &position.tileID) == 0) {
        if ((infos.player->position->getID() != position.tileID) || (infos.player->position->getzoneID() != position.zoneID)) {
            if ((tile = pvulture.map.get(position))) {
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                }
                if (infos.player->pvsend(pvulture.server, "[reset][green]ti muovi alla zona %d, tile %d[n]", position.zoneID, position.tileID) > 0) return 1;
                if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                    if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name scompare in una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) return 1;
                    if (spvsend(pvulture.server, tile, "[reset][n][yellow]$name appare da una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) return 1;
                    if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (tile->spvsend(pvulture.server, sshell) > 0) return 1;
                }
                if (tile->addplayer(infos.player) > 0) return 1;
                if (environmentcommands.lookenvironment() > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste questa posizione![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]sei gia' qui![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare una posizione![n]") > 0) return 1;
    return 0;
}

int PVmovement::shift(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *message = NULL, *command = NULL, *charactername = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (charactername = strings.vpop(&message)) && (message)) {
            if ((player = pvulture.characters.getplayer(charactername))) {
                value = shift(player, message);
            } else if ((mob = pvulture.characters.getmob(charactername))) {
                value = shift(mob, message);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nome e una posizione![n]") > 0) return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
    } else return 1;
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (message) {
        pvfree(message);
        message = NULL;
    }
    return value;
}

int PVmovement::shift(Cplayer *player, char *message) {
    Ctile *tile = NULL;
    tileinfos position;
    char *buffer = NULL;
    if (player->getID() != infos.player->getID()) {
        if (strings.vsscanf(message, '.', "dd", &position.zoneID, &position.tileID) == 0) {
            if ((infos.player->position->getID() != position.tileID) || (infos.player->position->getzoneID() != position.zoneID)) {
                if ((tile = pvulture.map.get(position))) {
                    if (infos.player->position->delplayer(player->getID()) > 0) return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][green]fatto![n]") > 0) return 1;
                    if (player->pvsend(pvulture.server, "[reset][n][yellow]sei appena stat%s spostato[n]", (player->getsex() != MALE) ? "a" : "o") > 0) return 1;
                    if (player->logics.hasvalue("STATUS", "Hide") != 0) {
                        if (infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                        }
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[n][yellow]$name viene preso da $name e viene trascinat%s via[n]", (player->getsex() != MALE) ? "a" : "o")), (Ccharacter *) player, (Ccharacter *) infos.player) > 0) return 1;
                        if (buffer) {
                            pvfree(buffer);
                            buffer = NULL;
                        }
                        if (spvsend(pvulture.server, tile, (buffer = allocate.vsalloc("[n][yellow]$name viene trascinat%s a terra da $name[n]", (player->getsex() != MALE) ? "a" : "o")), (Ccharacter *) player, (Ccharacter *) infos.player) > 0) return 1;
                        if (buffer) {
                            pvfree(buffer);
                            buffer = NULL;
                        }
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (tile->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (!infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->addplayer(infos.player) > 0) return 1;
                        }
                    }
                    if (tile->addplayer(player) > 0) return 1;
                    if (player->spvsend(pvulture.server, sshell) > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste questa posizione nella mappa![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare una posizione![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;
    return 0;
}

int PVmovement::shift(Cmob *mob, char *message) {
    Ctile *tile = NULL;
    tileinfos position;
    char *buffer = NULL;
    if (strings.vsscanf(message, '.', "dd", &position.zoneID, &position.tileID) == 0) {
        if ((infos.player->position->getID() != position.tileID) || (infos.player->position->getzoneID() != position.zoneID)) {
            if ((tile = pvulture.map.get(position))) {
                if (infos.player->position->delmob(mob->getID()) > 0) return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]fatto![n]") > 0) return 1;
                if (mob->logics.hasvalue("STATUS", "Hide") != 0) {
                    if (infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                    }
                    if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[n][yellow]$name viene preso da $name e viene trascinat%s via[n]", (mob->getsex() != MALE) ? "a" : "o")), (Ccharacter *) mob, (Ccharacter *) infos.player) > 0) return 1;
                    if (buffer) {
                        pvfree(buffer);
                        buffer = NULL;
                    }
                    if (spvsend(pvulture.server, tile, (buffer = allocate.vsalloc("[n][yellow]$name viene trascinat%s a terra da $name[n]", (mob->getsex() != MALE) ? "a" : "o")), (Ccharacter *) mob, (Ccharacter *) infos.player) > 0) return 1;
                    if (buffer) {
                        pvfree(buffer);
                        buffer = NULL;
                    }
                    if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (tile->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (!infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->addplayer(infos.player) > 0) return 1;
                    }
                }
                if (tile->addmob(mob) > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste questa posizione nella mappa![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare una posizione![n]") > 0) return 1;
    return 0;
}

int PVmovement::follow(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    char *message = NULL, *command = NULL, *charactername = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                if (infos.player->logics.hascategory("FOLLOW") != 0) {
                    if (infos.player->logics.addcategory("FOLLOW") > 0) LOG_ERROR("Unable to add FOLLOW Category");
                }
                if (infos.player->logics.addvalue("FOLLOW", "Player", player->getID()) > 0) LOG_ERROR("Unable to add FOLLOW->Player Logic");
                else if (infos.player->pvsend(pvulture.server, "[reset][green]cominci a seguire %s[n]", charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
                if (charactername) {
                    pvfree(charactername);
                    charactername = NULL;
                }
            } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                if (infos.player->logics.hascategory("FOLLOW") != 0) {
                    if (infos.player->logics.addcategory("FOLLOW") > 0) LOG_ERROR("Unable to add FOLLOW Category");
                }
                if (infos.player->logics.addvalue("FOLLOW", "Mob", mob->getID()) > 0) LOG_ERROR("Unable to add FOLLOW->Player Logic");
                else if (infos.player->pvsend(pvulture.server, "[reset][green]cominci a seguire %s[n]", charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
                if (charactername) {
                    pvfree(charactername);
                    charactername = NULL;
                }
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nome![n]") > 0) return 1;
    } else return 1;
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (message) {
        pvfree(message);
        message = NULL;
    }
    return 0;
}
PVmovement movementcommands;

int runmovement(void) {
    if ((compare.vcmpcase(infos.message, CSTRSIZE("nord")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("sud")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("ovest")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("est")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("nordest")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("sudest")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("nordovest")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("sudovest")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("su")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("giu")) == 0)) {
        if (movementcommands.movement() > 0) LOG_ERROR("Unable to run MOVEMENTCOMMANDS.MOVEMENT()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("muovi")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0))) {
        if (movementcommands.shift() > 0) LOG_ERROR("Unable to run MOVEMENTCOMMANDS.SHIFT()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("vai")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (movementcommands.bounce() > 0) LOG_ERROR("Unable to run MOVEMENTCOMMANDS.BOUNCE()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("segui")) == 0) {
        if (movementcommands.follow() > 0) LOG_ERROR("Unable to run MOVEMENTCOMMANDS.FOLLOW()");
    } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
    return 0;
}
