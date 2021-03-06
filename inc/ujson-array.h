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
  * ujson-array.h
  *
  */

#ifndef _UJ_LIST_H
#define _UJ_LIST_H

#include <stdint.h>
#include "ujson-value.h"

typedef struct ujvalue ujvalue;
typedef struct ujarray ujarray;

struct __attribute__ ((__packed__)) ujarray {
	uint16_t start;
	uint16_t end;
	uint16_t size;
	ujvalue* values[];
};

ujarray* array_allot(uint16_t len);
uint16_t array_length(ujarray* a);
ujarray* array_push(ujarray* a, ujvalue* v);
ujvalue* array_pop(ujarray* a);
ujvalue* array_shift(ujarray* a);
ujarray* array_each(ujarray* a, void(*f)(ujvalue** v));
ujarray* array_map(ujarray* a, ujvalue*(*f)(ujvalue* v));
void array_release(ujarray** a);

#endif

