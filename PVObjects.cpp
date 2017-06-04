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
#include "PVObjects.h"

int PVobject::duplicate(void) {
    Cobject *object = NULL;
    objectslist *copy = NULL;
    char *command = NULL, *message = NULL, *objectname = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((object = pvulture.objects.get(message, infos.player))) {
                if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
                    if (getvalue("USAGE", "Duplicable", object->logics, 0) != 0) {
                        if ((copy = pvulture.objects.gameobjects.addobject(object))) {
                            if ((pvulture.objects.getweight(copy->object) + pvulture.objects.getweight(infos.player)) <= getvalue("STATS", "WPoints", infos.player->logics, 0)) {
                                if (infos.player->addobject(copy->object) > 0) return 1;
                                if (infos.player->pvsend(pvulture.server, "[reset][green]hai creato %s[n]", objectname = pvulture.objects.getcompletename(copy->object)) > 0) return 1;
                                if (objectname) {
                                    pvfree(objectname);
                                    objectname = NULL;
                                }
                            } else {
                                if (infos.player->position->addobject(copy->object) > 0) return 1;
                                if (infos.player->pvsend(pvulture.server, "[reset][green]hai creato %s ma l'hai lasciato a terra[n]", objectname = pvulture.objects.getcompletename(copy->object)) > 0) return 1;
                                if (objectname) {
                                    pvfree(objectname);
                                    objectname = NULL;
                                }
                            }
                        } else return 1;
                    } else if (infos.player->pvsend(pvulture.server, "[reset]l'oggetto non e' duplicabile![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non stai portando nulla del genere![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare il nome di un oggetto![n]") > 0) return 1;
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

int PVobject::destroy(void) {
    Cobject *object = NULL;
    char *command = NULL, *message = NULL, *objectname = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((object = pvulture.objects.get(message, infos.player))) {
                if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai distrutto %s[n]", objectname = pvulture.objects.getcompletename(object)) > 0) return 1;
                    if (objectname) {
                        pvfree(objectname);
                        objectname = NULL;
                    }
                    if (infos.player->delobject(object->getID()) > 0) return 1;
                    if (pvulture.objects.gameobjects.delobject(object->getID()) > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
            } else if ((object = pvulture.objects.get(message, infos.player->position))) {
                if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai distrutto %s[n]", objectname = pvulture.objects.getcompletename(object)) > 0) return 1;
                    if (objectname) {
                        pvfree(objectname);
                        objectname = NULL;
                    }
                    if (infos.player->position->delobject(object->getID()) > 0) return 1;
                    if (pvulture.objects.gameobjects.delobject(object->getID()) > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare il nome di un oggetto![n]") > 0) return 1;
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

int PVobject::generate(void) {
    Cobject *object = NULL;
    objectslist *copy = NULL;
    FILE *configurationfile = NULL;
    char path[_DEFAULT_MIND_LEN], *message = NULL, *command = NULL, *objectname = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            snprintf(path, _DEFAULT_MIND_LEN, _PVFILES "objects/%s.do", message);
            if ((configurationfile = fopen(path, "r"))) {
                if ((object = new(Cobject)) && (object->load(configurationfile) == 0)) {
                    if ((copy = pvulture.objects.gameobjects.addobject(object))) {
                        if ((pvulture.objects.getweight(copy->object) + pvulture.objects.getweight(infos.player)) <= getvalue("STATS", "WPoints", infos.player->logics, 0)) {
                            if (infos.player->addobject(copy->object) > 0) return 1;
                            if (infos.player->pvsend(pvulture.server, "[reset][green]hai creato %s[n]", objectname = pvulture.objects.getcompletename(copy->object)) > 0) return 1;
                            if (objectname) {
                                pvfree(objectname);
                                objectname = NULL;
                            }
                        } else {
                            if (infos.player->position->addobject(copy->object) > 0) return 1;
                            if (infos.player->pvsend(pvulture.server, "[reset][green]hai creato %s ma l'hai lasciato a terra[n]", objectname = pvulture.objects.getcompletename(copy->object)) > 0) return 1;
                            if (objectname) {
                                pvfree(objectname);
                                objectname = NULL;
                            }
                        }
                    } else return 1;
                    object->unload();
                    delete(object);
                } else LOG_ERROR("Unable to read TEMPLATE %s", message);
                fclose(configurationfile);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non esiste un template con quel nome![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare il nome di un template oggetto![n]") > 0) return 1;
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

int PVaction::use(void) {
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if (compare.vcmpcase(command, CSTRSIZE("mangia")) == 0) {
                if (eat(&message) > 0) LOG_ERROR("Unable to run OBJECTS.EAT()");
            } else if (compare.vcmpcase(command, CSTRSIZE("bevi")) == 0) {
                if (drink(&message) > 0) LOG_ERROR("Unable to run OBJECTS.DRINK()");
            } else if (compare.vcmpcase(command, CSTRSIZE("leggi")) == 0) {
                if (read(&message) > 0) LOG_ERROR("Unable to run OBJECTS.READ()");
            } else if (compare.vcmpcase(command, CSTRSIZE("scrivi")) == 0) {
                if (write(&message) > 0) LOG_ERROR("Unable to run OBJECTS.WRITE()");
            } else if (compare.vcmpcase(command, CSTRSIZE("cancella")) == 0) {
                if (remove(&message) > 0) LOG_ERROR("Unable to run OBJECTS.WRITE()");
            } else if ((compare.vcmpcase(command, CSTRSIZE("usa")) == 0) || (compare.vcmpcase(command, CSTRSIZE("indossa")) == 0)) {
                if (equip(&message) > 0) LOG_ERROR("Unable to run OBJECTS.EQUIP()");
            } else if (infos.player->pvsend(pvulture.server, "[reset]non e' un tipo di utilizzo consono![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un'azione e un oggetto![n]") > 0) return 1;
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

int PVaction::drop(void) {
    Cobject *object = NULL;
    char *command = NULL, *message = NULL, *buffer = NULL, *objectname = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((object = pvulture.objects.get(message, infos.player))) {
                if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
                    if (infos.player->delobject(object->getID()) > 0) return 1;
                    if (infos.player->position->addobject(object) > 0) return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai lasciato %s[n]", objectname = pvulture.objects.getcompletename(object)) > 0) return 1;
                    if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                        if (infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                        }
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name lascia a terra %s[n]", objectname)), (Ccharacter *) infos.player) > 0) return 1;
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (!infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->addplayer(infos.player) > 0) return 1;
                        }
                    }
                    if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                        if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
                    if (objectname) {
                        pvfree(objectname);
                        objectname = NULL;
                    }
                } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non stai portando nulla del genere![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un oggetto![n]") > 0) return 1;
    } else return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVaction::put(void) {
    Cobject *contained = NULL, *container = NULL;
    int value = 0;
    char *command = NULL, *message = NULL, *backup = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((backup = strings.vpop(&message)) && (message) &&
                    ((container = pvulture.objects.get(message, infos.player->position)) ||
                    (container = pvulture.objects.get(message, infos.player))) &&
                    (contained = pvulture.objects.get(backup, infos.player))) {
                value = put(container, contained);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla con quel nome![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare cosa vuol mettere e dove![n]") > 0) return 1;
    } else return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (backup) {
        pvfree(backup);
        container = NULL;
    }
    return value;
}

int PVaction::put(Cobject *container, Cobject *object) {
    char *objectname = NULL, *containername = NULL, *buffer = NULL;
    if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
        if (container->logics.hascategory("CONTAINER") == 0) {
            if (getvalue("CONTAINER", "Open", container->logics, 0) == 1) {
                if ((pvulture.objects.getweight(object) + pvulture.objects.getweight(container)) <= getvalue("CONTAINER", "Weight", container->logics, 0)) {
                    if (container->addobject(object) > 0) return 1;
                    if (infos.player->delobject(object->getID()) > 0) return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai messo %s in %s[n]", objectname = pvulture.objects.getcompletename(object), containername = pvulture.objects.getcompletename(container)) > 0) return 1;
                    if (objectname) {
                        pvfree(objectname);
                        objectname = NULL;
                    }
                    if (containername) {
                        pvfree(containername);
                        containername = NULL;
                    }
                    if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                        if (infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                        }
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name mette qualcosa in %s[n]", containername)), (Ccharacter *) infos.player) > 0) return 1;
                        if (buffer) {
                            pvfree(buffer);
                            buffer = NULL;
                        }
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (!infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->addplayer(infos.player) > 0) return 1;
                        }
                    }
                } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' contenere altri oggetti![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]e' chiuso![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' contenere oggetti![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
    return 0;
}

int PVaction::replace(void) {
    Cobject *object = NULL;
    char *command = NULL, *message = NULL, *objectname = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((object = pvulture.objects.get(message, infos.player))) {
                if (getvalue("USAGE", "Use", object->logics, 0) == 1) {
                    if ((objectname = pvulture.objects.getcompletename(object))) {
                        if (pvulture.characters.setmalus(infos.player, object) > 0) return 1;
                        if (object->logics.addvalue("USAGE", "Use", 0) > 0) LOG_ERROR("Unable to add USAGE->Use Logic");
                        if (infos.player->pvsend(pvulture.server, "[reset][green]hai riposto %s[n]", objectname) > 0) return 1;
                    } else return 1;
                    if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                        if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
                    if (objectname) {
                        pvfree(objectname);
                        objectname = NULL;
                    }
                } else if (infos.player->pvsend(pvulture.server, "[reset]non e' in uso![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non stai portando nulla del genere![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un oggetto in uso![n]") > 0) return 1;
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

int PVaction::take(void) {
    Cobject *contained = NULL, *container = NULL;
    int value = 0;
    char *command = NULL, *message = NULL, *backup = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((backup = strings.vpop(&message)) && (message)) {
                if ((container = pvulture.objects.get(message, infos.player->position))) {
                    if ((contained = pvulture.objects.get(backup, container))) {
                        value = take(container, contained);
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere li dentro![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere qui attorno![n]") > 0) return 1;
            } else if ((contained = pvulture.objects.get(backup, infos.player->position))) {
                value = take(contained);
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere qui attorno![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un oggetto![n]") > 0) return 1;
    } else return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return value;
}

int PVaction::take(Cobject *object) {
    char *objectname = NULL, *buffer = NULL;
    if (getvalue("USAGE", "Transportable", object->logics, 1) == 1) {
        if ((pvulture.objects.getweight(object) + pvulture.objects.getweight(infos.player)) <= getvalue("STATS", "WPoints", infos.player->logics, 0)) {
            object->logics.addvalue("USAGE", "Use", 0);
            if (infos.player->addobject(object) > 0) return 1;
            if (infos.player->position->delobject(object->getID()) > 0) return 1;
            if (infos.player->pvsend(pvulture.server, "[reset][green]hai preso %s da terra[n]", objectname = pvulture.objects.getcompletename(object)) > 0) return 1;
            if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                }
                if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name prende %s da terra[n]", objectname)), (Ccharacter *) infos.player) > 0) return 1;
                if (buffer) {
                    pvfree(buffer);
                    buffer = NULL;
                }
                if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                if (!infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->addplayer(infos.player) > 0) return 1;
                }
            }
            if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
        } else if (infos.player->pvsend(pvulture.server, "[reset]e' troppo pesante, non riesci a portarlo![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]risulta piuttosto difficile prenderlo![n]") > 0) return 1;
    if (objectname) {
        pvfree(objectname);
        objectname = NULL;
    }
    return 0;
}

int PVaction::take(Cobject *container, Cobject *object) {
    char *containername = NULL, *objectname = NULL, *buffer = NULL;
    if (container->logics.hascategory("CONTAINER") == 0) {
        if (getvalue("CONTAINER", "Open", container->logics, 0) == 1) {
            if (getvalue("USAGE", "Transportable", object->logics, 0) == 1) {
                if ((pvulture.objects.getweight(object) + pvulture.objects.getweight(infos.player)) <= getvalue("STATS", "WPoints", infos.player->logics, 0)) {
                    object->logics.addvalue("USAGE", "Use", 0);
                    if (infos.player->addobject(object) > 0) return 1;
                    if (container->delobject(object->getID()) > 0) return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai preso %s da %s[n]", objectname = pvulture.objects.getcompletename(object), containername = pvulture.objects.getcompletename(container)) > 0) return 1;
                    if (infos.player->logics.hasvalue("STATUS", "Hide") != 0) {
                        if (infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                        }
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name prende qualcosa da %s[n]", containername)), (Ccharacter *) infos.player) > 0) return 1;
                        if (buffer) {
                            pvfree(buffer);
                            buffer = NULL;
                        }
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (!infos.player->position->getplayer(infos.player->getID())) {
                            if (infos.player->position->addplayer(infos.player) > 0) return 1;
                        }
                    }
                    if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                        if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
                } else if (infos.player->pvsend(pvulture.server, "[reset]e' troppo pesante, non riesci a portarlo![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]risulta piuttosto difficile prenderlo![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]e' chiuso![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' contenere oggetti![n]") > 0) return 1;
    if (objectname) {
        pvfree(objectname);
        objectname = NULL;
    }
    if (containername) {
        pvfree(containername);
        containername = NULL;
    }
    return 0;
}

int PVaction::give(void) {
    Cmob *mob = NULL;
    Cobject *object = NULL;
    Cplayer *player = NULL;
    int value = 0;
    char *command = NULL, *message = NULL, *pointer = NULL, *objectname = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) &&
                (message) &&
                (pointer = strchr(message, ':'))) {
            for (objectname = pointer + 1; *objectname == ' '; objectname++);
            do {
                *pointer-- = '\0';
            } while ((pointer > message) && (*pointer == ' '));
            if ((strlen(objectname) > 0) && (strlen(message) > 0)) {
                if ((object = pvulture.objects.get(objectname, infos.player))) {
                    if ((player = pvulture.characters.getplayer(message, infos.player->position))) {
                        value = give(player, object);
                    } else if ((mob = pvulture.characters.getmob(message, infos.player->position))) {
                        value = give(mob, object);
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla con quel nome qui![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nome ed un oggetto[n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un nome ed un oggetto[n]") > 0) return 1;
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

int PVaction::give(Cplayer *player, Cobject *object) {
    char *charactername = NULL, *targetname = NULL, *objectname = NULL;
    if (player->getID() != infos.player->getID()) {
        if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
            if ((pvulture.objects.getweight(object) + pvulture.objects.getweight(player)) <= getvalue("STATS", "WPoints", player->logics, 0)) {
                if (infos.player->delobject(object->getID()) > 0) return 1;
                if (player->addobject(object) > 0) return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]hai dato %s a %s[n]", objectname = pvulture.objects.getcompletename(object), charactername = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
                if (player->pvsend(pvulture.server, "[reset][n][yellow]hai ricevuto %s da %s[n]", objectname, targetname = pvulture.characters.gettargetname(infos.player, player)) > 0) return 1;
                if (objectname) {
                    pvfree(objectname);
                    objectname = NULL;
                }
                if (charactername) {
                    pvfree(charactername);
                    charactername = NULL;
                }
                if (targetname) {
                    pvfree(targetname);
                    targetname = NULL;
                }
                if ((infos.player->logics.hasvalue("STATUS", "Hide") != 0) &&
                        (player->logics.hasvalue("STATUS", "Hide") != 0)) {
                    if (infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                    }
                    if (player->position->getplayer(player->getID())) {
                        if (player->position->delplayer(player->getID()) > 0) return 1;
                    }
                    if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name riceve qualcosa da $name[n]", (Ccharacter *) player, (Ccharacter *) infos.player) > 0) return 1;
                    if (!player->position->getplayer(player->getID())) {
                        if (player->position->addplayer(player) > 0) return 1;
                    }
                    if (player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (!infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->addplayer(infos.player) > 0) return 1;
                    }
                }
                if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                    if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
            } else {
                if (infos.player->pvsend(pvulture.server, "[reset]non puo' portare altro[n]") > 0) return 1;
                if (player->pvsend(pvulture.server, "[reset][n]sei troppo caric%s per poter ricevere %s da %s[n]", (player->getsex() != MALE) ? "a" : "o", objectname = pvulture.objects.getcompletename(object), charactername = pvulture.characters.gettargetname(infos.player, player)) > 0) return 1;
                if (player->spvsend(pvulture.server, sshell) > 0) return 1;
                if (objectname) {
                    pvfree(objectname);
                    objectname = NULL;
                }
                if (charactername) {
                    pvfree(charactername);
                    charactername = NULL;
                }
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]stai parlando di te stess%s![n]", (infos.player->getsex() != MALE) ? "a" : "o") > 0) return 1;


    return 0;
}

int PVaction::give(Cmob *mob, Cobject *object) {
    char *charactername = NULL, *objectname = NULL;
    if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
        if ((pvulture.objects.getweight(object) + pvulture.objects.getweight(mob)) <= getvalue("STATS", "WPoints", mob->logics, 0)) {
            if (infos.player->delobject(object->getID()) > 0) return 1;
            if (mob->addobject(object) > 0) return 1;
            if (infos.player->pvsend(pvulture.server, "[reset]hai dato %s a %s[n]", objectname = pvulture.objects.getcompletename(object), charactername = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
            if (charactername) {
                pvfree(charactername);
                charactername = NULL;
            }
            if ((infos.player->logics.hasvalue("STATUS", "Hide") != 0) ||
                    (mob->logics.hasvalue("STATUS", "Hide") != 0)) {
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                }
                if (spvsend(pvulture.server, infos.player->position, "[reset][n][yellow]$name riceve qualcosa da $name[n]", (Ccharacter *) mob, (Ccharacter *) infos.player) > 0) return 1;
                if (!infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->addplayer(infos.player) > 0) return 1;
                }
            }
            if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
        } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' portare altro[n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
    return 0;
}

int PVaction::eat(char **message) {
    Cobject *object = NULL;
    char *objectname = NULL;
    if ((object = pvulture.objects.get(*message, infos.player)) ||
            ((object = pvulture.objects.get(*message, infos.player->position)) &&
            (object->logics.hasvalue("USAGE", "Single") != 0))) {
        if (object->logics.hasvalue("USAGE", "Eat") == 0) {
            if ((objectname = pvulture.objects.getcompletename(object))) {
                if (pvulture.characters.setbonus(infos.player, object) > 0) return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]gnam gnam![n]") > 0) return 1;
                if (object->logics.hasvalue("USAGE", "Single") == 0) {
                    if (getvalue("USAGE", "Elements", object->logics, 1) <= 1) {
                        if (infos.player->delobject(object->getID()) > 0) return 1;
                        if (pvulture.objects.gameobjects.delobject(object->getID()) > 0) return 1;
                    } else if (object->logics.addvalue("USAGE", "Elements", getvalue("USAGE", "Elements", object->logics, 1) - 1) > 0) LOG_ERROR("Unable to add USAGE->Elements Logic");
                }
                if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                    if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
            } else return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
    return 0;
}

int PVaction::drink(char **message) {
    Cobject *object = NULL;
    char *objectname = NULL;
    if ((object = pvulture.objects.get(*message, infos.player)) ||
            ((object = pvulture.objects.get(*message, infos.player->position)) &&
            (object->logics.hasvalue("USAGE", "Single") != 0))) {
        if (object->logics.hasvalue("USAGE", "Drink") == 0) {
            if ((objectname = pvulture.objects.getcompletename(object))) {
                if (pvulture.characters.setbonus(infos.player, object) > 0) return 1;
                if (infos.player->pvsend(pvulture.server, "[reset][green]glup glup![n]") > 0) return 1;
                if (object->logics.hasvalue("USAGE", "Single") == 0) {
                    if (getvalue("USAGE", "Elements", object->logics, 1) <= 1) {
                        if (infos.player->delobject(object->getID()) > 0) return 1;
                        if (pvulture.objects.gameobjects.delobject(object->getID()) > 0) return 1;
                    } else if (object->logics.addvalue("USAGE", "Elements", getvalue("USAGE", "Elements", object->logics, 1) - 1) > 0) LOG_ERROR("Unable to add USAGE->Elements Logic");
                }
                if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                    if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
            } else return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
    return 0;
}

int PVaction::read(char **message) {
    Cobject *object = NULL;
    int index = 0, counter = 0;
    char *objectname = NULL, *backup = NULL, *text = NULL;
    if ((backup = strings.vpop(message)) && (*message)) index = (_APAGE * (atoi(*message) - 1));
    if (index < 0) index = 0;
    if ((object = pvulture.objects.get(backup, infos.player)) ||
            (object = pvulture.objects.get(backup, infos.player->position))) {
        if ((objectname = pvulture.objects.getcompletename(object))) {
            if (object->logics.hasvalue("USAGE", "Read") == 0) {
                if (object->logics.hascategory("TEXT") == 0) {
                    if (infos.player->pvsend(pvulture.server, "[reset]%s, leggendo dalla riga %d:[n]", objectname, index) > 0) return 1;
                    while ((text = object->logics.getvalue("TEXT", index++)) && (counter++ < _APAGE))
                        if (infos.player->pvsend(pvulture.server, "[reset][yellow]<%d> %s[n]", index - 1, text) > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' essere letto![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
        } else return 1;
        if (objectname) {
            pvfree(objectname);
            objectname = NULL;
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return 0;
}

int PVaction::write(char **message) {
    Cobject *object = NULL;
    int index = 0;
    char *objectname = NULL, *backup = NULL;
    if ((backup = strings.vpop(message)) && (*message)) {
        if ((object = pvulture.objects.get(backup, infos.player)) ||
                (object = pvulture.objects.get(backup, infos.player->position))) {
            if ((objectname = pvulture.objects.getcompletename(object))) {
                if (object->logics.hasvalue("USAGE", "Write") == 0) {
                    if (object->logics.hascategory("TEXT") == 0) {
                        if (infos.player->logics.hascategory("WRITER") == 0) {
                            if (strlen(*message) <= _ALINE) {
                                while (object->logics.hasvalue("TEXT", index++) == 0);
                                if (object->logics.addvalue("TEXT", *message, index - 1) > 0) LOG_ERROR("Unable to add TEXT->Message Logic");
                                if (infos.player->pvsend(pvulture.server, "[reset][green]hai aggiunto una riga a %s[n]", objectname) > 0) return 1;
                            } else if (infos.player->pvsend(pvulture.server, "[reset]troppo lunga! al massimo sono %d caratteri per riga![n]", _ALINE) > 0) return 1;
                        } else if (infos.player->pvsend(pvulture.server, "[reset]non possiedi nulla per scrivere![n]") > 0) return 1;
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' essere scritto![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
            } else return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare una riga di massimo %d caratteri da aggiungere![n]", _ALINE) > 0) return 1;
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return 0;
}

int PVaction::remove(char **message) {
    Cobject *object = NULL;
    int index = 0;
    char *objectname = NULL;
    if ((object = pvulture.objects.get(*message, infos.player)) ||
            (object = pvulture.objects.get(*message, infos.player->position))) {
        if ((objectname = pvulture.objects.getcompletename(object))) {
            if (object->logics.hasvalue("USAGE", "Write") == 0) {
                if (object->logics.hascategory("TEXT") == 0) {
                    if (infos.player->logics.hascategory("WRITER") == 0) {
                        while (object->logics.hasvalue("TEXT", index++) == 0);
                        if (object->logics.hasvalue("TEXT", (index - 2)) == 0) {
                            if (object->logics.delvalue("TEXT", (index - 2)) > 0) LOG_ERROR("Unable to del TEXT->Message Logic");
                            if (infos.player->pvsend(pvulture.server, "[reset][green]hai cancellato una riga da %s[n]", objectname) > 0) return 1;
                        } else if (infos.player->pvsend(pvulture.server, "[reset]non c'e' scritto nulla![n]") > 0) return 1;
                    } else if (infos.player->pvsend(pvulture.server, "[reset]non possiedi nulla per scrivere![n]") > 0) return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]non puo' essere scritto![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
        } else return 1;
        if (objectname) {
            pvfree(objectname);
            objectname = NULL;
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
    return 0;
}

int PVaction::equip(char **message) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    Cobject *object = NULL;
    int value = 0;
    char *string = NULL;
    if ((string = strings.vpop(message))) {
        if ((object = pvulture.objects.get(string, infos.player))) {
            if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
                if (object->logics.hasvalue("USAGE", "Equip") == 0) {
                    if (*message) {
                        if ((player = pvulture.characters.getplayer(*message, infos.player->position))) {
                            value = equip(player, object);
                        } else if ((mob = pvulture.characters.getmob(*message, infos.player->position))) {
                            value = equip(mob, object);
                        } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0) return 1;
                    } else value = equip(infos.player, object);
                } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]lo stai usando proprio ora![n]") > 0) return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nulla del genere![n]") > 0) return 1;
    } else return 1;
    if (string) {
        pvfree(string);
        string = NULL;
    }
    return value;
}

int PVaction::equip(Cplayer *player, Cobject *object) {
    char *objectname = NULL, *playername = NULL, *targetname = NULL, *buffer = NULL;
    if ((player->getID() == infos.player->getID()) ||
            (object->logics.hasvalue("TARGETS", "Player") == 0)) {
        if (((object->logics.hasvalue("USAGE", "RA") == 0) ||
                (object->logics.hasvalue("USAGE", "RH") == 0) ||
                (object->logics.hasvalue("USAGE", "LA") == 0) ||
                (object->logics.hasvalue("USAGE", "LH") == 0)) &&
                (getvalue("STATS", "Arms", player->logics, 0) <= 0)) {
            if (infos.player->pvsend(pvulture.server, "[reset]%s le braccia gravemente danneggiate![n]", (player->getID() == infos.player->getID()) ? "hai" : "ha") > 0) return 1;
        } else if (((object->logics.hasvalue("USAGE", "RL") == 0) ||
                (object->logics.hasvalue("USAGE", "RF") == 0) ||
                (object->logics.hasvalue("USAGE", "LL") == 0) ||
                (object->logics.hasvalue("USAGE", "LF") == 0)) &&
                (getvalue("STATS", "Legs", player->logics, 0) <= 0)) {
            if (infos.player->pvsend(pvulture.server, "[reset]%s le gambe gravemente danneggiate![n]", (player->getID() == infos.player->getID()) ? "hai" : "ha") > 0) return 1;
        } else {
            if ((objectname = pvulture.objects.getcompletename(object))) {
                if (pvulture.characters.setbonus(player, object) > 0) return 1;
                if (infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                }
                if (object->logics.hasvalue("USAGE", "Single") == 0) {
                    if (player->getID() == infos.player->getID()) {
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name ha usato %s[n]", objectname)), (Ccharacter *) infos.player) > 0) return 1;
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (infos.player->pvsend(pvulture.server, "[reset][green]hai usato %s![n]", objectname) > 0) return 1;
                    } else {
                        if (player->position->getplayer(player->getID())) {
                            if (player->position->delplayer(player->getID()) > 0) return 1;
                        }
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name ha usato %s su $name[n]", objectname)), (Ccharacter *) infos.player, (Ccharacter *) player) > 0) return 1;
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (infos.player->pvsend(pvulture.server, "[reset][green]hai usato %s su %s![n]", objectname, targetname = pvulture.characters.gettargetname(player, infos.player)) > 0) return 1;
                        if (player->pvsend(pvulture.server, "[reset][n][green]%s ha usato %s su di te![n]", playername = pvulture.characters.gettargetname(infos.player, player), objectname) > 0) return 1;
                        if (player->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (playername) {
                            pvfree(playername);
                            playername = NULL;
                        }
                        if (targetname) {
                            pvfree(targetname);
                            targetname = NULL;
                        }
                        if (!player->position->getplayer(player->getID())) {
                            if (player->position->addplayer(player) > 0) return 1;
                        }
                    }
                } else {
                    if (player->getID() == infos.player->getID()) {
                        if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name ha equipaggiato %s[n]", objectname)), (Ccharacter *) infos.player) > 0) return 1;
                        if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                        if (infos.player->pvsend(pvulture.server, "[reset][green]hai equipaggiato %s![n]", objectname) > 0) return 1;
                    } else if (give(player, object) > 0) return 1;
                }
                if (!infos.player->position->getplayer(infos.player->getID())) {
                    if (infos.player->position->addplayer(infos.player) > 0) return 1;
                }
                if (object->logics.addvalue("USAGE", "Use", 1) > 0) LOG_ERROR("Unable to add USAGE->Use Logic");
                if (object->logics.hasvalue("USAGE", "Single") == 0) {
                    if (getvalue("USAGE", "Elements", object->logics, 1) <= 1) {
                        if ((infos.player->delobject(object->getID()) > 0) &&
                                (infos.player->position->delobject(object->getID()) > 0)) return 1;
                        if (pvulture.objects.gameobjects.delobject(object->getID()) > 0) return 1;
                    } else if (object->logics.addvalue("USAGE", "Elements", getvalue("USAGE", "Elements", object->logics, 1) - 1) > 0) LOG_ERROR("Unable to add USAGE->Elements Logic");
                }
                if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                    if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
            } else return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVaction::equip(Cmob *mob, Cobject *object) {
    char *objectname = NULL, *targetname = NULL, *buffer = NULL;
    if (object->logics.hasvalue("TARGETS", "Mob") == 0) {
        if (((object->logics.hasvalue("USAGE", "RA") == 0) ||
                (object->logics.hasvalue("USAGE", "RH") == 0) ||
                (object->logics.hasvalue("USAGE", "LA") == 0) ||
                (object->logics.hasvalue("USAGE", "LH") == 0)) &&
                (getvalue("STATS", "Arms", mob->logics, 0) <= 0)) {
            if (infos.player->pvsend(pvulture.server, "[reset]ha le braccia gravemente danneggiate![n]") > 0) return 1;
        } else if (((object->logics.hasvalue("USAGE", "RL") == 0) ||
                (object->logics.hasvalue("USAGE", "RF") == 0) ||
                (object->logics.hasvalue("USAGE", "LL") == 0) ||
                (object->logics.hasvalue("USAGE", "LF") == 0)) &&
                (getvalue("STATS", "Legs", mob->logics, 0) <= 0)) {
            if (infos.player->pvsend(pvulture.server, "[reset]ha le gambe gravemente danneggiate![n]") > 0) return 1;
        } else {
            if ((objectname = pvulture.objects.getcompletename(object))) {
                if (pvulture.characters.setbonus(mob, object) > 0) return 1;
                if (object->logics.hasvalue("USAGE", "Single") == 0) {
                    if (infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->delplayer(infos.player->getID()) > 0) return 1;
                    }
                    if (spvsend(pvulture.server, infos.player->position, (buffer = allocate.vsalloc("[reset][n][yellow]$name ha usato %s su $name[n]", objectname)), (Ccharacter *) infos.player, (Ccharacter *) mob) > 0) return 1;
                    if (infos.player->position->spvsend(pvulture.server, sshell) > 0) return 1;
                    if (infos.player->pvsend(pvulture.server, "[reset][green]hai usato %s su %s![n]", objectname, targetname = pvulture.characters.gettargetname(mob, infos.player)) > 0) return 1;
                    if (targetname) {
                        pvfree(targetname);
                        targetname = NULL;
                    }
                    if (!infos.player->position->getplayer(infos.player->getID())) {
                        if (infos.player->position->addplayer(infos.player) > 0) return 1;
                    }
                } else if (give(mob, object) > 0) return 1;
                if (object->logics.addvalue("USAGE", "Use", 1) > 0) LOG_ERROR("Unable to add USAGE->Use Logic");
                if (object->logics.hasvalue("USAGE", "Single") == 0) {
                    if (getvalue("USAGE", "Elements", object->logics, 1) <= 1) {
                        if ((infos.player->delobject(object->getID()) > 0) &&
                                (infos.player->position->delobject(object->getID()) > 0)) return 1;
                        if (pvulture.objects.gameobjects.delobject(object->getID()) > 0) return 1;
                    } else if (object->logics.addvalue("USAGE", "Elements", getvalue("USAGE", "Elements", object->logics, 1) - 1) > 0) LOG_ERROR("Unable to add USAGE->Elements Logic");
                }
                if ((infos.player->logics.hascategory("FIGHT") == 0) && (infos.player->logics.hasvalue("FIGHT", "Last") == 0))
                    if (infos.player->logics.addvalue("FIGHT", "Last", (infos.player->logics.getvalue("FIGHT", "Last") + _AADELAY)) > 0) LOG_ERROR("Unable to add FIGHT->Last Logic");
            } else return 1;
            if (objectname) {
                pvfree(objectname);
                objectname = NULL;
            }
        }
    } else if (infos.player->pvsend(pvulture.server, "[reset]non e' nulla che possa essere usato in questo modo![n]") > 0) return 1;
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}
PVobject objects;
PVaction actions;

int runobjects(void) {
    if (compare.vcmpcase(infos.message, CSTRSIZE("prendi")) == 0) {
        if (actions.take() > 0) LOG_ERROR("Unable to run OBJECTS.TAKE()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("lascia")) == 0) {
        if (actions.drop() > 0) LOG_ERROR("Unable to run OBJECTS.DROP()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("dai")) == 0) {
        if (actions.give() > 0) LOG_ERROR("Unable to run OBJECTS.GIVE()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("metti")) == 0) {
        if (actions.put() > 0) LOG_ERROR("Unable to run OBJECTS.PUT()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("mangia")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("bevi")) == 0)) {
        if (actions.use() > 0) LOG_ERROR("Unable to run OBJECTS.USE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("leggi")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("scrivi")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("cancella")) == 0)) {
        if (actions.use() > 0) LOG_ERROR("Unable to run OBJECTS.USE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("usa")) == 0) ||
            (compare.vcmpcase(infos.message, CSTRSIZE("indossa")) == 0)) {
        if (actions.use() > 0) LOG_ERROR("Unable to run OBJECTS.USE()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("riponi")) == 0) {
        if (actions.replace() > 0) LOG_ERROR("Unable to run OBJECTS.REPLACE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("duplica")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (objects.duplicate() > 0) LOG_ERROR("Unable to run OBJECTS.DUPLICTE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("genera")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (objects.generate() > 0) LOG_ERROR("Unable to run OBJECTS.GENERATE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("distruggi")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (objects.destroy() > 0) LOG_ERROR("Unable to run OBJECTS.DESTROY()");
    } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
    return 0;
}
