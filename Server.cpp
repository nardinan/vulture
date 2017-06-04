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
#include <stdio.h>
#include "PVInterface.h"
#include "PVActions.h"
#include "PVulture.h"

int loadcommands(void) {
    if (pvulture.usrcommands.addcommand(":", "[azione]<emoticon>", personal, 1) > 0) return 1;
    if (pvulture.usrcommands.addcommand("alza", "[no parametri]", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("siedi", "[no parametri]", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("sdraia", "[no parametri]", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("passwd", "[password]", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("status", "[no parametri]", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("inventario", "<pg/npc>", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("presenta", "[pg/npc]", personal, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("nord", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("sud", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("ovest", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("est", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("nordest", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("nordovest", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("sudest", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("sudovest", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("su", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("giu", "[no parametri]", runmovement, 5) > 0) return 1;
    if (pvulture.usrcommands.addcommand("segui", "[pg/npc]", runmovement, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("prendi", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("lascia", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("dai", "[pg/npc]: [oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("metti", "[oggetto] [oggetto contenitore]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("mangia", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("bevi", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("leggi", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("scrivi", "[oggetto] [testo]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("cancella", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("usa", "[oggetto] <pg/npc>", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("indossa", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("riponi", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("attacca", "[pg/npc]", runfight, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("pace", "[no parametri]", runfight, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("modo", "[modalita]", runfight, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("colpisci", "[parte corpo]", runfight, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("decapita", "[no parametri]", runfight, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("smetti", "[no parametri]", runfight, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("di", "<pg/npc>: [testo]<emoticon>", dialog, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("sussurra", "[pg/npc]: [testo]<emoticon>", dialog, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("pensa", "[testo]", dialog, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("data", "[no parametri]", environment, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("guarda", "<pg/npc>/<oggetto>", environment, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("online", "[no parametri]", environment, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("invoke", "[no parametri]", environment, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("gruppo", "[comandi gruppo]", group, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("scappa", "[no parametri]", runescape, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("apri", "[direzione]", runlock, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("chiudi", "[direzione]", runlock, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("blocca", "[direzione]", runlock, 4) > 0) return 1;
    if (pvulture.usrcommands.addcommand("sblocca", "[direzione]", runlock, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("aspetto", "[nome]:[aggettivo]:[descrizione]:[razza]", personal, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("valore", "<pg/npc>: [categoria].[chiave].[valore]", personal, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("nascondi", "[no parametri]", personal, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("evento", "[descrizione]", environment, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("pacifica", "[no parametri]", environment, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("caccia", "[pg/npc]", environment, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("vai", "<pg/npc>/<[zona].[tile]>", runmovement, 4) > 0) return 1;
    if (pvulture.modcommands.addcommand("genera", "[template]", runobjects, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("duplica", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("distruggi", "[oggetto]", runobjects, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("relog", "[no parametri]", environment, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("ascolta", "[no parametri]", dialog, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("annuncia", "[messaggio]", dialog, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("muovi", "[pg/npc]: [zona].[tile]", runmovement, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("richiama", "[template]", runintelligence, 4) > 0) return 1;
    if (pvulture.admcommands.addcommand("puntatori", "[no parametri]", vshow, 4) > 0) return 1;
    return 0;
}

int unload(bool sigerror) {
    FILE *configurationfile = NULL;
    playerslist *list = pvulture.characters.gamecharacters.playersroot;
    if (sigerror) if (pvinterface.loading(0, "error", true) > 0) return 1;
    if (pvinterface.unloading(5, "writing server") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "server.ds", "w"))) {
            if (pvulture.server.save(configurationfile) > 0) printf("writing server: failed!\n");
            fclose(configurationfile);
        } else printf("writing server: failed (Unable to open file)\n");
    } else printf("writing server: ABORT!\n");
    if (pvinterface.unloading(5, "unloading commands ...") > 0) return 1;
    pvulture.usrcommands.unload();
    pvulture.modcommands.unload();
    pvulture.admcommands.unload();
    if (pvinterface.unloading(5, "saving environments") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "game.dg", "w"))) {
            if (savegame(configurationfile) > 0) printf("saving environments: failed!\n");
            fclose(configurationfile);
        } else printf("saving environments: failed (Unable to open file)\n");
    } else printf("saving environments: ABORT!\n");
    pvulture.logger.unload();
    if (pvinterface.unloading(5, "writing time & date") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "time.dt", "w"))) {
            if (pvulture.time.save(configurationfile) > 0) printf("writing time & date: failed!\n");
            fclose(configurationfile);
        } else printf("writing time & date: failed (Unable to open file)\n");
    } else printf("writing time & date: ABORT!\n");
    pvulture.time.unload();
    if (pvinterface.unloading(5, "writing pages") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "pages.dj", "w"))) {
            if (pvulture.pages.save(configurationfile) > 0) printf("writing pages: failed!\n");
            fclose(configurationfile);
        } else printf("writing pages: failed (Unable to open file)\n");
    } else printf("writing pages: ABORT!\n");
    if (pvinterface.unloading(5, "removing characters' references") > 0) return 1;
    if (pvulture.map.gamemap.delcharacters() > 0) printf("removing characters' references: failed!\n");
    if (pvinterface.unloading(20, "writing players (TMTSM)") > 0) return 1;
    while (list) {
        infos.player = list->player;
        list = list->next;
        if (personalcommands.logout(false) > 0) printf("writing players: failed!\n");
    }
    if (pvinterface.unloading(20, "writing world (TMTSM)") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "world.dw", "w"))) {
            if (pvulture.map.gamemap.save(configurationfile, pvulture.objects.gameobjects, pvulture.characters.gamecharacters) > 0) printf("writing world: failed!\n");
            fclose(configurationfile);
        } else printf("writing world: failed (Unable to open file)\n");
    } else printf("writing world: ABORT!\n");
    if (pvinterface.unloading(10, "writing mobs (TMTSM)") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "mobs.dm", "w"))) {
            if (pvulture.characters.gamecharacters.savemobs(configurationfile, pvulture.objects.gameobjects) > 0) printf("writing mobs: failed!\n");
            fclose(configurationfile);
        } else printf("writing mobs: failed (Unable to open file)\n");
    } else printf("writing mobs: ABORT!\n");
    pvulture.characters.unload();
    pvulture.map.unload();
    if (pvinterface.unloading(10, "writing objects (TMTSM)") > 0) return 1;
    if (!sigerror) {
        if ((configurationfile = fopen(_PVFILES "objects.do", "w"))) {
            if (pvulture.objects.gameobjects.save(configurationfile) > 0) printf("writing objects: failed!\n");
            fclose(configurationfile);
        } else printf("writing objects: failed (Unable to open file)\n");
    } else printf("writing objects: ABORT!\n");
    if (pvinterface.unloading(10, "clean memory") > 0) return 1;
    diary.unload();
    groups.unload();
    events.unload();
    kickid.unload();
    players.unload();
    pvulture.pages.unload();
    pvulture.server.unload();
    pvulture.objects.unload();
    if (subtitle) {
        pvfree(subtitle);
        subtitle = NULL;
    }
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (sshell) {
        pvfree(sshell);
        sshell = NULL;
    }
    if (logo) {
        pvfree(logo);
        logo = NULL;
    }
    if (!allocationsroot) {
        if (pvinterface.unloading(1, "complete! (esc to quit)") > 0) return 1;
    } else {
        if (pvinterface.unloading(1, "Warning, you miss some pointers! (esc to quit)") > 0) return 1;
        vdeallocate();
    }
    return 0;
}

int load(void) {
    FILE *configurationfile = NULL;
    serverinfos server = {"Vulture Project", "*", "*", "*", "*", 5090};
    if (pvinterface.loading(5, "loading server") > 0) return 1;
    srand((unsigned int) time(NULL));
    if ((configurationfile = fopen(_PVFILES "server.ds", "r"))) {
        if (pvulture.server.load(configurationfile) > 0) {
            printf("loading server: failed (can't read the server.ds)!\n");
            return 1;
        } else fclose(configurationfile);
    } else if (pvulture.server.initializing(server) > 0) {
        printf("loading server: failed  (Unable to open file)\n");
        return 1;
    }
    if (pvulture.server.initializing() > 0) {
        printf("loading server: failed!\n");
        return 1;
    }
    if (pvinterface.loading(10, "loading commands") > 0) return 1;
    if (loadcommands() > 0) {
        printf("Failed!\n");
        return 1;
    }
    if (pvinterface.loading(5, "setting environments") > 0) return 1;
    if ((configurationfile = fopen(_PVFILES "game.dg", "r"))) {
        if (loadgame(configurationfile) > 0) {
            printf("setting environments: failed!\n");
            return 1;
        } else if (pvulture.logger.load() > 0) {
            printf("setting environments: failed!\n");
            return 1;
        } else fclose(configurationfile);
    } else {
        printf("setting environments: failed (Unable to open file)\n");
        return 1;
    }
    if (pvinterface.loading(5, "loading pages") > 0) return 1;
    if ((configurationfile = fopen(_PVFILES "pages.dj", "r"))) {
        if (pvulture.pages.load(configurationfile) > 0) {
            printf("loading pages: failed!\n");
            return 1;
        } else fclose(configurationfile);
    } else {
        printf("loading pages: failed (Unable to open file)\n");
        return 1;
    }
    if (pvinterface.loading(5, "loading time & date") > 0) return 1;
    if ((configurationfile = fopen(_PVFILES "time.dt", "r"))) {
        if (pvulture.time.load(configurationfile) > 0) {
            printf("loading time & date: failed!\n");
            return 1;
        } else fclose(configurationfile);
    } else {
        printf("loading time & date: failed (Unable to open file)\n");
        return 1;
    }
    if (pvinterface.loading(10, "loading objects") > 0) return 1;
    if ((configurationfile = fopen(_PVFILES "objects.do", "r"))) {
        if (pvulture.objects.gameobjects.load(configurationfile) > 0) {
            printf("loading objects: failed!\n");
            return 1;
        } else fclose(configurationfile);
    } else {
        printf("loading objects: failed (Unable to open file)\n");
        return 1;
    }
    if (pvinterface.loading(50, "loading world (TMTSM)") > 0) return 1;
    if ((configurationfile = fopen(_PVFILES "world.dw", "r"))) {
        if (pvulture.map.gamemap.load(configurationfile, pvulture.objects.gameobjects, pvulture.characters.gamecharacters) > 0) {
            printf("loading world: failed!\n");
            return 1;
        } else fclose(configurationfile);
    } else {
        printf("loading world: failed (Unable to open file)\n");
        return 1;
    }
    if (pvinterface.loading(10, "loading mobs (TMTSM)") > 0) return 1;
    if ((configurationfile = fopen(_PVFILES "mobs.dm", "r"))) {
        if (pvulture.characters.gamecharacters.loadmobs(configurationfile, pvulture.objects.gameobjects, pvulture.map.gamemap) > 0) {
            printf("loading mobs: failed!\n");
            return 1;
        } else fclose(configurationfile);
    } else {
        printf("loading mobs: failed (Unable to open file)\n");
        return 1;
    }
    pvulture.time.start();
    pvulture.stopwatch.start();
    pvulture.logger.setvdecode(false);
    return 0;
}

int executeplayer(void) {
    int value = 0;
    bool pquit = false;
    char *commandword = NULL, *backup = NULL;
    if (infos.player->pvrecv(pvulture.server, &infos.message, 0, 1) == 1) pquit = true;
    if (pvulture.stopwatch.getsecs(getvalue("STATUS", "Last", infos.player->logics, 0)) >= timetimeout) pquit = true;
    if (infos.player->logics.hasvalue("STATUS", "Online") == 0) {
        if ((infos.player->logics.hascategory("FIGHT") == 0) && (getvalue("STATS", "LPoints", infos.player->logics, 0) > pointstun)) {
            if (round((Ccharacter *) infos.player) > 0) LOG_ERROR("Unable to run FIGHT.ROUND()");
        }
        if (infos.player->logics.hascategory("BLOOD") == 0) {
            if (personalevents.blood(infos.player) > 0) LOG_ERROR("Unable to run BLOOD()");
        }
        if (infos.player->logics.hascategory("NEWOBJECTS") == 0) {
            if (personalevents.equip(infos.player) > 0) LOG_ERROR("Unable to run EQUIP()");
        }
        if (getvalue("STATS", "Arms", infos.player->logics, 0) <= 0) {
            if (personalevents.damageArms(infos.player) > 0) LOG_ERROR("Unable to run DAMAGEARMS()");
        }
        if (getvalue("STATS", "LPoints", infos.player->logics, 0) <= pointstun) {
            if ((value = personalevents.damageLife(infos.player)) > 0) LOG_ERROR("Unable to run DAMAGELIFE()");
            else if (value < 0) {
                pquit = true;
            }
            if (infos.message) {
                pvfree(infos.message);
                infos.message = NULL;
            }
        }
    }
    if (infos.message) {
        if (compare.vcmpcase(infos.message, "n") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("nord") + 1))) {
                strcpy(infos.message, "nord");
                infos.message[sizeof ("nord")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "s") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("sud") + 1))) {
                strcpy(infos.message, "sud");
                infos.message[sizeof ("sud")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "o") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("ovest") + 1))) {
                strcpy(infos.message, "ovest");
                infos.message[sizeof ("ovest")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "e") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("est") + 1))) {
                strcpy(infos.message, "est");
                infos.message[sizeof ("est")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "se") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("sudest") + 1))) {
                strcpy(infos.message, "sudest");
                infos.message[sizeof ("sudest")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "ne") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("nordest") + 1))) {
                strcpy(infos.message, "nordest");
                infos.message[sizeof ("nordest")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "no") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("nordovest") + 1))) {
                strcpy(infos.message, "nordovest");
                infos.message[sizeof ("nordovest")] = '\0';
            } else return 1;
        } else if (compare.vcmpcase(infos.message, "so") == 0) {
            pvfree(infos.message);
            infos.message = NULL;
            if ((infos.message = (char *) pvmalloc(sizeof ("sudovest") + 1))) {
                strcpy(infos.message, "sudovest");
                infos.message[sizeof ("sudovest")] = '\0';
            } else return 1;
        }
        if (infos.player->logics.addvalue("STATUS", "Last", (int) pvulture.stopwatch.pause()) > 0) return 1;
        pvulture.logger.log("COMMANDS", "%s (from %s)", infos.message, pvulture.characters.getsimplename(infos.player));
        if (compare.vcmpcase(infos.message, CSTRSIZE("quit")) == 0) pquit = true;
        else {
            if ((infos.player->logics.hasvalue("STATUS", "Account") == 0) ||
                    (infos.player->logics.hasvalue("STATUS", "Password") == 0)) {
                if (personalcommands.login() < 0) pquit = true;
            } else {
                if (strlen(infos.message) > 0) {
                    if ((compare.vcmpcase(infos.message, CSTRSIZE("unload")) == 0) &&
                            (infos.player->logics.hasvalue("RANK", "Admin") == 0)) {
                        if (infos.player->pvsend(pvulture.server, "[reset]richiesta inoltrata al server!") > 0) return 1;
                        if (pvulture.characters.gamecharacters.pvsend(pvulture.server, "[reset][n][red]unload del server! (richiesta da %s)[n]", pvulture.characters.getsimplename(infos.player)) > 0) return 1;
                        if (pvulture.characters.gamecharacters.spvsend(pvulture.server, sshell) > 0) return 1;
                        gstop = true;
                        shell = false;
                    } else if (infos.player->logics.hasvalue("STATS", "Stunned") != 0) {
                        if ((backup = (char *) pvmalloc(strlen(infos.message)))) {
                            strcpy(backup, infos.message);
                            backup[strlen(infos.message)] = '\0';
                            if ((commandword = strings.vpop(&backup))) {
                                if ((pvulture.usrcommands.runcommand(commandword) > 0) &&
                                        (!((getkind(infos.player, MODERATOR)) && (pvulture.modcommands.runcommand(commandword) == 0))) &&
                                        (!((getkind(infos.player, ADMIN)) && (pvulture.admcommands.runcommand(commandword) == 0)))) {
                                    if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
                                }
                            } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
                        } else return 1;
                    } else if (infos.player->pvsend(pvulture.server, "[reset][red]sei a terra privo di sensi![n]") > 0) return 1;
                }
                if (shell) {
                    if (infos.player->spvsend(pvulture.server, sshell) > 0) return 1;
                } else {
                    shell = true;
                }
            }
        }
    }
    if ((infos.player) && (pquit)) {
        pvulture.logger.log("LOGOUT", "New logout request (from %s)", pvulture.characters.getsimplename(infos.player));
        if (compare.vcmpcase(infos.player->getaccount(), CSTRSIZE("###")) != 0) {
            if (personalcommands.logout(true) > 0) LOG_ERROR("Unable to LOG OUT player");
        } else if (personalcommands.logout(false) > 0) LOG_ERROR("Unable to LOG OUT player");
    }
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    if (commandword) {
        pvfree(commandword);
        commandword = NULL;
    }
    if (infos.message) {
        pvfree(infos.message);
        infos.message = NULL;
    }
    return 0;
}

int executemob(void) {
    bool pquit = false;
    objectslist *currentobject = NULL, *nextobject = NULL;
    char *objectname = NULL;
    pvulture.stopwatch.pause();
    if (runintelligenceevents() > 0) LOG_ERROR("Unable to run INTELLIGENCEEVENTS()");
    if ((infos.mob->logics.hascategory("FIGHT") == 0) && (getvalue("STATS", "LPoints", infos.mob->logics, 0) > pointstun)) {
        if (round((Ccharacter *) infos.mob) > 0) LOG_ERROR("Unable to run FIGHT.ROUND()");
    }
    if (infos.mob->logics.hascategory("BLOOD") == 0) {
        if (pvulture.stopwatch.getsecs(getvalue("BLOOD", "Last", infos.mob->logics, 0)) >= timeblood) {
            if (infos.mob->logics.addvalue("BLOOD", "Last", (int) pvulture.stopwatch.pause()) > 0) LOG_ERROR("Unable to add BLOOD->Last Logic");
            if (setvalue("STATS", "LPoints", infos.mob->logics, getvalue("BLOOD", "Damage", infos.mob->logics, 1)*-1) > 0) LOG_ERROR("Unable to set STATS->LPoints Logic");
            if (spvsend(pvulture.server, infos.mob->position, "[reset][n][yellow]$name sta' sanguinando![n]", (Ccharacter *) infos.mob) > 0) return 1;
            if (infos.mob->position->spvsend(pvulture.server, sshell) > 0) return 1;
        }
    }
    if (getvalue("STATS", "LPoints", infos.mob->logics, 0) <= 0) {
        if (infos.mob->logics.addvalue("STATS", "LPoints", 0) > 0) return 1;
        if (spvsend(pvulture.server, infos.mob->position, "[reset][n][red]$name <E' MORTO!> proprio davanti ai tuoi occhi![n]", (Ccharacter *) infos.mob) > 0) return 1;
        infos.mob->resetvalue();
        nextobject = infos.mob->nextvalue();
        while (nextobject) {
            currentobject = nextobject;
            nextobject = infos.mob->nextvalue();
            if (infos.mob->delobject(currentobject->object->getID()) > 0) return 1;
            if (infos.mob->position->addobject(currentobject->object) > 0) return 1;
            if (currentobject->object->logics.addvalue("USAGE", "USE", 0) > 0) LOG_ERROR("Unable to add USAGE->Use Logic");
            if (infos.mob->position->pvsend(pvulture.server, "[reset]\t[green]cadendo lascia a terra %s[n]", objectname = pvulture.objects.getcompletename(currentobject->object)) > 0) return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
        }
        if (infos.mob->position->spvsend(pvulture.server, sshell) > 0) return 1;
        pquit = true;
    }
    if (pquit) {
        if (infos.mob->position->delmob(infos.mob->getID()) > 0) return 1;
        if (pvulture.characters.gamecharacters.delmob(infos.mob->getID()) > 0) return 1;
        infos.mob = NULL;
    }
    return 0;
}

int loop(void) {
    mobslist *mobslist = NULL;
    playerslist *player = NULL, *playerslist = NULL;
    int socketID = 0, diaryID = 0;
    unsigned int playerID = NplayerID + 1;
    while (!gstop) {
        timeloop = pvulture.stopwatch.pause();
        if ((socketID = pvulture.server.search(0, 1)) > -1) {
            pvulture.logger.log("LOGIN", "New connection request (from %d)", playerID);
            if ((pvulture.characters.gamecharacters.addplayer(_PVFILES "template.dp", pvulture.objects.gameobjects, pvulture.map.gamemap, socketID, playerID++) >= 0) &&
                    (player = pvulture.characters.gamecharacters.getplayer(playerID - 1))) {
                if ((diary.getelements() - 1) > 0) {
                    if (diary.getelements() == 1) diaryID = 1;
                    else diaryID = (rand() % (diary.getelements() - 1)) + 1;
                }
                if (player->player->opvsend(pvulture.server, "[reset][cls]%s[n]%s%s%sAccount:", logo, subtitle, (diary.getvalue(diaryID) ? diary.getvalue(diaryID) : ""), message) > 0) return 1;
                if (player->player->logics.addvalue("STATUS", "Account", 1) > 0) return 1;
                if (player->player->logics.addvalue("STATUS", "Last", (int) pvulture.stopwatch.pause()) > 0) return 1;
            } else LOG_ERROR("Unable to build PLAYER'S shell");
        }
        if (runenvironmentevents() > 0) LOG_ERROR("Unable to run RUNENVIRONMENTEVENTS()");
        playerslist = pvulture.characters.gamecharacters.playersroot;
        mobslist = pvulture.characters.gamecharacters.mobsroot;
        while (playerslist) {
            if (gstop) playerslist = NULL;
            else {
                infos.player = playerslist->player;
                playerslist = playerslist->next;
                if (executeplayer() > 0) return 1;
            }
        }
        while (mobslist) {
            if (gstop) mobslist = NULL;
            else {
                infos.mob = mobslist->mob;
                mobslist = mobslist->next;
                if (executemob() > 0) return 1;
            }
        }
        if (pvinterface.showme() > 0) gstop = true;
        pvinterface.update();
    }
    pvulture.logger.log("SHUTDOWN", "Quitting ...");
    return 0;
}

int main(int argc, char *argv[]) {
    bool sigerror = false;
    if (pvinterface.initialize() > 0) return 1;
    if (pvinterface.intro() > 0) return 1;
    if (load() == 0) loop();
    else sigerror = true;
    unload(sigerror);
    while (pvinterface.getinput().ckey != KESCAPE);
    sdlunload();
    return 0;
}
