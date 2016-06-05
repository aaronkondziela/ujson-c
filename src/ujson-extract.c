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
  * ujson-extract.c
  * de-serialize elements by applying a schema generated
  * by ujson-format, to data generated by ujson-data. Also used by
  * ujson-decode which handles inlined schemas.
  *
  */

#include "ujson-endian.h"
#include "ujson-movebytes.h"
#include "schematags.h"
#include "ujson-extract.h"

void extract_bool(uint8_t** nextbuf, uint8_t* val)
{
	*val = (**nextbuf == 't' ? uj_true : uj_false);
	(*nextbuf) += 1;
}

void extract_uint8(uint8_t** nextbuf, uint8_t* val)
{
	*val = (uint8_t)(**nextbuf);
	(*nextbuf) += 1;
}

void extract_int8(uint8_t** nextbuf, int8_t* val)
{
	*val = (int8_t)(**nextbuf);
	(*nextbuf) += 1;
}

void extract_uint16(uint8_t** nextbuf, uint16_t* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 2 );
	*val = htoj16(*val);
	(*nextbuf) += 2;
}

void extract_int16(uint8_t** nextbuf, int16_t* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 2 );
	*val = htoj16(*val);
	(*nextbuf) += 2;
}

void extract_uint32(uint8_t** nextbuf, uint32_t* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 4 );
	*val = htoj32(*val);
	(*nextbuf) += 4;
}

void extract_int32(uint8_t** nextbuf, int32_t* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 4 );
	*val = htoj32(*val);
	(*nextbuf) += 4;
}

void extract_uint64(uint8_t** nextbuf, uint64_t* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 8 );
	*val = htoj64(*val);
	(*nextbuf) += 8;
}

void extract_int64(uint8_t** nextbuf, int64_t* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 8 );
	*val = htoj64(*val);
	(*nextbuf) += 8;
}

void extract_string(uint8_t** nextbuf, uint8_t* str)
{
	uint16_t len = 0;
	extract_uint16(nextbuf, &len);
	movebytes(str, *nextbuf, len);
	str[len] = '\0';
	(*nextbuf) += len;
}

void extract_float(uint8_t** nextbuf, float* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 4 );
	*val = htojf(*val);
	(*nextbuf) += 4;
}

void extract_double(uint8_t** nextbuf, double* val)
{
	movebytes( (uint8_t*)val, *nextbuf, 8 );
	*val = htojd(*val);
	(*nextbuf) += 8;
}

static uint16_t _extract_arraylen(uint8_t** buf, uint8_t** schema);
static uint16_t _extract_objectlen(uint8_t** buf, uint8_t** schema);

static uint8_t _tagsize(uint8_t** buf, uint8_t** schema)
{
	uint16_t l, al;
	uint8_t tag = *((*schema)++);
	switch(tag) {
		case uj_bool_tag:
		case uj_null_tag:
		case uj_uint8_tag:
		case uj_int8_tag:
			(*buf) += 1;
			return 1;
		case uj_uint16_tag:
		case uj_int16_tag:
			(*buf) += 2;
			return 2;
		case uj_uint32_tag:
		case uj_int32_tag:
		case uj_float_tag:
			(*buf) += 4;
			return 4;
		case uj_uint64_tag:
		case uj_int64_tag:
		case uj_double_tag:
			(*buf) += 8;
			return 8;
		case uj_string_tag:
			extract_uint16(buf, &l);
			(*buf) += l;
			return l + 2;
		case uj_array_tag:
			extract_uint16(buf, &l);
			(*buf) -= 2;
			_extract_arraylen(buf, schema);
			return l + 2;
		case uj_object_tag:
			extract_uint16(buf, &l);
			(*buf) -= 2;
			_extract_objectlen(buf, schema);
			return l + 2;
		default:
			return 0;
	}
}

