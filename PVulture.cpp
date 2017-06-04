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
#include "PVulture.h"
PVvulture pvulture;
sinformation infos;
bool shell = true;
bool gstop = false;
char *logo = NULL;
char *message = NULL;
char *subtitle = NULL;
char *sshell = NULL;

int timeclimate = 300;
int timemessage = 120;
int timetimeout = 600;
int timeattack = 5;
int timeblood = 8;
int timestun = 300;
int pointstun = 9;
int pointstamina = 8;
int logintries = 3;
int long timeloop = 0;

Cdatabank diary;
Cdatabank groups;
Cdatabank players;
Cdatabank events;
Cdatabank kickid;

int loadgame(FILE *configurationfile) {
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/PVULTURE]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[PVULTURE]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("LOGO")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &logo) != 0) LOG_ERROR("Unable to edit key %s", name);
                        logo[strlen(string) - strlen(name) - 2] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("SUBTITLE")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &subtitle) != 0) LOG_ERROR("Unable to edit key %s", name);
                        subtitle[strlen(string) - strlen(name) - 2] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("[DIARY]")) == 0) {
                        if (diary.load(configurationfile, "DIARY") > 0) LOG_ERROR("Unable to load DIARY");
                    } else if (compare.vcmpcase(string, CSTRSIZE("[GROUPS]")) == 0) {
                        if (groups.load(configurationfile, "GROUPS") > 0) LOG_ERROR("Unable to load GROUPS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("[PLAYERS]")) == 0) {
                        if (players.load(configurationfile, "PLAYERS") > 0) LOG_ERROR("Unable to load PLAYERS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("MESSAGE")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &message) != 0) LOG_ERROR("Unable to edit key %s", name);
                        message[strlen(string) - strlen(name) - 2] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("PLAYERS")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &NplayerID) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("OBJECTS")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &NobjectID) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("MOBS")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &NmobID) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIMECLIMATE")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &timeclimate) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIMEMESSAGE")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &timemessage) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIMETIMEOUT")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &timetimeout) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIMEROUND")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &timeattack) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIMEBLOOD")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &timeblood) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIMESTUN")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &timestun) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("ATTACKLIFE")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &pointstun) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("ATTACKSTAMINA")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &pointstamina) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("LOGINTRIES")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &logintries) != 0) LOG_ERROR("Unable to edit key %s", name);
                    } else if (compare.vcmpcase(string, CSTRSIZE("SHELL")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &sshell) != 0) LOG_ERROR("Unable to edit key %s", name);
                        sshell[strlen(string) - strlen(name) - 2] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Main] %s (unkown command)", string);
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                if (text) {
                    pvfree(text);
                    text = NULL;
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int savegame(FILE *configurationfile) {
    if (configurationfile) {
        fprintf(configurationfile, "[PVULTURE]\n");
        fprintf(configurationfile, "LOGO=%s\nMESSAGE=%s\nSUBTITLE=%s\n", logo, message, subtitle);
        if (players.save(configurationfile, "PLAYERS") > 0) return 1;
        fputc('\n', configurationfile);
        if (diary.save(configurationfile, "DIARY") > 0) return 1;
        fputc('\n', configurationfile);
        if (groups.save(configurationfile, "GROUPS") > 0) return 1;
        fprintf(configurationfile, "\nPLAYERS=%d\nOBJECTS=%d\nMOBS=%d\nSHELL=%s\n", NplayerID, NobjectID, NmobID, sshell);
        fprintf(configurationfile, "TIMECLIMATE=%d\nTIMEMESSAGE=%d\nTIMETIMEOUT=%d\nTIMEROUND=%d\nTIMEBLOOD=%d\nTIMESTUN=%d\nATTACKLIFE=%d\nATTACKSTAMINA=%d\nLOGINTRIES=%d\n", timeclimate, timemessage, timetimeout, timeattack, timeblood, timestun, pointstun, pointstamina, logintries);
        fprintf(configurationfile, "[/PVULTURE]");
    } else return 1;
    return 0;
}

bool getkind(Cplayer *player, ekind kind) {
    if (kind == MODERATOR) {
        if (player->logics.hasvalue("RANK", "Moderator") == 0) return true;
        if (player->logics.hasvalue("RANK", "Admin") == 0) return true;
    } else if (kind == ADMIN) {
        if (player->logics.hasvalue("RANK", "Admin") == 0) return true;
    } else if (kind == USER) return true;
    return false;
}
