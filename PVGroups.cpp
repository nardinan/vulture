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
#include "PVGroups.h"

int PVgroup::build(char *string) {
    if (infos.player->logics.hascategory("GROUP") != 0) {
        if (groups.hasvalue(string) != 0) {
            if (groups.addvalue(string, 1) > 0) 
                LOG_ERROR("Unable to add GROUP->%s Logic", message);
            else {
                if (infos.player->logics.addcategory("GROUP") > 0) 
                    LOG_ERROR("Unable to add GROUP Category");
                else {
                    if (infos.player->logics.addvalue("GROUP", "Admin", 1) > 0)
                        LOG_ERROR("Unable to add GROUP->Admin Logic");
                    else if (infos.player->logics.addvalue("GROUP", "Capo", 2) > 0) 
                        LOG_ERROR("Unable to add GROUP->Rule Logic");
                    else if (infos.player->logics.addvalue("GROUP", string, 3) > 0) 
                        LOG_ERROR("Unable to add GROUP->Name Logic");
                    else if (infos.player->pvsend(pvulture.server, "[reset][green]hai fondato il gruppo \"%s\"[n]", string) > 0) 
                        return 1;
                }
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]esiste gia' un'altro gruppo con lo stesso nome![n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]fai gia' parte di un gruppo![n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::add(char *string) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    char *charactername = NULL;
    if (string) {
        if ((player = pvulture.characters.getplayer(string, infos.player->position))) {
            if (infos.player->getID() != player->getID()) {
                if (player->logics.hascategory("GROUP") != 0) {
                    if (player->logics.addcategory("GROUP") > 0) 
                        LOG_ERROR("Unable to add GROUP Category");
                    else {
                        if (player->logics.addvalue("GROUP", infos.player->logics.getvalue("GROUP", 3), 3) > 0) 
                            LOG_ERROR("Unable to add GROUP->Name Logic");
                        else if (player->logics.addvalue("GROUP", "Membro", 2) > 0) 
                            LOG_ERROR("Unable to add GROUP->Rule Logic");
                        else {
                            if (player->pvsend(pvulture.server, "[reset][n][green]sei ora membro del gruppo \"%s\"[n]", 
                                               infos.player->logics.getvalue("GROUP", 3)) > 0) 
                                return 1;
                            if (infos.player->pvsend(pvulture.server, "[reset][green]%s e' ora un membro del tuo gruppo[n]", 
                                                     charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) 
                                return 1;
                            if (charactername) {
                                pvfree(charactername);
                                charactername = NULL;
                            }
                            if (player->spvsend(pvulture.server, sshell) > 0) 
                                return 1;
                        }
                    }
                } else if (infos.player->pvsend(pvulture.server, "[reset]fa gia' parte di un'altro gruppo[n]") > 0) 
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset][reset]stai parlando di te stess%s![n]", 
                                            (infos.player->getsex() != MALE) ? "a" : "o") > 0) 
                return 1;
        } else if ((mob = pvulture.characters.getmob(string, infos.player->position))) {
            if (mob->logics.hascategory("GROUP") != 0) {
                if (mob->logics.addcategory("GROUP") > 0) 
                    LOG_ERROR("Unable to add GROUP Category");
                else {
                    if (mob->logics.addvalue("GROUP", infos.player->logics.getvalue("GROUP", 3), 3) > 0) 
                        LOG_ERROR("Unable to add GROUP->Name Logic");
                    else if (mob->logics.addvalue("GROUP", "Membro", 2) > 0) 
                        LOG_ERROR("Unable to add GROUP->Rule Logic");
                    else if (infos.player->pvsend(pvulture.server, "[reset][green]%s e' ora un membro del tuo gruppo[n]", 
                                                  charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) 
                        return 1;
                    if (charactername) {
                        pvfree(charactername);
                        charactername = NULL;
                    }
                }
            } else if (infos.player->pvsend(pvulture.server, "[reset]fa gia' parte di un'altro gruppo[n]") > 0) 
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::rule(char *string) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *message = NULL, *pointer = NULL, *rule = NULL;
    if (string) {
        if ((message = (char *) pvmalloc(strlen(string) + 1))) {
            strcpy(message, string);
            message[strlen(string)] = '\0';
            if ((pointer = strchr(message, ':'))) {
                for (rule = pointer + 1; *rule == ' '; rule++);
                do {
                    *pointer-- = '\0';
                } while ((pointer > message) && (*pointer == ' '));
                if ((strlen(rule) > 0) && (strlen(message) > 0)) {
                    if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                        value = this->rule(player, rule);
                    } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                        value = this->rule(mob, rule);
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) 
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare almeno un ruolo[n]") > 0) 
                    return 1;
            } else {
                value = this->rule(infos.player, message);
            }
        } else 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare almeno un ruolo[n]") > 0) 
        return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    return value;
}

int PVgroup::rule(Cplayer *player, char *string) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if ((player->logics.hascategory("GROUP") == 0) ||
                (player->logics.hasvalue("GROUP", infos.player->logics.getvalue("GROUP", 3)) == 0)) {
            if (player->logics.delvalue("GROUP", 2) > 0)
                LOG_ERROR("Unable to delete GROUP->Rule Logic");
            if (player->logics.addvalue("GROUP", string, 2) > 0) 
                LOG_ERROR("Unable to add GROUP RULE->Rule Logic");
            else {
                if (player->pvsend(pvulture.server, "[reset][n][green]sei ora nel gruppo con il ruolo di \"%s\"[n]", string) > 0) 
                    return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]%s ora ha il ruolo di \"%s\"[n]", charactername = pvulture.characters.gettargetname(player, infos.player), string) > 0) 
                    return 1;
                if (charactername) {
                    pvfree(charactername);
                    charactername = NULL;
                }
                if (player->spvsend(pvulture.server, sshell) > 0) 
                    return 1;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo gruppo![n]") > 0) 
            return 1;
    } else {
        if (infos.player->logics.delvalue("GROUP", 2) > 0) 
            LOG_ERROR("Unable to delete GROUP->Rule Logic");
        if (infos.player->logics.addvalue("GROUP", string, 2) > 0) 
            LOG_ERROR("Unable to add GROUP->Rule Logic");
        else if (infos.player->pvsend(pvulture.server, "[reset][green]hai modificato il tuo ruolo in \"%s\"[n]", message) > 0) 
            return 1;
    }
    return 0;
}

int PVgroup::rule(Cmob *mob, char *string) {
    char *charactername = NULL;
    if ((mob->logics.hascategory("GROUP") == 0) ||
            (mob->logics.hasvalue("GROUP", infos.player->logics.getvalue("GROUP", 3)) == 0)) {
        if (mob->logics.delvalue("GROUP", 2) > 0) 
            LOG_ERROR("Unable to delete GROUP->Rule Logic");
        if (mob->logics.addvalue("GROUP", string, 2) > 0) 
            LOG_ERROR("Unable to add GROUP->Rule Logic");
        else {
            if (infos.player->pvsend(pvulture.server, "[reset][green]%s ora ha il ruolo di \"%s\"[n]", 
                                     charactername = pvulture.characters.gettargetname(mob, infos.player), string) > 0) 
                return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo gruppo![n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::exit(void) {
    if (infos.player->logics.hasvalue("GROUP", "Admin") != 0) {
        if (infos.player->logics.delcategory("GROUP") > 0) LOG_ERROR("Unable to delete GROUP Category");
        else if (infos.player->pvsend(pvulture.server, "[reset][red]hai abbandonato il gruppo[n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]prima di abbandonare devi cedere il trono, o sciogliere direttamente il gruppo[n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::kick(char *string) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *charactername = string;
    if ((player = pvulture.characters.getplayer(charactername, infos.player->position))) {
        value = kick(player);
    } else if ((mob = pvulture.characters.getmob(charactername, infos.player->position))) {
        value = kick(mob);
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) 
        return 1;
    return value;
}

int PVgroup::kick(Cplayer *player) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if (player->logics.hasvalue("GROUP", infos.player->logics.getvalue("GROUP", 3)) == 0) {
            if ((player->logics.hasvalue("GROUP", "Admin") != 0) &&
                    ((player->logics.hasvalue("GROUP", "Moderator") != 0) || (infos.player->logics.hasvalue("GROUP", "Moderator") != 0))) {
                if (player->logics.delcategory("GROUP") > 0) 
                    LOG_ERROR("Unable to delete GROUP Category");
                else {
                    if (infos.player->pvsend(pvulture.server, "[reset][blue]%s non fa piu' parte del tuo gruppo[n]", 
                                             charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) 
                        return 1;
                    if (charactername) {
                        pvfree(charactername);
                        charactername = NULL;
                    }
                    if (player->pvsend(pvulture.server, "[reset][n][blue]sei stato cacciato dal gruppo da %s[n]", 
                                       charactername = pvulture.characters.gettargetname(infos.player, player)) > 0) 
                        return 1;
                    if (charactername) {
                        pvfree(charactername);
                        charactername = NULL;
                    }
                    if (player->spvsend(pvulture.server, sshell) > 0) 
                        return 1;
                }
            } else if (infos.player->pvsend(pvulture.server, "[reset]non hai sufficienti privilegi![n]") > 0) 
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]%s non fa parte del tuo gruppo![n]", 
                                        (player->getsex() != MALE) ? "lei" : "lui") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset][reset]stai parlando di te stess%s![n]", 
                                    (infos.player->getsex() != MALE) ? "a" : "o") > 0) 
        return 1;
    return 0;
}

int PVgroup::kick(Cmob *mob) {
    char *charactername = NULL;
    if (mob->logics.hasvalue("GROUP", infos.player->logics.getvalue("GROUP", 3)) == 0) {
        if ((mob->logics.hasvalue("GROUP", "Moderator") != 0) || (infos.player->logics.hasvalue("GROUP", "Moderator") != 0)) {
            if (mob->logics.delcategory("GROUP") > 0) LOG_ERROR("Unable to delete GROUP Category");
            else {
                if (infos.player->pvsend(pvulture.server, "[reset][blue]%s non fa piu' parte del tuo gruppo[n]",
                                         charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) 
                    return 1;
                if (charactername) {
                    pvfree(charactername);
                    charactername = NULL;
                }
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non hai sufficienti privilegi![n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]%s non fa parte del tuo gruppo![n]", 
                                    (mob->getsex() != MALE) ? "lei" : "lui") > 0) 
        return 1;
    return 0;
}

int PVgroup::transfer(char *string) {
    Cplayer *player = NULL;
    char *charactername = string;
    if ((player = pvulture.characters.getplayer(charactername, infos.player->position))) {
        if (player->getID() != infos.player->getID()) {
            if ((player->logics.hascategory("GROUP") == 0) &&
                    (compare.vcmpcase(infos.player->logics.getvalue("GROUP", 3), player->logics.getvalue("GROUP", 3)) == 0)) {
                player->logics.delvalue("GROUP", "Moderator");
                player->logics.delvalue("GROUP", "User");
                if ((player->logics.addvalue("GROUP", "Admin", 1) > 0) ||
                        (player->logics.delvalue("GROUP", 2) > 0) ||
                        (player->logics.addvalue("GROUP", "Capitano", 2) > 0)) 
                    LOG_ERROR("Unable to add GROUP->Rule Logic");
                else if ((infos.player->logics.delvalue("GROUP", "Admin") > 0) ||
                        (infos.player->logics.addvalue("GROUP", "User", 1) > 0) ||
                        (infos.player->logics.delvalue("GROUP", 2) > 0) ||
                        (infos.player->logics.addvalue("GROUP", "Membro", 2) > 0)) 
                    LOG_ERROR("Unable to add GROUP->Rule Logic");
                else {
                    if (player->pvsend(pvulture.server, "[reset][n][blue]sei diventat%s l'admin del gruppo![n]", 
                                       (player->getsex() != MALE) ? "a" : "o") > 0) 
                        return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][blue]hai ceduto la tua carica di admin a %s[n]", 
                                             charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) 
                        return 1;
                    if (charactername) {
                        pvfree(charactername);
                        charactername = NULL;
                    }
                    if (player->spvsend(pvulture.server, sshell) > 0) 
                        return 1;
                }
            } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", 
                                            (infos.player->getsex() != MALE) ? "a" : "o") > 0) 
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo stesso gruppo![n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::moderator(char *string) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value = 0;
    if ((player = pvulture.characters.getplayer(string, infos.player->position))) {
        value = moderator(player);
    } else if ((mob = pvulture.characters.getmob(string, infos.player->position))) {
        value = moderator(mob);
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) 
        return 1;
    return value;
}

int PVgroup::moderator(Cplayer *player) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if ((player->logics.hascategory("GROUP") == 0) &&
                (compare.vcmpcase(infos.player->logics.getvalue("GROUP", 3), player->logics.getvalue("GROUP", 3)) == 0)) {
            if (player->logics.hasvalue("GROUP", "Moderator") != 0) {
                player->logics.delvalue("GROUP", "User");
                if ((player->logics.addvalue("GROUP", "Moderator", 1) > 0) ||
                        (player->logics.delvalue("GROUP", 2) > 0) ||
                        (player->logics.addvalue("GROUP", "Vice", 2) > 0)) 
                    LOG_ERROR("Unable to add/remove GROUP->Rule Logic");
                else {
                    if (infos.player->pvsend(pvulture.server, "[reset][blue]%s e' ora un vice[n]", 
                                             charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) 
                        return 1;
                    if (player->pvsend(pvulture.server, "[reset][n][blue]sei diventat%s vice del gruppo![n]", 
                                       (player->getsex() != MALE) ? "a una" : "o un") > 0) 
                        return 1;
                    if (player->spvsend(pvulture.server, sshell) > 0) 
                        return 1;
                }
            } else {
                player->logics.delvalue("GROUP", "Moderator");
                if ((player->logics.addvalue("GROUP", "User", 1) > 0) ||
                        (player->logics.delvalue("GROUP", 2) > 0) ||
                        (player->logics.addvalue("GROUP", "Membro", 2) > 0)) 
                    LOG_ERROR("Unable to add/remove GROUP->Rule Logic");
                else {
                    if (infos.player->pvsend(pvulture.server, "[reset][blue]%s non e' piu' %s vice[n]", 
                                             charactername = pvulture.characters.gettargetname(player, infos.player), 
                                             (player->getsex() != MALE) ? "una" : "un") > 0) 
                        return 1;
                    if (player->pvsend(pvulture.server, "[reset][n][blue]l'incarico di vice ti e' stato tolto![n]") > 0) 
                        return 1;
                    if (player->spvsend(pvulture.server, sshell) > 0) 
                        return 1;
                }
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo stesso gruppo![n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", 
                                    (infos.player->getsex() != MALE) ? "a" : "o") > 0) 
        return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    return 0;
}

int PVgroup::moderator(Cmob *mob) {
    char *charactername = NULL;
    if ((mob->logics.hascategory("GROUP") == 0) &&
            (compare.vcmpcase(infos.player->logics.getvalue("GROUP", 3), mob->logics.getvalue("GROUP", 3)) == 0)) {
        if (mob->logics.hasvalue("GROUP", "Moderator") != 0) {
            mob->logics.delvalue("GROUP", "User");
            if ((mob->logics.addvalue("GROUP", "Moderator", 1) > 0) ||
                    (mob->logics.delvalue("GROUP", 2) > 0) ||
                    (mob->logics.addvalue("GROUP", "Vice", 2) > 0)) 
                LOG_ERROR("Unable to add/remove GROUP->Rule Logic");
            else if (infos.player->pvsend(pvulture.server, "[reset][blue]%s e' ora %s vice[n]", 
                                          charactername = pvulture.characters.gettargetname(mob, infos.player), 
                                          (mob->getsex() != MALE) ? "una" : "un") > 0) 
                return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
        } else {
            mob->logics.delvalue("GROUP", "Moderator");
            if ((mob->logics.addvalue("GROUP", "User", 1) > 0) ||
                    (mob->logics.delvalue("GROUP", 2) > 0) ||
                    (mob->logics.addvalue("GROUP", "Membro", 2) > 0)) 
                LOG_ERROR("Unable to add/remove GROUP->Rule Logic");
            else if (infos.player->pvsend(pvulture.server, "[reset][blue]%s non e' piu' %s vice[n]", 
                                          charactername = pvulture.characters.gettargetname(mob, infos.player), 
                                          (mob->getsex() != MALE) ? "una" : "un") > 0) 
                return 1;
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo stesso gruppo![n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::bounce(char *string) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    if ((player = pvulture.characters.getplayer(string)) && (player->position)) {
        if (infos.player->getID() != player->getID()) {
            if ((player->logics.hascategory("GROUP") == 0) &&
                    (compare.vcmpcase(player->logics.getvalue("GROUP", 3), infos.player->logics.getvalue("GROUP", 3)) == 0)) {
                if ((infos.player->position->getID() != player->position->getID()) || (infos.player->position->getzoneID() != player->position->getzoneID())) {
                    if (infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->delplayer(infos.player->getID()) > 0) 
                            return 1;
                    }
                    if (infos.player->pvsend(pvulture.server, "[reset][green]ti muovi presso %s[n]", string) > 0) 
                        return 1;
                    if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                        if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name scompare in una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) 
                            return 1;
                        if (spvsend(pvulture.server, player->position, "[reset][n][yellow]$name appare da una nuvola di fumo[n]", (Ccharacter *) infos.player) > 0) 
                            return 1;
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) 
                            return 1;
                        if (player->position->spvsend(pvulture.server, sshell) > 0) 
                            return 1;
                    }
                    if (player->position->addplayer(infos.player) > 0) 
                        return 1;
                    if (environmentcommands.lookenvironment() > 0)
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) 
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo gruppo![n]") > 0) 
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset][reset]stai parlando di te stess%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) 
            return 1;
    } else if ((mob = pvulture.characters.getmob(message)) && (mob->position)) {
        if ((mob->logics.hascategory("GROUP") == 0) &&
                (compare.vcmpcase(mob->logics.getvalue("GROUP", 3), infos.player->logics.getvalue("GROUP", 3)) == 0)) {
            if ((infos.player->position->getID() != mob->position->getID()) || (infos.player->position->getzoneID() != mob->position->getzoneID())) {
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) 
                        return 1;
                }
                if (infos.player->pvsend(pvulture.server, "[reset][green]ti muovi presso %s[n]", string) > 0) 
                    return 1;
                if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                    if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name scompare in una nuvola di fumo[n]", 
                                (Ccharacter *) infos.player) > 0) 
                        return 1;
                    if (spvsend(pvulture.server, player->position, "[reset][n][yellow]$name appare da una nuvola di fumo[n]", 
                                (Ccharacter *) infos.player) > 0) 
                        return 1;
                    if (infos.player->position->spvsend(pvulture.server, sshell) > 0) 
                        return 1;
                    if (mob->position->spvsend(pvulture.server, sshell) > 0)
                        return 1;
                }
                if (mob->position->addplayer(infos.player) > 0) 
                    return 1;
                if (environmentcommands.lookenvironment() > 0) 
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]si trova proprio qui![n]") > 0) 
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]non fa parte del tuo gruppo![n]") > 0) 
            return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non c'e' nessuno con quel nome in questo mondo![n]") > 0) 
        return 1;
    return 0;
}

int PVgroup::chat(char *string) {
    playerslist *player = pvulture.characters.gamecharacters.playersroot;
    char *groupname = infos.player->logics.getvalue("GROUP", 3), *charactername = NULL, *emoticon = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(message, string);
        message[strlen(string)] = '\0';
        emoticon = getemoticon(&message);
        if (message) {
            while (player) {
                if ((infos.player->getID() != player->player->getID()) && (player->player->logics.hasvalue("GROUP", groupname) == 0)) {
                    if (player->player->pvsend(pvulture.server, "[n][reset][blue]%s vi dice %s: \"%s\"[reset][n]",
                            charactername = pvulture.characters.gettargetname(infos.player, player->player), 
                                               (emoticon) ? emoticon : "", message) > 0) 
                        return 1;
                    if (player->player->spvsend(pvulture.server, sshell) > 0) 
                        return 1;
                    if (charactername) {
                        pvfree(charactername);
                        charactername = NULL;
                    }
                }
                player = player->next;
            }
            if (infos.player->pvsend(pvulture.server, "[reset][blue]dici al tuo gruppo %s: \"%s\"[reset][n]", 
                                     (emoticon) ? emoticon : "", message) > 0) 
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un messaggio![n]") > 0) 
            return 1;
    } else 
        return 1;
    if (emoticon) {
        pvfree(emoticon);
        emoticon = NULL;
    }
    if (message) {
        pvfree(message);
        message = NULL;
    }
    return 0;
}

int PVgroup::list(void) {
    mobslist *mob = pvulture.characters.gamecharacters.mobsroot;
    playerslist *player = pvulture.characters.gamecharacters.playersroot;
    char *groupname = infos.player->logics.getvalue("GROUP", 3);
    if (infos.player->pvsend(pvulture.server, "membri del gruppo \"[bold]%s[reset]\" ora collegati:[n]", groupname) > 0) 
        return 1;
    while (player) {
        if ((player->player->getID() != infos.player->getID()) && (player->player->logics.hasvalue("GROUP", groupname) == 0)) {
            if (player->player->logics.hasvalue("STATUS", "Password") != 0) {
                if (infos.player->pvsend(pvulture.server, "\t[reset][green](online)[reset]%s(%d) %s[n]",
                        ((player->player->logics.hasvalue("GROUP", "Admin") != 0) ? ((player->player->logics.hasvalue("GROUP", "Moderator") != 0) ? "" : "[yellow]") : "[green]"),
                        player->player->getID(), pvulture.characters.getsimplename(player->player)) > 0) 
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "\t[reset][yellow](passwd)[reset]%s(%d) %s[n]",
                    ((player->player->logics.hasvalue("GROUP", "Admin") != 0) ? ((player->player->logics.hasvalue("GROUP", "Moderator") != 0) ? "" : "[yellow]") : "[green]"),
                    player->player->getID(), pvulture.characters.getsimplename(player->player)) > 0) 
                return 1;
        }
        player = player->next;
    }
    while (mob) {
        if (mob->mob->logics.hasvalue("GROUP", groupname) == 0)
            if (infos.player->pvsend(pvulture.server, "\t[reset][green](online)[reset]%s(%d) %s[n]",
                    (infos.player->logics.hasvalue("GROUP", "Moderator") != 0) ? "" : "[yellow]", mob->mob->getID(), 
                                     pvulture.characters.getsimplename(mob->mob)) > 0)
                return 1;
        mob = mob->next;
    }
    return 0;
}

