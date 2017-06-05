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
#include "PVEnvironment.h"

int PVenvironment::look(void) {
    char *message = NULL, *command = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (!message)) {
            if (lookenvironment() > 0)
                return 1;
        } else if ((lookobject(message) < 0) &&
                (lookcharacter(message) < 0)) {
            if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere qui attorno![n]") > 0)
                return 1;
        }
    } else
        return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    return 0;
}

int PVenvironment::lookenvironment(Cplayer *player) {
    char *backup = NULL;
    if (player->pvsend(pvulture.server, "%s",
            (backup = pvulture.map.getlook(player->position, pvulture.time.ptime.hour)) ? backup : "") > 0)
        return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (player->pvsend(pvulture.server, "%s",
            (backup = pvulture.characters.getlook(player->position, player)) ? backup : "") > 0)
        return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (player->pvsend(pvulture.server, "%s",
            (backup = pvulture.objects.getlook(player->position)) ? backup : "") > 0)
        return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return 0;

}

int PVenvironment::lookobject(char *name) {
    Cobject *object = NULL;
    char *message = NULL;
    if ((object = pvulture.objects.get(name, infos.player->position)) ||
            (object = pvulture.objects.get(name, infos.player))) {
        if ((message = pvulture.objects.getlook(object))) {
            if (infos.player->pvsend(pvulture.server, message) > 0)
                return 1;
            if (message) {
                pvfree(message);
                message = NULL;
            }
        } else
            return 1;
    } else
        return -1;
    return 0;
}

int PVenvironment::lookcharacter(char *name) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    char *message = NULL, *charactername = NULL;
    if ((player = pvulture.characters.getplayer(name, infos.player->position))) {
        if (infos.player->getID() != player->getID()) {
            if ((message = pvulture.characters.getlook(player, infos.player, pvulture.time.pdate))) {
                if (infos.player->pvsend(pvulture.server, message) > 0)
                    return 1;
                if (message) {
                    pvfree(message);
                    message = NULL;
                }
                if ((message = pvulture.objects.getlook(player))) {
                    if (infos.player->pvsend(pvulture.server, message) > 0)
                        return 1;
                    if (player->pvsend(pvulture.server, "[reset][n][yellow]%s ti sta' fissando![n]",
                            charactername = pvulture.characters.gettargetname(infos.player, player)) > 0)
                        return 1;
                    if (player->spvsend(pvulture.server, sshell) > 0)
                        return 1;
                    if (charactername) {
                        pvfree(charactername);
                        charactername = NULL;
                    }
                }
                if (message) {
                    pvfree(message);
                    message = NULL;
                }
            } else
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]",
                (infos.player->getsex() != MALE) ? "a" : "o") > 0)
            return 1;
    } else if ((mob = pvulture.characters.getmob(name, infos.player->position))) {
        if ((message = pvulture.characters.getlook(mob, infos.player, pvulture.time.pdate))) {
            if (infos.player->pvsend(pvulture.server, message) > 0)
                return 1;
            if (message) {
                pvfree(message);
                message = NULL;
            }
            if ((message = pvulture.objects.getlook(mob))) {
                if (infos.player->pvsend(pvulture.server, message) > 0)
                    return 1;
            }
            if (message) {
                pvfree(message);
                message = NULL;
            }
        } else
            return 1;
    } else
        return -1;
    return 0;
}

int PVenvironment::date(void) {
    datalist *day = NULL, *month = NULL;
    pvulture.time.update();
    if ((day = pvulture.time.getday()) && (month = pvulture.time.getmonth())) {
        if (infos.player->pvsend(pvulture.server, "[reset][green]e' il %s (%d) del %s (%d) dell'anno %d[n]",
                day->completename, pvulture.time.pdate.day, month->completename, pvulture.time.pdate.month, pvulture.time.pdate.year) > 0)
            return 1;
        if (infos.player->pvsend(pvulture.server, "[reset][green]sono le ore %d:%2d[n]",
                pvulture.time.ptime.hour, pvulture.time.ptime.minutes) > 0)
            return 1;
    } else 
        return 1;
    return 0;
}

int PVenvironment::action(void) {
    Cplayer *player = NULL;
    int value;
    char *message = NULL, *text = NULL, *command = NULL, *pointer = NULL, *buffer = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((pointer = strchr(message, ':'))) {
                for (text = pointer + 1; *text == ' '; text++);
                do {
                    *pointer-- = '\0';
                } while ((pointer > message) && (*pointer == ' '));
                if (strlen(text) > 0) {
                    if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                        value = action(player, text);
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0)
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un'azione![n]") > 0)
                    return 1;
            } else {
                value = action(message);
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un'azione![n]") > 0)
            return 1;
    } else
        return 1;
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return value;
}

