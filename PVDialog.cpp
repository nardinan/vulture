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
#include "PVDialog.h"

int PVdialog::say(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value;
    char *message = NULL, *text = NULL, *emoticon = NULL, *command = NULL, *pointer = NULL, *buffer = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        emoticon = getemoticon(&message);
        if ((command = strings.vpop(&message)) && (message)) {
            if ((pointer = strchr(message, ':'))) {
                for (text = pointer + 1; *text == ' '; text++);
                do {
                    *pointer-- = '\0';
                } while ((pointer > message) && (*pointer == ' '));
                if (strlen(text) > 0) {
                    if ((player = pvulture.characters.getplayer(message, infos.player->position)))
                        value = say(player, emoticon, text);
                    else if ((mob = pvulture.characters.getmob(message, infos.player->position)))
                        value = say(mob, emoticon, text);
                    else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0)
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un messaggio![n]") > 0)
                    return 1;
            } else
                value = say(emoticon, message);
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un messaggio![n]") > 0)
            return 1;
    } else
        return 1;
    if (command) {
        pvfree(command);
        command = NULL;
    }
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
    return value;
}

int PVdialog::say(Cplayer *player, char *emoticon, char *text) {
    char *charactername = NULL, *buffer = NULL;
    int index = strlen(text) - 1;
    while ((index > 0) && (text[index] != '?'))
        index--;
    if (player->getID() != infos.player->getID()) {
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]%s a %s %s:\"%s\"[n]",
                (text[index] != '?') ? "dici" : "chiedi",
                charactername = pvulture.characters.gettargetname(player, infos.player),
                (emoticon) ? emoticon : "",
                text) > 0)
            return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
        if (player->pvsend(pvulture.server, "[reset][n][green]%s ti %s %s:\"%s\"[n]",
                charactername = pvulture.characters.gettargetname(infos.player, player),
                (text[index] != '?') ? "dice" : "chiede",
                (emoticon) ? emoticon : "",
                text) > 0)
            return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0)
                return 1;
        }
        if (infos.player->position->delplayer(player->getID()) > 0)
            return 1;
        if (spvsend(pvulture.server, infos.player->position,
                (buffer = allocate.vsalloc("[n][yellow]$name %s a $name %s:\"%s\"[n]",
                (text[index] != '?') ? "dice" : "chiede",
                (emoticon) ? emoticon : "",
                text)),
                (Ccharacter *) infos.player, (Ccharacter *) player) > 0)
            return 1;
        if (!player->position->getplayer(player->getID())) {
            if (player->position->addplayer(player) > 0)
                return 1;
        }
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0)
                return 1;
        }
    } else {
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]%s a te stess%s %s:\"%s\"[n]",
                (text[index] != '?') ? "dici" : "chiedi",
                (infos.player->getsex() != MALE) ? "a" : "o",
                (emoticon) ? emoticon : "",
                text) > 0)
            return 1;
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0)
                return 1;
        }
        if (spvsend(pvulture.server, infos.player->position,
                (buffer = allocate.vsalloc("[n][yellow]$name %s a se stess%s %s:\"%s\"[n]",
                (text[index] != '?') ? "dice" : "chiede",
                (infos.player->getsex() != MALE) ? "a" : "o",
                (emoticon) ? emoticon : "",
                text)),
                (Ccharacter *) infos.player) > 0)
            return 1;
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0)
                return 1;
        }
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVdialog::say(Cmob *mob, char *emoticon, char *text) {
    char *charactername = NULL, *buffer = NULL;
    int index = strlen(text) - 1;
    while ((index > 0) && (text[index] != '?'))
        index--;
    if (infos.player->pvsend(pvulture.server, "[reset][yellow]%s a %s %s:\"%s\"[n]",
            (text[index] != '?') ? "dici" : "chiedi",
            charactername = pvulture.characters.gettargetname(mob, infos.player),
            (emoticon) ? emoticon : "",
            text) > 0)
        return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    if (infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->delplayer(infos.player->getID()) > 0)
            return 1;
    }
    if (spvsend(pvulture.server, infos.player->position,
            (buffer = allocate.vsalloc("[n][yellow]$name %s a $name %s:\"%s\"[n]",
            (text[index] != '?') ? "dice" : "chiede",
            (emoticon) ? emoticon : "",
            text)),
            (Ccharacter *) infos.player, (Ccharacter *) mob) > 0)
        return 1;
    if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
        return 1;
    if (!infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->addplayer(infos.player) > 0)
            return 1;
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVdialog::say(char *emoticon, char *text) {
    char *buffer = NULL;
    int index = strlen(text) - 1;
    while ((index > 0) && (text[index] != '?'))
        index--;
    if (infos.player->pvsend(pvulture.server, "[reset][yellow]%s %s:\"%s\"[n]",
            (text[index] != '?') ? "dici" : "chiedi",
            (emoticon) ? emoticon : "",
            text) > 0)
        return 1;
    if (infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->delplayer(infos.player->getID()) > 0)
            return 1;
    }
    if (spvsend(pvulture.server, infos.player->position,
            (buffer = allocate.vsalloc("[n][yellow]$name %s %s:\"%s\"[n]",
            (text[index] != '?') ? "dice" : "chiede",
            (emoticon) ? emoticon : "",
            text)),
            (Ccharacter *) infos.player) > 0)
        return 1;
    if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
        return 1;
    if (!infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->addplayer(infos.player) > 0)
            return 1;
    }
    if (buffer) {
        pvfree(buffer);
        buffer = NULL;
    }
    return 0;
}

