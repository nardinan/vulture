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
#include "VPlayer.h"
#include "VWorld.h"

Cplayer::Cplayer(void) : Ccharacter() {
    if (!(birth = (datainfos *) pvmalloc(sizeof (datainfos)))) LOG_ERROR("Unable to alloc datainfos");
    isonline = false;
    islogging = false;
    account = NULL;
    password = NULL;
    position = NULL;
    objectsroot = NULL;
    objectsthis = NULL;
    charactersroot = NULL;
    definitionsroot = NULL;
    kind = PLAYER;
}

int Cplayer::load(FILE *configurationfile, Cobjectslist& objects, Cworld& world) {
    tileslist *tile = NULL;
    tileinfos position = {0, 0};
    int integer, zone;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL, *category = NULL, *keyname = NULL;
    if (configurationfile) {
        awayposition = -1;
        descriptions.deldescription(0);
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/PLAYER]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[PLAYER]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("ID")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &characterID) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("PASSWORD")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("Unable to edit key %s", &string);
                        text[strlen(text) - 1] = '\0';
                        setpassword(text);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) == 0) {
                        if (descriptions.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("SEX")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &integer) > 0) LOG_ERROR("Unable to edit key %s", string);
                        sex = (sexdefinition) integer;
                    } else if (compare.vcmpcase(string, CSTRSIZE("NPLAYER")) == 0) characterID = NplayerID++;
                    else if (compare.vcmpcase(string, CSTRSIZE("[POINTS]")) == 0) {
                        if (logics.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[OBJECTS]")) == 0) {
                        if (loadobjects(configurationfile, objects) > 0) LOG_ERROR("Unable to load OBJECTS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("[CHARACTERS]")) == 0) {
                        if (loadcharacters(configurationfile) > 0) LOG_ERROR("Unable to load CHARACTERS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("BIRTH")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) != 0) LOG_ERROR("Unable to load BIRTH");
                        else if (strings.vsscanf(text, ':', "ddd", &birth->day, &birth->month, &birth->year) > 0) LOG_ERROR("Unable to edit birth Date %s", text);
                    } else if (compare.vcmpcase(string, CSTRSIZE("POSITION")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) != 0) LOG_ERROR("Unable to load POSITION");
                        else if (strings.vsscanf(text, ':', "dd", &zone, &integer) != 0) LOG_ERROR("Unable to edit POSITION %s", text);
                        else {
                            position.zoneID = zone;
                            position.tileID = integer;
                            if ((tile = world.gettile(position)) || (tile = world.gettilesroot())) {
                                if (tile->tile->addplayer(this) > 0) LOG_ERROR("Unable to add PLAYER %d", characterID);
                            } else LOG_ERROR("Unable to get PLAYER tile");
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("DEFINITION")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) != 0) LOG_ERROR("Unable to load DEFINITION");
                        else {
                            if (name) {
                                pvfree(name);
                                name = NULL;
                            }
                            if (strings.vsscanf(text, ':', "sssd", &name, &category, &keyname, &integer) != 0) LOG_ERROR("Unable to load DEFINITION %s", text);
                            else if (adddefinition(name, category, keyname, integer) > 0) LOG_ERROR("Unable to load DEFINITION %s", name);
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cplayer] %s (unkown command)", string);
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                if (text) {
                    pvfree(text);
                    text = NULL;
                }
                if (category) {
                    pvfree(category);
                    category = NULL;
                }
                if (keyname) {
                    pvfree(keyname);
                    keyname = NULL;
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cplayer::loadskeleton(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int characterID) {
    FILE *skeleton = fopen(configurationfile, "r");
    char string[_DEFAULT_MAXD_LEN];
    if (skeleton) {
        if (fgets(string, _DEFAULT_MAXD_LEN, skeleton)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/PLAYER]")) != 0) && (!feof(skeleton))) {
                if (compare.vcmpcase(string, CSTRSIZE("[PLAYER]")) == 0) {
                    if (load(skeleton, objects, world) > 0) LOG_ERROR("Unable to load PLAYER'S shell");
                } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cplayer] %s (unkown command)", string);
                fgets(string, _DEFAULT_MAXD_LEN, skeleton);
            }
            this->characterID = characterID;
            if (descriptions.adddescription(0, "###", "###", "###") > 0) return 1;
            if (setpassword((char *) "###") > 0) return 1;
            if (setaccount((char *) "###") > 0) return 1;
        } else return 1;
        fclose(skeleton);
    } else return 1;
    return 0;
}

