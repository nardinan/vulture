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
#include "PVGMap.h"

Ctile *Cgmap::get(tileinfos position) {
    tileslist *tile = gamemap.gettile(position);
    if (!tile) return NULL;
    return tile->tile;
}

Ctile *Cgmap::get(char *name, Czone *zone, int hour) {
    Ctile *result = NULL;
    tileslist *list = zone->gettilesroot();
    int value = 0;
    char *tilename = NULL;
    if ((value = getname(name, &tilename)) > 0) {
        while ((!result) && (list)) {
            if ((compare.vstep(list->tile->descriptions.completename(hour), tilename) == 0) ||
                    (compare.vcmpcase(list->tile->descriptions.smalldescription(hour), LSTRSIZE(tilename)) == 0)) {
                if (--value < 1) result = list->tile;
            }
            list = list->next;
        }
    } else return NULL;
    if (tilename) {
        pvfree(tilename);
        tilename = NULL;
    }
    return result;
}

Czone *Cgmap::get(char *name) {
    Czone *result = NULL;
    zoneslist *list = gamemap.getzonesroot();
    char *zonename = NULL;
    int value = 0, position = 0;
    if ((value = getname(name, &zonename)) > 0) {
        while ((!result) && (list)) {
            position = getvalue("SYSTEM", "Position", list->zone->logics, 0);
            if ((compare.vstep(list->zone->descriptions.completename(position), zonename) == 0) ||
                    (compare.vcmpcase(list->zone->descriptions.smalldescription(position), LSTRSIZE(zonename)) == 0)) {
                if (--value < 1) result = list->zone;
            }
            list = list->next;
        }
    } else return NULL;
    if (zonename) {
        pvfree(zonename);
        zonename = NULL;
    }
    return result;
}

char *Cgmap::getlook(Ctile *tile, int hour) {
    datalist *list = NULL;
    pointslist *points = NULL;
    archeslist *archeslist = tile->getarchesroot();
    bool tags = false;
    char *buffer = NULL, string[_DEFAULT_MAXD_LEN], backup[_DEFAULT_MIND_LEN];
    if ((points = tile->logics.getcategory("TAGS"))) {
        points->values->resetvalue();
        while ((list = points->values->nextvalue())) {
            if (!tags) 
                snprintf(backup, _DEFAULT_MIND_LEN, "%s[n]", list->completename);
            else {
                strncat(backup, list->completename, _DEFAULT_MIND_LEN);
                strncat(backup, "[n]", _DEFAULT_MIND_LEN);
            }
            tags = true;
        }
    }
    snprintf(string, _DEFAULT_MAXD_LEN, "%s[bold]<%s>[n]([under]%s[reset])[n]%s[n]", (tags) ? backup : "", tile->descriptions.completename(hour), tile->descriptions.smalldescription(hour), tile->descriptions.largedescription(hour));
    if (archeslist) {
        strncat(string, "Da qui riesci a vedere ", _DEFAULT_MAXD_LEN);
        while (archeslist) {
            snprintf(backup, _DEFAULT_MIND_LEN, "[bold]%s[reset] a %s%s", archeslist->tile->descriptions.completename(hour), archeslist->information->completename, ((archeslist->close) ? ((archeslist->lock) ? "([red]bloccato[reset])" : "([red]chiuso[reset])") : ""));
            if (archeslist->next) {
                (!archeslist->next->next) ? strncat(backup, " e ", _DEFAULT_MIND_LEN) : strncat(backup, ", ", _DEFAULT_MIND_LEN);
            }
            strncat(string, backup, _DEFAULT_MAXD_LEN);
            archeslist = archeslist->next;
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
        if (allocate.vadd(&buffer, "[n]") > 0)
            return NULL;
    } else 
        return NULL;
    return buffer;
}
