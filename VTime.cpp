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
#include "VTime.h"

Ctime::Ctime(void) {
    pdate.day = pdate.month = pdate.year = 1;
    ptime.hour = ptime.minutes = 0;
    if (!(information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) LOG_ERROR("unable to allocate basicinfos");
}

int Ctime::load(FILE *configurationfile) {
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/TIME]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[TIME]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("NAME")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                        strncpy(information->completename, text, _DEFAULT_NAME_LEN);
                        information->completename[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("SMALL")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                        strncpy(information->smalldescription, text, _DEFAULT_MIND_LEN);
                        information->smalldescription[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("LARGE")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                        strncpy(information->largedescription, text, _DEFAULT_MAXD_LEN);
                        information->largedescription[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("[MONTHS]")) == 0) {
                        if (loadmonths(configurationfile) > 0) LOG_ERROR("unable to load MONTHS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("[DAYS]")) == 0) {
                        if (loaddays(configurationfile) > 0) LOG_ERROR("unable to load DAYS");
                    } else if (compare.vcmpcase(string, CSTRSIZE("TIME")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                        else if (strings.vsscanf(text, ':', "dd", &ptime.hour, &ptime.minutes) > 0) LOG_ERROR("unable to edit key %s", text);
                    } else if (compare.vcmpcase(string, CSTRSIZE("DATE")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                        else if (strings.vsscanf(text, ':', "ddd", &pdate.day, &pdate.month, &pdate.year) > 0) LOG_ERROR("unable to edit key %s", text);
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Ctime] %s (unknown command)", string);
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

int Ctime::loadmonths(FILE *configurationfile) {
    int element, size;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/MONTHS]")) != 0) && (!feof(configurationfile))) {
                if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[MONTHS]")) != 0)) {
                    if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                    else if (strings.vsscanf(text, ':', "dd", &element, &size) > 0) LOG_ERROR("unable to edit key %s", text);
                    else {
                        if (mname.addvalue(name, element) > 0) return 1;
                        if (msize.addvalue(name, size) > 0) return 1;
                    }
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

int Ctime::loaddays(FILE *configurationfile) {
    int element, size;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/DAYS]")) != 0) && (!feof(configurationfile))) {
                if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[DAYS]")) != 0)) {
                    if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", string);
                    else if (strings.vsscanf(text, ':', "dd", &element, &size) > 0) LOG_ERROR("unable to edit key %s", text);
                    else {
                        if (dname.addvalue(name, element) > 0) return 1;
                        if (dsize.addvalue(name, size) > 0) return 1;
                    }
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

int Ctime::save(FILE *configurationfile) {
    if (configurationfile) {
        fprintf(configurationfile, "[TIME]\n");
        fprintf(configurationfile, "NAME=%s\nSMALL=%s\nLARGE=%s\n", information->completename, information->smalldescription, information->largedescription);
        fprintf(configurationfile, "TIME=%d:%d\nDATE=%d:%d:%d\n", ptime.hour, ptime.minutes, pdate.day, pdate.month, pdate.year);
        fprintf(configurationfile, "[MONTHS]\n");
        if (savemonths(configurationfile) > 0) return 1;
        fprintf(configurationfile, "[/MONTHS]\n[DAYS]\n");
        if (savedays(configurationfile) > 0) return 1;
        fprintf(configurationfile, "[/DAYS]\n[/TIME]");
    } else return 1;
    return 0;
}

int Ctime::savemonths(FILE *configurationfile) {
    datalist *month = NULL;
    int size;
    if (configurationfile) {
        mname.resetvalue();
        while ((month = mname.nextvalue())) {
            try {
                size = msize.getvalue(month->completename);
            } catch (...) {
                LOG_ERROR("unable to find SIZE of %s", month->completename);
                size = 31;
            }
            fprintf(configurationfile, "%s=%d:%d\n", month->completename, month->value, size);
        }
    } else return 1;
    return 0;
}

int Ctime::savedays(FILE *configurationfile) {
    datalist *day = NULL;
    int size;
    if (configurationfile) {
        dname.resetvalue();
        while ((day = dname.nextvalue())) {
            try {
                size = dsize.getvalue(day->completename);
            } catch (...) {
                LOG_ERROR("unable to find SIZE of %s", day->completename);
                size = 24;
            }
            fprintf(configurationfile, "%s=%d:%d\n", day->completename, day->value, size);
        }
    } else return 1;
    return 0;
}

int Ctime::update(void) {
    time_t check = (gettime() - seed);
    datalist *month = NULL, *day = NULL;
    int daysize, monthsize;
    ptime.minutes += (int) (check / Vltime);
    while (ptime.minutes >= 60) {
        ptime.minutes -= 60;
        ptime.hour += 1;
    }
    if ((day = getday())) {
        try {
            daysize = dsize.getvalue(day->completename);
        } catch (...) {
            daysize = 24;
        }
        while (ptime.hour >= (unsigned int) daysize) {
            ptime.hour -= daysize;
            pdate.day++;
            if ((month = getmonth())) {
                try {
                    monthsize = msize.getvalue(month->completename);
                } catch (...) {
                    monthsize = 31;
                }
                if (pdate.day > (unsigned int) monthsize) {
                    pdate.day = 1;
                    pdate.month += 1;
                }
                if (pdate.month > (unsigned int) mname.getelements()) {
                    pdate.month = 1;
                    pdate.year += 1;
                }
            } else return 1;
        }
    } else return 1;
    seed = gettime();
    return 0;
}

datalist *Ctime::getday(void) {
    datalist *currentmonth = NULL, *monthlist = NULL, *daylist = NULL;
    int daysize, weeksize;
    msize.resetvalue();
    dname.resetvalue();
    if ((currentmonth = getmonth())) {
        daysize = pdate.day;
        while ((monthlist = msize.nextvalue()) && (compare.vcmpcase(monthlist->completename, LSTRSIZE(currentmonth->completename)) != 0)) daysize += monthlist->value;
        weeksize = daysize % dname.getelements();
        while ((daylist = dname.nextvalue()) && (daylist->value != weeksize));
        if (daylist->value != weeksize) return NULL;
    } else return NULL;
    return daylist;
}

datalist *Ctime::getmonth(void) {
    datalist *currentmonth = NULL;
    mname.resetvalue();
    while ((currentmonth = mname.nextvalue()) && ((unsigned int) currentmonth->value != (pdate.month - 1)));
    return currentmonth;
}

void Ctime::unload(void) {
    mname.unload();
    msize.unload();
    dname.unload();
    dsize.unload();
    if (information) {
        pvfree(information);
        information = NULL;
    }
}
