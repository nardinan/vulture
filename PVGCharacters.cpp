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
#include "PVGCharacters.h"
shash chacharas[] = {
    {"ST", "forza        "},
    {"DE", "destrezza    "},
    {"IN", "intelligenza "},
    {"CH", "carisma      "},
    {"CO", "costituzione "},
    {"AC", "precisione   "},
    {"SP", "velocita'    "},
    {"NULL", "NULL"}
};

Cplayer *Cgcharacters::getplayer(char *name, Ctile *tile) {
    Cplayer *result = NULL;
    playerslist *list = tile->getplayersroot();
    char *playername = NULL;
    int value = 0, position = 0;
    if ((value = getname(name, &playername)) > 0) {
        while ((!result) && (list)) {
            position = getvalue("SYSTEM", "Position", list->player->logics, 0);
            if ((compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(playername)) == 0) ||
                (compare.vcmpcase(list->player->descriptions.smalldescription(position), LSTRSIZE(playername)) == 0) ||
                ((list->player->logics.hasvalue("RACE", 0) == 0) && (compare.vcmpcase(playername, LSTRSIZE(list->player->logics.getvalue("RACE", 0))) == 0))) {
                if (--value < 1) 
                    result = list->player;
            }
            list = list->next;
        }
    } else return NULL;
    if (playername) {
        pvfree(playername);
        playername = NULL;
    }
    return result;
}

Cplayer *Cgcharacters::getplayer(char *name) {
    playerslist *list = gamecharacters.getplayer(name);
    if (!list) 
        return NULL;
    return list->player;
}

Cmob *Cgcharacters::getmob(char *name, Ctile *tile) {
    Cmob *result = NULL;
    mobslist *list = tile->getmobsroot();
    char *mobname = NULL;
    int value = 0, position = 0;
    if ((value = getname(name, &mobname)) > 0) {
        while ((!result) && (list)) {
            position = getvalue("SYSTEM", "Position", list->mob->logics, 0);
            if ((compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(mobname)) == 0) ||
                (compare.vcmpcase(list->mob->descriptions.smalldescription(position), LSTRSIZE(mobname)) == 0) ||
                ((list->mob->logics.hasvalue("RACE", 0) == 0) && (compare.vcmpcase(mobname, LSTRSIZE(list->mob->logics.getvalue("RACE", 0))) == 0))) {
                if (--value < 1) 
                    result = list->mob;
            }
            list = list->next;
        }
    } else return NULL;
    if (mobname) {
        pvfree(mobname);
        mobname = NULL;
    }
    return result;
}

Cmob *Cgcharacters::getmob(char *name) {
    mobslist *list = gamecharacters.getmob(name);
    if (!list) 
        return NULL;
    return list->mob;
}

int Cgcharacters::addobject(Cplayer *player, Cobject *object) {
    if (getvalue("USAGE", "Transportable", object->logics, 0) == 1) {
        if (object->logics.hascategory("POSITION") == 0) {
            if (object->logics.delcategory("POSITION") > 0) 
                return 1;
            if (object->logics.addcategory("POSITION") > 0) 
                return 1;
        }
        if (object->logics.addvalue("POSITION", "IN", 4) > 0) 
            return 1;
        if (object->logics.addvalue("USAGE", "Use", 0) > 0) 
            return 1;
        if (player->addobject(object) > 0) 
            return 1;
    } else
        return -1;
    return 0;
}

int Cgcharacters::delobject(Cplayer *player, Cobject *object) {
    if (getvalue("USAGE", "Use", object->logics, 0) == 0) {
        if (object->logics.hascategory("POSITION") == 0) {
            if (object->logics.delcategory("POSITION") > 0) 
                return 1;
            if (object->logics.addcategory("POSITION") > 0) 
                return 1;
        }
        if (player->delobject(object->getID()) > 0) 
            return 1;
    } else 
        return -1;
    return 0;
}

char *Cgcharacters::getsimplename(Ccharacter *character) {
    if (character->getkind() != MOB)
        return getsimplename((Cplayer *) character);
    else 
        return getsimplename((Cmob *) character);
}

char *Cgcharacters::getsimplename(Cplayer *player) {
    int position = getvalue("SYSTEM", "Position", player->logics, 0);
    return player->descriptions.completename(position);
}

char *Cgcharacters::getsimplename(Cmob *mob) {
    int position = getvalue("SYSTEM", "Position", mob->logics, 0);
    return mob->descriptions.completename(position);
}

char *Cgcharacters::getdescription(Cplayer *player) {
    int position = getvalue("SYSTEM", "Position", player->logics, 0);
    return player->descriptions.largedescription(position);
}

char *Cgcharacters::getdescription(Cmob *mob) {
    int position = getvalue("SYSTEM", "Position", mob->logics, 0);
    return mob->descriptions.largedescription(position);
}