int PVgroup::destroy(void) {
    mobslist *mob = pvulture.characters.gamecharacters.mobsroot;
    playerslist *player = pvulture.characters.gamecharacters.playersroot;
    char *groupname = infos.player->logics.getvalue("GROUP", 3);
    if (groups.delvalue(groupname) > 0) 
        LOG_ERROR("Unable to delete GROUPS->%s Logic", groupname);
    while (player) {
        if ((infos.player->getID() != player->player->getID()) && (player->player->logics.hasvalue("GROUP", groupname) == 0)) {
            if (player->player->logics.delcategory("GROUP") > 0) 
                LOG_ERROR("Unable to delete GROUP Category");
            if (player->player->pvsend(pvulture.server, "[n][blue]Il tuo gruppo e' stato sciolto![n]") > 0) 
                return 1;
            if (player->player->spvsend(pvulture.server, sshell) > 0) 
                return 1;
        }
        player = player->next;
    }
    while (mob) {
        if (mob->mob->logics.hasvalue("GROUP", groupname) == 0) {
            if (player->player->logics.delcategory("GROUP") > 0) 
                LOG_ERROR("Unable to delete GROUP Category");
        }
        mob = mob->next;
    }
    if (infos.player->logics.delcategory("GROUP") > 0)
        LOG_ERROR("Unable to delete GROUP Category");
    return 0;
}

