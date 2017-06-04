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
#ifndef __VALLOCHP__
#define __VALLOCHP__
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "VData.h"
#include "VLog.h"

typedef struct vcoords {
    const char *file;
    int line;
} vcoords;

typedef struct allocationslist {
    void *pointer;
    size_t buffersize;
    struct allocationslist *next;
    struct allocationslist *back;
} allocationslist;
extern unsigned int memory;
extern unsigned int pointers;
extern allocationslist *allocationsroot;
extern void *vmalloc(size_t size, const char *file, int line);
extern void *vrealloc(void *pointer, size_t size);
extern void vfree(void *pointer);
extern void vdeallocate(void);
extern int vshow(void);
#define pvmalloc(a) vmalloc(a, __FILE__, __LINE__)
#define pvrealloc(a, b) vrealloc(a, b)
#define pvfree(a) vfree(a)
#endif
