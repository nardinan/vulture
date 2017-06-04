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
#include "PVFight.h"

int PVfight::attack(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if (getvalue("STATUS", "Peaceful", infos.player->position->logics, 0) != 1) {
                if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                    value = attack(player);
                } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                    value = attack(mob);
                } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset][green]qui deve regnare la pace![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nome![n]") > 0) return 1;
    } else return 1;
    infos.player->stopwatch.start();
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

int PVfight::attack(Cplayer *player) {
    char *playername = NULL, *targetname = NULL;
    if (player->getID() != infos.player->getID()) {
        if ((infos.player->logics.hasvalue("STATUS", "Seated") != 0) && (infos.player->logics.hasvalue("STATUS", "Stretched") != 0)) {
            if (infos.player->logics.hascategory("FIGHT") != 0) {
                if (infos.player->logics.addcategory("FIGHT") > 0) LOG_ERROR("Unable to add FIGHT Category");
                else if ((infos.player->logics.addvalue("FIGHT", "Target", player->getID()) > 0) ||
                        (infos.player->logics.addvalue("FIGHT", "Kind", (int) PLAYER) > 0) ||
                        (infos.player->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) ||
                        (infos.player->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) ||
                        (infos.player->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause()) > 0)) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
            } else {
                if (infos.player->logics.getvalue("FIGHT", "Target") != player->getID()) {
                    if (infos.player->logics.addvalue("FIGHT", "Target", player->getID()) > 0) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][red]cambi bersaglio![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]sei gia' in combattimento con %s![n]", (player->getsex() != MALE) ? "lei" : "lui") > 0) return 1;
            }
            if (player->logics.hascategory("FIGHT") != 0) {
                if (player->logics.addcategory("FIGHT") > 0) LOG_ERROR("Unable to add FIGHT Category");
                else if ((player->logics.addvalue("FIGHT", "Target", infos.player->getID()) > 0) ||
                        (player->logics.addvalue("FIGHT", "Kind", (int) PLAYER) > 0) ||
                        (player->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) ||
                        (player->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) ||
                        (player->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause()) > 0)) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
            } else if (player->logics.addvalue("FIGHT", "Target", infos.player->getID()) > 0) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (infos.player->position->delplayer(player->getID()) > 0) return 1;
            if (spvsend(pvulture.server, infos.player->position, "[n][yellow]$name attacca $name![n]", (Ccharacter *) infos.player, (Ccharacter *) player) > 0) return 1;
            if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            if (!player->position->getplayer(player->getID())) {
                if (player->position->addplayer(player) > 0) return 1;
            }
            if (!infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->addplayer(infos.player) > 0) return 1;
            }
            if (((player->logics.hasvalue("STATUS", "Stretched") != 0) &&
                    (player->logics.hasvalue("STATUS", "Seated") != 0) &&
                    (infos.player->logics.hasvalue("STATUS", "Hide") != 0)) ||
                    (player->logics.hascategory("FIGHT") != 0)) {
                if (player->pvsend(pvulture.server, "[reset][n][red]%s ti attacca![n]", playername = pvulture.characters.gettargetname(infos.player, player)) > 0) return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]attacchi %s![n]", targetname = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
                if (player->spvsend(pvulture.server, sshell) > 0) return 1;
                if (playername) {
                    pvfree(playername);
                    playername = NULL;
                }
                if (targetname) {
                    pvfree(targetname);
                    targetname = NULL;
                }
            } else {
                shell = false;
                if (player->pvsend(pvulture.server, "[reset][n][red]%s ti attacca a sorpesa!", playername = pvulture.characters.gettargetname(infos.player, player)) > 0) return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]attacchi a sorpresa %s!", targetname = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
                if (round((Ccharacter *) infos.player, (Ccharacter *) player) > 0) return 1;
                if (playername) {
                    pvfree(playername);
                    playername = NULL;
                }
                if (targetname) {
                    pvfree(targetname);
                    targetname = NULL;
                }
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non e' consigliabile da terra[n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;
    return 0;
}