int PVenvironment::action(char *text) {
    if (infos.player->pvsend(pvulture.server, "[reset][yellow]fatto![n]") > 0)
        return 1;
    if (infos.player->spvsend(pvulture.server, sshell) > 0)
        return 1;
    if (infos.player->position->pvsend(pvulture.server, "[reset][n][yellow]%s[n]", text) > 0)
        return 1;
    if (infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->delplayer(infos.player->getID()) > 0)
            return 1;
    }
    if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
        return 1;
    if (!infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->addplayer(infos.player) > 0)
            return 1;
    }
    return 0;
}

int PVenvironment::action(Cplayer *player, char *text) {
    if (player->getID() != infos.player->getID()) {
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]fatto![n]") > 0)
            return 1;
        if (player->pvsend(pvulture.server, "[reset][n][yellow]%s[n]", text) > 0)
            return 1;
        if (player->spvsend(pvulture.server, sshell) > 0)
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]",
            (infos.player->getsex() != MALE) ? "a" : "o") > 0)
        return 1;
    return 0;
}

int PVenvironment::invoke(void) {
    archeslist *direction = NULL;
    char directionarray[][_DEFAULT_NAME_LEN] = {"nordovest", "nord", "nordest", "ovest", "here", "est", "sudovest", "sud", "sudest"};
    char *buffer = NULL;
    int index = 0;
    if (allocate.vadd(&buffer, "$i:") > 0)
        return 1;
    for (index = 0; index < 9; index++) {
        if (compare.vcmpcase(directionarray[index], "here") != 0) {
            if ((direction = infos.player->position->getarch(directionarray[index]))) {
                if (direction->close) {
                    if (direction->lock) {
                        if (allocate.vadd(&buffer, "$LCK:") > 0)
                            return 1;
                    } else if (allocate.vadd(&buffer, "$CLS:") > 0)
                        return 1;
                } else if (direction->tile->logics.getvalue("TERRAIN", 0)) {
                    if (allocate.vadd(&buffer, "%s:", direction->tile->logics.getvalue("TERRAIN", 0)) > 0)
                        return 1;
                } else if (allocate.vadd(&buffer, "$NUL:") > 0)
                    return 1;
            } else if (allocate.vadd(&buffer, "$NUL:") > 0)
                return 1;
        } else if (infos.player->position->logics.getvalue("TERRAIN", 0)) {
            if (allocate.vadd(&buffer, "%s:", infos.player->position->logics.getvalue("TERRAIN", 0)) > 0)
                return 1;
        } else if (allocate.vadd(&buffer, "$NUL:") > 0)
            return 1;
    }
    if (allocate.vadd(&buffer, "%d:%d:%d:%d:",
            getvalue("STATS", "LPoints", infos.player->logics, 0),
            getvalue("STATS", "SPoints", infos.player->logics, 0),
            pvulture.time.ptime.hour,
            pvulture.time.ptime.minutes) > 0)
        return 1;
    if (infos.player->logics.hascategory("GROUP") == 0) {
        if (allocate.vadd(&buffer, "true:%s:%s: ",
                infos.player->logics.getvalue("GROUP", 3),
                infos.player->logics.getvalue("GROUP", 2)) > 0)
            return 1;
    } else {
        if (allocate.vadd(&buffer, "false:$NUL:$NUL:") > 0)
            return 1;
    }
    if (allocate.vadd(&buffer, "%s:", (infos.player->logics.hasvalue("STATUS", "Hide") != 0) ? "false" : "true") > 0)
        return 1;
    if (infos.player->logics.hasvalue("RANK", "Admin") == 0) {
        if (allocate.vadd(&buffer, "$ADM") > 0)
            return 1;
    } else if (infos.player->logics.hasvalue("RANK", "Moderator") == 0) {
        if (allocate.vadd(&buffer, "$MOD") > 0)
            return 1;
    } else {
        if (allocate.vadd(&buffer, "$USR") > 0)
            return 1;
    }
    if (infos.player->pvsend(pvulture.server, "[reset]%s[n]", buffer) > 0)
        return 1;
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVenvironment::kick(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((player = pvulture.characters.getplayer(message)) && (player->position)) {
                value = kick(player);
            } else if ((mob = pvulture.characters.getmob(message)) && (mob->position)) {
                value = kick(mob);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non c'e' nessuno con quel nome in questo mondo![n]") > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare dove vuoi muoverti[n]") > 0)
            return 1;
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

int PVenvironment::kick(Cplayer *player) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if (kickid.addvalue("Player", player->getID()) > 0)
            LOG_ERROR("Unable to add KickID %d value", player->getID());
        else {
            if (player->pvsend(pvulture.server, "[reset][n][yellow]sei stat%s da %s[n]",
                    (player->getsex() != MALE) ? "a cacciata" : "o cacciato",
                    charactername = pvulture.characters.gettargetname(infos.player, player)) > 0)
                return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
            if (infos.player->pvsend(pvulture.server, "[reset]hai cacciato dal gioco %s![n]", charactername = pvulture.characters.gettargetname(player, infos.player)) > 0)
                return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
        }
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0)
                return 1;
        }
        if (player->position->getplayer(player->getID())) {
            if (player->position->delplayer(player->getID()) > 0)
                return 1;
        }
        if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name viene cacciato dal gioco[n]",
                (Ccharacter *) player) > 0)
            return 1;
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (!(player->position->getplayer(player->getID()))) {
            if (player->position->addplayer(player) > 0)
                return 1;
        }
        if (!(infos.player->position->getplayer(infos.player->getID()))) {
            if (infos.player->position->addplayer(infos.player) > 0)
                return 1;
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]",
            (infos.player->getsex() != MALE) ? "a" : "o") > 0)
        return 1;
    return 0;
}

