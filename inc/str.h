/*
 * Copyright (c) 2016 Aaron Kondziela <aaron@aaronkondziela.com> 
 *
 * This file is part of ujson-c
 *
 * ujson-c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ujson-c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
 /*
  * Part of ujson-c - Implements microjson in C - see ujson.org
  * and https://github.com/aaronkondziela/ujson-c/
  *
  * str.h
  *
  */

#ifndef _STR_H
#define _STR_H

#include <stdint.h>

#ifndef STACKSTR_SIZE
	#define STACKSTR_SIZE (256)
//	#warning STACKSTR_SIZE undefined, using default of 256
#endif

typedef struct {
	uint8_t* data;
	uint16_t length;
	uint8_t buffer[0];
} str;

typedef struct {
	uint8_t* data;
	uint16_t length;
	uint8_t buffer[STACKSTR_SIZE];
} stackstr;

str* str_allot(uint16_t len);
void str_release(str* s);
int str_eq(str* a, str* b);
uint16_t str_findlen(const uint8_t* s);

#endif