int Cplayer::save(FILE *configurationfile, Cobjectslist& objects) {
    if (configurationfile) {
        fprintf(configurationfile, "[PLAYER]\n");
        fprintf(configurationfile, "ID=%d\nSEX=%d\n", characterID, (int) sex);
        fprintf(configurationfile, "BIRTH=%d:%d:%d\n", birth->day, birth->month, birth->year);
        if (position) fprintf(configurationfile, "POSITION=%d:%d\n", position->getzoneID(), position->getID());
        if (password) fprintf(configurationfile, "PASSWORD=%s\n", password);
        if (savedefinitions(configurationfile) > 0) return 1;
        if (descriptions.save(configurationfile) > 0) return 1;
        fputc('\n', configurationfile);
        if (logics.save(configurationfile) > 0) LOG_ERROR("Unable to save LOGICS of Player %d", characterID);
        fputc('\n', configurationfile);
        if (saveobjects(configurationfile, objects) > 0) return 1;
        fputc('\n', configurationfile);
        if (savecharacters(configurationfile) > 0) return 1;
        fprintf(configurationfile, "\n[/PLAYER]");
    } else return 1;
    return 0;
}

int Cplayer::setaccount(char *account) {
    if (strlen(account) < Vfile) {
        if (this->account) {
            pvfree(this->account);
            this->account = NULL;
        }
        if ((this->account = (char *) pvmalloc(strlen(account) + 1))) {
            strncpy(this->account, account, strlen(account));
            this->account[strlen(account)] = '\0';
        } else return 1;
    } else return 1;
    return 0;
}

int Cplayer::setpassword(char *password) {
    if (strlen(password) < Vpass) {
        if (this->password) {
            pvfree(this->password);
            this->password = NULL;
        }
        if ((this->password = (char *) pvmalloc(strlen(password) + 1))) {
            strncpy(this->password, password, strlen(password));
            this->password[strlen(password)] = '\0';
        } else return 1;
    } else return 1;
    return 0;
}

int Cplayer::delaccount(void) {
    if (this->account) {
        pvfree(this->account);
        this->account = NULL;
    } else return 1;
    return 0;
}

int Cplayer::delpassword(void) {
    if (this->password) {
        pvfree(this->password);
        this->password = NULL;
    } else return 1;
    return 0;
}

int Cplayer::pvrecv(Cserver& server, char **message, unsigned int sec, unsigned int usec) {
    int value = 0;
    value = server.pvrecv(socketID, message, sec, usec);
    if ((islogging) && (*message))
        if (logger.log("[NULL]", "%s[n]", *message) > 0) return 1;
    return value;
}

int Cplayer::pvsend(Cserver& server, const char *format, ...) {
    va_list args;
    int length = 0;
    char *message = NULL;
    if (isonline) {
        va_start(args, format);
        length = allocate.vstrlen(format, args);
        va_end(args);
        if (length > 0) {
            if ((message = (char *) pvmalloc(length + 1))) {
                va_start(args, format);
                vsnprintf(message, length, format, args);
                va_end(args);
                if (islogging)
                    if (logger.log("[NULL]", message) > 0) 
                        return 1;
                if (server.pvsend(socketID, message) > 0) 
                    return 1;
            }
            if (message) {
                pvfree(message);
                message = NULL;
            }
        } else 
            return 1;
    }
    return 0;
}