int PVenvironment::kick(Cmob *mob) {
    char *charactername = NULL;
    if (kickid.addvalue("Mob", mob->getID()) > 0)
        LOG_ERROR("Unable to add KickID %d value", mob->getID());
    else {
        if (infos.player->pvsend(pvulture.server, "[reset]hai cacciato dal gioco %s![n]",
                charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0)
            return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
    }
    if (infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->delplayer(infos.player->getID()) > 0)
            return 1;
    }
    if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name viene cacciato dal gioco[n]", (Ccharacter *) mob) > 0)
        return 1;
    if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
        return 1;
    if (!(infos.player->position->getplayer(infos.player->getID()))) {
        if (infos.player->position->addplayer(infos.player) > 0)
            return 1;
    }
    return 0;
}

int PVenvironment::peace(void) {
    int peaceful = getvalue("STATUS", "Peaceful", infos.player->position->logics, 0);
    if (peaceful != 0) {
        peaceful = 0;
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]questa stanza non e' piu' pacifica![n]") > 0)
            return 1;
    } else {
        peaceful = 1;
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]questa stanza e' ora pacifica![n]") > 0)
            return 1;
    }
    if (infos.player->position->logics.addvalue("STATUS", "Peaceful", peaceful) > 0)
        LOG_ERROR("Unable to add STATUS->Peaceful Logic");
    return 0;
}

int PVenvironment::relog(void) {
    if (pvulture.logger.load() == 0) {
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]un nuovo file di log e' ora in uso![n]") > 0)
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non e' stato possibile generare un nuovo file![n]") > 0)
        return 1;
    return 0;
}

int PVenvironment::list(void) {
    playerslist *list = pvulture.characters.gamecharacters.playersroot;
    if (infos.player->pvsend(pvulture.server, "[reset][yellow]attualmente online:[n]") > 0)
        return 1;
    while (list) {
        if (list->player->logics.hasvalue("STATUS", "Account") != 0) {
            if (list->player->logics.hasvalue("STATUS", "Password") != 0) {
                if (infos.player->pvsend(pvulture.server, "\t[green](online)[reset](%d) %s[n]",
                        list->player->getID(), pvulture.characters.getsimplename(list->player)) > 0)
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "\t[yellow](passwd)[reset](%d) %s[n]",
                    list->player->getID(), pvulture.characters.getsimplename(list->player)) > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "\t[red](login )[reset] someone[n]") > 0)
            return 1;
        list = list->next;
    }
    return 0;
}

PVenvironment environmentcommands;

int environment(void) {
    if (compare.vcmpcase(infos.message, CSTRSIZE("data")) == 0) {
        if (environmentcommands.date() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.DATE()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("guarda")) == 0) {
        if (environmentcommands.look() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.LOOK()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("online")) == 0) {
        if (environmentcommands.list() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.ONLINE()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("invoke")) == 0) {
        if (environmentcommands.invoke() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.INVOKE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("caccia")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (environmentcommands.kick() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.KICK()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("evento")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (environmentcommands.action() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.ACTION()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("pacifica")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (environmentcommands.peace() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.PEACE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("relog")) == 0) &&
            (infos.player->logics.hasvalue("RANK", "Admin") == 0)) {
        if (environmentcommands.relog() > 0)
            LOG_ERROR("Unable to run ENVIRONMENTSCOMMANS.RELOG()");
    } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0)
        return 1;
    return 0;
}
