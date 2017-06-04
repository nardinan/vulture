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
#include "PVPersonal.h"

int PVpersonal::login(void) {
    FILE *configurationfile = NULL;
    char *pathname = NULL;
    int length = 0;
    int times = 0;
    if (infos.player->logics.hasvalue("STATUS", "Account") == 0) {
        if ((pathname = (char *) pvmalloc(length = (sizeof (_PVFILES "players/") + strlen(infos.message) + sizeof (".dp") + 1)))) {
            snprintf(pathname, (length), _PVFILES "players/%s.dp", infos.message);
            if ((configurationfile = fopen(pathname, "r"))) {
                if (!(pvulture.characters.gamecharacters.getaccount(infos.message))) {
                    if (infos.player->load(configurationfile, pvulture.objects.gameobjects, pvulture.map.gamemap) > 0) return 1;
                    else {
                        if (infos.player->setaccount(infos.message) > 0) return 1;
                        if (infos.player->opvsend(pvulture.server, "[reset][bold][green]Account esistente![n]Password:[hide]") > 0) return 1;
                        if (infos.player->logics.delvalue("STATUS", "Account") > 0) LOG_ERROR("Unable to find STATUS->Account Logic");
                        if (infos.player->logics.addvalue("STATUS", "Password", 1) > 0) LOG_ERROR("Unable to add STATUS->Password Logic");
                    }
                } else if (infos.player->opvsend(pvulture.server, "[reset][bold][red]Account gia' in uso![n]Account:") > 0) return 1;
                fclose(configurationfile);
            } else {
                if (infos.player->opvsend(pvulture.server, "[reset][bold][red]Account non esistente![n]") > 0) return 1;
                times = getvalue("TIMES", "Account", infos.player->logics, 0);
                if (++times >= logintries) {
                    if (infos.player->opvsend(pvulture.server, "[reset][red]hai sbagliato account per %d volte![n]", logintries) > 0) return 1;
                    if (pathname) {
                        pvfree(pathname);
                        pathname = NULL;
                    }
                    return -1;
                } else {
                    infos.player->logics.addvalue("TIMES", "Account", times);
                    if (infos.player->opvsend(pvulture.server, "Account:") > 0) return 1;
                }
            }
        } else return 1;
        if (pathname) {
            pvfree(pathname);
            pathname = NULL;
        }
    } else if (infos.player->logics.hasvalue("STATUS", "Password") == 0) {
        if (compare.vcmpcase(infos.message, LSTRSIZE(infos.player->getpassword())) == 0) {
            if (infos.player->opvsend(pvulture.server, "[reset][bold][green]Password corretta![n]") > 0) return 1;
            if (infos.player->logics.delvalue("STATUS", "Password") > 0) LOG_ERROR("Unable to delete STATUS->Password Logic");
            if (infos.player->logics.addvalue("STATUS", "Online", pvulture.stopwatch.pause()) > 0) LOG_ERROR("Unable to add STATUS->Online Logic");
            if (!infos.player->position) {
                if (!(infos.player->position = pvulture.map.gamemap.gettilesroot()->tile)) return 1;
            }
            infos.player->setonline();
            if (infos.player->setlogging(true) > 0) LOG_ERROR("Unable to run SETLOGGING()");
            if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                }
                if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name entra in gioco[n]", (Ccharacter *) infos.player) > 0) return 1;
                if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                if (!infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->addplayer(infos.player) > 0) return 1;
                }
            }
            if (environmentcommands.lookenvironment() > 0) return 1;
            if (players.hasvalue(infos.player->getID()) != 0) {
                if (players.addvalue(pvulture.characters.getsimplename(infos.player), infos.player->getID()) > 0) return 1;
                else if (infos.player->pvsend(pvulture.server, "[reset][green]sei stato aggiunto alla lista utenti di gioco![n]") > 0) return 1;
            }
            if (infos.player->logics.hascategory("GROUP") == 0) {
                if (groups.hasvalue(infos.player->logics.getvalue("GROUP", 3)) != 0) {
                    if (infos.player->logics.delcategory("GROUP") > 0) return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][red]il tuo gruppo si e' sciolto![n]") > 0) return 1;
                }
            }
            if (infos.player->spvsend(pvulture.server, sshell) > 0) return 1;
        } else {
            if (infos.player->opvsend(pvulture.server, "[reset][bold][red]Password Errata![n]") > 0) return 1;
            times = getvalue("TIMES", "Password", infos.player->logics, 0);
            if (++times >= logintries) {
                if (infos.player->opvsend(pvulture.server, "[reset][red]hai sbagliato password per %d volte![n]", logintries) > 0) return 1;
                return -1;
            } else {
                infos.player->logics.addvalue("TIMES", "Password", times);
                if (infos.player->opvsend(pvulture.server, "Password:[hide]") > 0) return 1;
            }
        }
    }
    return 0;
}