int PVfight::attack(Cmob *mob) {
    char *targetname = NULL;
    if ((infos.player->logics.hasvalue("STATUS", "Seated") != 0) && (infos.player->logics.hasvalue("STATUS", "Stretched") != 0)) {
        if (infos.player->logics.hascategory("FIGHT") != 0) {
            if (infos.player->logics.addcategory("FIGHT") > 0) LOG_ERROR("Unable to add FIGHT Category");
            else if ((infos.player->logics.addvalue("FIGHT", "Target", mob->getID()) > 0) ||
                    (infos.player->logics.addvalue("FIGHT", "Kind", (int) MOB) > 0) ||
                    (infos.player->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) ||
                    (infos.player->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) ||
                    (infos.player->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause()) > 0)) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
        } else {
            if (infos.player->logics.getvalue("FIGHT", "Target") != mob->getID()) {
                if (infos.player->logics.addvalue("FIGHT", "Target", mob->getID()) > 0) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
                if (infos.player->pvsend(pvulture.server, "[reset][red]cambi bersaglio![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]sei gia' in combattimento con %s![n]", (mob->getsex() != MALE) ? "lei" : "lui") > 0) return 1;
        }
        if (mob->logics.hascategory("FIGHT") != 0) {
            if (mob->logics.addcategory("FIGHT") > 0) LOG_ERROR("Unable to add FIGHT Category");
            else if ((mob->logics.addvalue("FIGHT", "Target", infos.player->getID()) > 0) ||
                    (mob->logics.addvalue("FIGHT", "Kind", (int) PLAYER) > 0) ||
                    (mob->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) ||
                    (mob->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) ||
                    (mob->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause()) > 0)) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
        } else if (mob->logics.addvalue("FIGHT", "Target", infos.player->getID()) > 0) LOG_ERROR("Unable to add FIGHT->Target/Mode/Hit Logic");
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
        }
        if (spvsend(pvulture.server, infos.player->position, "[n][yellow]$name attacca $name![n]", (Ccharacter *) infos.player, (Ccharacter *) mob) > 0) return 1;
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0) return 1;
        }
        if (((mob->logics.hasvalue("STATUS", "Stretched") != 0) &&
                (mob->logics.hasvalue("STATUS", "Seated") != 0) &&
                (infos.player->logics.hasvalue("STATUS", "Hide") != 0)) ||
                (mob->logics.hascategory("FIGHT") == 0)) {
            if (infos.player->pvsend(pvulture.server, "[reset][green]attacchi %s![n]", targetname = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
            if (targetname) {
                pvfree(targetname);
                targetname = NULL;
            }
        } else {
            shell = false;
            if (infos.player->pvsend(pvulture.server, "[reset][green]attacchi a sorpresa %s!", targetname = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
            if (targetname) {
                pvfree(targetname);
                targetname = NULL;
            }
            if (round((Ccharacter *) infos.player, (Ccharacter *) mob) > 0) return 1;
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non e' consigliabile da terra[n]") > 0) return 1;
    return 0;
}

int PVfight::stop(void) {
    if (infos.player->logics.hascategory("FIGHT") == 0) {
        if (infos.player->logics.delcategory("FIGHT") > 0) LOG_ERROR("Unable to add FIGHT Category");
        else {
            if (infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
            }
            if (infos.player->pvsend(pvulture.server, "[reset][yellow]smetti di combattere[n]") > 0) return 1;
            if (spvsend(pvulture.server, infos.player->position, "[n][yellow]$name smette di combattere[n]", infos.player) > 0) return 1;
            if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
            if (!infos.player->position->getplayer(infos.player->getID())) {
                if (infos.player->position->addplayer(infos.player) > 0) return 1;
            }
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non sei in combattimento[n]") > 0) return 1;
    return 0;
}

int PVfight::mode(void) {
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if (infos.player->logics.hascategory("FIGHT") == 0) {
                if (compare.vcmpcase(message, CSTRSIZE("berserk")) == 0) {
                    if (infos.player->logics.addvalue("FIGHT", "Mode", (int) BERSERK) > 0) LOG_ERROR("Unable to add FIGHT->Mode Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][green]entri in modalita' berserk![n]") > 0) return 1;
                } else if (compare.vcmpcase(message, CSTRSIZE("aggressivo")) == 0) {
                    if (infos.player->logics.addvalue("FIGHT", "Mode", (int) AGGRESSIVE) > 0) LOG_ERROR("Unable to add FIGHT->Mode Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][green]entri in modalita' aggressiva![n]") > 0) return 1;
                } else if (compare.vcmpcase(message, CSTRSIZE("difesa")) == 0) {
                    if (infos.player->logics.addvalue("FIGHT", "Mode", (int) DEFENSE) > 0) LOG_ERROR("Unable to add FIGHT->Mode Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][green]entri in modalita' difensiva![n]") > 0) return 1;
                } else if (compare.vcmpcase(message, CSTRSIZE("normale")) == 0) {
                    if (infos.player->logics.addvalue("FIGHT", "Mode", (int) NORMAL) > 0) LOG_ERROR("Unable to add FIGHT->Mode Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][green]torni in modalita' normale![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]questa modalita' non esiste![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non sei in combattimento![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare una modalita'![n]") > 0) return 1;
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

int PVfight::position(void) {
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && ((message) ||
                (compare.vcmpcase(command, CSTRSIZE("decapita")) == 0) ||
                (compare.vcmpcase(command, CSTRSIZE("smetti")) == 0))) {
            if (infos.player->logics.hascategory("FIGHT") == 0) {
                if ((compare.vcmpcase(message, CSTRSIZE("testa")) == 0) || (compare.vcmpcase(command, CSTRSIZE("decapita")) == 0)) {
                    if (infos.player->logics.addvalue("FIGHT", "Hit", (int) HEAD) > 0) LOG_ERROR("Unable to add FIGHT->Hit Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][yellow]tenti di decapitare il nemico[n]") > 0) return 1;
                } else if (compare.vcmpcase(message, CSTRSIZE("braccia")) == 0) {
                    if (infos.player->logics.addvalue("FIGHT", "Hit", (int) ARMS) > 0) LOG_ERROR("Unable to add FIGHT->Hit Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][yellow]tenti di disarmare il nemico[n]") > 0) return 1;
                } else if (compare.vcmpcase(message, CSTRSIZE("gambe")) == 0) {
                    if (infos.player->logics.addvalue("FIGHT", "Hit", (int) LEGS) > 0) LOG_ERROR("Unable to add FIGHT->Hit Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][yellow]tenti di buttare a terra il nemico[n]") > 0) return 1;
                } else if ((compare.vcmpcase(message, CSTRSIZE("torso")) == 0) || (compare.vcmpcase(command, CSTRSIZE("smetti")) == 0)) {
                    if (infos.player->logics.addvalue("FIGHT", "Hit", (int) TORSO) > 0) LOG_ERROR("Unable to add FIGHT->Hit Logic");
                    if (infos.player->pvsend(pvulture.server, "[reset][yellow]cerchi ora semplicemente, di ferire il nemico[n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non puoi colpire questa parte![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non sei in combattimento![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare una parte del corpo![n]") > 0) return 1;
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

PVinfos *PVround::getnames(Ccharacter *attacker, Ccharacter *target, Cdatabank& events) {
    Cobject *object = NULL;
    PVinfos *backup = (PVinfos *) pvmalloc(sizeof (PVinfos));
    int weapon;
    char *buffer = NULL;
    if (backup) {
        backup->weaponname = NULL;
        backup->playername = NULL;
        backup->targetname = NULL;
        if (attacker->getkind() != MOB) {
            object = pvulture.objects.getweapon((char *) "*", (Cplayer *) attacker);
            if (target->getkind() != MOB) {
                backup->playername = pvulture.characters.gettargetname((Cplayer *) attacker, (Cplayer *) target);
                backup->targetname = pvulture.characters.gettargetname((Cplayer *) target, (Cplayer *) attacker);
            } else backup->targetname = pvulture.characters.gettargetname((Cmob *) target, (Cplayer *) attacker);
        } else {
            object = pvulture.objects.getweapon((char *) "*", (Cmob *) attacker);
            if (target->getkind() != MOB) backup->playername = pvulture.characters.gettargetname((Cmob *) attacker, (Cplayer *) target);
        }
        if ((object) && (object->logics.hasvalue("WEAPON", "DI") != 0)) {
            backup->weaponname = pvulture.objects.getcompletename(object);
        } else {
            if (events.hasvalue("Weapon") == 0) {
                weapon = events.getvalue("Weapon");
                if ((buffer = attacker->logics.getvalue("WEAPONS", weapon))) {
                    if ((backup->weaponname = (char *) pvmalloc(strlen(buffer) + 1))) {
                        strcpy(backup->weaponname, buffer);
                        backup->weaponname[strlen(buffer)] = '\0';
                    } else return NULL;
                } else return NULL;
            } else {
                if ((backup->weaponname = (char *) pvmalloc(strlen(_AAWEAPON) + 1))) {
                    strcpy(backup->weaponname, _AAWEAPON);
                    backup->weaponname[strlen(_AAWEAPON)] = '\0';
                } else return NULL;
            }
        }
    } else return NULL;
    return backup;
}

int PVround::messages(Ccharacter *attacker, Ccharacter *target, Cdatabank& events) {
    Cplayer *backup = NULL;
    PVinfos *names = getnames(attacker, target, events);
    int damage = events.getvalue("Damage");
    char *player = NULL, *victim = NULL, *others = NULL;
    if (names) {
        if (events.hasvalue("Head") == 0) {
            allocate.vadd(&player, "[reset][n][red]colpisci alla testa %s con %s ", (names->targetname) ? names->targetname : "*", names->weaponname);
            allocate.vadd(&victim, "[reset][n][red]%s ti colpisce in testa con %s ", (names->playername) ? names->playername : "*", names->weaponname);
            allocate.vadd(&others, "[reset][n][yellow]$name colpisce $name in testa con %s ", names->weaponname);
            if (events.hasvalue("Block") != 0) {
                if (events.hasvalue("Parry") != 0) {
                    if (events.getvalue("Head") < 0) {
                        allocate.vadd(&player, "[bold]DECPITANDOL%s![n]", (target->getsex() == MALE) ? "O" : "A");
                        allocate.vadd(&victim, "[bold]DECPITANDOTI![n]");
                        allocate.vadd(&others, "[bold]DECPITANDOL%s![n]", (target->getsex() == MALE) ? "O" : "A");
                    } else {
                        if ((target->logics.hasvalue("RANK", "Immortal") == 0) && (target->logics.getvalue("RANK", "Immortal") == 1)) {
                            damage = 0;
                            allocate.vadd(&player, "ma non puo' essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "ma non puoi essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&others, "ma non puoi ferirl%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                        } else if (damage > 0) {
                            allocate.vadd(&player, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "ferendoti![n]");
                            allocate.vadd(&others, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        } else {
                            allocate.vadd(&player, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "non riuscendo pero' a ferirti![n]");
                            allocate.vadd(&others, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        }
                    }
                } else {
                    allocate.vadd(&player, "ma prontamente vieni respint%s![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&victim, "ma l%s respingi prontamente![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&others, "ma prontamente l%s respinge![n]", (attacker->getsex() == MALE) ? "o" : "a");
                }
            } else {
                allocate.vadd(&player, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&victim, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&others, "[reset][n][yellow]$name e $name si muovono, pronti a scattare![n]");

            }
        } else if (events.hasvalue("Arms") == 0) {
            allocate.vadd(&player, "[reset][n][red]colpisci al braccio %s con %s ", (names->targetname) ? names->targetname : "*", names->weaponname);
            allocate.vadd(&victim, "[reset][n][red]%s ti colpisce al braccio con %s ", (names->playername) ? names->playername : "*", names->weaponname);
            allocate.vadd(&others, "[reset][n][yellow]$name colpisce $name al braccio con %s ", names->weaponname);
            if (events.hasvalue("Block") != 0) {
                if (events.hasvalue("Parry") != 0) {
                    if (events.getvalue("Arms") < 0) {
                        allocate.vadd(&player, "[bold]riuscendo a disarmarl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        allocate.vadd(&victim, "[bold]facendoti cadere l'arma dalla mano![n][bold][red]SEI STATO DISARMAT%s![reset][red]Devi recuperare l'arma![n]", (target->getsex() == MALE) ? "O" : "A");
                        allocate.vadd(&others, "[bold]facendo%s cadere l'arma dalla mano![n]", (target->getsex() == MALE) ? "gli" : "le");
                    } else {
                        if ((target->logics.hasvalue("RANK", "Immortal") == 0) && (target->logics.getvalue("RANK", "Immortal") == 1)) {
                            damage = 0;
                            allocate.vadd(&player, "ma non puo' essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "ma non puoi essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&others, "ma non puoi ferirl%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                        } else if (damage > 0) {
                            allocate.vadd(&player, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "ferendoti![n]");
                            allocate.vadd(&others, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        } else {
                            allocate.vadd(&player, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "non riuscendo pero' a ferirti![n]");
                            allocate.vadd(&others, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        }
                    }
                } else {
                    allocate.vadd(&player, "ma prontamente vieni respint%s![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&victim, "ma l%s respingi prontamente![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&others, "ma prontamente l%s respinge![n]", (attacker->getsex() == MALE) ? "o" : "a");
                }
            } else {
                allocate.vadd(&player, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&victim, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&others, "[reset][n][yellow]$name e $name si muovono, pronti a scattare![n]");

            }
        } else if (events.hasvalue("Legs") == 0) {
            allocate.vadd(&player, "[reset][n][red]colpisci alle gambe %s con %s ", (names->targetname) ? names->targetname : "*", names->weaponname);
            allocate.vadd(&victim, "[reset][n][red]%s ti colpisce alle gambe con %s ", (names->playername) ? names->playername : "*", names->weaponname);
            allocate.vadd(&others, "[reset][n][yellow]$name colpisce $name alle gambe con %s ", names->weaponname);
            if (events.hasvalue("Block") != 0) {
                if (events.hasvalue("Parry") != 0) {
                    if (events.getvalue("Legs") < 0) {
                        allocate.vadd(&player, "[bold]riuscendo a farl%s cadere a terra![n]", (target->getsex() == MALE) ? "o" : "a");
                        allocate.vadd(&victim, "[bold]facendoti perdere l'equilibrio![n]");
                        allocate.vadd(&others, "[bold]facendol%s cadere a terra![n]", (target->getsex() == MALE) ? "o" : "a");
                    } else {
                        if ((target->logics.hasvalue("RANK", "Immortal") == 0) && (target->logics.getvalue("RANK", "Immortal") == 1)) {
                            damage = 0;
                            allocate.vadd(&player, "ma non puo' essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "ma non puoi essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&others, "ma non puoi ferirl%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                        } else if (damage > 0) {
                            allocate.vadd(&player, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "ferendoti![n]");
                            allocate.vadd(&others, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        } else {
                            allocate.vadd(&player, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                            allocate.vadd(&victim, "non riuscendo pero' a ferirti![n]");
                            allocate.vadd(&others, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        }
                    }
                } else {
                    allocate.vadd(&player, "ma prontamente vieni respint%s![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&victim, "ma l%s respingi prontamente![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&others, "ma prontamente l%s respinge![n]", (attacker->getsex() == MALE) ? "o" : "a");
                }
            } else {
                allocate.vadd(&player, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&victim, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&others, "[reset][n][yellow]$name e $name si muovono, pronti a scattare![n]");
            }
        } else {
            allocate.vadd(&player, "[reset][n][red]colpisci %s con %s ", (names->targetname) ? names->targetname : "*", names->weaponname);
            allocate.vadd(&victim, "[reset][n][red]%s ti colpisce con %s ", (names->playername) ? names->playername : "*", names->weaponname);
            allocate.vadd(&others, "[reset][n][yellow]$name colpisce $name con %s ", names->weaponname);
            if (events.hasvalue("Block") != 0) {
                if (events.hasvalue("Parry") != 0) {
                    if ((target->logics.hasvalue("RANK", "Immortal") == 0) && (target->logics.getvalue("RANK", "Immortal") == 1)) {
                        damage = 0;
                        allocate.vadd(&player, "ma non puo' essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                        allocate.vadd(&victim, "ma non puoi essere ferit%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                        allocate.vadd(&others, "ma non puoi ferirl%s in quanto immortale![n]", (target->getsex() == MALE) ? "o" : "a");
                    } else if (damage > 0) {
                        allocate.vadd(&player, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        allocate.vadd(&victim, "ferendoti![n]");
                        allocate.vadd(&others, "ferendol%s![n]", (target->getsex() == MALE) ? "o" : "a");
                    } else {
                        allocate.vadd(&player, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                        allocate.vadd(&victim, "non riuscendo pero' a ferirti![n]");
                        allocate.vadd(&others, "non riuscendo pero' a ferirl%s![n]", (target->getsex() == MALE) ? "o" : "a");
                    }
                } else {
                    allocate.vadd(&player, "ma prontamente vieni respint%s![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&victim, "ma l%s respingi prontamente![n]", (attacker->getsex() == MALE) ? "o" : "a");
                    allocate.vadd(&others, "ma prontamente l%s respinge![n]", (attacker->getsex() == MALE) ? "o" : "a");
                }
            } else {
                allocate.vadd(&player, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&victim, "[reset][n][red]vi muovete pronti a scattare![n]");
                allocate.vadd(&others, "[reset][n][yellow]$name e $name si muovono, pronti a scattare![n]");
            }
        }
        if ((attacker->logics.hasvalue("STATUS", "Stretched") == 0) ||
                (attacker->logics.hasvalue("STATUS", "Seated") == 0)) {
            allocate.vadd(&player, "[bold][red]SEI A TERRA![reset][red] devi alzarti il prima possibile![n]");
        }
        if ((target->logics.hasvalue("STATUS", "Stretched") == 0) ||
                (target->logics.hasvalue("STATUS", "Seated") == 0)) {
            allocate.vadd(&victim, "[bold][red]SEI A TERRA![reset][red] devi alzati il prima possibile![n]");
        }
        if (getvalue("STATS", "SPoints", target->logics, 0) <= 0) {
            target->logics.addvalue("STATS", "SPoints", 0);
            allocate.vadd(&player, "[red]%s tuo avversari%s e' visibilmente stanco![n]", (target->getsex() == MALE) ? "il" : "la", (target->getsex() == MALE) ? "o" : "a");
            allocate.vadd(&victim, "[red]hai il fiato corto![n]");
        }
        if (getvalue("STATS", "SPoints", attacker->logics, 0) <= 0) {
            attacker->logics.addvalue("STATS", "SPoints", 0);
            allocate.vadd(&player, "[red]hai il fiato corto![n]");
            allocate.vadd(&victim, "[red]%s tuo avversari%s e' visibilmente stanco![n]", (attacker->getsex() == MALE) ? "il" : "la", (attacker->getsex() == MALE) ? "o" : "a");
        }
        if (getvalue("STATS", "LPoints", target->logics, 0) <= 0) {
            target->logics.addvalue("STATS", "LPoints", 0);
            allocate.vadd(&player, "[red]hai UCCISO %s![n]", (target->getsex() == MALE) ? "il tuo avversario" : "la tua avversaria");
            allocate.vadd(&victim, "[red]sei stato UCCISO dal%s![n]", (attacker->getsex() == MALE) ? " tuo avversario" : "la tua avversaria");
        }
        if (attacker->getkind() != MOB) {
            backup = (Cplayer *) attacker;
            if (backup->position->delplayer(backup->getID()) > 0) return 1;
            if (backup->pvsend(pvulture.server, player) > 0) return 1;
            if (backup->spvsend(pvulture.server, sshell) > 0) return 1;
        }
        if (target->getkind() != MOB) {
            backup = (Cplayer *) target;
            if (backup->position->delplayer(backup->getID()) > 0) return 1;
            if (backup->pvsend(pvulture.server, victim) > 0) return 1;
            if (backup->spvsend(pvulture.server, sshell) > 0) return 1;
        }
        if (spvsend(pvulture.server, attacker->position, others, attacker, target) > 0) LOG_ERROR("Unable to run SSEND()");
        if (attacker->position->spvsend(pvulture.server, sshell) > 0) return 1;
        if (target->getkind() != MOB) {
            if (target->position->addplayer(backup = (Cplayer *) target) > 0) return 1;
        }
        if (attacker->getkind() != MOB) {
            if (attacker->position->addplayer(backup = (Cplayer *) attacker) > 0) return 1;
        }
        if (victim) {
            pvfree(victim);
            victim = NULL;
        }
        if (player) {
            pvfree(player);
            player = NULL;
        }
        if (others) {
            pvfree(others);
            others = NULL;
        }
        if (names->playername) {
            pvfree(names->playername);
            names->playername = NULL;
        }
        if (names->targetname) {
            pvfree(names->targetname);
            names->targetname = NULL;
        }
        if (names->weaponname) {
            pvfree(names->weaponname);
            names->weaponname = NULL;
        }
        if (names) {
            pvfree(names);
            names = NULL;
        }
    } else return 1;
    return 0;
}

int PVround::attack(Cpoints& attacker, Cpoints& target) {
    emode mode = NORMAL;
    int offense = 0, defense = 0;
    defense = getvalue("CHARS", "Def", target, 0) +
            ((rand() + 1) % (getvalue("CHARS", "CO", target, 0) + 1)) +
            ((rand() + 1) % (getvalue("CHARS", "SP", target, 0) + 1)) +
            ((rand() + 1) % (getvalue("STATS", "SPoints", target, 0) + 1));
    if (getvalue("STATS", "SPoints", attacker, 0) > 0) {
        offense = getvalue("CHARS", "Att", attacker, 0) +
                ((rand() + 1) % (getvalue("CHARS", "ST", attacker, 0) + 1)) +
                ((rand() + 1) % (getvalue("CHARS", "SP", attacker, 0) + 1)) +
                ((rand() + 1) % (getvalue("STATS", "SPoints", attacker, 0) + 1));
        defense -= ((((rand() + 1) % 20) * (target.hasvalue("STATUS", "Seated") == 0)) ? 1 : 0);
        offense -= ((((rand() + 1) % 40) * (attacker.hasvalue("STATUS", "Seated") == 0)) ? 1 : 0);
        defense -= ((((rand() + 1) % 40) * (target.hasvalue("STATUS", "Stretched") == 0)) ? 1 : 0);
        offense -= ((((rand() + 1) % 60) * (attacker.hasvalue("STATUS", "Stretched") == 0)) ? 1 : 0);
        mode = (emode) getvalue("FIGHT", "Mode", attacker, 0);
        switch (mode) {
            case BERSERK: offense += ((rand() + 1) % (getvalue("CHARS", "ST", attacker, 0) + 1)) + ((rand() + 1) % (getvalue("CHARS", "CO", attacker, 0) + 1));
                break;
            case DEFENSE: offense -= ((rand() + 1) % (getvalue("CHARS", "ST", attacker, 0) + 1));
                break;
            case AGGRESSIVE: offense += ((rand() + 1) % (getvalue("CHARS", "CO", attacker, 0) + 1));
                break;
            case NORMAL: break;
        }
        mode = (emode) getvalue("FIGHT", "Mode", target, 0);
        switch (mode) {
            case BERSERK: defense -= ((rand() + 1) % (getvalue("CHARS", "DE", target, 0) + 1)) + ((rand() + 1) % (getvalue("CHARS", "ST", target, 0) + 1));
                break;
            case DEFENSE: defense += ((rand() + 1) % (getvalue("CHARS", "DE", target, 0) + 1));
                break;
            case AGGRESSIVE: defense -= ((rand() + 1) % (getvalue("CHARS", "CO", target, 0) + 1));
                break;
            case NORMAL: break;
        }
    } else return (defense * -1);
    return (offense - defense);
}

int PVround::escape(Cpoints& attacker, Cpoints& target) {
    int offense = 0, defense = 0;
    defense = ((rand() + 1) % (getvalue("CHARS", "DE", target, 0) + 1)) +
            ((rand() + 1) % (getvalue("CHARS", "SP", target, 0) + 1)) +
            ((rand() + 1) % (getvalue("STATS", "SPoints", target, 0) + 1)) +
            (getvalue("STATS", "Legs", target, 0));
    if (getvalue("STATS", "SPoints", attacker, 0) > 0) {
        offense = ((rand() + 1) % (getvalue("CHARS", "DE", attacker, 0) + 1)) +
                ((rand() + 1) % (getvalue("CHARS", "SP", attacker, 0) + 1)) +
                ((rand() + 1) % (getvalue("STATS", "SPoints", attacker, 0) + 1)) +
                (getvalue("STATS", "Arms", attacker, 0));
        if ((target.hasvalue("STATUS", "Seated") == 0) || (target.hasvalue("STATUS", "Stretched") == 0)) defense = 0;
        if ((attacker.hasvalue("STATUS", "Seated") == 0) || (attacker.hasvalue("STATUS", "Stretched") == 0)) offense = 0;
    } else return defense;
    return (offense - defense);
}

int PVround::damage(Cpoints& attacker, int value) {
    int result = 0;
    if (value > 0) {
        result = ((rand() + 1) % (getvalue("CHARS", "Dam", attacker, 0) + 1)) + (value % 10);
        if (attacker.hasvalue("STATUS", "Seated") == 0) result = numbers.vdiv(result, 20);
        else if (attacker.hasvalue("STATUS", "Stretched") == 0) result = numbers.vdiv(result, 40);
    }
    return result;
}

int PVround::disarm(Ccharacter *target, int value) {
    Cobject *object = NULL;
    int damage = (getvalue("STATS", "Arms", target->logics, 0) - value), lpoints = getvalue("STATS", "LPoints", target->logics, 0);
    if ((((rand() + 1) % (numbers.vdiv(damage, 2) + 1)) + value) >= damage) {
        if (target->getkind() != PLAYER) {
            if (pvulture.characters.setmalus((Cmob *) target, object) > 0) return 1;
        } else object = pvulture.objects.getweapon((char *) "*", (Cplayer *) target);
        if (object) {
            if (target->getkind() != PLAYER) {
                if (pvulture.characters.setmalus((Cmob *) target, object) > 0) return 1;
            } else if (pvulture.characters.setmalus((Cplayer *) target, object) > 0) return 1;
            if (object->logics.addvalue("USAGE", "Use", 0) > 0) return 1;
            if (target->delobject(object->getID()) > 0) return 1;
            if (target->position->addobject(object) > 0) return 1;
        }
    }
    if ((target->logics.hasvalue("RANK", "Immortal") != 0) || (target->logics.getvalue("RANK", "Immortal") != 1)) {
        if (target->logics.addvalue("STATS", "Arms", (damage >= 0) ? damage : 0) > 0) LOG_ERROR("Unable to add STATS->Arms Logic");
        if (target->logics.addvalue("STATS", "LPoints", ((lpoints - numbers.vdiv(value, 2)) >= 0) ? (lpoints - numbers.vdiv(value, 2)) : 0) > 0) LOG_ERROR("Unable to add STATS->LPoints Logic");
    }
    return (object) ? (-1) : 0;
}

int PVround::head(Ccharacter *target, int value) {
    int damage = getvalue("STATS", "LPoints", target->logics, 0);
    if ((((rand() + 1) % (numbers.vdiv(damage, 2) + 1)) + value) >= damage) {
        if ((((rand() + 1) % (getvalue("STATS", "CO", target->logics, 0) + 1)) + 1) <= 1) {
            if ((target->logics.hasvalue("RANK", "Immortal") != 0) || (target->logics.getvalue("RANK", "Immortal") != 1)) {
                if (target->logics.addvalue("STATS", "LPoints", 0) > 0) LOG_ERROR("Unable to add STATS->LPoints Logic");
                damage = -1;
            }
        }
    }
    return (damage < 0) ? (-1) : 0;
}

int PVround::legs(Ccharacter *target, int value) {
    bool down = false;
    int damage = (getvalue("STATS", "Legs", target->logics, 0) - value), lpoints = getvalue("STATS", "LPoints", target->logics, 0);
    if ((((rand() + 1) % (numbers.vdiv(damage, 2) + 1)) + value) >= damage) {
        target->logics.addvalue("STATUS", "Seated", 1);
        target->logics.delvalue("STATUS", "Stretched");
        down = true;
    }
    if ((target->logics.hasvalue("RANK", "Immortal") != 0) || (target->logics.getvalue("RANK", "Immortal") != 1)) {
        if (target->logics.addvalue("STATS", "Legs", (damage >= 0) ? damage : 0) > 0) LOG_ERROR("Unable to add STATS->Arms Logic");
        if (target->logics.addvalue("STATS", "LPoints", ((lpoints - numbers.vdiv(value, 2)) >= 0) ? (lpoints - numbers.vdiv(value, 2)) : 0) > 0) LOG_ERROR("Unable to add STATS->LPoints Logic");
    }
    return (down) ? (-1) : 0;
}

PVfight fight;
PVround attack;

int round(Ccharacter *attacker, Ccharacter *target) {
    Cdatabank data;
    Cobject *weapon = NULL;
    etarget position = (etarget) getvalue("FIGHT", "Hit", attacker->logics, 0);
    int factor = attack.attack(attacker->logics, target->logics), injury = attack.damage(attacker->logics, factor), lpoints = 0;
    if (getvalue("STATS", "LPoints", target->logics, 0) > 0) {
        if (attacker->logics.hasvalue("STATUS", "Hide") == 0) {
            if (attacker->logics.delvalue("STATUS", "Hide") > 0) LOG_ERROR("Unable to delete STATUS->Hide Logic");
        }
        if (target->logics.hasvalue("STATUS", "Hide") == 0) {
            if (target->logics.delvalue("STATUS", "Hide") > 0) LOG_ERROR("Unable to delete STATUS->Hide Logic");
        }
        if (data.addvalue("Damage", injury) > 0) LOG_ERROR("Unable to add Damage Logic");
        if (!(weapon = pvulture.objects.getweapon((char *) "*", attacker)))
            if (data.addvalue("Weapon", ((rand() + 1) % getvalue("WEAPONS", "NULL", attacker->logics, 0))) > 0) LOG_ERROR("Unable to get WEAPONS->Null Logic");
        switch (position) {
            case HEAD: if (data.addvalue("Head", attack.head(target, injury)) > 0) LOG_ERROR("Unable to add Head Logic");
                break;
            case ARMS:
                if (data.addvalue("Arms", attack.disarm(target, injury)) > 0) LOG_ERROR("Unable to add Arms Logic");
                else if ((getvalue("STATS", "Arms", target->logics, 0) < _AAMINDAM) &&
                        (target->logics.hascategory("BLOOD") != 0)) {
                    if ((weapon) && (weapon->logics.hasvalue("WEAPON", "CU") == 0)) {
                        if (((rand() + 1) % (injury < 1 ? 1 : injury)) > numbers.vdiv(injury, 2)) {
                            if (target->logics.addcategory("BLOOD") > 0) LOG_ERROR("Unable to add BLOOD Category");
                            else if (target->logics.addvalue("BLOOD", "Last", (int) pvulture.stopwatch.pause() - timeblood) > 0) LOG_ERROR("Unable to add BLOOD->Last Logic");
                            else if (target->logics.addvalue("BLOOD", "Damage", 1) > 0) LOG_ERROR("Unable to add BLOOD->Damage Logic");
                        }
                    }
                }
                break;
            case LEGS:
                if (data.addvalue("Legs", attack.legs(target, injury)) > 0) LOG_ERROR("Unable to add Legs Logic");
                else if ((getvalue("STATS", "Legs", target->logics, 0) < _AAMINDAM) &&
                        (target->logics.hascategory("BLOOD") != 0)) {
                    if ((weapon) && (weapon->logics.hasvalue("WEAPON", "CU") == 0)) {
                        if (((rand() + 1) % (injury < 1 ? 1 : injury)) > numbers.vdiv(injury, 2)) {
                            if (target->logics.addcategory("BLOOD") > 0) LOG_ERROR("Unable to add BLOOD Category");
                            else if (target->logics.addvalue("BLOOD", "Last", (int) pvulture.stopwatch.pause() - timeblood) > 0) LOG_ERROR("Unable to add BLOOD->Last Logic");
                            else if (target->logics.addvalue("BLOOD", "Damage", 1) > 0) LOG_ERROR("Unable to add BLOOD->Damage Logic");
                        }
                    }
                }
                break;
            default:
                lpoints = (getvalue("STATS", "LPoints", target->logics, 0) - injury);
                if (data.addvalue("Normal", injury) > 0) LOG_ERROR("Unable to add Damage Logic");
                else if ((lpoints < _AAMINDAM) &&
                        (target->logics.hascategory("BLOOD") != 0)) {
                    if ((weapon) && (weapon->logics.hasvalue("WEAPON", "CU") == 0)) {
                        if ((numbers.vmin(rand(), 1) % (injury < 1 ? 1 : injury)) > numbers.vdiv(injury, 2)) {
                            if (target->logics.addcategory("BLOOD") > 0) LOG_ERROR("Unable to add BLOOD Category");
                            else if (target->logics.addvalue("BLOOD", "Last", (int) pvulture.stopwatch.pause() - timeblood) > 0) LOG_ERROR("Unable to add BLOOD->Last Logic");
                            else if (target->logics.addvalue("BLOOD", "Damage", 1) > 0) LOG_ERROR("Unable to add BLOOD->Damage Logic");
                        }
                    }
                }
                if ((target->logics.hasvalue("RANK", "Immortal") != 0) || (target->logics.getvalue("RANK", "Immortal") != 1))
                    if (target->logics.addvalue("STATS", "LPoints", lpoints) > 0) LOG_ERROR("Unable to add STATS->LPoints Logic");
        }
        if (setvalue("STATS", "SPoints", attacker->logics, pointstamina*-1) > 0) LOG_ERROR("Unable to set SYSTEM->SPoints Logic");
        if (factor < 0) {
            if (data.addvalue("Parry", factor) > 0) LOG_ERROR("Unable to add Parry Logic");
        } else if (factor == 0) {
            if (data.addvalue("Block", 0) > 0) LOG_ERROR("Unable to add Block Logic");
        }
        if (attack.messages(attacker, target, data) > 0) return 1;
        data.unload();
    } else if (attacker->logics.delcategory("FIGHT") > 0) LOG_ERROR("Unable to delete FIGHT Category");
    return 0;
}

int escape(Ccharacter *escaper, Ccharacter *target) {
    Cplayer *backup = NULL;
    char *direction = escaper->position->randomize();
    int factor = attack.escape(escaper->logics, target->logics);
    if ((infos.player->logics.hasvalue("STATUS", "Seated") != 0) &&
            (infos.player->logics.hasvalue("STATUS", "Stretched") != 0)) {
        if (factor > 0) {
            if (escaper->getkind() != MOB) {
                backup = (Cplayer *) escaper;
                if (backup->pvsend(pvulture.server, "[reset][green]provi a fuggire via dal combattimento ...[n]") > 0) return 1;
                if (direction) {
                    if (movementcommands.movement(backup, direction, true) > 0) return 1;
                } else if (backup->pvsend(pvulture.server, "\t[reset][green]... ma sei in trappola![n]") > 0) return 1;
            } else if (direction) {
                if (movementcommands.movement(backup, direction, true) > 0) return 1;
            }
            if (escaper->logics.hascategory("FIGHT") == 0) {
                if (escaper->logics.delcategory("FIGHT") > 0) LOG_ERROR("Unable to delete FIGHT Category");
            }
            if (target->logics.hascategory("FIGHT") == 0) {
                if (target->logics.delcategory("FIGHT") > 0) LOG_ERROR("Unable to delete FIGHT Category");
            }
        } else {
            if (escaper->getkind() != MOB) {
                backup = (Cplayer *) escaper;
                if (backup->pvsend(pvulture.server, "[reset][green]non riesci a fuggire dal combattimento e cadi a terra![n]") > 0) return 1;
            }
            if (target->getkind() != MOB) {
                backup = (Cplayer *) target;
                if (backup->pvsend(pvulture.server, "[reset][n][green]il tuo avversario cade goffamente a terra![n]") > 0) return 1;
            }
            if (escaper->logics.hasvalue("STATUS", "Seated") == 0) {
                if (escaper->logics.delvalue("STATUS", "Seated") > 0) LOG_ERROR("Unable to delete STATUS->Seated Logic");
            }
            if (escaper->logics.addvalue("STATUS", "Stretched", 1) > 0) LOG_ERROR("Unable to add STATUS->Stretched Logic");
            if (round(target, escaper) > 0) return 1;
            shell = false;
        }
        if (setvalue("STATS", "SPoints", escaper->logics, pointstamina*-5) > 0) LOG_ERROR("Unable to add SYSTEM->SPoints Logic");
    } else {
        if (escaper->getkind() != MOB) {
            backup = (Cplayer *) escaper;
            if (backup->pvsend(pvulture.server, "[reset]sei a terra, non puoi fuggire![n]") > 0) return 1;
        }
    }
    return 0;
}

int round(Ccharacter *attacker) {
    mobslist *mob = NULL;
    Cplayer *backup = NULL;
    playerslist *player = NULL;
    int speed = getvalue("CHARS", "SP", attacker->logics, 0), character = getvalue("FIGHT", "Target", attacker->logics, 0), value = 0;
    pvulture.stopwatch.pause();
    if (pvulture.stopwatch.getsecs(getvalue("FIGHT", "Last", attacker->logics, 0) - (numbers.vdiv(speed * (timeattack - 1), 100))) >= timeattack) {
        if (getvalue("STATUS", "Peaceful", attacker->position->logics, 0) == 0) {
            attacker->logics.addvalue("FIGHT", "Last", (int) pvulture.stopwatch.pause());
            if (getvalue("FIGHT", "Kind", attacker->logics, 0) != MOB) {
                if ((player = attacker->position->getplayer(character))) {
                    value = round(attacker, (Ccharacter *) player->player);
                } else {
                    attacker->logics.delcategory("FIGHT");
                }
            } else {
                if ((mob = attacker->position->getmob(character))) {
                    value = round(attacker, (Ccharacter *) mob->mob);
                } else {
                    attacker->logics.delcategory("FIGHT");
                }
            }
        } else {
            if (attacker->getkind() != MOB) {
                backup = (Cplayer *) attacker;
                if (backup->pvsend(pvulture.server, "[reset][n][green]qui deve regnare la pace![n]") > 0) return 1;
                if (backup->spvsend(pvulture.server, sshell) > 0) return 1;
            }
            if (attacker->logics.delcategory("FIGHT") > 0) LOG_ERROR("Unable to delete FIGHT Category");
        }
    }
    return value;
}

int escape(Ccharacter *escaper) {
    mobslist *mob = NULL;
    playerslist *player = NULL;
    int character = getvalue("FIGHT", "Target", escaper->logics, 0), value = 0;
    if (getvalue("FIGHT", "Kind", escaper->logics, 0) != MOB) {
        if ((player = escaper->position->getplayer(character))) {
            value = escape(escaper, (Ccharacter *) player->player);
        } else {
            escaper->logics.delcategory("FIGHT");
        }
    } else {
        if ((mob = escaper->position->getmob(character))) {
            value = escape(escaper, (Ccharacter *) mob->mob);
        } else {
            escaper->logics.delcategory("FIGHT");
        }
    }
    return value;
}

int runfight(void) {
    if ((compare.vcmpcase(infos.message, CSTRSIZE("uccidi")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("attacca")) == 0)) {
        if (fight.attack() > 0) LOG_ERROR("Unable to run FIGHT.ATTACK()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("pace")) == 0) {
        if (fight.stop() > 0) LOG_ERROR("Unable to run FIGHT.STOP()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("modo")) == 0) {
        if (fight.mode() > 0) LOG_ERROR("Unable to run FIGHT.MODE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("colpisci")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("decapita")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("smetti")) == 0)) {
        if (fight.position() > 0) LOG_ERROR("Unable to run FIGHT.POSITION()");
    } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
    return 0;
}

int runescape(void) {
    if (infos.player->logics.hascategory("FIGHT") == 0) return escape((Ccharacter *) infos.player);
    else if (infos.player->pvsend(pvulture.server, "[reset]non sei in combattimento![n]") > 0) return 1;
    return 0;
}
