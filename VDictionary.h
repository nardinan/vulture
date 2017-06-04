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
#ifndef __VDICTIONARYHP__
#define __VDICTIONARYHP__
#include <string.h>
#include "VDescriptions.h"
#include "VTools.h"
#include "VAlloc.h"
#include "VData.h"
#include "VLog.h"

typedef struct wordslist {
    char *word;
    char *substitute;
    struct wordslist *next;
    struct wordslist *back;
} wordslist;

class Cdictionary {
private:
    unsigned int dictionaryID;
    wordslist *wordsroot;
public:
    basicinfos *information;
    Cdictionary(void);

    virtual ~Cdictionary(void) {
    };

    int load(FILE *configurationfile);
    int save(FILE *configurationfile);

    int addword(const char *word, const char *substitute);
    int delword(const char *word);
    wordslist *getword(const char *word);

    int rebuild(char **string);

    inline unsigned int getID(void) {
        return dictionaryID;
    };

    inline void setID(unsigned int dictionaryID) {
        this->dictionaryID = dictionaryID;
    };

    void unload(void);
};
#endif
