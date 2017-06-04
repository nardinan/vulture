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
#include "VWorld.h"
#include "VMob.h"

Cmob::Cmob(void) : Ccharacter() {
    if (!(birth = (datainfos *) pvmalloc(sizeof (datainfos)))) LOG_ERROR("Unable to alloc datainfos");
    argumentsroot = NULL;
    position = NULL;
    objectsroot = NULL;
    objectsthis = NULL;
    charactersroot = NULL;
    definitionsroot = NULL;
    kind = MOB;
}

int Cmob::load(FILE *configurationfile, Cobjectslist& objects, Cworld& world) {
    tileslist *tile = NULL;
    tileinfos position = {0, 0};
    int integer, zone;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL, *category = NULL, *key = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/MOB]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[MOB]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("ID")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &characterID) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) == 0) {
                        if (descriptions.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("SEX")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &integer) > 0) LOG_ERROR("Unable to edit key %s", string);
                        sex = (sexdefinition) integer;
                    } else if (compare.vcmpcase(string, CSTRSIZE("NMOB")) == 0) characterID = NmobID++;
                    else if (compare.vcmpcase(string, CSTRSIZE("[POINTS]")) == 0) {
                        if (logics.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[AI]")) == 0) {
                        if (AI.load(configurationfile, "AI") > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[OBJECTS]")) == 0) {
                        if (loadobjects(configurationfile, objects) > 0) LOG_ERROR("Unable to load OBJECTS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("[ARGUMENT]")) == 0) {
                        if (loadarguments(configurationfile) > 0) LOG_ERROR("Unable to load ARGUMENTS");
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
                                if (tile->tile->addmob(this) > 0) LOG_ERROR("Unable to add PLAYER %d", characterID);
                            } else LOG_ERROR("Unable to get PLAYER tile");
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("DEFINITION")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) != 0) LOG_ERROR("Unable to load DEFINITION");
                        else {
                            if (name) {
                                pvfree(name);
                                name = NULL;
                            }
                            if (strings.vsscanf(text, ':', "sssd", &name, &category, &key, &integer) != 0) LOG_ERROR("Unable to load DEFINITION %s", text);
                            else if (adddefinition(name, category, key, integer) > 0) LOG_ERROR("Unable to load DEFINITION %s", name);
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cmob] %s (unkown command)", string);
                }
                if (category) {
                    pvfree(category);
                    category = NULL;
                }
                if (key) {
                    pvfree(key);
                    key = NULL;
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

int Cmob::loadskeleton(const char *configurationfile, Cobjectslist& objects, Cworld& world, unsigned int characterID) {
    FILE *skeleton = fopen(configurationfile, "r");
    char string[_DEFAULT_MAXD_LEN];
    if (skeleton) {
        if (fgets(string, _DEFAULT_MAXD_LEN, skeleton)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/MOB]")) != 0) && (!feof(skeleton))) {
                if (compare.vcmpcase(string, CSTRSIZE("[MOB]")) == 0) {
                    if (load(skeleton, objects, world) > 0) LOG_ERROR("Unable to load MOB'S shell");
                } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cmob] %s (unkown command)", string);
                fgets(string, _DEFAULT_MAXD_LEN, skeleton);
            }
            this->characterID = characterID;
            if (descriptions.adddescription(0, "###", "###", "###") > 0) return 1;
        } else return 1;
        fclose(skeleton);
    } else return 1;
    return 0;
}

