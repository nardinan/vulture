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
#include "PVEvents.h"

int PVintelligence::fight(Cmob *mob) {
    playerslist *playerslist = mob->position->getplayersroot(), *target = NULL;
    char *targetname = NULL;
    if (getvalue("STATUS", "Peaceful", mob->position->logics, 0) != 1) {
        if (mob->AI.hascategory("ATTACK") == 0) {
            while ((mob->logics.hascategory("FIGHT") != 0) && (playerslist)) {
                target = pvulture.characters.gamecharacters.getplayer(playerslist->player->getID());
                if (playerslist->player->logics.hasvalue("STATUS", "Hide") != 0) {
                    if ((playerslist->player->logics.hasvalue("RACE", 0) == 0) &&
                            (mob->AI.hasvalue("ATTACK", target->player->logics.getvalue("RACE", 0)) == 0) &&
                            ((mob->logics.hasvalue("STATUS", "Seated") != 0) && (mob->logics.hasvalue("STATUS", "Stretched") != 0))) {
                        if (mob->logics.addcategory("FIGHT") > 0)
                            LOG_ERROR("Unable to add FIGHT Category");
                        else if ((mob->logics.addvalue("FIGHT", "Target", target->player->getID()) > 0) ||
                                (mob->logics.addvalue("FIGHT", "Kind", (int) PLAYER) > 0) ||
                                (mob->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) ||
                                (mob->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) ||
                                (mob->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause()) > 0))
                            LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
                        if (target->player->logics.addcategory("FIGHT") > 0)
                            LOG_ERROR("Unable to add FIGHT Category");
                        else if ((target->player->logics.addvalue("FIGHT", "Target", mob->getID()) > 0) ||
                                (target->player->logics.addvalue("FIGHT", "Kind", (int) MOB) > 0) ||
                                (target->player->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) ||
                                (target->player->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) ||
                                (target->player->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause()) > 0))
                            LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
                        if (target->player->position->getplayer(target->player->getID())) {
                            if (target->player->position->delplayer(target->player->getID()) > 0)
                                return 1;
                        }
                        if (spvsend(pvulture.server, mob->position, "[n][yellow]$name attacca $name![n]",
                                (Ccharacter *) mob,
                                (Ccharacter *) target->player) > 0)
                            return 1;
                        if (!target->player->position->getplayer(target->player->getID())) {
                            if (target->player->position->addplayer(target->player) > 0)
                                return 1;
                        }
                        if (target->player->pvsend(pvulture.server, "[reset][n][red]%s ti attacca![n]",
                                targetname = pvulture.characters.gettargetname(mob, target->player)) > 0)
                            return 1;
                        if (target->player->position->spvsend(pvulture.server, sshell) > 0)
                            return 1;
                        if (targetname) {
                            pvfree(targetname);
                            targetname = NULL;
                        }
                    }
                }
                playerslist = playerslist->next;
            }
        }
    }
    return 0;
}

int PVintelligence::move(Cmob *mob) {
    char *direction = mob->position->randomize();
    if ((mob->AI.hascategory("MOVEMENT") == 0) && (mob->logics.hascategory("FIGHT") != 0)) {
        if ((mob->AI.hasvalue("MOVEMENT", "Last") != 0) || (pvulture.stopwatch.getsecs(mob->AI.getvalue("MOVEMENT", "Last")) >= getvalue("MOVEMENT", "Delay", mob->AI, _AIMOVEMENT))) {
            if (mob->AI.addvalue("MOVEMENT", "Last", (int) pvulture.stopwatch.pause()) > 0)
                LOG_ERROR("Unable to add MOVEMENT->Last Logic");
            if (direction) {
                if (movementcommands.movement(mob, direction, false) > 0)
                    return 1;
            }
        }
    }
    return 0;
}