int PVdialog::whisper(void) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    int value;
    char *message = NULL, *text = NULL, *emoticon = NULL, *command = NULL, *pointer = NULL, *buffer = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        emoticon = getemoticon(&message);
        if ((command = strings.vpop(&message)) && (message)) {
            if ((pointer = strchr(message, ':'))) {
                for (text = pointer + 1; *text == ' '; text++);
                do {
                    *pointer-- = '\0';
                } while ((pointer > message) && (*pointer == ' '));
                if (strlen(text) > 0) {
                    if ((player = pvulture.characters.getplayer(message, infos.player->position)))
                        value = whisper(player, emoticon, text);
                    else if ((mob = pvulture.characters.getmob(message, infos.player->position)))
                        value = whisper(mob, emoticon, text);
                    else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0)
                        return 1;
                } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un messaggio![n]") > 0)
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]non vedi nessuno con quel nome qui![n]") > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un messaggio![n]") > 0)
            return 1;
    } else
        return 1;
    if (command) {
        pvfree(command);
        command = NULL;
    }
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
    return value;
}

int PVdialog::whisper(Cplayer *player, char *emoticon, char *text) {
    char *charactername = NULL;
    if (player->getID() != infos.player->getID()) {
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]sussurri a %s %s:\"%s\"[n]",
                charactername = pvulture.characters.gettargetname(player, infos.player),
                (emoticon) ? emoticon : "",
                text) > 0)
            return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
        if (player->pvsend(pvulture.server, "[reset][n][green]%s ti sussurra %s:\"%s\"[n]",
                charactername = pvulture.characters.gettargetname(infos.player, player),
                (emoticon) ? emoticon : "",
                text) > 0)
            return 1;
        if (charactername) {
            pvfree(charactername);
            charactername = NULL;
        }
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0)
                return 1;
        }
        if (infos.player->position->delplayer(player->getID()) > 0)
            return 1;
        if (spvsend(pvulture.server, infos.player->position, "[n][yellow]$name sussurra qualcosa a $name[n]",
                (Ccharacter *) infos.player,
                (Ccharacter *) player) > 0)
            return 1;
        if (!player->position->getplayer(player->getID())) {
            if (player->position->addplayer(player) > 0)
                return 1;
        }
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0)
                return 1;
        }
    } else {
        if (infos.player->pvsend(pvulture.server, "[reset][yellow]sussurri tra te e te %s:\"%s\"[n]",
                (emoticon) ? emoticon : "",
                text) > 0)
            return 1;
        if (infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->delplayer(infos.player->getID()) > 0)
                return 1;
        }
        if (spvsend(pvulture.server, infos.player->position, "[n][yellow]$name sussurra qualcosa tra se e se[n]",
                (Ccharacter *) infos.player) > 0)
            return 1;
        if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
            return 1;
        if (!infos.player->position->getplayer(infos.player->getID())) {
            if (infos.player->position->addplayer(infos.player) > 0)
                return 1;
        }
    }
    return 0;
}