int Cplayer::opvsend(Cserver& server, const char *format, ...) {
    va_list args;
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
            if (islogging)
                if (logger.log("[NULL] %s", message) > 0) return 1;
            if (server.pvsend(socketID, message) > 0) return 1;
        }
        if (message) {
            pvfree(message);
            message = NULL;
        }
    } else return 1;
    return 0;
}

int Cplayer::spvsend(Cserver& server, const char *format, ...) {
    va_list args;
    definitionslist *definition = NULL;
    int integer = 0, length = 0;
    char *string = NULL, *code = NULL, *pointer = NULL;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((string = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(string, length, format, args);
            va_end(args);
            pointer = string;
            while ((code = strchr(pointer, '<'))) {
                *code++ = '\0';
                if (pvsend(server, "%s", pointer) > 0) return 1;
                if ((pointer = strchr(code, '>')) && (pointer > code)) {
                    *pointer++ = '\0';
                    if ((definition = getdefinition(code))) {
                        integer = 0;
                        if (logics.hasvalue(definition->category, definition->keyname) == 0) integer = logics.getvalue(definition->category, definition->keyname);
                        if (pvsend(server, "%s%d[reset]", ((integer < 50) ? ((integer < 10) ? "[red]" : "[yellow]") : "[green]"), integer) > 0) return 1;
                    } else if (compare.vcmpcase(code, CSTRSIZE("hide")) == 0) {
                        if (logics.hasvalue("STATUS", "Hide") == 0) {
                            if (pvsend(server, "([bold]H[reset])") > 0) return 1;
                        }
                    } else if (compare.vcmpcase(code, CSTRSIZE("group")) == 0) {
                        if (logics.hascategory("GROUP") == 0) {
                            if (pvsend(server, "([blue]G[reset])") > 0) return 1;
                        }
                    } else LOG_ERROR("Unable to find CODE %s of Player %d", code, characterID);
                } else pointer = code;
            }
            if (pvsend(server, pointer) > 0) return 1;
        } else return 1;
    }
    if (string) {
        pvfree(string);
        string = NULL;
    }
    return 0;
}

int Cplayer::setlogging(bool islogging) {
    time_t current;
    struct tm *local;
    char buffername[_DEFAULT_MIND_LEN], buffertime[_DEFAULT_MIND_LEN];
    if (this->islogging != islogging) {
        if (islogging) {
            this->islogging = true;
            if (!logger.islogging()) {
                current = time(NULL);
                local = localtime(&current);
                strftime(buffertime, _DEFAULT_MIND_LEN, "%d.%a.%b", local);
                snprintf(buffername, _DEFAULT_MIND_LEN, "./logs/%s.%d-%s@LOG.html", account, characterID, buffertime);
                if (logger.load(buffername) > 0) return 1;
                else logger.setvdecode(true);
            } else return 1;
        } else {
            this->islogging = false;
            if (logger.islogging()) logger.unload();
            else return 1;
        }
    }
    return 0;
}

void Cplayer::unload(void) {
    logics.unload();
    descriptions.unload();
    if (logger.islogging()) logger.unload();
    if (password) {
        pvfree(password);
        password = NULL;
    }
    if (account) {
        pvfree(account);
        account = NULL;
    }
    if (birth) {
        pvfree(birth);
        birth = NULL;
    }
    while (definitionsroot) {
        if (deldefinition(definitionsroot->completename) > 0) {
            LOG_ERROR("Unable to unload Definition %s", definitionsroot->completename);
            definitionsroot = definitionsroot->next;
        }
    }
    while (objectsroot) {
        if (delobject(objectsroot->object->getID()) > 0) {
            LOG_ERROR("Unable to unload Object %d", objectsroot->object->getID());
            objectsroot = objectsroot->next;
        }
    }
    while (charactersroot) {
        if (delcharacter(charactersroot->characterID, charactersroot->character) > 0) {
            LOG_ERROR("Unable to unload Character %d", charactersroot->characterID);
            charactersroot = charactersroot->next;
        }
    }
}
