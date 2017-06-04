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
#include "VDatabank.h"
#include "VPoints.h"
#include "VObject.h"
#include "VPlayer.h"
#include "VTile.h"
#include "VZone.h"
#include "VData.h"
#include "VMob.h"
Cdatabank Vexcategory;
unsigned int Vflevelup = 100;
unsigned int Vltime = 1;
unsigned int Vport = 5090;
unsigned int Vflog = 1;
unsigned int Vplog = 1;
unsigned int Vfile = 15;
unsigned int Vpass = 15;
unsigned int Vcheck = 5;
unsigned int NobjectID = 1;
unsigned int NplayerID = 1;
unsigned int NmobID = 1;
unsigned int NtileID = 1;
unsigned int NzoneID = 1;

int getvalue(const char *category, const char *value, Cpoints& points, int defaultvalue) {
    if (points.hasvalue(category, value) != 0) {
        if (points.hascategory(category) != 0) {
            if (points.addcategory(category) > 0) {
                throw "unable to add category";
                return -1;
            }
        }
        if (points.addvalue(category, value, defaultvalue) > 0) {
            throw "unable to add value";
            return -1;
        }
    }
    return points.getvalue(category, value);
}

int sgetvalue(const char *category, const char *value, Cpoints& points, int defaultvalue) {
    int integer = defaultvalue;
    if (points.hasvalue(category, value) == 0) integer = points.getvalue(category, value);
    return integer;
}

int setvalue(const char *category, const char *value, Cpoints& points, int modificator, int maxvalue) {
    int backupvalue = getvalue(category, value, points, 0);
    if ((backupvalue + modificator) > maxvalue) backupvalue = maxvalue;
    else if ((backupvalue + modificator) < 0) backupvalue = 0;
    else backupvalue += modificator;
    return points.addvalue(category, value, backupvalue);
}

int setvalue(const char *category, const char *value, Cpoints& points, int modificator) {
    int backupvalue = getvalue(category, value, points, 0);
    int maxvalue = getvalue("MAXVALUES", category, points, backupvalue);
    if ((backupvalue + modificator) > maxvalue) backupvalue = maxvalue;
    else if ((backupvalue + modificator) < 0) backupvalue = 0;
    else backupvalue += modificator;
    return points.addvalue(category, value, backupvalue);
}