char *Cgcharacters::gettargetname(Cplayer *target, Cplayer *pvsender) {
    char *buffer = NULL, string[_DEFAULT_MIND_LEN];
    int position = getvalue("SYSTEM", "Position", target->logics, 0);
    if (pvsender->getcharacter(target->getID(), PLAYER)) 
        strcpy(string, target->descriptions.completename(position));
    else {
        if (target->logics.hasvalue("RACE", 0) == 0) 
            snprintf(string, _DEFAULT_MIND_LEN, "%s %s ", ((target->getsex() != MALE) ? "una" : "un"), target->logics.getvalue("RACE", 0));
        else 
            snprintf(string, _DEFAULT_MAXD_LEN, "%s indefinibile ", ((target->getsex() != MALE) ? "una" : "un"));
        strncat(string, target->descriptions.smalldescription(position), _DEFAULT_MIND_LEN);
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::gettargetname(Cmob *target, Cplayer *pvsender) {
    char *buffer = NULL, string[_DEFAULT_MIND_LEN];
    int position = getvalue("SYSTEM", "Position", target->logics, 0);
    if (pvsender->getcharacter(target->getID(), MOB)) 
        strcpy(string, target->descriptions.completename(position));
    else {
        if (target->logics.hasvalue("RACE", 0) == 0) 
            snprintf(string, _DEFAULT_MIND_LEN, "%s %s ", ((target->getsex() != MALE) ? "una" : "un"), target->logics.getvalue("RACE", 0));
        else 
            snprintf(string, _DEFAULT_MAXD_LEN, "%s indefinibile ", ((target->getsex() != MALE) ? "una" : "un"));
        strncat(string, target->descriptions.smalldescription(position), _DEFAULT_MIND_LEN);
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else return NULL;
    return buffer;
}

char *Cgcharacters::getlookstring(Cplayer *player, Cplayer *pvsender) {
    char *buffer = NULL, *race = player->logics.getvalue("RACE", 0), string[_DEFAULT_MIND_LEN];
    int position = getvalue("SYSTEM", "Position", player->logics, 0), lpoints = getvalue("STATS", "LPoints", player->logics, 0);
    if (!pvsender->getcharacter(player->getID(), PLAYER)) {
        snprintf(string, _DEFAULT_MIND_LEN, "[M] %s %s ", (player->getsex() != MALE) ? "una" : "un", (race) ? race : "indefinita");
    } else {
        if (player->getsex() != MALE) 
            snprintf(string, _DEFAULT_MIND_LEN, "[M] %s, la %s ", player->descriptions.completename(position), (race) ? race : "indefinita");
        else if ((*race == 'a') || (*race == 'A') || (*race == 'e') || (*race == 'E') || (*race == 'i') || (*race == 'I') ||
                 (*race == 'o') || (*race == 'O') || (*race == 'u') || (*race == 'U'))
            snprintf(string, _DEFAULT_MIND_LEN, "[P] %s, l'%s ", player->descriptions.completename(position), (race) ? race : "indefinito");
        else 
            snprintf(string, _DEFAULT_MIND_LEN, "[P] %s, il %s ", player->descriptions.completename(position), (race) ? race : "indefinito");
    }
    strncat(string, player->descriptions.smalldescription(position), _DEFAULT_MIND_LEN);
    if (player->logics.hasvalue("STATS", "Stunned") == 0) {
        if (player->getsex() != MALE) 
            strncat(string, " e' qui priva di sensi. ", _DEFAULT_MIND_LEN);
        else
            strncat(string, " e' qui privo di sensi. ", _DEFAULT_MIND_LEN);
    } else if (player->logics.hasvalue("STATUS", "Seated") == 0) {
        if (player->getsex() != MALE) 
            strncat(string, " e' qui seduta. ", _DEFAULT_MIND_LEN);
        else 
            strncat(string, " e' qui seduto. ", _DEFAULT_MIND_LEN);
    } else if (player->logics.hasvalue("STATUS", "Stretched") == 0) {
        if (player->getsex() != MALE) 
            strncat(string, " e' qui sdraiata. ", _DEFAULT_MIND_LEN);
        else 
            strncat(string, " e' qui sdraiato. ", _DEFAULT_MIND_LEN);
    } else 
        strncat(string, " e' qui. ", _DEFAULT_MIND_LEN);
    if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
        string[_DEFAULT_MIND_LEN - 4] = '.';
        string[_DEFAULT_MIND_LEN - 3] = '.';
        string[_DEFAULT_MIND_LEN - 2] = '.';
        string[_DEFAULT_MIND_LEN - 1] = '\0';
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
        if (getvalue("RANK", "Immortal", player->logics, 0) == 1) {
            if (allocate.vadd(&buffer, "[[green]I[reset]]") > 0) 
                return NULL;
        } else {
            if (lpoints < 3) {
                if (allocate.vadd(&buffer, "[[red]P[reset]]") > 0) 
                    return NULL;
            } else if (lpoints < 6) {
                if (allocate.vadd(&buffer, "[[yellow]D[reset]]") > 0) 
                    return NULL;
            } else {
                if (allocate.vadd(&buffer, "[[green]O[reset]]") > 0) 
                    return NULL;
            }
        }
        if (((player->logics.hascategory("GROUP") == 0) && (pvsender->logics.hascategory("GROUP") == 0)) &&
            (compare.vcmpcase(player->logics.getvalue("GROUP", 3), pvsender->logics.getvalue("GROUP", 3)) == 0)) {
            if (player->logics.hasvalue("GROUP", "Admin") == 0) {
                if (allocate.vadd(&buffer, "|[blue]A[reset]|") > 0) 
                    return NULL;
            } else if (player->logics.hasvalue("GROUP", "Moderator") == 0) {
                if (allocate.vadd(&buffer, "|[blue]C[reset]|") > 0) 
                    return NULL;
            } else if (allocate.vadd(&buffer, "|[blue]M[reset]|") > 0) 
                return NULL;
        }
        if (allocate.vadd(&buffer, "%s[n]", (player->logics.hasvalue("STATUS", "Hide") == 0) ? "[hide]" : "") > 0) 
            return NULL;
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getlookstring(Cmob *mob, Cplayer *pvsender) {
    char *buffer = NULL, *race = mob->logics.getvalue("RACE", 0), string[_DEFAULT_MIND_LEN];
    int position = getvalue("SYSTEM", "Position", mob->logics, 0), lpoints = getvalue("STATS", "LPoints", mob->logics, 0);
    if (!pvsender->getcharacter(mob->getID(), MOB)) {
        snprintf(string, _DEFAULT_MIND_LEN, "[M] %s %s ", (mob->getsex() != MALE) ? "una" : "un", (race) ? race : "indefinita");
    } else {
        if (mob->getsex() != MALE) snprintf(string, _DEFAULT_MIND_LEN, "[M] %s, la %s ", mob->descriptions.completename(position), (race) ? race : "indefinita");
        else if ((*race == 'a') || (*race == 'A') || (*race == 'e') || (*race == 'E') || (*race == 'i') || (*race == 'I') ||
                 (*race == 'o') || (*race == 'O') || (*race == 'u') || (*race == 'U'))
            snprintf(string, _DEFAULT_MIND_LEN, "[M] %s, l'%s ", mob->descriptions.completename(position), (race) ? race : "indefinito");
        else 
            snprintf(string, _DEFAULT_MIND_LEN, "[M] %s, il %s ", mob->descriptions.completename(position), (race) ? race : "indefinito");
    }
    strncat(string, mob->descriptions.smalldescription(position), _DEFAULT_MIND_LEN);
    if (mob->logics.hasvalue("STATUS", "Seated") == 0) 
        strncat(string, " e' qui seduto. ", _DEFAULT_MIND_LEN);
    else if (mob->logics.hasvalue("STATUS", "Stretched") == 0) 
        strncat(string, " e' qui sdraiato. ", _DEFAULT_MIND_LEN);
    else 
        strncat(string, " e' qui. ", _DEFAULT_MIND_LEN);
    if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
        string[_DEFAULT_MIND_LEN - 4] = '.';
        string[_DEFAULT_MIND_LEN - 3] = '.';
        string[_DEFAULT_MIND_LEN - 2] = '.';
        string[_DEFAULT_MIND_LEN - 1] = '\0';
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
        if (getvalue("RANK", "Immortal", mob->logics, 0) == 1) {
            if (allocate.vadd(&buffer, "[[green]I[reset]]") > 0) 
                return NULL;
        } else {
            if (lpoints < 3) {
                if (allocate.vadd(&buffer, "[[red]P[reset]]") > 0) 
                    return NULL;
            } else if (lpoints < 6) {
                if (allocate.vadd(&buffer, "[[yellow]D[reset]]") > 0) 
                    return NULL;
            } else {
                if (allocate.vadd(&buffer, "[[green]O[reset]]") > 0) 
                    return NULL;
            }
        }
        if (((mob->logics.hascategory("GROUP") == 0) && (pvsender->logics.hascategory("GROUP") == 0)) &&
            (compare.vcmpcase(mob->logics.getvalue("GROUP", 3), pvsender->logics.getvalue("GROUP", 3)) == 0)) {
            if (mob->logics.hasvalue("GROUP", "Moderator") == 0) {
                if (allocate.vadd(&buffer, "|[blue]C[reset]|") > 0) 
                    return NULL;
            } else if (allocate.vadd(&buffer, "|[blue]M[reset]|") > 0) 
                return NULL;
        }
        if (allocate.vadd(&buffer, "%s[n]", (mob->logics.hasvalue("STATUS", "Hide") == 0) ? "[hide]" : "") > 0) 
            return NULL;
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getappearance(Cpoints& points, bool male, unsigned int age) {
    char *buffer = NULL, *strength = NULL, *charism = NULL, *appearance = NULL, *intelligence = NULL, string[_DEFAULT_MAXD_LEN];
    if ((strength = getstrength(points, male)) && (charism = getcharism(points, male)) &&
        (appearance = getage(age, male)) && (intelligence = getIntelligence(points))) {
        snprintf(string, _DEFAULT_MAXD_LEN, "Si tratta di %s %s, %s e %s. Riesci a cogliere dal suo sguardo%s.[n]", appearance, points.getvalue("RACE", 0), charism, strength, intelligence);
        if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
            strcpy(buffer, string);
            buffer[strlen(string)] = '\0';
        } else 
            return NULL;
    } else 
        return NULL;
    if (strength) {
        pvfree(strength);
        strength = NULL;
    }
    if (charism) {
        pvfree(charism);
        charism = NULL;
    }
    if (appearance) {
        pvfree(appearance);
        appearance = NULL;
    }
    if (intelligence) {
        pvfree(intelligence);
        intelligence = NULL;
    }
    return buffer;
}

char *Cgcharacters::getage(unsigned int age, bool male) {
    char *buffer = NULL, string[_DEFAULT_MIND_LEN];
    if (age < 10) 
        snprintf(string, _DEFAULT_MIND_LEN, "%s piccolissim%s", (male) ? "un" : "una", (male) ? "o" : "a");
    else if (age < 20) 
        snprintf(string, _DEFAULT_MIND_LEN, "%s piccol%s", (male) ? "un" : "una", (male) ? "o" : "a");
    else if (age < 30) 
        snprintf(string, _DEFAULT_MIND_LEN, "%s giovanissim%s", (male) ? "un" : "una", (male) ? "o" : "a");
    else if (age < 40) 
        snprintf(string, _DEFAULT_MIND_LEN, "%s giovane", (male) ? "un" : "una");
    else if (age < 60) 
        snprintf(string, _DEFAULT_MIND_LEN, "%s", (male) ? "un" : "una");
    else if (age < 70) 
        snprintf(string, _DEFAULT_MIND_LEN, "%s %s", (male) ? "un" : "una", (male) ? "anziano" : "anziana");
    else
        snprintf(string, _DEFAULT_MIND_LEN, "%s %s", (male) ? "un" : "una", (male) ? "vecchio" : "vecchia");
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getcharism(Cpoints& points, bool male) {
    char *buffer = NULL, string[_DEFAULT_MIND_LEN];
    int charism = getvalue("CHARS", "CH", points, 0);
    if (charism < 10) 
        snprintf(string, _DEFAULT_MIND_LEN, "incredibilmente brutt%s", (male) ? "o" : "a");
    else if (charism < 20) 
        snprintf(string, _DEFAULT_MIND_LEN, "decisamente brutt%s", (male) ? "o" : "a");
    else if (charism < 30) 
        snprintf(string, _DEFAULT_MIND_LEN, "bruttin%s", (male) ? "o" : "a");
    else if (charism < 40) 
        snprintf(string, _DEFAULT_MIND_LEN, "discretamente piacente");
    else if (charism < 60) 
        snprintf(string, _DEFAULT_MIND_LEN, "discretamente carin%s", (male) ? "o" : "a");
    else if (charism < 70)
        snprintf(string, _DEFAULT_MIND_LEN, "piuttosto bell%s", (male) ? "o" : "a");
    else if (charism < 90) 
        snprintf(string, _DEFAULT_MIND_LEN, "decisamente bell%s", (male) ? "o" : "a");
    else 
        snprintf(string, _DEFAULT_MIND_LEN, "incredibilmente bell%s", (male) ? "o" : "a");
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else
        return NULL;
    return buffer;
}

char *Cgcharacters::getstrength(Cpoints& points, bool male) {
    char *buffer = NULL, string[_DEFAULT_MIND_LEN];
    int strength = getvalue("CHARS", "ST", points, 0);
    if (strength < 10) 
        snprintf(string, _DEFAULT_MIND_LEN, "dall'aspetto estremamente debole");
    else if (strength < 20) 
        snprintf(string, _DEFAULT_MIND_LEN, "dall'aspetto mingherlin%s", (male) ? "o" : "a");
    else if (strength < 30) 
        snprintf(string, _DEFAULT_MIND_LEN, "poco muscolos%s", (male) ? "o" : "a");
    else if (strength < 40) 
        snprintf(string, _DEFAULT_MIND_LEN, "relativamente muscolos%s", (male) ? "o" : "a");
    else if (strength < 60) 
        snprintf(string, _DEFAULT_MIND_LEN, "in discreta forma fisica");
    else if (strength < 70)
        snprintf(string, _DEFAULT_MIND_LEN, "in un'ottima forma fisica");
    else if (strength < 90) 
        snprintf(string, _DEFAULT_MIND_LEN, "dall'aspetto estremamente violento");
    else 
        snprintf(string, _DEFAULT_MIND_LEN, "dall'aspetto drasticamente violento");
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getIntelligence(Cpoints& points) {
    char *buffer = NULL, string[_DEFAULT_MIND_LEN];
    int intelligence = getvalue("CHARS", "IN", points, 0);
    if (intelligence < 10) 
        snprintf(string, _DEFAULT_MIND_LEN, " da primate, un'aria particolarmente stupida");
    else if (intelligence < 20) 
        snprintf(string, _DEFAULT_MIND_LEN, " da primate, un'aria abbastanza stupida");
    else if (intelligence < 30) 
        snprintf(string, _DEFAULT_MIND_LEN, " da primate, un'aria sufficientemente sciocca");
    else if (intelligence < 40) 
        snprintf(string, _DEFAULT_MIND_LEN, ", un'aria non eccessivamente intelligente");
    else if (intelligence < 60)
        snprintf(string, _DEFAULT_MIND_LEN, " vispo, un'aria sveglia");
    else if (intelligence < 70)
        snprintf(string, _DEFAULT_MIND_LEN, " vispo, un'aria particolarmente sveglia");
    else if (intelligence < 90) 
        snprintf(string, _DEFAULT_MIND_LEN, " vispo, un'aria particolarmente intelligente");
    else 
        snprintf(string, _DEFAULT_MIND_LEN, " particolarmente vispo, un'aria estremamente intelligente");
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getlook(Cplayer *player, Cplayer *pvsender, datainfos& present) {
    int position = getvalue("SYSTEM", "Position", player->logics, 0);
    char *buffer = NULL, *status = NULL, *description = NULL, *race = player->logics.getvalue("RACE", 0), string[_DEFAULT_MAXD_LEN], backup[_DEFAULT_MIND_LEN];
    if (!pvsender->getcharacter(player->getID(), PLAYER)) {
        snprintf(string, _DEFAULT_MIND_LEN, "osservi %s %s ", (player->getsex() != MALE) ? "una" : "un", (race) ? race : "indefinita");
    } else {
        if (player->getsex() != MALE) snprintf(string, _DEFAULT_MIND_LEN, "osservi [bold]%s[reset], la %s ", player->descriptions.completename(position), (race) ? race : "indefinita");
        else if ((*race == 'a') || (*race == 'A') || (*race == 'e') || (*race == 'E') || (*race == 'i') || (*race == 'I') ||
                 (*race == 'o') || (*race == 'O') || (*race == 'u') || (*race == 'U'))
            snprintf(string, _DEFAULT_MIND_LEN, "osservi [bold]%s[reset], l'%s ", player->descriptions.completename(position), (race) ? race : "indefinito");
        else 
            snprintf(string, _DEFAULT_MIND_LEN, "osservi [bold]%s[reset], il %s ", player->descriptions.completename(position), (race) ? race : "indefinito");
    }
    snprintf(backup, _DEFAULT_MIND_LEN, "%s che e' qui%s. %s", player->descriptions.smalldescription(position),
             ((player->logics.hasvalue("STATUS", "Seated") == 0) ? " seduto" : ((player->logics.hasvalue("STATUS", "Stretched") == 0) ? " sdraiato" : "")), status = getstatus(player->logics));
    if (status) {
        pvfree(status);
        status = NULL;
    }
    strncat(string, backup, _DEFAULT_MAXD_LEN);
    if (((player->logics.hascategory("GROUP") == 0) && (pvsender->logics.hascategory("GROUP") == 0)) &&
        (compare.vcmpcase(player->logics.getvalue("GROUP", 3), pvsender->logics.getvalue("GROUP", 3)) == 0)) {
        if (player->logics.hasvalue("GROUP", "Admin") == 0) 
            strncat(string, "[n]E' il capo del tuo gruppo, ed ha la carica di ", _DEFAULT_MAXD_LEN);
        else if (player->logics.hasvalue("GROUP", "Moderator") == 0) 
            strncat(string, "[n]E' uno dei vice del tuo gruppo, ed ha la carica di ", _DEFAULT_MAXD_LEN);
        else 
            strncat(string, "[n]E' un membro del tuo gruppo, ed ha la carica di ", _DEFAULT_MAXD_LEN);
        strncat(string, player->logics.getvalue("GROUP", 2), _DEFAULT_MAXD_LEN);
    }
    strncat(string, "[n]", _DEFAULT_MAXD_LEN);
    if (player->logics.hasvalue("SYSTEM", "Description") == 0) {
        strncat(string, description = this->getappearance(player, present), _DEFAULT_MAXD_LEN);
        if (description) {
            pvfree(description);
            description = NULL;
        }
    }
    if (strlen(string) >= (_DEFAULT_MAXD_LEN - 1)) {
        string[_DEFAULT_MAXD_LEN - 4] = '.';
        string[_DEFAULT_MAXD_LEN - 3] = '.';
        string[_DEFAULT_MAXD_LEN - 2] = '.';
        string[_DEFAULT_MAXD_LEN - 1] = '\0';
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
        if (allocate.vadd(&buffer, "%s[n]", this->getdescription(player)) > 0) 
            return NULL;
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getlook(Cmob *mob, Cplayer *pvsender, datainfos& present) {
    int position = getvalue("SYSTEM", "Position", mob->logics, 0);
    char *buffer = NULL, *status = NULL, *description = NULL, *race = mob->logics.getvalue("RACE", 0), string[_DEFAULT_MAXD_LEN], backup[_DEFAULT_MIND_LEN];
    if (!pvsender->getcharacter(mob->getID(), MOB)) {
        snprintf(string, _DEFAULT_MIND_LEN, "osservi %s %s ", (mob->getsex() != MALE) ? "una" : "un", (race) ? race : "indefinita");
    } else {
        if (mob->getsex() != MALE) snprintf(string, _DEFAULT_MIND_LEN, "osservi [bold]%s[reset], la %s ", mob->descriptions.completename(position), (race) ? race : "indefinita");
        else if ((*race == 'a') || (*race == 'A') || (*race == 'e') || (*race == 'E') || (*race == 'i') || (*race == 'I') ||
                 (*race == 'o') || (*race == 'O') || (*race == 'u') || (*race == 'U'))
            snprintf(string, _DEFAULT_MIND_LEN, "osservi [bold]%s[reset], l'%s ", mob->descriptions.completename(position), (race) ? race : "indefinito");
        else 
            snprintf(string, _DEFAULT_MIND_LEN, "osservi [bold]%s[reset], il %s ", mob->descriptions.completename(position), (race) ? race : "indefinito");
    }
    snprintf(backup, _DEFAULT_MIND_LEN, "%s che e' qui%s. %s.", mob->descriptions.smalldescription(position),
             ((mob->logics.hasvalue("STATUS", "Seated") == 0) ? " seduto" : ((mob->logics.hasvalue("STATUS", "Stretched") == 0) ? " sdraiato" : "")), status = getstatus(mob->logics));
    if (status) {
        pvfree(status);
        status = NULL;
    }
    strncat(string, backup, _DEFAULT_MAXD_LEN);
    if (((mob->logics.hascategory("GROUP") == 0) && (pvsender->logics.hascategory("GROUP") == 0)) &&
        (compare.vcmpcase(mob->logics.getvalue("GROUP", 3), pvsender->logics.getvalue("GROUP", 3)) == 0)) {
        if (mob->logics.hasvalue("GROUP", "Moderator") == 0) 
            strncat(string, "[n]E' uno dei vice del tuo gruppo, ed ha la carica di ", _DEFAULT_MAXD_LEN);
        else 
            strncat(string, "[n]E' un membro del tuo gruppo, ed ha la carica di ", _DEFAULT_MAXD_LEN);
        strncat(string, mob->logics.getvalue("GROUP", 2), _DEFAULT_MAXD_LEN);
    }
    strncat(string, "[n]", _DEFAULT_MAXD_LEN);
    if (mob->logics.hasvalue("SYSTEM", "Description") == 0) {
        strncat(string, description = this->getappearance(mob, present), _DEFAULT_MAXD_LEN);
        if (description) {
            pvfree(description);
            description = NULL;
        }
    }
    if (strlen(string) >= (_DEFAULT_MAXD_LEN - 1)) {
        string[_DEFAULT_MAXD_LEN - 4] = '.';
        string[_DEFAULT_MAXD_LEN - 3] = '.';
        string[_DEFAULT_MAXD_LEN - 2] = '.';
        string[_DEFAULT_MAXD_LEN - 1] = '\0';
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
        if (allocate.vadd(&buffer, " %s[n]", this->getdescription(mob)) > 0) 
            return NULL;
    } else 
        return NULL;
    return buffer;
}

char *Cgcharacters::getlook(Ctile *tile, Cplayer *player) {
    mobslist *mobslist = player->position->getmobsroot();
    playerslist *playerslist = player->position->getplayersroot();
    char *buffer = NULL, *backup = NULL;
    while (playerslist) {
        if ((player->getID() != playerslist->player->getID()) &&
            ((playerslist->player->logics.hasvalue("STATUS", "Hide") != 0) ||
             (player->logics.hasvalue("RANK", "Admin") == 0) ||
             (player->logics.hasvalue("RANK", "Moderator") == 0))) {
            if ((backup = this->getlookstring(playerslist->player, player))) {
                if (allocate.vadd(&buffer, backup) > 0) {
                    if (backup) {
                        pvfree(backup);
                        backup = NULL;
                    }
                    return NULL;
                }
            }
            if (backup) {
                pvfree(backup);
                backup = NULL;
            }
        }
        playerslist = playerslist->next;
    }
    while (mobslist) {
        if ((mobslist->mob->logics.hasvalue("STATUS", "Hide") != 0) ||
            (player->logics.hasvalue("RANK", "Admin") == 0) ||
            (player->logics.hasvalue("RANK", "Moderator") == 0)) {
            if ((backup = this->getlookstring(mobslist->mob, player))) {
                if (allocate.vadd(&buffer, backup) > 0) {
                    if (backup) {
                        pvfree(backup);
                        backup = NULL;
                    }
                    return NULL;
                }
            }
            if (backup) {
                pvfree(backup);
                backup = NULL;
            }
        }
        mobslist = mobslist->next;
    }
    return buffer;
}

bool Cgcharacters::infight(Ctile *tile, Ccharacter *character) {
    Cmob *mob = NULL;
    Cplayer *player = NULL;
    if (character->getkind() != PLAYER) {
        mob = (Cmob *) character;
        return infight(tile, mob);
    } else {
        player = (Cplayer *) character;
        return infight(tile, player);
    }
    return false;
}

bool Cgcharacters::infight(Ctile *tile, Cplayer *player) {
    mobslist *mobbackup = NULL, *mobslist = tile->getmobsroot();
    playerslist *playerbackup = NULL, *playerslist = tile->getplayersroot();
    bool fight = false;
    while ((playerslist) && (!fight)) {
        playerbackup = playerslist;
        playerslist = playerslist->next;
        if ((playerbackup->player->logics.hascategory("FIGHT") == 0) &&
            (playerbackup->player->logics.hasvalue("FIGHT", "Target", player->getID()) == 0) &&
            (playerbackup->player->logics.hasvalue("FIGHT", "Kind", (int) PLAYER) == 0)) 
            fight = true;
    }
    while ((mobslist) && (!fight)) {
        mobbackup = mobslist;
        mobslist = mobslist->next;
        if ((mobbackup->mob->logics.hascategory("FIGHT") == 0) &&
            (mobbackup->mob->logics.hasvalue("FIGHT", "Target", player->getID()) == 0) &&
            (mobbackup->mob->logics.hasvalue("FIGHT", "Kind", (int) PLAYER) == 0)) 
            fight = true;
    }
    return fight;
}

bool Cgcharacters::infight(Ctile *tile, Cmob *mob) {
    mobslist *mobbackup = NULL, *mobslist = tile->getmobsroot();
    playerslist *playerbackup = NULL, *playerslist = tile->getplayersroot();
    bool fight = false;
    while ((playerslist) && (!fight)) {
        playerbackup = playerslist;
        playerslist = playerslist->next;
        if ((playerbackup->player->logics.hascategory("FIGHT") == 0) &&
            (playerbackup->player->logics.hasvalue("FIGHT", "Target", mob->getID()) == 0) &&
            (playerbackup->player->logics.hasvalue("FIGHT", "Kind", (int) MOB) == 0)) 
            fight = true;
    }
    while ((mobslist) && (!fight)) {
        mobbackup = mobslist;
        mobslist = mobslist->next;
        if ((mobbackup->mob->logics.hascategory("FIGHT") == 0) &&
            (mobbackup->mob->logics.hasvalue("FIGHT", "Target", mob->getID()) == 0) &&
            (mobbackup->mob->logics.hasvalue("FIGHT", "Kind", (int) MOB) == 0)) 
            fight = true;
    }
    return fight;
}

char *Cgcharacters::getability(Cplayer *player, const char *name) {
    int index = 0;
    char *buffer = NULL, *status = NULL, string[_DEFAULT_MIND_LEN];
    while ((compare.vcmpcase(chacharas[index].representation, CSTRSIZE("NULL")) != 0) &&
           (compare.vcmpcase(chacharas[index].representation, LSTRSIZE(name)) != 0)) index++;
    if ((compare.vcmpcase(chacharas[index].representation, LSTRSIZE(name)) == 0) &&
        (status = funny.vbar(getvalue("CHARS", chacharas[index].representation, player->logics, 0), 100))) {
        snprintf(string, _DEFAULT_MIND_LEN, "%s (%3d) %s[n]", chacharas[index].string,
                 getvalue("CHARS", chacharas[index].representation, player->logics, 0), status);
        if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
            strcpy(buffer, string);
            buffer[strlen(string)] = '\0';
        } else 
            return NULL;
    } else 
        return NULL;
    if (status) {
        pvfree(status);
        status = NULL;
    }
    return buffer;
}

char *Cgcharacters::getability(Cmob *mob, const char *name) {
    int index = 0;
    char *buffer = NULL, *status = NULL, string[_DEFAULT_MIND_LEN];
    while ((compare.vcmpcase(chacharas[index].representation, CSTRSIZE("NULL")) != 0) &&
           (compare.vcmpcase(chacharas[index].representation, LSTRSIZE(name)) != 0)) 
        index++;
    if ((compare.vcmpcase(chacharas[index].representation, LSTRSIZE(name)) == 0) &&
        (status = funny.vbar(getvalue("CHARS", chacharas[index].representation, mob->logics, 0), 100))) {
        snprintf(string, _DEFAULT_MIND_LEN, "%s (%3d) %s[n]", chacharas[index].string,
                 getvalue("CHARS", chacharas[index].representation, mob->logics, 0), status);
        if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
            strcpy(buffer, string);
            buffer[strlen(string)] = '\0';
        } else 
            return NULL;
    } else 
        return NULL;
    if (status) {
        pvfree(status);
        status = NULL;
    }
    return buffer;
}

int Cgcharacters::setbonus(Cplayer *player, Cobject *object) {
    datalist *list = NULL;
    pointslist *points = NULL;
    char *value = NULL, *category = NULL;
    if (object->logics.hascategory("BONUS") == 0) {
        if ((points = object->logics.getcategory("BONUS"))) {
            points->values->resetvalue();
            while ((list = points->values->nextvalue())) {
                if (strchr(list->completename, ':')) {
                    if (strings.vsscanf(list->completename, ':', "ss", &category, &value) > 0) 
                        return 1;
                    if (setvalue(category, value, player->logics, list->value) > 0) 
                        return 1;
                } else if (strchr(list->completename, '.')) {
                    if (strings.vsscanf(list->completename, '.', "ss", &category, &value) > 0) 
                        return 1;
                    if (list->value < 0) {
                        if (player->logics.hasvalue(category, value) == 0)
                            if (player->logics.delvalue(category, value) > 0) LOG_ERROR("Unable to delete %s->%s Logic", category, value);
                    } else {
                        if (player->logics.hasvalue(category, value) != 0)
                            if (player->logics.addvalue(category, value, 0) > 0) LOG_ERROR("Unable to add %s->%s Logic", category, value);
                    }
                } else {
                    if (list->value < 0) {
                        if (player->logics.hascategory(list->completename) == 0)
                            if (player->logics.delcategory(list->completename) > 0) LOG_ERROR("Unable to delete %s Category", list->completename);
                    } else {
                        if (player->logics.hascategory(list->completename) != 0)
                            if (player->logics.addcategory(list->completename) > 0) LOG_ERROR("Unable to add %s Category", list->completename);
                    }
                }
                if (category) {
                    pvfree(category);
                    category = NULL;
                }
                if (value) {
                    pvfree(value);
                    value = NULL;
                }
            }
        } else 
            return 1;
    }
    return 0;
}

int Cgcharacters::setmalus(Cplayer *player, Cobject *object) {
    datalist *list = NULL;
    pointslist *points = NULL;
    char *value = NULL, *category = NULL;
    if (object->logics.hascategory("MALUS") == 0) {
        if ((points = object->logics.getcategory("MALUS"))) {
            points->values->resetvalue();
            while ((list = points->values->nextvalue())) {
                if (strchr(list->completename, ':')) {
                    if (strings.vsscanf(list->completename, ':', "ss", &category, &value) > 0) 
                        return 1;
                    if (setvalue(category, value, player->logics, (list->value * -1)) > 0) 
                        return 1;
                } else if (strchr(list->completename, '.')) {
                    if (strings.vsscanf(list->completename, '.', "ss", &category, &value) > 0) 
                        return 1;
                    if (list->value < 0) {
                        if (player->logics.hasvalue(category, value) == 0)
                            if (player->logics.delvalue(category, value) > 0) 
                                LOG_ERROR("Unable to delete %s->%s Logic", category, value);
                    } else {
                        if (player->logics.hasvalue(category, value) != 0)
                            if (player->logics.addvalue(category, value, 0) > 0)
                                LOG_ERROR("Unable to add %s->%s Logic", category, value);
                    }
                } else {
                    if (list->value < 0) {
                        if (player->logics.hascategory(list->completename) == 0)
                            if (player->logics.delcategory(list->completename) > 0)
                                LOG_ERROR("Unable to delete %s Category", list->completename);
                    } else {
                        if (player->logics.hascategory(list->completename) != 0)
                            if (player->logics.addcategory(list->completename) > 0) 
                                LOG_ERROR("Unable to add %s Category", list->completename);
                    }
                }
                if (category) {
                    pvfree(category);
                    category = NULL;
                }
                if (value) {
                    pvfree(value);
                    value = NULL;
                }
            }
        } else
            return 1;
    }
    return 0;
}

int Cgcharacters::setbonus(Cmob *mob, Cobject *object) {
    datalist *list = NULL;
    pointslist *points = NULL;
    char *value = NULL, *category = NULL;
    if (object->logics.hascategory("BONUS") == 0) {
        if ((points = object->logics.getcategory("BONUS"))) {
            points->values->resetvalue();
            while ((list = points->values->nextvalue())) {
                if (strchr(list->completename, ':')) {
                    if (strings.vsscanf(list->completename, ':', "ss", &category, &value) > 0) 
                        return 1;
                    if (setvalue(category, value, mob->logics, list->value) > 0) return 1;
                } else if (strchr(list->completename, '.')) {
                    if (strings.vsscanf(list->completename, '.', "ss", &category, &value) > 0) 
                        return 1;
                    if (list->value < 0) {
                        if (mob->logics.hasvalue(category, value) == 0)
                            if (mob->logics.delvalue(category, value) > 0) 
                                LOG_ERROR("Unable to delete %s->%s Logic", category, value);
                    } else {
                        if (mob->logics.hasvalue(category, value) != 0)
                            if (mob->logics.addvalue(category, value, 0) > 0) 
                                LOG_ERROR("Unable to add %s->%s Logic", category, value);
                    }
                } else {
                    if (list->value < 0) {
                        if (mob->logics.hascategory(list->completename) == 0)
                            if (mob->logics.delcategory(list->completename) > 0) 
                                LOG_ERROR("Unable to delete %s Category", list->completename);
                    } else {
                        if (mob->logics.hascategory(list->completename) != 0)
                            if (mob->logics.addcategory(list->completename) > 0) 
                                LOG_ERROR("Unable to add %s Category", list->completename);
                    }
                }
                if (category) {
                    pvfree(category);
                    category = NULL;
                }
                if (value) {
                    pvfree(value);
                    value = NULL;
                }
            }
        } else 
            return 1;
    }
    return 0;
}

int Cgcharacters::setmalus(Cmob *mob, Cobject *object) {
    datalist *list = NULL;
    pointslist *points = NULL;
    char *value = NULL, *category = NULL;
    if (object->logics.hascategory("MALUS") == 0) {
        if ((points = object->logics.getcategory("MALUS"))) {
            points->values->resetvalue();
            while ((list = points->values->nextvalue())) {
                if (strchr(list->completename, ':')) {
                    if (strings.vsscanf(list->completename, ':', "ss", &category, &value) > 0) 
                        return 1;
                    if (setvalue(category, value, mob->logics, (list->value * -1)) > 0) 
                        return 1;
                } else if (strchr(list->completename, '.')) {
                    if (strings.vsscanf(list->completename, '.', "ss", &category, &value) > 0) 
                        return 1;
                    if (list->value < 0) {
                        if (mob->logics.hasvalue(category, value) == 0)
                            if (mob->logics.delvalue(category, value) > 0) 
                                LOG_ERROR("Unable to delete %s->%s Logic", category, value);
                    } else {
                        if (mob->logics.hasvalue(category, value) != 0)
                            if (mob->logics.addvalue(category, value, 0) > 0) 
                                LOG_ERROR("Unable to add %s->%s Logic", category, value);
                    }
                } else {
                    if (list->value < 0) {
                        if (mob->logics.hascategory(list->completename) == 0)
                            if (mob->logics.delcategory(list->completename) > 0) 
                                LOG_ERROR("Unable to delete %s Category", list->completename);
                    } else {
                        if (mob->logics.hascategory(list->completename) != 0)
                            if (mob->logics.addcategory(list->completename) > 0) 
                                LOG_ERROR("Unable to add %s Category", list->completename);
                    }
                }
                if (category) {
                    pvfree(category);
                    category = NULL;
                }
                if (value) {
                    pvfree(value);
                    value = NULL;
                }
            }
        } else 
            return 1;
    }
    return 0;
}