int PVdialog::whisper(Cmob *mob, char *emoticon, char *text) {
    char *charactername = NULL;
    if (infos.player->pvsend(pvulture.server, "[reset][yellow]sussurri a %s %s:\"%s\"[n]",
            charactername = pvulture.characters.gettargetname(mob, infos.player),
            (emoticon) ? emoticon : "",
            text) > 0)
        return 1;
    if (charactername) {
        pvfree(charactername);
        charactername = NULL;
    }
    if (infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->delplayer(infos.player->getID()) > 0)
            return 1;
    }
    if (spvsend(pvulture.server, infos.player->position, "[n][yellow]$name sussurra qualcosa a $name[n]",
            (Ccharacter *) infos.player,
            (Ccharacter *) mob) > 0)
        return 1;
    if (infos.player->position->spvsend(pvulture.server, sshell) > 0)
        return 1;
    if (!infos.player->position->getplayer(infos.player->getID())) {
        if (infos.player->position->addplayer(infos.player) > 0)
            return 1;
    }
    return 0;
}

int PVdialog::announce(void) {
    char *message = NULL, *command = NULL, *emoticon = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        emoticon = getemoticon(&message);
        if ((command = strings.vpop(&message)) && (message)) {
            if (strlen(message) > 0) {
                if (infos.player->pvsend(pvulture.server, "[green]messaggio multiutente:") > 0)
                    return 1;
                if (pvulture.characters.gamecharacters.pvsend(pvulture.server, "[n][blue]%s annuncia %s: \"%s\"[n]",
                        pvulture.characters.getsimplename(infos.player),
                        (emoticon) ? emoticon : "",
                        message) > 0)
                    return 1;
                if (pvulture.characters.gamecharacters.spvsend(pvulture.server, sshell) > 0)
                    return 1;
                shell = false;
            } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un annuncio![n]") > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un annuncio![n]") > 0)
            return 1;
    } else
        return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    if (command) {
        pvfree(command);
        command = NULL;
    }
    if (emoticon) {
        pvfree(emoticon);
        emoticon = NULL;
    }
    return 0;
}

int PVdialog::think(void) {
    char buffer[_DEFAULT_MIND_LEN], *message = NULL, *command = NULL;
    if ((message = (char *) pvmalloc(strlen(infos.message) + 1))) {
        strcpy(message, infos.message);
        message[strlen(infos.message)] = '\0';
        if ((command = strings.vpop(&message)) && (message)) {
            if (strlen(message) > 0) {
                snprintf(buffer, _DEFAULT_MIND_LEN, "[ID: %d] %s", infos.player->getID(), pvulture.characters.getsimplename(infos.player));
                if (pvulture.pages.addpage(buffer, message) > 0) LOG_ERROR("Unable to add PAGE from %s", buffer);
                else if (infos.player->pvsend(pvulture.server, "[reset][green]richiesta inoltrata![n]") > 0)
                    return 1;
            } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un messaggio![n]") > 0)
                return 1;
        } else if (infos.player->pvsend(pvulture.server, "[reset]devi specificare un messaggio![n]") > 0)
            return 1;
    } else
        return 1;
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

int PVdialog::listen(void) {
    char *message = pvulture.pages.getpage(true);
    if (infos.player->pvsend(pvulture.server, message) > 0)
        return 1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    return 0;
}
PVdialog dialogcommands;

int dialog(void) {
    if (compare.vcmpcase(infos.message, CSTRSIZE("di")) == 0) {
        if (dialogcommands.say() > 0)
            LOG_ERROR("Unable to run DIALOGCOMMANDS.SAY()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("sussurra")) == 0) {
        if (dialogcommands.whisper() > 0)
            LOG_ERROR("Unable to run DIALOGCOMMANDS.WISHPER()");
    } else if (compare.vcmpcase(infos.message, CSTRSIZE("pensa")) == 0) {
        if (dialogcommands.think() > 0)
            LOG_ERROR("Unable to run DIALOGCOMMANDS.THINK()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("annuncia")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (dialogcommands.announce() > 0)
            LOG_ERROR("Unable to run DIALOGCOMMANDS.ANNOUNCE()");
    } else if ((compare.vcmpcase(infos.message, CSTRSIZE("ascolta")) == 0) &&
            ((infos.player->logics.hasvalue("RANK", "Admin") == 0) ||
            (infos.player->logics.hasvalue("RANK", "Moderator") == 0))) {
        if (dialogcommands.listen() > 0)
            LOG_ERROR("Unable to run DIALOGCOMMANDS.LISTEN()");
    } else if (infos.player->pvsend(pvulture.server, "[reset]prego?[n]") > 0)
        return 1;
    return 0;
}
