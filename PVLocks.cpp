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
#include "PVLocks.h"

char *PVlock::getopposite(char *direction) {
    char *opposite = NULL;
    if (compare.vcmpcase(direction, "nord") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("sud") + 1))) {
            strcpy(opposite, "sud");
            opposite[sizeof ("sud")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "sud") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("nord") + 1))) {
            strcpy(opposite, "nord");
            opposite[sizeof ("nord")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "ovest") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("est") + 1))) {
            strcpy(opposite, "est");
            opposite[sizeof ("est")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "est") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("ovest") + 1))) {
            strcpy(opposite, "ovest");
            opposite[sizeof ("ovest")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "su") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("giu") + 1))) {
            strcpy(opposite, "giu");
            opposite[sizeof ("giu")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "giu") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("su") + 1))) {
            strcpy(opposite, "su");
            opposite[sizeof ("su")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "nordovest") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("sudest") + 1))) {
            strcpy(opposite, "sudest");
            opposite[sizeof ("sudest")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "nordest") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("sudovest") + 1))) {
            strcpy(opposite, "sudovest");
            opposite[sizeof ("sudovest")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "sudovest") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("nordest") + 1))) {
            strcpy(opposite, "nordest");
            opposite[sizeof ("nordest")] = '\0';
        }
    } else if (compare.vcmpcase(direction, "sudest") == 0) {
        if ((opposite = (char *) pvmalloc(sizeof ("nordovest") + 1))) {
            strcpy(opposite, "nordovest");
            opposite[sizeof ("nordovest")] = '\0';
        }
    }
    return opposite;
}

