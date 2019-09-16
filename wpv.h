/**
 * WPV library
 * Version 1.0.0
 * Copyright (C) 2018-2019, Oleg Nemanov <lego12239@yandex.ru>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @file
 */
#ifndef __WPV_H__
#define __WPV_H__

#include <sys/uio.h>
#include "list.h"


/**
 * Use fmtiov.iov instead of fmtbuf.buf for the parsing result.
 */
#define WPV_FLAG_FMTIOV 1
/**
 * Do not allocate memory for a field struct on parsing or field adding - use
 * preallocated fields with wpv_rmake() or with wpv_ralloc_fields().
 */
#define WPV_FLAG_NOALLOC_FIELD 2
/**
 * Do not allocate memory for a result buffer in wpv_raddf() or
 * in wpv_rparse() - use preallocated buffer with wpv_rmake() or
 * wpv_ralloc_fmtbuf().
 */
#define WPV_FLAG_NOALLOC_BUF 4
/**
 * Do not copy source buffer to fmtbuf.buf before parse.
 */
#define WPV_FLAG_NOCOPY_ON_PARSE 8


struct wpvf {
	struct list_item_head list;
	int size;
	char *val;
};

struct wpvr {
	struct {
		struct wpvf *first;
		struct wpvf *last;
		unsigned int cnt;
		struct wpvf *_pool;
	} fields;
	struct {
		char *buf;
		unsigned int _size;
		unsigned int len;
	} fmtbuf;
	struct {
		struct iovec *iov;
		unsigned int _total;
		unsigned int cnt;
	} fmtiov;
	unsigned int _flags;
};	

#define wpvf_for_each(f, f_start) \
	for(lh = &(f_start)->list, f = f_start; lh; lh = lh->next, )

/**
 * @brief Make an empty wpv record structure.
 *
 * @param flags      - flags for record
 * @param fields_cnt - initial fields count (can be 0)
 * @param buf_size   - initial fmtbuf size(can be 0, but 1 byte is allocated
 *                     in any case for terminating '\n')
 *
 * @return  rec - on ok
 * @return  NULL - if no mem
 *
 * Make an empty wpv record with specified flags and fmtbuf initial size of size.
 */
struct wpvr* wpv_rmake(unsigned int flags, unsigned int fields_cnt, unsigned int buf_size);

/**
 * @brief Free space allocated for specified wpv record
 *
 * @param r - a wpv record
 *
 * Free allocated space for a record, fields and buffers.
 */
void wpv_rfree(struct wpvr *r);

/**
 * @brief Reset wpv record.
 *
 * @param r - a wpv record
 *
 * Move field into fields pool. Set fields.cnt to 0, fmtiov.cnt to 1
 * and fmtbuf.buf to "\n" (empty record).
 */
void wpv_rreset(struct wpvr *r);

/**
 * @brief Allocate empty fields and add it to the fields pool.
 *
 * @param r - a record
 * @param fields_cnt - total fields count we need
 *
 * @return >=0 - allocated fields count(must be equal to fields_cnt)
 *
 * If current fields count is greater or equal to fields_cnt, then
 * no action is performed.
 * If WPV_FLAG_FMTIOV flag is specified, than iov structs also allocated.
 */
int wpv_ralloc_fields(struct wpvr *r, unsigned int fields_cnt);

/**
 * @brief Reallocate r->fmtbuf.
 *
 * @param r - a record
 * @param size - a needed fmtbuf size
 *
 * @return 0 - ok
 * @return -1 - no mem
 *
 * If current size is greater or equal to specified size, then
 * no action is performed.
 */
int wpv_ralloc_fmtbuf(struct wpvr *r, unsigned int size);

/**
 * @brief Add a field with a specified value to a specified record.
 *
 * @param r    - a record to add field to
 * @param val  - a field value
 * @param size - a field value size
 *
 * @return 0 - ok
 * @return -1 - no mem
 * @return -2 - no free field in the pool
 * @return -5 - no enough space in fmtbuf
 * @return -6 - no enough iovec structs
 *
 * After call to this function a formatted record can be got from
 * fmtbuf.buf with fmtbuf.len length. If WPV_FLAG_FMTIOV is specified
 * on making, then formatted record can be accessed from fmtiov.iov,
 * with iov count in fmtiov.cnt.
 */
int wpv_raddf(struct wpvr *r, const char *val, int size);

/**
 * @brief Parse a specified buffer.
 *
 * @param r - a wpv record
 * @param buf - a buffer
 * @param size - a buffer size
 *
 * @return >0 - ok, count of bytes read
 * @return -1 - no mem
 * @return -2 - no free field in the pool
 * @return -3 - format error
 * @return -4 - record is incomplete
 * @return -7 - field width is negative
 * @return -8 - field width is too big
 */
int wpv_rparse(struct wpvr *r, char *buf, int size);

/*
 * Get a next wpv field.
 *
 * @param f - a current field
 *
 * @return field - if ok
 * @return NULL - if no next field
 */
struct wpvf* wpv_fget_next(struct wpvf *f);

/*
 * Get a wpv field by it index.
 *
 * @param r   - a wpv record
 * @param idx - a field index (starting from 0)
 *
 * @return field - if ok
 * @return NULL - if field with such index
 */
struct wpvf* wpv_fget(struct wpvr *r, int idx);

/**
 * @brief Return text message for specified error code.
 *
 * @param ecode - a error code returned by one of wpv_* function
 *
 * @return text error message
 */
const char* wpv_strerror(int ecode);

#endif  /* __WPV_H__ */
