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
  * ujson-fromjson.h
  *
  */

#ifndef _UJ_FROMJSON_H
#define _UJ_FROMJSON_H

#include <stdint.h>
#include <stdio.h>
#include "ujson-value.h"
#include "ujson-string.h"

typedef struct ujtmpa ujtmpa;
typedef struct ujtmpo ujtmpo;

struct ujtmpa {
	ujvalue* v;
	ujtmpa* next;
};

struct ujtmpo {
	ujstring* k;
	ujvalue* v;
	ujtmpo* next;
};

ujvalue* fromjson(FILE* fp);

#endif