int PVlock::open(char *direction, Cplayer *player) {
    archeslist *archeslist = NULL, *backuplist = NULL;
    char *opposite = NULL;
    if ((archeslist = player->position->getarch(direction))) {
        if (archeslist->door) {
            if (archeslist->lock) {
                if (player->pvsend(pvulture.server, "[reset]quella direzione e' bloccata![n]") > 0) return 1;
            } else if (archeslist->close) {
                if ((opposite = getopposite(direction)))
                    if ((backuplist = archeslist->tile->getarch(opposite))) {
                        backuplist->close = false;
                        backuplist->lock = false;
                    }
                archeslist->close = false;
                archeslist->lock = false;
                if (player->pvsend(pvulture.server, "[green]hai aperto l'uscita a %s![n]", direction) > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]quella direzione e' gia' aperta![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla da aprire li![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla a %s![n]", direction) > 0) return 1;
    if (opposite) {
        pvfree(opposite);
        opposite = NULL;
    }
    return 0;
}

int PVlock::open(Cobject *object, Cplayer *player) {
    if (object->logics.hascategory("CONTAINER") == 0) {
        if (getvalue("CONTAINER", "Open", object->logics, 0) == 0) {
            if (getvalue("CONTAINER", "Lock", object->logics, 0) == 0) {
                if (object->logics.addvalue("CONTAINER", "Open", 1) > 0) LOG_ERROR("Unable to add CONTAINER->Open Logic");
                else if (object->logics.addvalue("CONTAINER", "Lock", 0) > 0) LOG_ERROR("Unable to add CONTAINER->Open Logic");
                else if (player->pvsend(pvulture.server, "[reset][green]l'hai aperto![n]") > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]questo oggetto e' bloccato![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]questo oggetto e' gia' aperto![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]questo oggetto non puo' essere aperto![n]") > 0) return 1;
    return 0;
}

int PVlock::close(char *direction, Cplayer *player) {
    archeslist *archeslist = NULL, *backuplist = NULL;
    char *opposite = NULL;
    if ((archeslist = player->position->getarch(direction))) {
        if (archeslist->door) {
            if (!archeslist->close) {
                if ((opposite = getopposite(direction)))
                    if ((backuplist = archeslist->tile->getarch(opposite))) {
                        backuplist->close = true;
                        backuplist->lock = false;
                    }
                archeslist->close = true;
                archeslist->lock = false;
                if (player->pvsend(pvulture.server, "[green]hai chiuso l'uscita a %s![n]", direction) > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]quella direzione e' gia' chiusa![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla da chiudere li![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla a %s![n]", direction) > 0) return 1;
    if (opposite) {
        pvfree(opposite);
        opposite = NULL;
    }
    return 0;
}

int PVlock::close(Cobject *object, Cplayer *player) {
    if (object->logics.hascategory("CONTAINER") == 0) {
        if (getvalue("CONTAINER", "Open", object->logics, 0) == 1) {
            if (object->logics.addvalue("CONTAINER", "Open", 0) > 0) LOG_ERROR("Unable to add CONTAINER->Open Logic");
            else if (object->logics.addvalue("CONTAINER", "Lock", 0) > 0) LOG_ERROR("Unable to add CONTAINER->Lock Logic");
            else if (player->pvsend(pvulture.server, "[reset][green]l'hai chiuso![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]questo oggetto e' gia' chiuso![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]questo oggetto non puo' essere chiuso![n]") > 0) return 1;
    return 0;
}

int PVlock::lock(char *direction, Cplayer *player) {
    objectslist *objectslist = NULL;
    archeslist *archeslist = NULL, *backuplist = NULL;
    char *opposite = NULL;
    if ((archeslist = player->position->getarch(direction))) {
        if (archeslist->door) {
            if (archeslist->close) {
                if (!archeslist->lock) {
                    player->resetvalue();
                    while ((objectslist = player->nextvalue()) && (!archeslist->lock)) {
                        if (objectslist->object->logics.hasvalue("USAGE", "Key") == 0) {
                            if (objectslist->object->logics.getvalue("USAGE", "Key") == (int) archeslist->key) archeslist->lock = true;
                        }
                    }
                    if (archeslist->lock) {
                        if (player->pvsend(pvulture.server, "[green]hai bloccato l'uscita a %s![n]", direction) > 0) return 1;
                        if ((opposite = getopposite(direction))) {
                            if ((backuplist = archeslist->tile->getarch(opposite))) {
                                if (backuplist->key == archeslist->key) {
                                    backuplist->lock = true;
                                }
                                backuplist->close = true;
                            }
                        }
                    } else if (player->pvsend(pvulture.server, "[reset]non hai la chiave per bloccare l'uscita a %s![n]", direction) > 0) return 1;
                } else if (player->pvsend(pvulture.server, "[reset]quella direzione e' gia' bloccata![n]") > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]prima e' necessario chiudere l'uscita![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla da chiudere li![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla a %s![n]", direction) > 0) return 1;
    if (opposite) {
        pvfree(opposite);
        opposite = NULL;
    }
    return 0;
}

int PVlock::lock(Cobject *object, Cplayer *player) {
    objectslist *objectslist = NULL;
    int keycode = 0;
    if (object->logics.hascategory("CONTAINER") == 0) {
        if (getvalue("CONTAINER", "Open", object->logics, 0) == 0) {
            if (getvalue("CONTAINER", "Lock", object->logics, 0) == 0) {
                player->resetvalue();
                keycode = getvalue("CONTAINER", "Key", object->logics, 0);
                while ((objectslist = player->nextvalue()) && (getvalue("CONTAINER", "Lock", object->logics, 0) != 1)) {
                    if (objectslist->object->logics.hasvalue("USAGE", "Key") == 0) {
                        if (objectslist->object->logics.getvalue("USAGE", "Key") == keycode) {
                            if (object->logics.addvalue("CONTAINER", "Open", 0) > 0) LOG_ERROR("Unable to add CONTAINER->Open Logic");
                            else if (object->logics.addvalue("CONTAINER", "Lock", 1) > 0) LOG_ERROR("Unable to add CONTAINER->Lock Logic");
                        }
                    }
                }
                if (getvalue("CONTAINER", "Lock", object->logics, 0) == 1) {
                    if (player->pvsend(pvulture.server, "[reset][green]l'hai bloccato![n]") > 0) return 1;
                } else if (player->pvsend(pvulture.server, "[reset]non hai la chiave necessaria![n]") > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]questo oggetto e' gia' bloccato![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]prima e' necessario chiudere l'oggetto[n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]questo oggetto non puo' essere bloccato![n]") > 0) return 1;
    return 0;
}

int PVlock::unlock(char *direction, Cplayer *player) {
    objectslist *objectslist = NULL;
    archeslist *archeslist = NULL, *backuplist = NULL;
    char *opposite = NULL;
    if ((archeslist = player->position->getarch(direction))) {
        if (archeslist->door) {
            if (archeslist->lock) {
                player->resetvalue();
                while ((objectslist = player->nextvalue()) && (archeslist->lock)) {
                    if (objectslist->object->logics.hasvalue("USAGE", "Key") == 0) {
                        if (objectslist->object->logics.getvalue("USAGE", "Key") == (int) archeslist->key) archeslist->lock = false;
                    }
                }
                if (!archeslist->lock) {
                    if (player->pvsend(pvulture.server, "[green]hai sbloccato l'uscita a %s![n]", direction) > 0) return 1;
                    if ((opposite = getopposite(direction))) {
                        if ((backuplist = archeslist->tile->getarch(opposite))) {
                            if (backuplist->key == archeslist->key) {
                                backuplist->lock = false;
                            }
                            backuplist->close = true;
                        }
                    }
                } else if (player->pvsend(pvulture.server, "[reset]non hai la chiave per sbloccare l'uscita a %s![n]", direction) > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]quella direzione e' gia' sbloccata![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla da aprire li![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]non c'e' nulla a %s![n]", direction) > 0) return 1;
    if (opposite) {
        pvfree(opposite);
        opposite = NULL;
    }
    return 0;
}

int PVlock::unlock(Cobject *object, Cplayer *player) {
    objectslist *objectslist = NULL;
    int keycode = 0;
    if (object->logics.hascategory("CONTAINER") == 0) {
        if (getvalue("CONTAINER", "Lock", object->logics, 0) == 1) {
            player->resetvalue();
            keycode = getvalue("CONTAINER", "Key", object->logics, 0);
            while ((objectslist = player->nextvalue()) && (getvalue("CONTAINER", "Lock", object->logics, 0) != 0)) {
                if (objectslist->object->logics.hasvalue("USAGE", "Key") == 0) {
                    if (objectslist->object->logics.getvalue("USAGE", "Key") == keycode) {
                        if (object->logics.addvalue("CONTAINER", "Open", 0) > 0) LOG_ERROR("Unable to add CONTAINER->Open Logic");
                        else if (object->logics.addvalue("CONTAINER", "Lock", 0) > 0) LOG_ERROR("Unable to add CONTAINER->Lock Logic");
                    }
                }
            }
            if (getvalue("CONTAINER", "Lock", object->logics, 0) == 0) {
                if (player->pvsend(pvulture.server, "[reset][green]l'hai sbloccato![n]") > 0) return 1;
            } else if (player->pvsend(pvulture.server, "[reset]non hai la chiave necessaria![n]") > 0) return 1;
        } else if (player->pvsend(pvulture.server, "[reset]questo oggetto e' gia' sbloccato![n]") > 0) return 1;
    } else if (player->pvsend(pvulture.server, "[reset]questo oggetto non puo' essere sbloccato![n]") > 0) return 1;
    return 0;
}

PVlock lockcommands;

int runlock(void) {
    Cobject *object = NULL;
    char *command = NULL, *message = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if ((object = pvulture.objects.get(message, infos.player)) || (object = pvulture.objects.get(message, infos.player->position))) {
                if (compare.vcmpcase(command, CSTRSIZE("apri")) == 0) {
                    if (lockcommands.open(object, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.OPEN()");
                } else if (compare.vcmpcase(command, CSTRSIZE("chiudi")) == 0) {
                    if (lockcommands.close(object, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.CLOSE()");
                } else if (compare.vcmpcase(command, CSTRSIZE("blocca")) == 0) {
                    if (lockcommands.lock(object, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.LOCK()");
                } else if (compare.vcmpcase(command, CSTRSIZE("sblocca")) == 0) {
                    if (lockcommands.unlock(object, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.UNLOCK()");
                } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
            } else {
                if (compare.vcmpcase(message, "n") == 0) {
                    pvfree(message);
                    message = NULL;
                    if ((message = (char *) pvmalloc(sizeof ("nord") + 1))) {
                        strcpy(message, "nord");
                        message[sizeof ("nord")] = '\0';
                    } else return 1;
                } else if (compare.vcmpcase(message, "s") == 0) {
                    pvfree(message);
                    message = NULL;
                    if ((message = (char *) pvmalloc(sizeof ("sud") + 1))) {
                        strcpy(message, "sud");
                        message[sizeof ("sud")] = '\0';
                    } else return 1;
                } else if (compare.vcmpcase(message, "o") == 0) {
                    pvfree(message);
                    message = NULL;
                    if ((message = (char *) pvmalloc(sizeof ("ovest") + 1))) {
                        strcpy(message, "ovest");
                        message[sizeof ("ovest")] = '\0';
                    } else return 1;
                } else if (compare.vcmpcase(message, "e") == 0) {
                    pvfree(message);
                    message = NULL;
                    if ((message = (char *) pvmalloc(sizeof ("est") + 1))) {
                        strcpy(message, "est");
                        message[sizeof ("est")] = '\0';
                    } else return 1;
                }
                if (compare.vcmpcase(command, CSTRSIZE("apri")) == 0) {
                    if (lockcommands.open(message, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.OPEN()");
                } else if (compare.vcmpcase(command, CSTRSIZE("chiudi")) == 0) {
                    if (lockcommands.close(message, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.CLOSE()");
                } else if (compare.vcmpcase(command, CSTRSIZE("blocca")) == 0) {
                    if (lockcommands.lock(message, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.LOCK()");
                } else if (compare.vcmpcase(command, CSTRSIZE("sblocca")) == 0) {
                    if (lockcommands.unlock(message, infos.player) > 0) LOG_ERROR("Unable to run LOCKCOMMANDS.UNLOCK()");
                } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0) return 1;
            }
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un oggetto o una direzione[n]") > 0) return 1;
    } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un oggetto o una direzione[n]") > 0) return 1;
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