int Cmob::loadarguments(FILE *configurationfile) {
    argumentslist *argument = NULL;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *completename = NULL, *text = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/ARGUMENT]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[ARGUMENT]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("NAME")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &completename) > 0) LOG_ERROR("Unable to edit key %s", string);
                        else {
                            completename[strlen(completename) - 1] = '\0';
                            if (addargument(completename) < 0) LOG_ERROR("Unable to create Argument %s (MOB %d)", completename, characterID);
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("RESPONSE")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("Unable to edit key %s", string);
                        else {
                            text[strlen(text) - 1] = '\0';
                            if (addresponse(completename, text) > 0) LOG_ERROR("Unable to create Response %s (MOB %d)", text, characterID);
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("[KEYWORDS]")) == 0) {
                        if ((argument = getargument(completename))) {
                            if (argument->keywords->load(configurationfile, "KEYWORDS") > 0) LOG_ERROR("Unable to load KEYWORDS");
                        } else LOG_ERROR("Unable to load KEYWORDS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cmob] %s (unkown command)", string);
                }
                if (completename) {
                    pvfree(completename);
                    completename = NULL;
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
            if (argument) {
                pvfree(argument);
                argument = NULL;
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cmob::save(FILE *configurationfile, Cobjectslist& objects) {
    if (configurationfile) {
        fprintf(configurationfile, "[MOB]\n");
        fprintf(configurationfile, "ID=%d\nSEX=%d\n", characterID, (int) sex);
        fprintf(configurationfile, "BIRTH=%d:%d:%d\n", birth->day, birth->month, birth->year);
        if (position) fprintf(configurationfile, "POSITION=%d:%d\n", position->getzoneID(), position->getID());
        if (descriptions.save(configurationfile) > 0) LOG_ERROR("Unable to save DESCRIPTIONS of Mob %d", characterID);
        fputc('\n', configurationfile);
        if (logics.save(configurationfile) > 0) LOG_ERROR("Unable to save LOGICS of Mob %d", characterID);
        fputc('\n', configurationfile);
        if (AI.save(configurationfile, "AI") > 0) LOG_ERROR("Unable to save AI LOGIC of Mob %d", characterID);
        fputc('\n', configurationfile);
        if (savearguments(configurationfile) > 0) LOG_ERROR("Unable to save ARGUMENTS of Mob %d", characterID);
        if (saveobjects(configurationfile, objects) > 0) return 1;
        fputc('\n', configurationfile);
        if (savecharacters(configurationfile) > 0) return 1;
        fprintf(configurationfile, "\n[/MOB]");
    } else return 1;
    return 0;
}

int Cmob::savearguments(FILE *configurationfile) {
    argumentslist *list = argumentsroot;
    if (configurationfile) {
        while (list) {
            fprintf(configurationfile, "[ARGUMENT]\n");
            fprintf(configurationfile, "NAME=%s\n", list->completename);
            fprintf(configurationfile, "RESPONSE=%s\n", list->response);
            if (list->keywords->save(configurationfile, "KEYWORDS") > 0) LOG_ERROR("Unable to save ARGUMENTS of Mob %d", characterID);
            fprintf(configurationfile, "\n[/ARGUMENT]\n");
            list = list->next;
        }
    } else return 1;
    return 0;
}

int Cmob::addargument(const char *completename) {
    argumentslist *list = argumentsroot;
    if (argumentsroot) {
        while (list->next) list = list->next;
        if ((list->next = (argumentslist *) pvmalloc(sizeof (argumentslist)))) {
            if ((list->next->completename = (char *) pvmalloc(strlen(completename) + 1))) {
                strcpy(list->next->completename, completename);
                list->next->completename[strlen(completename)] = '\0';
                list->next->keywords = new (Cdatabank);
                list->next->back = list;
                list->next->next = NULL;
            } else return 1;
        } else return 1;
    } else {
        if ((argumentsroot = (argumentslist *) pvmalloc(sizeof (argumentslist)))) {
            if ((argumentsroot->completename = (char *) pvmalloc(strlen(completename) + 1))) {
                strcpy(argumentsroot->completename, completename);
                argumentsroot->completename[strlen(completename)] = '\0';
                argumentsroot->keywords = new (Cdatabank);
                argumentsroot->back = NULL;
                argumentsroot->next = NULL;
            } else return 1;
        } else return 1;
    }
    return 0;
}

int Cmob::addresponse(const char *completename, const char *response) {
    argumentslist *list = argumentsroot;
    if ((list = getargument(completename))) {
        if ((list->response = (char *) pvmalloc(strlen(response) + 1))) {
            strcpy(list->response, response);
            list->response[strlen(response)] = '\0';
        } else return 1;
    } else return 1;
    return 0;
}

int Cmob::delargument(const char *completename) {
    argumentslist *list = argumentsroot;
    if ((list = getargument(completename))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else argumentsroot = list->next;
        list->keywords->unload();
        delete(list->keywords);
        pvfree(list->completename);
        pvfree(list->response);
        pvfree(list);
    } else return 1;
    return 0;
}

argumentslist *Cmob::getargument(const char *completename) {
    argumentslist *list = argumentsroot;
    if (argumentsroot) {
        while ((compare.vcmpcase(list->completename, LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

void Cmob::unload(void) {
    logics.unload();
    AI.unload();
    descriptions.unload();
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
    while (argumentsroot) {
        if (delargument(argumentsroot->completename) > 0) {
            LOG_ERROR("Unable to unload Argument %s", argumentsroot->completename);
            argumentsroot = argumentsroot->next;
        }
    }
    while (charactersroot) {
        if (delcharacter(charactersroot->characterID, charactersroot->character) > 0) {
            LOG_ERROR("Unable to unload Character %d", charactersroot->characterID);
            charactersroot = charactersroot->next;
        }
    }
}