int PVpersonal::logout(bool message) {
    if (compare.vcmpcase(infos.player->getaccount(), CSTRSIZE("###")) != 0) {
        infos.player->logics.delvalue("STATUS", "Online");
        infos.player->logics.delvalue("STATUS", "Last");
        if (infos.player->logics.hascategory("FIGHT") == 0)
            if (infos.player->logics.delcategory("FIGHT") > 0) LOG_ERROR("Unable to delete FIGHT Category");
        if (infos.player->logics.hascategory("TIMES") == 0)
            if (infos.player->logics.delcategory("TIMES") > 0) LOG_ERROR("Unable to delete TIMES Category");
        if (infos.player->logics.hascategory("FOLLOW") == 0)
            if (infos.player->logics.delcategory("FOLLOW") > 0) LOG_ERROR("Unable to delete FOLLOW Category");
        if (pvulture.characters.gamecharacters.saveplayer(infos.player->getID(), _PVFILES "players/", pvulture.objects.gameobjects) > 0) return 1;
        if (infos.player->position) {
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (message) {
                if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name esce dal gioco[n]", (Ccharacter *) infos.player) > 0) return 1;
                if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            }
        }
    }
    if (pvulture.server.unload(infos.player->getsocketID()) > 0) return 1;
    if (pvulture.characters.gamecharacters.delplayer(infos.player->getID()) > 0) return 1;
    infos.player = NULL;
    return 0;
}

int PVpersonal::status(void) {
    char *command = NULL;
    char *message = NULL;
    Cplayer *player = NULL;
    Cmob *mob = NULL;
    int value = 0;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) &&
                ((!message) || ((infos.player->logics.hasvalue("RANK", "Admin") != 0) &&
                (infos.player->logics.hasvalue("RANK", "Moderator") != 0)))) {
            value = status(infos.player);
        } else {
            if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                value = status(player);
            } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                value = status(mob);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome![n]") > 0) return 1;
        }
    } else return 1;
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