int PVgroup::information(void) {
    mobslist *mob = pvulture.characters.gamecharacters.mobsroot;
    playerslist *player = pvulture.characters.gamecharacters.playersroot;
    int index = 0;
    char *groupname = infos.player->logics.getvalue("GROUP", 3);
    while (player) {
        if (player->player->logics.hasvalue("GROUP", groupname) == 0) 
            index++;
        player = player->next;
    }
    while (mob) {
        if (mob->mob->logics.hasvalue("GROUP", groupname) == 0) 
            index++;
        mob = mob->next;
    }
    if (infos.player->pvsend(pvulture.server, "[reset]fai parte del gruppo [blue]\"[bold]%s[reset][blue]\"[reset]",
                             infos.player->logics.getvalue("GROUP", 3)) > 0)
        return 1;
    if (compare.vcmpcase(infos.player->logics.getvalue("GROUP", 1), "Admin") == 0) {
        if (infos.player->pvsend(pvulture.server, " (di cui sei [green]il capo[reset]) ") > 0)
            return 1;
    } else if (compare.vcmpcase(infos.player->logics.getvalue("GROUP", 1), "Moderator") == 0) {
        if (infos.player->pvsend(pvulture.server, " (di cui sei [yellow]un vice[reset]) ") > 0) 
            return 1;
    }
    if (infos.player->pvsend(pvulture.server, "che conta %d membri![n]\t[reset]il tuo ruolo e' [blue]\"[bold]%s[reset][blue]\"[reset][n]", 
                             index, infos.player->logics.getvalue("GROUP", 2)) > 0) 
        return 1;
    return 0;
}
PVgroup groupcommands;