int PVintelligence::messages(Cmob *mob) {
    int maxvalue = 0;
    char *buffer = NULL;
    if ((mob->AI.hascategory("MESSAGESLIST") == 0) && (mob->AI.hascategory("MESSAGES") == 0) && (mob->logics.hascategory("FIGHT") != 0)) {
        if ((maxvalue = mob->AI.getvalue("MESSAGES", "NULLMESSAGES")) > 0) {
            if ((mob->AI.hasvalue("MESSAGES", "Last") != 0) || (pvulture.stopwatch.getsecs(mob->AI.getvalue("MESSAGES", "Last")) >= getvalue("MESSAGES", "Delay", mob->AI, _AIMOVEMENT))) {
                if (mob->AI.addvalue("MESSAGES", "Last", (int) pvulture.stopwatch.pause()) > 0)
                    LOG_ERROR("Unable to add MESSAGES->Last Logic");
                if (spvsend(pvulture.server, mob->position, (buffer = allocate.vsalloc("[n][yellow]$name dice :\"%s\"[n]", mob->AI.getvalue("MESSAGESLIST", (rand() % maxvalue)))), (Ccharacter *) mob) > 0)
                    return 1;
                if (mob->position->spvsend(pvulture.server, sshell) > 0)
                    return 1;
            }
        }
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVintelligence::emoticons(Cmob *mob) {
    int maxvalue = 0;
    char *buffer = NULL;
    if ((mob->AI.hascategory("EMOTICONSLIST") == 0) && (mob->AI.hascategory("MESSAGES") == 0) && (mob->logics.hascategory("FIGHT") != 0)) {
        if ((maxvalue = mob->AI.getvalue("MESSAGES", "NULLEMOTICONS")) > 0) {
            if ((mob->AI.hasvalue("MESSAGES", "Last") != 0) || (pvulture.stopwatch.getsecs(mob->AI.getvalue("MESSAGES", "Last")) >= getvalue("MESSAGES", "Delay", mob->AI, _AIMOVEMENT))) {
                if (mob->AI.addvalue("MESSAGES", "Last", (int) pvulture.stopwatch.pause()) > 0)
                    LOG_ERROR("Unable to add MESSAGES->Last Logic");
                if (spvsend(pvulture.server, mob->position, (buffer = allocate.vsalloc("[n][yellow]$name %s[n]", mob->AI.getvalue("EMOTICONSLIST", (rand() % maxvalue)))), (Ccharacter *) mob) > 0)
                    return 1;
                if (mob->position->spvsend(pvulture.server, sshell) > 0)
                    return 1;
            }
        }
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVintelligence::meet(Cmob *mob, Cplayer *player) {
    char *charactername = NULL;
    if (mob->AI.hasvalue("LOGIC", "Represent") == 0) {
        if (player->addcharacter(mob->getID(), MOB) > 0)
            return 1;
        if (player->pvsend(pvulture.server, "\t[reset][yellow]e %s ricambia, presentandosi a sua volta![n]", charactername = pvulture.characters.gettargetname(mob, player)) > 0)
            return 1;
    } else if (player->pvsend(pvulture.server, "\t[reset][yellow]ma vieni ignorato totalmente![n]", charactername = pvulture.characters.gettargetname(mob, player)) > 0)
        return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    return 0;
}

int PVintelligence::recall(void) {
    Cmob *mob = NULL;
    tileinfos position = {infos.player->position->getID(), infos.player->position->getzoneID()};
    FILE *configurationfile = NULL;
    char path[_DEFAULT_MIND_LEN], *message = NULL, *command = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            snprintf(path, _DEFAULT_MIND_LEN, _PVFILES "mobs/%s.dm", message);
            if ((configurationfile = fopen(path, "r"))) {
                if ((mob = new(Cmob)) && (mob->load(configurationfile, pvulture.objects.gameobjects, pvulture.map.gamemap) == 0)) {
                    mob->position = pvulture.map.get(position);
                    pvulture.characters.gamecharacters.addmob(mob);
                    if (infos.player->position->addmob(mob) > 0)
                        return 1;
                    else if (infos.player->pvsend(pvulture.server, "[reset][green]hai richiamato un MOB[n]") > 0)
                        return 1;
                } else LOG_ERROR("Unable to read TEMPLATE %s", message);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste un template con quel nome![n]") > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare il nome di un template mob![n]") > 0)
            return 1;
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

int PVpersonalevents::damageArms(Cplayer *player) {
    Cobject *object = NULL;
    objectslist *list = NULL, *backup = NULL;
    bool something = false;
    char *target = NULL, *objectname = NULL;
    player->resetvalue();
    list = player->nextvalue();
    if (player->position->getplayer(player->getID())) {
        if (player->position->delplayer(player->getID()) > 0)
            return 1;
    }
    while (list) {
        if (getvalue("USAGE", "Use", list->object->logics, 0) > 0) {
            if ((target = list->object->logics.getvalue("USAGE", 4)) &&
                    ((compare.vcmpcase(target, CSTRSIZE("RH")) == 0) ||
                    (compare.vcmpcase(target, CSTRSIZE("LH")) == 0) ||
                    (compare.vcmpcase(target, CSTRSIZE("RA")) == 0) ||
                    (compare.vcmpcase(target, CSTRSIZE("LA")) == 0))) {
                backup = player->nextvalue();
                object = list->object;
                if (object->logics.addvalue("USAGE", "Use", 0) > 0)
                    return 1;
                if (player->delobject(object->getID()) > 0)
                    return 1;
                if (player->position->addobject(object) > 0)
                    return 1;
                if (player->pvsend(pvulture.server, "[reset]%s[red]dalle mani ti cade %s[n]", (something) ? "" : "[n]", objectname = pvulture.objects.getcompletename(object)) > 0)
                    return 1;
                if (objectname) {
                    pvfree(objectname);
                    objectname = NULL;
                }
                something = true;
            }
        }
        if (object)
            list = backup;
        else
            list = player->nextvalue();
    }
    if (something) {
        if (spvsend(pvulture.server, player->position, "[reset][n][yellow]dalle mani di $name cade qualcosa[n]", (Ccharacter *) player) > 0)
            return 1;
        if (!player->position->getplayer(player->getID())) {
            if (player->position->addplayer(player) > 0)
                return 1;
        }
        if (player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
    } else if (!player->position->getplayer(player->getID())) {
        if (player->position->addplayer(player) > 0)
            return 1;
    }
    return 0;
}

int PVpersonalevents::damageLife(Cplayer *player) {
    objectslist *currentobject = NULL, *nextobject = NULL;
    char *objectname = NULL, *buffer = NULL;
    if (getvalue("STATS", "LPoints", player->logics, 0) <= 0) {
        if (player->logics.hasvalue("STATS", "Death") != 0) {
            if (player->logics.addvalue("STATS", "Death", 1) > 0)
                LOG_ERROR("Unable to add STATS->Death Logic");
            if (player->logics.addvalue("STATS", "LPoints", 0) > 0)
                LOG_ERROR("Unable to add STATS->LPoints Logic");
            if (player->position->getplayer(player->getID())) {
                if (player->position->delplayer(player->getID()) > 0)
                    return 1;
            }
            if (player->pvsend(pvulture.server, "[reset][n][red]<SEI MORT%s!>[n]", (player->getsex() != MALE) ? "A" : "O") > 0)
                return 1;
            if (spvsend(pvulture.server, player->position, (buffer = allocate.vsalloc("[reset][n][red]$name <E' MORT%s!> proprio davanti ai tuoi occhi![n]", (player->getsex() != MALE) ? "A" : "O")), (Ccharacter *) player) > 0)
                return 1;
            player->resetvalue();
            nextobject = player->nextvalue();
            while (nextobject) {
                currentobject = nextobject;
                nextobject = player->nextvalue();
                if (player->delobject(currentobject->object->getID()) > 0)
                    return 1;
                if (player->position->addobject(currentobject->object) > 0)
                    return 1;
                if (currentobject->object->logics.addvalue("USAGE", "Use", 0) > 0)
                    LOG_ERROR("Unable to add USAGE->Use Logic");
                if (player->position->pvsend(pvulture.server, "[reset]\t[green]cadendo lascia a terra %s[n]", objectname = pvulture.objects.getcompletename(currentobject->object)) > 0)
                    return 1;
                if (objectname) {
                    pvfree(objectname);
                    objectname = NULL;
                }
            }
            if (!player->position->getplayer(player->getID())) {
                if (player->position->addplayer(player) > 0)
                    return 1;
            }
            if (player->position->spvsend(pvulture.server, sshell) > 0)
                return 1;
        } else if (player->pvsend(pvulture.server, "[reset][n][red]%s e' morto![n][red]\tpresto il suo corpo esanime ricevera' una degna sepoltura![n]", pvulture.characters.getsimplename(player)) > 0)
            return 1;
        if (buffer) {
            pvfree(buffer);
            buffer = NULL;
        }
        return -1;
    } else {
        if (player->logics.hasvalue("STATS", "Stunned") == 0) {
            pvulture.stopwatch.pause();
            if (pvulture.stopwatch.getsecs(getvalue("STATS", "Stunned", player->logics, 0)) >= timestun) {
                if (player->logics.delvalue("STATS", "Stunned") > 0)
                    LOG_ERROR("Unable to delete STATS->Stunned Logic");
                else if (player->logics.addvalue("STATS", "LPoints", 10) > 0)
                    LOG_ERROR("Unable to add STATS->LPoints Logic");
                else {
                    if (player->position->getplayer(player->getID())) {
                        if (player->position->delplayer(player->getID()) > 0)
                            return 1;
                    }
                    if (player->pvsend(pvulture.server, "[reset][n][green]recuperi i sensi![n]") > 0)
                        return 1;
                    if (spvsend(pvulture.server, player->position, "[reset][n][yellow]$name recupera i sensi![n]", (Ccharacter *) player) > 0)
                        return 1;
                    if (!player->position->getplayer(player->getID())) {
                        if (player->position->addplayer(player) > 0)
                            return 1;
                    }
                    if (player->position->spvsend(pvulture.server, sshell) > 0)
                        return 1;
                }
            } else if (infos.message) {
                if (player->pvsend(pvulture.server, "[reset][red]sei a terra priv%s di sensi, non c'e' nulla che tu possa fare![n]", (player->getsex() != MALE) ? "a" : "o") > 0)
                    return 1;
                if (player->spvsend(pvulture.server, sshell) > 0)
                    return 1;
            }
        } else {
            if (player->logics.addvalue("STATS", "Stunned", (int) pvulture.stopwatch.pause()) > 0)
                LOG_ERROR("Unable to add STATS->Stunned Logic");
            else {
                if (player->position->getplayer(player->getID())) {
                    if (player->position->delplayer(player->getID()) > 0)
                        return 1;
                }
                if (player->pvsend(pvulture.server, "[reset][n][red]cadi a terra priv%s di sensi![n]", (player->getsex() != MALE) ? "a" : "o") > 0)
                    return 1;
                if (spvsend(pvulture.server, player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name cade a terra priv%s di sensi![n]", (player->getsex() != MALE) ? "a" : "o")), (Ccharacter *) player) > 0)
                    return 1;
                if (!player->position->getplayer(player->getID())) {
                    if (player->position->addplayer(player) > 0)
                        return 1;
                }
                if (player->position->spvsend(pvulture.server, sshell) > 0)
                    return 1;
            }
        }
        if (buffer) {
            pvfree(buffer);
            buffer = NULL;
        }
    }
    return 0;
}

int PVpersonalevents::blood(Cplayer *player) {
    pvulture.stopwatch.pause();
    if (pvulture.stopwatch.getsecs(getvalue("BLOOD", "Last", player->logics, 0)) >= timeblood) {
        if (player->logics.addvalue("BLOOD", "Last", (int) pvulture.stopwatch.pause()) > 0)
            LOG_ERROR("Unable to add BLOOD->Last Logic");
        if (setvalue("STATS", "LPoints", player->logics, getvalue("BLOOD", "Damage", player->logics, 1)*-1) > 0)
            LOG_ERROR("Unable to set STATS->LPoints Logic");
        if (player->position->getplayer(player->getID())) {
            if (player->position->delplayer(player->getID()) > 0)
                return 1;
        }
        if (player->pvsend(pvulture.server, "[reset][n][red]stai sangunando copiosamente! se non verrai curat%s al piu' presto morirai![n]", (player->getsex() != MALE) ? "a" : "o") > 0)
            return 1;
        if (spvsend(pvulture.server, player->position, "[reset][n][yellow]$name sta' sanguinando![n]", (Ccharacter *) player) > 0)
            return 1;
        if (!player->position->getplayer(player->getID())) {
            if (player->position->addplayer(player) > 0)
                return 1;
        }
        if (player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
    }
    return 0;
}

int PVpersonalevents::equip(Cplayer *player) {
    Cobject *object = NULL;
    objectslist *copy = NULL;
    FILE *configurationfile = NULL;
    char *fileobject = NULL, path[_DEFAULT_MIND_LEN];
    int objects = getvalue("NEWOBJECTS", "NULL", player->logics, 0);
    while (objects > 0) {
        if ((fileobject = player->logics.getvalue("NEWOBJECTS", --objects))) {
            snprintf(path, _DEFAULT_MIND_LEN, _PVFILES "objects/%s.do", fileobject);
            if ((configurationfile = fopen(path, "r"))) {
                if ((object = new(Cobject)) && (object->load(configurationfile) == 0)) {
                    if ((copy = pvulture.objects.gameobjects.addobject(object))) {
                        if ((pvulture.objects.getweight(copy->object) + pvulture.objects.getweight(player)) <= getvalue("STATS", "WPoints", player->logics, 0)) {
                            if (player->addobject(copy->object) > 0)
                                return 1;
                        } else if (player->position->addobject(copy->object) > 0)
                            return 1;
                    } else
                        return 1;
                    object->unload();
                    delete(object);
                } else
                    LOG_ERROR("Unable to read TEMPLATE %s", fileobject);
                fclose(configurationfile);
            } else
                LOG_ERROR("Unable to find TEMPLATE %s", fileobject);
        }
    }
    player->logics.delcategory("NEWOBJECTS");
    return 0;
}

int PVenvironmentsevents::weather(void) {
    tileslist *tileslist = NULL;
    zoneslist *zoneslist = pvulture.map.gamemap.getzonesroot();
    int randomize, present, climate;
    char string[_DEFAULT_MIND_LEN], buffer[_DEFAULT_MAXD_LEN];
    pvulture.stopwatch.pause();
    if ((events.hasvalue("climate") != 0) || (pvulture.stopwatch.getsecs(events.getvalue("climate")) >= timeclimate)) {
        if (events.addvalue("climate", (int) pvulture.stopwatch.pause()) > 0)
            LOG_ERROR("Unable to add Climate Logic");
        pvulture.time.update();
        snprintf(string, _DEFAULT_MIND_LEN, "CLIMATE:%d", pvulture.time.pdate.month);
        while (zoneslist) {
            randomize = (rand() % 6) + 1;
            switch (randomize + 1) {
                case 1: climate = getvalue(string, "Sunny", zoneslist->zone->logics, 0);
                    break;
                case 2: climate = getvalue(string, "Rainy", zoneslist->zone->logics, 0);
                    break;
                case 3: climate = getvalue(string, "Foggy", zoneslist->zone->logics, 0);
                    break;
                case 4: climate = getvalue(string, "Snowy", zoneslist->zone->logics, 0);
                    break;
                case 5: climate = getvalue(string, "Cloudy", zoneslist->zone->logics, 0);
                    break;
                case 6: climate = getvalue(string, "Stormy", zoneslist->zone->logics, 0);
                    break;
            }
            if (climate > 0) {
                present = getvalue("WEATHER", "Current", zoneslist->zone->logics, 0);
                if (present != randomize) {
                    if (zoneslist->zone->logics.addvalue("WEATHER", "Current", randomize) > 0)
                        LOG_ERROR("Unable to add WEATHER->Current Logic");
                    switch (present) {
                        case 1: strcpy(buffer, "[yellow]lentamente il cielo si oscura[n][yellow]");
                            break;
                        case 2: strcpy(buffer, "[yellow]la pioggia rapidamente cessa di cadere[n][yellow]");
                            break;
                        case 3: strcpy(buffer, "[yellow]la fitta nebbia si dirada lentamente[n][yellow]");
                            break;
                        case 4: strcpy(buffer, "[yellow]la neve cessa di cadere lentamente[n][yellow]");
                            break;
                        case 5: strcpy(buffer, "[yellow]le nuvole spariscono velocemente[n][yellow]");
                            break;
                        case 6: strcpy(buffer, "[yellow]la tempesta si placa[n][yellow]");
                            break;
                    }
                    switch (randomize) {
                        case 1:
                            if ((pvulture.time.ptime.hour < 5) || (pvulture.time.ptime.hour > 20))
                                strcat(buffer, "\te la luna spunta in alto nel cielo...");
                            else
                                strcat(buffer, "\te il sole spunta alto nel cielo...");
                            break;
                        case 2: strcat(buffer, "\te una fitta pioggia inizia a scendere dall'alto...");
                            break;
                        case 3: strcat(buffer, "\te una pesante cappa di nebbia si abbatte al suolo...");
                            break;
                        case 4: strcat(buffer, "\te una candida neve inizia a posarsi a terra...");
                            break;
                        case 5: strcat(buffer, "\te delle nuvole minacciose riempiono il cielo...");
                            break;
                        case 6: strcat(buffer, "\te una tempesta sprigiona la sua furia...");
                            break;
                    }
                } else {
                    switch (present) {
                        case 1:
                            if ((pvulture.time.ptime.hour < 5) || (pvulture.time.ptime.hour > 20))
                                strcpy(buffer, "[yellow]la luna continua a brillare in cielo...");
                            else
                                strcpy(buffer, "[yellow]il sole continua a brillare in cielo...");
                            break;
                        case 2: strcpy(buffer, "[yellow]la pioggia continua in perterrito a cadere...");
                            break;
                        case 3: strcpy(buffer, "[yellow]la nebbia non sembra avere intenzione di diradarsi...");
                            break;
                        case 4: strcpy(buffer, "[yellow]la neve continua a scendere rapidamente...");
                            break;
                        case 5: strcpy(buffer, "[yellow]il cielo e' ora completamente coperto da minacciose nubi...");
                            break;
                        case 6: strcpy(buffer, "[yellow]la tempesta prosegue il suo interminabile ciclo...");
                            break;
                    }
                }
                tileslist = zoneslist->zone->gettilesroot();
                while (tileslist) {
                    if (getvalue("SYSTEM", "Internal", tileslist->tile->logics, 0) != 1) {
                        if (tileslist->tile->pvsend(pvulture.server, "[reset][n]%s[n]", buffer) > 0)
                            return 1;
                        if (tileslist->tile->spvsend(pvulture.server, sshell) > 0)
                            return 1;
                    }
                    tileslist = tileslist->next;
                }

            }
            zoneslist = zoneslist->next;
        }
    }
    return 0;
}

int PVenvironmentsevents::stamina(void) {
    playerslist *playerslist = pvulture.characters.gamecharacters.playersroot;
    mobslist *mobslist = pvulture.characters.gamecharacters.mobsroot;
    int value = 0;
    pvulture.stopwatch.pause();
    if ((events.hasvalue("stamina") != 0) || (pvulture.stopwatch.getsecs(events.getvalue("stamina")) >= _ASTAMINA)) {
        if (events.addvalue("stamina", (int) pvulture.stopwatch.pause()) > 0)
            LOG_ERROR("Unable to add Stamina Logic");
        while (playerslist) {
            value = getvalue("STATS", "SPoints", playerslist->player->logics, 0);
            if (playerslist->player->logics.hasvalue("STATUS", "Seated") == 0)
                value += 2;
            else if (playerslist->player->logics.hasvalue("STATUS", "Stretched") == 0)
                value += 5;
            if (playerslist->player->logics.addvalue("STATS", "SPoints", (++value > 100) ? 100 : value) > 0)
                LOG_ERROR("Unable to add STATS->SPoints Logic");
            playerslist = playerslist->next;
        }
        while (mobslist) {
            value = getvalue("STATS", "SPoints", mobslist->mob->logics, 0);
            if (mobslist->mob->logics.hasvalue("STATUS", "Seated") == 0)
                value += 2;
            else if (mobslist->mob->logics.hasvalue("STATUS", "Stretched") == 0)
                value += 5;
            if (mobslist->mob->logics.addvalue("STATS", "SPoints", (++value > 100) ? 100 : value) > 0)
                LOG_ERROR("Unable to add STATS->SPoints Logic");
            mobslist = mobslist->next;
        }
    }
    return 0;
}

int PVenvironmentsevents::messages(void) {
    zoneslist *zoneslist = pvulture.map.gamemap.getzonesroot();
    int maxvalue;
    pvulture.stopwatch.pause();
    if ((events.hasvalue("messages") != 0) || (pvulture.stopwatch.getsecs(events.getvalue("messages")) >= timemessage)) {
        if (events.addvalue("messages", (int) pvulture.stopwatch.pause()) > 0)
            LOG_ERROR("Unable to add Messages Logic");
        while (zoneslist) {
            maxvalue = getvalue("MESSAGES", "NULL", zoneslist->zone->logics, 0) - 1;
            if (maxvalue > 0) {
                if (zoneslist->zone->pvsend(pvulture.server, "[reset][n][yellow]%s[n]", zoneslist->zone->logics.getvalue("MESSAGES", ((rand() % maxvalue) + 1))) > 0)
                    return 1;
                if (zoneslist->zone->spvsend(pvulture.server, sshell) > 0)
                    return 1;
            }
            zoneslist = zoneslist->next;
        }
    }
    return 0;
}

int PVenvironmentsevents::sunevent(void) {
    pvulture.time.update();
    if ((pvulture.time.ptime.hour == 6) && (events.hasvalue("sunrise") != 0)) {
        if (pvulture.characters.gamecharacters.pvsend(pvulture.server, "[reset][n][yellow]lentamente un nuovo sole sorge in cielo...[n]") > 0)
            return 1;
        if (pvulture.characters.gamecharacters.spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (events.addvalue("sunrise", 1) > 0)
            LOG_ERROR("Unable to add Sunrise Logic");
        if (events.hasvalue("sunset") == 0)
            events.delvalue("sunset");
    } else if ((pvulture.time.ptime.hour == 20) && (events.hasvalue("sunset") != 0)) {
        if (pvulture.characters.gamecharacters.pvsend(pvulture.server, "[reset][n][yellow]il sole lentamente cala dando spazio alle tenebre...[n]") > 0)
            return 1;
        if (pvulture.characters.gamecharacters.spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (events.addvalue("sunset", 1) > 0)
            LOG_ERROR("Unable to add Sunset Logic");
        if (events.hasvalue("sunrise") == 0)
            events.delvalue("sunrise");
    }
    return 0;
}

int PVenvironmentsevents::kicklist(void) {
    mobslist *mob = NULL;
    datalist *list = NULL;
    playerslist *player = NULL;
    kickid.resetvalue();
    while ((list = kickid.nextvalue())) {
        if ((compare.vcmpcase(list->completename, CSTRSIZE("Player")) == 0) && (player = pvulture.characters.gamecharacters.getplayer(list->value))) {
            if (compare.vcmpcase(infos.player->getaccount(), CSTRSIZE("###")) != 0) {
                player->player->logics.delvalue("STATUS", "Online");
                player->player->logics.delvalue("STATUS", "Last");
                if (player->player->logics.hascategory("FIGHT") == 0)
                    if (player->player->logics.delcategory("FIGHT") > 0)
                        LOG_ERROR("Unable to delete FIGHT Category");
                if (player->player->logics.hascategory("TIMES") == 0)
                    if (player->player->logics.delcategory("TIMES") > 0)
                        LOG_ERROR("Unable to delete TIMES Category");
                if (player->player->logics.hascategory("FOLLOW") == 0)
                    if (player->player->logics.delcategory("FOLLOW") > 0)
                        LOG_ERROR("Unable to delete FOLLOW Category");
                if (pvulture.characters.gamecharacters.saveplayer(player->player->getID(), _PVFILES "players/", pvulture.objects.gameobjects) > 0) return 1;
            }
            if (pvulture.server.unload(player->player->getsocketID()) > 0)
                return 1;
            if (player->player->position) {
                if (player->player->position->delplayer(player->player->getID()) > 0)
                    return 1;
            }
            if (pvulture.characters.gamecharacters.delplayer(player->player->getID()) > 0)
                return 1;
        } else if ((mob = pvulture.characters.gamecharacters.getmob(list->value))) {
            if (mob->mob->position) {
                if (mob->mob->position->delmob(mob->mob->getID()) > 0)
                    return 1;
            }
            if (pvulture.characters.gamecharacters.delmob(mob->mob->getID()) > 0)
                return 1;
        } else
            return 1;
    }
    kickid.unload();
    return 0;
}

PVintelligence intelligenceevents;
PVpersonalevents personalevents;
PVenvironmentsevents environmentevents;

int runenvironmentevents(void) {
    if (environmentevents.weather() > 0) {
        LOG_ERROR("Unable to run EVENTS.WEATHER()");
        return 1;
    }
    if (environmentevents.stamina() > 0) {
        LOG_ERROR("Unable to run EVENTS.STAMINA()");
        return 1;
    }
    if (environmentevents.messages() > 0) {
        LOG_ERROR("Unable to run EVENTS.MESSAGES()");
        return 1;
    }
    if (environmentevents.sunevent() > 0) {
        LOG_ERROR("Unable to run EVENTS.SUNEVENT()");
        return 1;
    }
    if (environmentevents.kicklist() > 0) {
        LOG_ERROR("Unable to run EVENTS.KICKLIST()");
        return 1;
    }
    return 0;
}

int runintelligenceevents(void) {
    if (intelligenceevents.fight(infos.mob) > 0) {
        LOG_ERROR("Unable to run INTELLIGENCEEVENTS.FIGHT()");
        return 1;
    }
    if (intelligenceevents.move(infos.mob) > 0) {
        LOG_ERROR("Unable to run INTELLIGENCEEVENTS.MOVE()");
        return 1;
    }
    if (((rand() + 1) % 10) > 4) {
        if (intelligenceevents.messages(infos.mob) > 0) {
            LOG_ERROR("Unable to run INTELLIGENCEEVENTS.MESSAGES()");
            return 1;
        }
    } else if (intelligenceevents.emoticons(infos.mob) > 0) {
        LOG_ERROR("Unable to run INTELLIGENCE.EMOTICONS()");
        return 1;
    }
    return 0;
}

int runintelligence(void) {
    if (compare.vcmpcase(infos.message, CSTRSIZE("richiama")) == 0) {
        if (intelligenceevents.recall() > 0)
            LOG_ERROR("Unable to run OBJECTS.TAKE()");
    }
    return 0;
}