int PVpersonal::status(Cplayer *player) {
    char *charactername = NULL;
    char *backup = NULL;
    int index = 0;
    if (infos.player->getID() != player->getID()) {
        if (infos.player->pvsend(pvulture.server, "Lo status di %s e' il seguente:[n]", charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
    }
    if (infos.player->pvsend(pvulture.server, "Livello Vita    %s[n]", (backup = funny.vbar(getvalue("STATS", "LPoints", player->logics, 0), 100)) ? backup : "") > 0) return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (infos.player->pvsend(pvulture.server, "Livello Stamina %s[n]", (backup = funny.vbar(getvalue("STATS", "SPoints", player->logics, 0), 100)) ? backup : "") > 0) return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (infos.player->pvsend(pvulture.server, "[reset][n][bold]ABILITA':[n]") > 0) return 1;
    while (compare.vcmpcase(chacharas[index].representation, CSTRSIZE("NULL")) != 0) {
        if ((backup = pvulture.characters.getability(player, chacharas[index].representation))) {
            if (infos.player->pvsend(pvulture.server, backup) > 0) return 1;
            if (backup) {
                pvfree(backup);
                backup = NULL;
            }
        }
        index++;
    }
    return 0;
}

int PVpersonal::status(Cmob *mob) {
    char *charactername = NULL;
    char *backup = NULL;
    int index = 0;
    if (infos.player->pvsend(pvulture.server, "Lo status di %s e' il seguente:[n]", charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    if (infos.player->pvsend(pvulture.server, "Livello Vita    %s[n]", (backup = funny.vbar(getvalue("STATS", "LPoints", mob->logics, 0), 100)) ? backup : "") > 0) return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (infos.player->pvsend(pvulture.server, "Livello Stamina %s[n]", (backup = funny.vbar(getvalue("STATS", "SPoints", mob->logics, 0), 100)) ? backup : "") > 0) return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (infos.player->pvsend(pvulture.server, "[reset][n][bold]ABILITA':[n]") > 0) return 1;
    while (compare.vcmpcase(chacharas[index].representation, CSTRSIZE("NULL")) != 0) {
        if ((backup = pvulture.characters.getability(mob, chacharas[index].representation))) {
            if (infos.player->pvsend(pvulture.server, backup) > 0) return 1;
            if (backup) {
                pvfree(backup);
                backup = NULL;
            }
        }
        index++;
    }
    return 0;
}

int PVpersonal::position(void) {
    if (compare.vcmpcase(infos.message, CSTRSIZE("alza")) == 0) {
        if (getvalue("STATS", "Legs", infos.player->logics, 0) > 0) {
            if ((infos.player->logics.hasvalue("STATUS", "Seated") == 0) ||
                    (infos.player->logics.hasvalue("STATUS", "Stretched") == 0)) {
                infos.player->logics.delvalue("STATUS", "Seated");
                infos.player->logics.delvalue("STATUS", "Stretched");
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                }
                if (infos.player->pvsend(pvulture.server, "[reset][green]ti alzi in piedi[n]") > 0) return 1;
                if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                    if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name si alza in piedi[n]", (Ccharacter *) infos.player) > 0) return 1;
                    if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                }
                if (!infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->addplayer(infos.player) > 0) return 1;
                }
            } else if (infos.player->pvsend(pvulture.server, "[reset]sei gia' in piedi![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]hai le gambe distrutte! non riesci a muoverti![n]") > 0) return 1;
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("siedi")) == 0) {
        if ((infos.player->logics.hasvalue("STATUS", "Seated") != 0) ||
                (infos.player->logics.hasvalue("STATUS", "Stretched") == 0)) {
            infos.player->logics.addvalue("STATUS", "Seated", 1);
            infos.player->logics.delvalue("STATUS", "Stretched");
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (infos.player->pvsend(pvulture.server, "[reset][green]ti metti a sedere[n]") > 0) return 1;
            if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name si siede a terra[n]", (Ccharacter *) infos.player) > 0) return 1;
                if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            }
            if (!infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->addplayer(infos.player) > 0) return 1;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]sei gia' sedut%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;
    } else {
        if ((infos.player->logics.hasvalue("STATUS", "Seated") == 0) ||
                (infos.player->logics.hasvalue("STATUS", "Stretched") != 0)) {
            infos.player->logics.delvalue("STATUS", "Seated");
            infos.player->logics.addvalue("STATUS", "Stretched", 1);
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (infos.player->pvsend(pvulture.server, "[reset][green]ti sdrai a terra[n]") > 0) return 1;
            if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name si sdraia a terra[n]", (Ccharacter *) infos.player) > 0) return 1;
                if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            }
            if (!infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->addplayer(infos.player) > 0) return 1;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]sei gia' sdraiat%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;
    }
    return 0;
}

