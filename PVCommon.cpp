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
#include "PVCommon.h"
struct shash emoticons[] = {
    {":))", "sorridendo"},
    {":)", "felice"},
    {":D", "ridendo"},
    {":d", "ridendo"},
    {":((", "disperato"},
    {":(", "tristemente"},
    {":\\", "mestamente"},
    {":P", "scherzosamente"},
    {":p", "scherzosamente"},
    {":>", "compiaciuto"},
    {":<", "adirato"},
    {";)", "ammiccando"},
    {":|", "perplesso"},
    {":O", "sconvolto"},
    {":o", "sconvolto"},
    {":h", "preoccupato"},
    {":H", "preoccupato"},
    {":@", "furibondo"},
    {":Z", "sbadigliando"},
    {":z", "sbadigliando"},
    {"XD", "michelando"},
    {"xd", "michelando"},
    {"NULL", "NULL"}
};

char *getemoticon(char **target) {
    int index = 0;
    char *emoticon = NULL;
    while ((index >= 0) && (compare.vcmpcase(emoticons[index].representation, CSTRSIZE("NULL")))) {
        if (strings.vreplace(target, emoticons[index].representation, "")) {
            if ((emoticon = (char *) pvmalloc(strlen(emoticons[index].string)))) {
                strcpy(emoticon, emoticons[index].string);
                emoticon[strlen(emoticons[index].string)] = '\0';
                index = -1;
            } else
                return NULL;
        } else
            index++;
    }
    return emoticon;
}

char *getstatus(Cpoints& points) {
    char *status = NULL;
    int lpoints = getvalue("STATS", "LPoints", points, 0);
    if (getvalue("RANK", "Immortal", points, 0) == 0) {
        if (lpoints < 10) {
            if ((status = (char *) pvmalloc(sizeof ("([red]e' in pessimo stato![reset])") + 1))) {
                strcpy(status, "([red]e' in pessimo stato![reset])");
                status[sizeof ("([red]e' in pessimo stato![reset])")] = '\0';
            } else
                return NULL;
        } else if (lpoints < 50) {
            if ((status = (char *) pvmalloc(sizeof ("([yellow]e' in discrete condizioni![reset])") + 1))) {
                strcpy(status, "([yellow]e' in discrete condizioni![reset])");
                status[sizeof ("([yellow]e' in discrete condizioni![reset])")] = '\0';
            } else
                return NULL;
        } else {
            if ((status = (char *) pvmalloc(sizeof ("([green]e' in ottime condizioni![reset])") + 1))) {
                strcpy(status, "([green]e' in ottime condizioni![reset])");
                status[sizeof ("([green]e' in ottime condizioni![reset])")] = '\0';
            } else
                return NULL;
        }
    } else {
        if ((status = (char *) pvmalloc(sizeof ("([green]e' immortale![reset])") + 1))) {
            strcpy(status, "([green]e' immortale![reset])");
            status[sizeof ("([green]e' immortale![reset])")] = '\0';
        } else
            return NULL;
    }
    return status;
}

int getname(char *string, char **name) {
    int value = 1;
    char *message = NULL, *pointer = NULL, *backup = NULL;
    if ((message = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(message, string);
        message[strlen(string)] = '\0';
        backup = message;
        if ((pointer = strchr(backup, '.'))) {
            *pointer = '\0';
            value = atoi(backup);
            backup = pointer + 1;
            while (*backup == ' ')
                backup++;
        }
        if (value < 1)
            value = 1;
        if ((*name = (char *) pvmalloc(strlen(backup) + 1)))
            strcpy(*name, backup);
        else
            return -1;
    } else
        return -1;
    if (message) {
        pvfree(message);
        message = NULL;
    }
    return value;
}

int spvsend(Cserver& server, Ctile *tile, const char *format, ...) {
    va_list args;
    Ccharacter *character = NULL;
    playerslist *list = tile->getplayersroot();
    char string[_DEFAULT_MIND_LEN], *message = NULL;
    int position = 0, elements = strings.voccurrence(format, "$name"), index = 0;
    if (elements > 0) {
        while (list) {
            va_start(args, format);
            if ((message = (char *) pvmalloc(strlen(format) + 1))) {
                strcpy(message, format);
                message[strlen(format)] = '\0';
                for (index = 0; index < elements; index++) {
                    if ((character = va_arg(args, Ccharacter*))) {
                        position = getvalue("SYSTEM", "Position", character->logics, 0);
                        if (list->player->getcharacter(character->getID(), character->getkind()))
                            strcpy(string, character->descriptions.completename(position));
                        else {
                            if (character->logics.hasvalue("RACE", 0) == 0)
                                snprintf(string, _DEFAULT_MIND_LEN, "%s %s ",
                                    ((character->getsex() != MALE) ? "una" : "un"),
                                    character->logics.getvalue("RACE", 0));
                            else
                                snprintf(string, _DEFAULT_MIND_LEN, "%s indefinibile ",
                                    ((character->getsex() != MALE) ? "una" : "un"));
                            strncat(string, character->descriptions.smalldescription(position), _DEFAULT_MIND_LEN);
                        }
                        if (!(strings.vsreplace(&message, "$name", string)))
                            index = elements;
                    } else
                        index = elements;
                }
                if (list->player->pvsend(server, message) > 0)
                    return 1;
            } else
                return 1;
            if (message) {
                pvfree(message);
                message = NULL;
            }
            va_end(args);
            list = list->next;
        }
    } else {
        if (tile->pvsend(server, format) > 0)
            return 1;
    }
    return 0;
}
