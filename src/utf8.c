/*
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009
 * Robert Lougher <rob@jamvm.org.uk>.
 *
 * This file is part of JamVM.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "jam.h"
#include "hash.h"
// todo HT SIZE
//1 << 10
//Changed Default size of UTF8 hash 1 << 10
#define HASHTABSZE 1<<16
#define HASH(ptr) utf8Hash(ptr)
#define COMPARE(ptr1, ptr2, hash1, hash2) (ptr1 == ptr2) || \
                  (utf8Comp(ptr1, ptr2) && (hash1 == hash2))
#define PREPARE(ptr) ptr
#define SCAVENGE(ptr) FALSE
#define FOUND(ptr1, ptr2) ptr2

static HashTable hash_table;
static HashTable native_hash_table;

/*XXX NVM VARIABLES - UTF8.C */
static char* utf8_name = "utf8_ht";
static char* native_name = "nativemethods_ht";
static int testing_mode = FALSE;
static int is_persistent = FALSE;

#define GET_UTF8_CHAR(ptr, c)                         \
{                                                     \
    int x = *ptr++;                                   \
    if(x & 0x80) {                                    \
        int y = *ptr++;                               \
        if(x & 0x20) {                                \
            int z = *ptr++;                           \
            c = ((x&0xf)<<12)+((y&0x3f)<<6)+(z&0x3f); \
        } else                                        \
            c = ((x&0x1f)<<6)+(y&0x3f);               \
    } else                                            \
        c = x;                                        \
}

int utf8Len(char *utf8) {
    int count;

    for(count = 0; *utf8; count++) {
        int x = *utf8;
        utf8 += (x & 0x80) ? ((x & 0x20) ? 3 : 2) : 1;
    }

    return count;
}

void convertUtf8(char *utf8, unsigned short *buff) {
    while(*utf8)
        GET_UTF8_CHAR(utf8, *buff++);
}

int utf8Hash(char *utf8) {
    int hash = 0;

    while(*utf8) {
        unsigned short c;
        GET_UTF8_CHAR(utf8, c);
        hash = hash * 37 + c;
    }

    return hash;
}

int utf8Comp(char *ptr, char *ptr2) {
     while(*ptr && *ptr2) {
        unsigned short c, c2;

        GET_UTF8_CHAR(ptr, c);
        GET_UTF8_CHAR(ptr2, c2);
        if(c != c2)
            return FALSE;
    }
    if(*ptr || *ptr2)
        return FALSE;

    return TRUE;
}

char *findHashedUtf8(char *string, int add_if_absent) {
    char *interned = NULL;
    findHashEntry(hash_table, string, interned, add_if_absent, FALSE, TRUE, utf8_name, TRUE);
   	return interned;
}
char *findHashedNativeMethodUtf8(char *string, int add_if_absent) {
    char *interned = NULL;
    findHashEntry(native_hash_table, string, interned, add_if_absent, FALSE, TRUE, native_name, TRUE);
   	return interned;
}
void *getHashedNativeMethodUtf8() {
#define ITERATE(name) reinitClass((char*)name)
    hashIterate(native_hash_table);
#undef ITERATE
}

char *copyUtf8(char *string) {
	/*XXX NVM CHANGE 004.001.030	*/
    char *buff = strcpy(sysMalloc_persistent(strlen(string) + 1), string);
    char *found = findHashedUtf8(buff, TRUE);

    if(strcmp(found,buff) != 0 )
   	/*XXX NVM CHANGE 004.003.003	*/
        sysFree_persistent(buff);

    return found;
}

char *dots2Slash(char *utf8) {
    char *pntr;

    for(pntr = utf8; *pntr != '\0'; pntr++)
        if(*pntr == '.')
            *pntr = '/';

    return utf8;
}

char *slash2dots(char *utf8) {
    int len = strlen(utf8);
    char *conv = sysMalloc(len+1);
    int i;

    for(i = 0; i <= len; i++)
        if(utf8[i] == '/')
            conv[i] = '.';
        else
            conv[i] = utf8[i];

    return conv;
}

char *slash2DotsDup(char *utf8) {
    int len = strlen(utf8);
    char *buff = sysMalloc(len + 1);
    int i;

    for(i = 0; i <= len; i++) {
        char c = utf8[i];
        buff[i] = c == '/' ? '.' : c;
    }

    return buff;
}

char *slash2DotsBuff(char *utf8, char *buff, int buff_len) {
    char *pntr = buff;

    while(*utf8 != '\0' && --buff_len) {
        char c = *utf8++;
        *pntr++ = c == '/' ? '.' : c;
    }

    *pntr = '\0';
    return buff;
}

int initialiseUtf8(InitArgs *args) {
	if(args->testmode == TRUE){
		testing_mode = TRUE;
	}
	if(args->persistence == TRUE){
		is_persistent = TRUE;
	}

    /* Init hash table, and create lock */
    /* XXX NVM CHANGE 005.001.009 - UTF8 HT - Y*/
    initHashTable(hash_table, HASHTABSZE, TRUE, utf8_name, TRUE);
    initHashTable(native_hash_table,HASHTABSZE, TRUE, native_name, TRUE);
    /* XXX DOC CHANGE */
    if(is_persistent){
    	OPC *ph_value = get_opc_ptr();
    	hash_table.hash_count = ph_value->utf8_hash_count;
      native_hash_table.hash_count = ph_value->utf8_native_hash_count;
      //printf("hashcount:%d\n",hash_table.hash_count);
    }
    return TRUE;
}

#ifndef NO_JNI
/* Functions used by JNI */

int utf8CharLen(unsigned short *unicode, int len) {
    int count = 0;

    for(; len > 0; len--) {
        unsigned short c = *unicode++;
        count += c == 0 || c > 0x7f ? (c > 0x7ff ? 3 : 2) : 1;
    }

    return count;
}

char *unicode2Utf8(unsigned short *unicode, int len, char *utf8) {
    char *ptr = utf8;

    for(; len > 0; len--) {
        unsigned short c = *unicode++;
        if((c == 0) || (c > 0x7f)) {
            if(c > 0x7ff) {
                *ptr++ = (c >> 12) | 0xe0;
                *ptr++ = ((c >> 6) & 0x3f) | 0x80;
            } else
                *ptr++ = (c >> 6) | 0xc0;
            *ptr++ = (c&0x3f) | 0x80;
        } else
            *ptr++ = c;
    }

    *ptr = '\0';
    return utf8;
}
#endif


/*	XXX NVM CHANGE 009.004.001	*/
int get_utf8_HC()
{
	return hash_table.hash_count;
}

int get_utf8_native_HC()
{
  return native_hash_table.hash_count;
}

int msync_utf8(){
  uintptr_t *mem = hash_table.hash_table;
  uintptr_t *mem2 = native_hash_table.hash_table;
  uintptr_t size = alignSize(sizeof(HashEntry)*hash_table.hash_size);
  uintptr_t size2 = alignSize(sizeof(HashEntry)*native_hash_table.hash_size);
  *mem--;
  *mem--;
  *mem2--;
  *mem2--;
  //printf("mem:%p,size:%d\n",mem,size);
  if(msync(mem,size,MS_SYNC) == -1 || msync(mem2,size2,MS_SYNC) == -1){
    perror("msync at Utf8");
    return FALSE;
  }
  return TRUE;
}