static uint16_t _extract_arraylen(uint8_t** buf, uint8_t** schema)
{
	uint16_t size = 0, n = 0;
	extract_uint16(buf, &size);
	while (size) {
		n++;
		size -= _tagsize(buf, schema);
	}
	return n;
}

static uint16_t _extract_objectlen(uint8_t** buf, uint8_t** schema)
{
	uint16_t size = 0, n = 0, kl = 0;
	extract_uint16(buf, &size);
	while (size) {
		n++;
		extract_uint16(buf, &kl);
		size -= kl + 2;
		(*buf) += kl;
		size -= _tagsize(buf, schema);
	}
	return n;
}

static ujvalue* _extract(uint8_t** buf, uint8_t** schema)
{
	uint8_t t;
	uint16_t l, m;
	ujstring* k;
	ujvalue* v;
	uint8_t* b;
	uint8_t* s;
	v = ujvalue_new();
	t = *((*schema)++);
	switch(t) {
		case uj_bool_tag:
			extract_bool(buf, &v->type);
			break;
		case uj_null_tag:
			v->type = uj_null;
			(*buf) += 1;
			break;
		case uj_uint8_tag:
			v->type = uj_number;
			v->numbertype = uj_uint8;
			extract_uint8(buf, &v->data_as.uint8);
			break;
		case uj_int8_tag:
			v->type = uj_number;
			v->numbertype = uj_int8;
			extract_int8(buf, &v->data_as.int8);
			break;
		case uj_uint16_tag:
			v->type = uj_number;
			v->numbertype = uj_uint16;
			extract_uint16(buf, &v->data_as.uint16);
			break;
		case uj_int16_tag:
			v->type = uj_number;
			v->numbertype = uj_int16;
			extract_int16(buf, &v->data_as.int16);
			break;
		case uj_uint32_tag:
			v->type = uj_number;
			v->numbertype = uj_uint32;
			extract_uint32(buf, &v->data_as.uint32);
			break;
		case uj_int32_tag:
			v->type = uj_number;
			v->numbertype = uj_int32;
			extract_int32(buf, &v->data_as.int32);
			break;
		case uj_uint64_tag:
			v->type = uj_number;
			v->numbertype = uj_uint64;
			extract_uint64(buf, &v->data_as.uint64);
			break;
		case uj_int64_tag:
			v->type = uj_number;
			v->numbertype = uj_int64;
			extract_int64(buf, &v->data_as.int64);
			break;
		case uj_string_tag:
			v->type = uj_string;
			extract_uint16(buf, &l);
			(*buf) -= 2;
			v->data_as.string = string_allot(l);
			v->data_as.string->length = l;
			extract_string(buf, v->data_as.string->data);
			break;
		case uj_float_tag:
			v->type = uj_number;
			v->numbertype = uj_float;
			extract_float(buf, &v->data_as.f);
			break;
		case uj_double_tag:
			v->type = uj_number;
			v->numbertype = uj_double;
			extract_double(buf, &v->data_as.d);
			break;
		case uj_array_tag:
			v->type = uj_array;
			b = *buf;
			s = *schema;
			l = _extract_arraylen(&b, &s);
			v->data_as.array = array_allot(l);
			(*buf) += 2;
			while (l--) array_push(v->data_as.array, _extract(buf, schema));
			break;
		case uj_object_tag:
			v->type = uj_object;
			b = *buf;
			s = *schema;
			l = _extract_objectlen(&b, &s);
			v->data_as.object = object_allot(l);
			(*buf) += 2;
			while (l--) {
				extract_uint16(buf, &m);
				(*buf) -= 2;
				k = string_allot(m);
				extract_string(buf, k->data);
				object_set(v->data_as.object, k, _extract(buf, schema));
			}
			break;
		default:
			break;
	}
	return v;
}

ujvalue* extract(uint8_t* buf, uint8_t* schema)
{
	uint8_t* bufp = buf;
	uint8_t* schp = schema;
	return _extract(&bufp, &schp);
}