int group(void) {
    char *message = NULL, *command = NULL, *subcommand = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (subcommand = strings.vpop(&message))) {
            if (compare.vcmpcase(subcommand, CSTRSIZE("crea")) == 0) {
                if (message) {
                    if (groupcommands.build(message) > 0) 
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) 
                    return 1;
            } else if (infos.player->logics.hascategory("GROUP") == 0) {
                if (compare.vcmpcase(subcommand, CSTRSIZE("abbandona")) == 0) {
                    if (groupcommands.exit() > 0) 
                        return 1;
                } else if (compare.vcmpcase(subcommand, CSTRSIZE("lista")) == 0) {
                    if (groupcommands.list() > 0) 
                        return 1;
                } else if (compare.vcmpcase(subcommand, CSTRSIZE("info")) == 0) {
                    if (groupcommands.information() > 0) 
                        return 1;
                } else if (compare.vcmpcase(subcommand, CSTRSIZE("di")) == 0) {
                    if (message) {
                        if (groupcommands.chat(message) > 0) 
                            return 1;
                    } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un messaggio![n]") > 0) 
                        return 1;
                } else if ((infos.player->logics.hasvalue("GROUP", "Admin") == 0) || (infos.player->logics.hasvalue("GROUP", "Moderator") == 0)) {
                    if (compare.vcmpcase(subcommand, CSTRSIZE("caccia")) == 0) {
                        if (message) {
                            if (groupcommands.kick(message) > 0) 
                                return 1;
                        } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) return 1;
                    } else if (compare.vcmpcase(subcommand, CSTRSIZE("aggiungi")) == 0) {
                        if (message) {
                            if (groupcommands.add(message) > 0) 
                                return 1;
                        } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) return 1;
                    } else if (infos.player->logics.hasvalue("GROUP", "Admin") == 0) {
                        if (compare.vcmpcase(subcommand, CSTRSIZE("ruolo")) == 0) {
                            if (message) {
                                if (groupcommands.rule(message) > 0) 
                                    return 1;
                            } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un ruolo![n]") > 0) return 1;
                        } else if (compare.vcmpcase(subcommand, CSTRSIZE("moderatore")) == 0) {
                            if (message) {
                                if (groupcommands.moderator(message) > 0) return 1;
                            } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) return 1;
                        } else if (compare.vcmpcase(subcommand, CSTRSIZE("cedi")) == 0) {
                            if (message) {
                                if (groupcommands.transfer(message) > 0) 
                                    return 1;
                            } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) return 1;
                        } else if (compare.vcmpcase(subcommand, CSTRSIZE("vai")) == 0) {
                            if (message) {
                                if (groupcommands.bounce(message) > 0) 
                                    return 1;
                            } else if (infos.player->pvsend(pvulture.server, "[reset]e' necessario specificare un nome![n]") > 0) return 1;
                        } else if (compare.vcmpcase(subcommand, CSTRSIZE("sciogli")) == 0) {
                            if (groupcommands.destroy() > 0) 
                                return 1;
                            if (infos.player->pvsend(pvulture.server, "[reset][green]hai sciolto il tuo gruppo![n]") > 0) 
                                return 1;
                        } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste quel comando[n]") > 0) 
                            return 1;
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste quel comando[n]") > 0) 
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste quel comando[n]") > 0) 
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste quel comando[n]") > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) 
            return 1;
    } else 
        return 1;
    if (subcommand) {
        pvfree(subcommand);
        subcommand = NULL;
    }
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