int PVpersonal::inventory(void) {
    char *message = NULL;
    char *command = NULL;
    char *backup = NULL;
    char *charactername = NULL;
    Cplayer *player = NULL;
    Cmob *mob = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) &&
                ((!message) || ((infos.player->logics.hasvalue("RANK", "Admin") != 0) &&
                (infos.player->logics.hasvalue("RANK", "Moderator") != 0)))) {
            if (!(backup = pvulture.objects.getinventory(infos.player))) return 1;
        } else {
            if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                if (!(backup = pvulture.objects.getinventory(player))) return 1;
            } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                if (!(backup = pvulture.objects.getinventory(mob))) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome![n]") > 0) return 1;
        }
    }
    if (backup) {
        if (mob) {
            if (infos.player->pvsend(pvulture.server, "%s sta' portando:[n]", charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
        } else if (player) {
            if (player->getID() != infos.player->getID()) {
                if (infos.player->pvsend(pvulture.server, "%s sta' portando:[n]", charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "stai portando:[n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "stai portando:[n]") > 0) return 1;
        if (infos.player->pvsend(pvulture.server, backup) > 0) return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
        pvfree(backup);
        backup = NULL;
    }
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

int PVpersonal::points(void) {
    char *command = NULL;
    char *message = NULL;
    char *text = NULL;
    char *pointer = NULL;
    Cplayer *player = NULL;
    Cmob *mob = NULL;
    int value = 0;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
        }
        if ((command = strings.vpop(&message)) && (message)) {
            if ((pointer = strchr(message, ':'))) {
                for (text = pointer + 1; *text == ' '; text++);
                do {
                    *pointer-- = '\0';
                } while ((pointer > message) && (*pointer == ' '));
                if (strlen(text) > 0) {
                    if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                        value = points(player, text);
                    } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                        value = points(mob, text);
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare una categoria e un punteggio[n]") > 0) return 1;
            } else {
                value = points(infos.player, message);
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare una categoria e un punteggio[n]") > 0) return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0) return 1;
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
    return 0;
}

int PVpersonal::points(Cplayer *player, char *message) {
    char *charactername = NULL;
    char *category = NULL;
    char *key = NULL;
    int value = 0;
    if ((strings.vsscanf(message, '.', "ssd", &category, &key, &value) == 0) &&
            (player->logics.hasvalue(category, key) == 0)) {
        if (player->logics.delvalue(category, key) > 0) LOG_ERROR("Unable to delete %s->%s Logic", category, key);
        else if (player->logics.addvalue(category, key, value) > 0) LOG_ERROR("Unable to add %s->%s Logic", category, key);
        else if (player->getID() != infos.player->getID()) {
            if (infos.player->pvsend(pvulture.server, "[reset][green]hai modificato il valore di %s.%s di %s in %d[n]", category, key, charactername = pvulture.characters.gettargetname(player, infos.player), value) > 0) return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset][green]hai modificato il tuo valore di %s.%s in %d[n]", category, key, value) > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste una simile categoria/chiave nella logica[n]") > 0) return 1;
    if (key) {
        pvfree(key);
        key = NULL;
    }
    if (category) {
        pvfree(category);
        category = NULL;
    }
    return 0;
}

int PVpersonal::points(Cmob *mob, char *message) {
    char *charactername = NULL;
    char *category = NULL;
    char *key = NULL;
    int value = 0;
    if ((strings.vsscanf(message, '.', "ssd", &category, &key, &value) == 0) &&
            (mob->logics.hasvalue(category, key) == 0)) {
        if (mob->logics.delvalue(category, key) > 0) LOG_ERROR("Unable to delete %s->%s Logic", category, key);
        else if (mob->logics.addvalue(category, key, value) > 0) LOG_ERROR("Unable to add %s->%s Logic", category, key);
        else if (mob->getID() != infos.player->getID()) {
            if (infos.player->pvsend(pvulture.server, "[reset]hai modificato il valore di %s.%s di %s in %d[n]", category, key, charactername = pvulture.characters.gettargetname(mob, infos.player), value) > 0) return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset][green]hai modificato il tuo valore di %s.%s in %d[n]", category, key, value) > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste una simile categoria/chiave nella logica[n]") > 0) return 1;
    if (key) {
        pvfree(key);
        key = NULL;
    }
    if (category) {
        pvfree(category);
        category = NULL;
    }
    return 0;
}

int PVpersonal::password(void) {
    char *message = NULL;
    char *command = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if (infos.player->setpassword(message) > 0) {
                if (infos.player->pvsend(pvulture.server, "[reset]la password e' troppo corta![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset][green]hai cambiato la password![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare la nuova password![n]") > 0) return 1;
    } else return 1;
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

int PVpersonal::appearance(void) {
    int position = 0;
    char *message = NULL;
    char *command = NULL;
    char *completename = NULL;
    char *smalldescription = NULL;
    char *largedescription = NULL;
    char *race = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            position = getvalue("SYSTEM", "Position", infos.player->logics, 0);
            if (strings.vsscanf(message, ':', "ssss", &completename, &smalldescription, &largedescription, &race) == 0) {
                if (infos.player->logics.hasvalue("RACE", 0) == 0) {
                    if (infos.player->logics.delvalue("RACE", 0) > 0) LOG_ERROR("Unable to delete RACE->%s Logic", race);
                }
                if (infos.player->logics.addvalue("RACE", race, 0) > 0) LOG_ERROR("Unable to add RACE->%s Logic", race);
                if (infos.player->descriptions.deldescription(position) == 0) {
                    infos.player->descriptions.adddescription(position, completename, smalldescription, largedescription);
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai cambiato il tuo aspetto e la tua razza![n]") > 0) return 1;
                } else return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nuovo aspetto per il tuo personaggio![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nuovo aspetto per il tuo personaggio![n]") > 0) return 1;
    } else return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (completename) {
        pvfree(completename);
        completename = NULL;
    }
    if (smalldescription) {
        pvfree(smalldescription);
        smalldescription = NULL;
    }
    if (largedescription) {
        pvfree(largedescription);
        largedescription = NULL;
    }
    if (race) {
        pvfree(race);
        race = NULL;
    }
    return 0;
}

int PVpersonal::emoticon(void) {
    char *message = NULL;
    char *text = NULL;
    char *emoticon = NULL;
    char *buffer = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        emoticon = getemoticon(&message);
        for (text = message + 1; *text == ' '; text++);
        if (strlen(text) > 0) {
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (infos.player->pvsend(pvulture.server, "[reset][green]ti vedi mentre il tuo [bold]'io'[reset][green], %s %s[n]", text, (emoticon) ? emoticon : "") > 0) return 1;
            if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[n][yellow]$name %s %s[n]", text, (emoticon) ? emoticon : "")), (Ccharacter *) infos.player) > 0) return 1;
            }
            if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            if (!infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->addplayer(infos.player) > 0) return 1;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un'azione![n]") > 0) return 1;
    } else return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (emoticon) {
        pvfree(emoticon);
        emoticon = NULL;
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVpersonal::meet(void) {
    char *command = NULL;
    char *message = NULL;
    Cplayer *player = NULL;
    Cmob *mob = NULL;
    int value = 0;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                value = meet(player);
            } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                value = meet(mob);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) return 1;
    } else return 1;
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

int PVpersonal::meet(Cplayer *player) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if (!(player->getcharacter(infos.player->getID(), PLAYER))) {
            if (player->addcharacter(infos.player->getID(), PLAYER) > 0) return 1;
            if (infos.player->pvsend(pvulture.server, "[reset][yellow]ti presenti a %s[n]", charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
            if (player->pvsend(pvulture.server, "[reset][n][yellow]%s ti si presenta[n]", pvulture.characters.getsimplename(infos.player)) > 0) return 1;
            if (player->spvsend(pvulture.server, sshell) > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]da quel che sembra, lui ti conosce di gia'![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    return 0;
}

int PVpersonal::meet(Cmob *mob) {
    char *charactername = NULL;
    if (!(mob->getcharacter(infos.player->getID(), PLAYER))) {
        if (mob->addcharacter(infos.player->getID(), PLAYER) > 0) return 1;
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]ti presenti a %s[n]", charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
        if (intelligenceevents.meet(mob, infos.player) > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]da quel che sembra, lui ti conosce di gia'![n]") > 0) return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    return 0;
}

int PVpersonal::hide(void) {
    if (infos.player->logics.hasvalue("STATUS", "Hide") == 0) {
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
        }
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]spunti fuori dall'ombra![n]") > 0) return 1;
        if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name appare da una nuvola di fumo![n]", (Ccharacter *) infos.player) > 0) return 1;
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0) return 1;
        }
        if (infos.player->logics.delvalue("STATUS", "Hide") > 0) LOG_ERROR("Unable to delete STATUS->Hide Logic");
    } else {
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
        }
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]ti nascondi nell'ombra![n]") > 0) return 1;
        if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name scompare in una nuvola di fumo![n]", (Ccharacter *) infos.player) > 0) return 1;
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0) return 1;
        }
        if (infos.player->logics.addvalue("STATUS", "Hide", 1) > 0) LOG_ERROR("Unable to add STATUS->Hide Logic");
    }
    return 0;
}
PVpersonal personalcommands;

int personal(void) {
    if (compare.vcmpcase(infos.message, CSTRSIZE("status")) == 0) {
        if (personalcommands.status() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.STATUS()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("inventario")) == 0) {
        if (personalcommands.inventory() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.INVENTORY()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("passwd")) == 0) {
        if (personalcommands.password() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.PASSWORD()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("alza")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("siedi")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("sdraia")) == 0)) {
        if (personalcommands.position() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.POSITION()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("aspetto")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (personalcommands.appearance() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.APPEARANCE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("valore")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (personalcommands.points() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.POINTS()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE(":")) == 0) {
        if (personalcommands.emoticon() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.EMOTICON()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("presenta")) == 0) {
        if (personalcommands.meet() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.MEET()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("nascondi")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (personalcommands.hide() > 0) LOG_ERROR("Unable to run PERSONALCOMMANDS.HIDE()");
    } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
    return 0;
}
