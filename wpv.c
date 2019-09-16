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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "wpv.h"

struct parser_ctx {
	char *tok_start;
	char *tok_end;
	char *buf;
	int size;
};


static int _wpv_radd_to_buf(struct wpvr *r, char **fval_off, const char *val, int size);
static int _wpv_radd_to_iov(struct wpvr *r, char **fval_off, const char *val, int size);
static int _wpv_ralloc_fmtiov(struct wpvr *r, unsigned int cnt);


/*
 * Take a field from the fields pool.
 * get:
 *   r - a wpv record
 * ret:
 *   NULL - if fields pool is empty
 *   f - a field
 */
static struct wpvf*
_wpv_rfpool_pop(struct wpvr *r)
{
	struct wpvf *f;

	assert(r != NULL);
	
	f = r->fields._pool;
	if (!f)
		return NULL;
	r->fields._pool = list_item(f->list.next, struct wpvf, list);
	list_rm(&f->list);

	return f;
}

/**
 * @brief Add a field to the fields pool.
 * @param r  a wpv record
 * @param f  a wpv field
 */
static void
_wpv_rfpool_push(struct wpvr *r, struct wpvf *f)
{
	assert(r != NULL);
	assert(f != NULL);
	
	f->val = NULL;
	f->size = 0;
	if (r->fields._pool)
		list_add_before(&f->list, &r->fields._pool->list);
	r->fields._pool = f;
}

/**
 * @brief Add a field to the fields list.
 *
 * @param r  a wpv record
 * @param f  a wpv field
 */
static void
_wpv_rflist_add(struct wpvr *r, struct wpvf *f)
{
	assert(r != NULL);
	assert(f != NULL);
	
	r->fields.cnt++;
	if (r->fields.last)
		list_add(&f->list, &r->fields.last->list);
	else
		r->fields.first = f;
	r->fields.last = f;
}

static void
_wpv_rflist_rm(struct wpvr *r, struct wpvf *f)
{
	struct wpvf *fh;
	struct list_item_head *lnext, *lprev;
	
	assert(r != NULL);
	assert(f != NULL);
	
	fh = list_item(list_get_head(&f->list), struct wpvf, list);
	assert(fh == r->fields.first);
		
	r->fields.cnt--;
	lnext = f->list.next;
	lprev = f->list.prev;
	list_rm(&f->list);
	if (r->fields.first == f)
		r->fields.first = list_item(lnext, struct wpvf, list);
	if (r->fields.last == f)
		r->fields.last = list_item(lprev, struct wpvf, list);
}

/*
 * Make an empty wpv field.
 */
static struct wpvf*
_wpv_fmake(void)
{
	struct wpvf *f;
	
	f = malloc(sizeof(*f));
	if (!f)
		return NULL;
	list_item_head_init(&f->list);
	memset(f, 0, sizeof(*f));

	return f;
}

/*
 * Free allocated memory for specified wpv field.
 * Space allocated for val is also freed if val not NULL.
 */
static void
_wpv_ffree(struct wpvf *f)
{
	if (!f)
		return;
	free(f);
}

static int
_wpv_raddf(struct wpvr *r, const char *val, int size)
{
	struct wpvf *f;
	
	f = _wpv_rfpool_pop(r);
	if (!f) {
		if (r->_flags & WPV_FLAG_NOALLOC_FIELD)
			return -2;
		f = _wpv_fmake();
		if (!f)
			return -1;
	}
	_wpv_rflist_add(r, f);
	
	f->val = (char*)val;
	f->size = size;
	
	return 0;
}

static void
_wpv_rrmf(struct wpvr *r, struct wpvf *f)
{
	if (f == NULL)
		return;
	_wpv_rflist_rm(r, f);
	_wpv_rfpool_push(r, f);
}

/**
 * @brief Make an empty wpv record.
 *
 * @param flags - flags for created record
 * @param fields_cnt - initial fields count
 * @param buf_size - initial fmtbuf size(can be 0, but 1 byte is allocated
 *                   in any case for terminating '\n')
 *
 * @return  rec - on ok
 * @return  NULL - if no mem
 *
 * Make an empty wpv record with specified flags and fmtbuf initial size of size
 * or fmtiov .
 */
struct wpvr*
wpv_rmake(unsigned int flags, unsigned int fields_cnt, unsigned int buf_size)
{
	struct wpvr *r;
	int ret;
	
	r = malloc(sizeof(*r));
	if (!r)
		return NULL;
	memset(r, 0, sizeof(*r));
	r->_flags = flags;
	
	if (buf_size == 0)
		buf_size = 1;
	ret = wpv_ralloc_fmtbuf(r, buf_size);
	if (ret != 0) {
		free(r);
		return NULL;
	}
	if (fields_cnt > 0) {
		ret = wpv_ralloc_fields(r, fields_cnt);
		if (ret != fields_cnt) {
			wpv_rfree(r);
			return NULL;
		}
	} else if (r->_flags & WPV_FLAG_FMTIOV) {
		ret = _wpv_ralloc_fmtiov(r, 1);
		if (ret < 0)
			return NULL;
	}
	r->fmtbuf.buf[0] = '\n';
	r->fmtbuf.len = 1;
	if (r->_flags & WPV_FLAG_FMTIOV) {
		r->fmtiov.iov[0].iov_base = r->fmtbuf.buf;
		r->fmtiov.iov[0].iov_len = 1;
		r->fmtiov.cnt = 1;
	}

	return r;
}

/**
 * @brief Free space allocated for specified wpv record and all it fields.
 * @param r - a wpv record
 *
 * Free allocated space for a record, fields and field vals.
 */
void
wpv_rfree(struct wpvr *r)
{
	struct list_item_head *lh, *tmp;
	struct wpvf *f;
	
	if (!r)
		return;
	if (r->fields.first)
		list_for_each_safe(tmp, lh, &r->fields.first->list) {
			f = list_item(lh, struct wpvf, list);
			_wpv_ffree(f);
		}
	if (r->_flags & WPV_FLAG_FMTIOV) {
		if (r->fmtiov.iov)
			free(r->fmtiov.iov);
	}
	if (r->fmtbuf.buf)
		free(r->fmtbuf.buf);
	if (r->fields._pool)
		list_for_each_safe(tmp, lh, &r->fields._pool->list) {
			f = list_item(lh, struct wpvf, list);
			_wpv_ffree(f);
		}
	free(r);
}

/**
 * @brief Free allocated fields values if any; and move field structs
 * into fields pool. Set fcnt to 0.
 * @param r - a wpv record
 */
void
wpv_rreset(struct wpvr *r)
{
	if (!r)
		return;
	while (r->fields.first)
		_wpv_rrmf(r, r->fields.first);
	r->fields.first = NULL;
	r->fields.last = NULL;
	r->fields.cnt = 0;
	r->fmtbuf.buf[0] = '\n';
	r->fmtbuf.len = 1;
	if (r->_flags & WPV_FLAG_FMTIOV) {
		r->fmtiov.iov[0].iov_base = r->fmtbuf.buf;
		r->fmtiov.iov[0].iov_len = 1;
		r->fmtiov.cnt = 1;
	}
}

/**
 * @brief Allocate or reallocate r->fmtiov.
 *
 * @param r - a record
 * @param total - a needed fmtiov count
 *
 * @return 0 - ok
 * @return -1 - no mem
 */
static int
_wpv_ralloc_fmtiov(struct wpvr *r, unsigned int total)
{
	struct iovec *iov;
	
	if (r->fmtiov._total >= total)
		return 0;
	iov = realloc(r->fmtiov.iov, sizeof(*r->fmtiov.iov) * total);
	if (!iov) {
		return -1;
	}
	r->fmtiov.iov = iov;
	r->fmtiov._total = total;
	
	return 0;
}

/**
 * @brief Allocate specified fields.
 *
 * @param r - a record
 * @param fields_cnt - a fields count
 *
 * @return >=0 - total fields count(must be equal to fields_cnt)
 */
int
wpv_ralloc_fields(struct wpvr *r, unsigned int fields_cnt)
{
	struct wpvf *f;
	struct list_item_head *lh;
	unsigned int i, cur_fcnt;
	int ret;
	
	/* Calculate current total fields count */
	cur_fcnt = r->fields.cnt;
	if (r->fields._pool) {
		list_for_each(lh, &r->fields._pool->list) {
			cur_fcnt++;
		}
	}
	
	if (cur_fcnt >= fields_cnt)
		return cur_fcnt;
	if (r->_flags & WPV_FLAG_FMTIOV) {
		ret = _wpv_ralloc_fmtiov(r, fields_cnt * 2 + 1);
		if (ret < 0)
			return ret;
	}
	
	fields_cnt -= cur_fcnt;
	for(i = 0; i < fields_cnt; i++) {
		f = _wpv_fmake();
		if (!f)
			break;
		_wpv_rfpool_push(r, f);
	}
	
	return cur_fcnt + i;
}

/**
 * @brief Allocate or reallocate r->fmtbuf.
 *
 * @param r - a record
 * @param size - a needed fmtbuf size
 *
 * @return 0 - ok
 * @return -1 - no mem
 */
int
wpv_ralloc_fmtbuf(struct wpvr *r, unsigned int size)
{
	char *prev;
	struct wpvf *f;
	struct list_item_head *lh;
	int i;
	
	prev = r->fmtbuf.buf;
	r->fmtbuf.buf = realloc(r->fmtbuf.buf, size);
	if (!r->fmtbuf.buf) {
		r->fmtbuf._size = 0;
		return -1;
	}
	r->fmtbuf._size = size;

	if (r->_flags & WPV_FLAG_FMTIOV) {
		for(i = 0; i < r->fmtiov.cnt; i += 2)
			r->fmtiov.iov[i].iov_base = r->fmtbuf.buf +
			  ((char*)r->fmtiov.iov[i].iov_base - prev);
	} else {
		if ((prev != r->fmtbuf.buf) && (r->fields.first)) {
			list_for_each(lh, &r->fields.first->list) {
				f = list_item(lh, struct wpvf, list);
				f->val = r->fmtbuf.buf + (f->val - prev);
			}
		}
	}

	return 0;
}

/**
 * @brief Make a field with a specified value and add it to a specified
 * record.
 *
 * @param r - a record to add field to
 * @param val - a field value
 * @param size - a field value size
 *
 * @return 0 - ok
 * @return -1 - no mem
 * @return -2 - no free field in the pool
 * @return -5 - no enough space in fmtbuf
 */
int
wpv_raddf(struct wpvr *r, const char *val, int size)
{
	char *fval_off;
	int ret;
	
	ret = _wpv_raddf(r, NULL, size);
	if (ret < 0)
		return ret;
	
	if (r->_flags & WPV_FLAG_FMTIOV)
		ret = _wpv_radd_to_iov(r, &fval_off, val, size);
	else
		ret = _wpv_radd_to_buf(r, &fval_off, val, size);
	if (ret < 0)
		goto out;
	
	r->fields.last->val = fval_off;

	return 0;
out:
	_wpv_rrmf(r, r->fields.last);
	return ret;
}

/**
 * @brief Make a field with a specified value and add it to a specified
 * record.
 *
 * @param r - a record to add field to
 * @param val - a field value
 * @param size - a field value size
 *
 * @return 0 - ok
 * @return -1 - no mem
 * @return -2 - no free field in the pool
 * @return -5 - no enough space in fmtbuf
 */
static int
_wpv_radd_to_buf(struct wpvr *r, char **fval_off, const char *val, int size)
{
	int s, ret, err;
	char *pos;
	
	s = 2 + size + snprintf(NULL, 0, "%d", size);
	if ((r->fmtbuf.len + s) > r->fmtbuf._size) {
		err = -5;
		if (r->_flags & WPV_FLAG_NOALLOC_BUF)
			goto out;
		err = wpv_ralloc_fmtbuf(r, r->fmtbuf.len + s);
		if (err < 0)
			goto out;
	}
	
	pos = r->fmtbuf.buf + r->fmtbuf.len - 1;
	/*
	 * If size is 0, then "<%d>\0" string is larger than s.
	 * So, append '>' later.
	 */
	ret = snprintf(pos, s, "<%d", size);
	assert((ret + size + 1) == s);
	pos[ret] = '>';
	pos += ret + 1;
	memcpy(pos, val, size);
	pos[size] = '\n';
	r->fmtbuf.len += s;
	
	*fval_off = pos;
	
	return 0;
out:
	return err;
}

/**
 * @brief Make a field with a specified value and add it to a specified
 * record.
 *
 * @param r - a record to add field to
 * @param val - a field value
 * @param size - a field value size
 *
 * @return 0 - ok
 * @return -1 - no mem
 * @return -2 - no free field in the pool
 * @return -5 - no enough space in fmtbuf
 */
static int
_wpv_radd_to_iov(struct wpvr *r, char **fval_off, const char *val, int size)
{
	int s, ret, cnt, err;
	char *pos;
	
	s = 2 + snprintf(NULL, 0, "%d", size);
	if ((r->fmtbuf.len + s) > r->fmtbuf._size) {
		err = -5;
		if (r->_flags & WPV_FLAG_NOALLOC_BUF)
			goto out;
		err = wpv_ralloc_fmtbuf(r, r->fmtbuf.len + s);
		if (err < 0)
			goto out;
	}
	
	/* allocate iovec structs */
	cnt = r->fields.cnt * 2 + 1;
	if (cnt > r->fmtiov._total) {
		if (r->_flags & WPV_FLAG_NOALLOC_BUF)
			return -6;
		ret = _wpv_ralloc_fmtiov(r, cnt);
		if (ret < 0)
			return ret;
	}
	
	pos = r->fmtbuf.buf + r->fmtbuf.len - 1;
	/*
	 * If size is 0, then "<%d>\0" string is larger than s.
	 * So, append '>' later.
	 */
	ret = snprintf(pos, s, "<%d", size);
	assert((ret + 1) == s);
	r->fmtiov.iov[r->fmtiov.cnt - 1].iov_base = pos;
	r->fmtiov.iov[r->fmtiov.cnt - 1].iov_len = s;
	r->fmtiov.cnt++;
	pos += ret;
	
	r->fmtiov.iov[r->fmtiov.cnt - 1].iov_base = (void*)val;
	r->fmtiov.iov[r->fmtiov.cnt - 1].iov_len = size;
	r->fmtiov.cnt++;
	
	*pos = '>';
	pos++;
	*pos = '\n';
	r->fmtiov.iov[r->fmtiov.cnt - 1].iov_base = pos;
	r->fmtiov.iov[r->fmtiov.cnt - 1].iov_len = 1;
	
	r->fmtbuf.len += s;
	
	*fval_off = (char*)val;
	
	return 0;
out:
	return err;
}

/*
 * Get a next field size.
 * get:
 *   pctx - a parser context
 * return:
 *   >=0 - ok, field size
 *   -1 - no mem
 *   -3 - format error
 *   -4 - record is incomplete
 */
static int
_wpv_rparse_getfsize(struct parser_ctx *pctx)
{
	char *s, *e;
	long int ret;
	
	/* pctx->buf must contain at least '<N>' */
	if (pctx->size < 3)
		return -4;
	if (*pctx->buf != '<')
		return -3;
	pctx->buf++;
	pctx->size--;
	if (*pctx->buf == '>')
		return -3;
	
	s = pctx->buf;
	while ((pctx->size) && (*pctx->buf != '>')) {
		pctx->buf++;
		pctx->size--;
	}
	if (pctx->size == 0)
		return -4;
	ret = strtol(s, &e, 10);
	if (*e != '>')
		return -3;
	pctx->buf++;
	pctx->size--;
	
	if (ret < 0)
		return -7;
	if ((ret == LONG_MAX) || (ret >= INT_MAX))
		return -8;
	
	return (int)ret;
}

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
int
wpv_rparse(struct wpvr *r, char *buf, int size)
{
	struct parser_ctx pctx;
	int ret, s, ecode = -4;
	char *start;
	
	wpv_rreset(r);
	if (size == 0)
		goto out;
	if (*buf == '\n') {
		buf++;
		ecode = 1;
		goto out;
	}
	if (!(r->_flags & WPV_FLAG_NOCOPY_ON_PARSE)) {
		if (size > r->fmtbuf._size) {
			if (r->_flags & WPV_FLAG_NOALLOC_BUF)
				return -5;
			ret = wpv_ralloc_fmtbuf(r, size);
			if (ret < 0)
				return ret;
		}
		memcpy(r->fmtbuf.buf, buf, size);
		r->fmtbuf.len = size;
		buf = r->fmtbuf.buf;
	}
	pctx.buf = buf;
	pctx.size = size;
	start = buf;
	
	while ((s = _wpv_rparse_getfsize(&pctx)) >= 0) {
		if (s > pctx.size)
			goto out;
		ret = _wpv_raddf(r, pctx.buf, s);
		if (ret != 0) {
			ecode = ret;
			goto out;
		}
		pctx.buf += s;
		pctx.size -= s;
		buf = pctx.buf;
		if (pctx.size == 0)
			goto out;
		if (*pctx.buf == '\n') {
			buf++;
			s = 0;
			break;
		}
	}
	
	if (s == 0)
		ecode = buf - start;
	else
		ecode = s;
out:
	return ecode;
}

/*
 * Get a next wpv field.
 *
 * @param f - a current field
 *
 * @return field - if ok
 * @return NULL - if no next field
 */
struct wpvf*
wpv_fget_next(struct wpvf *f)
{
	if (!f)
		return NULL;
	
	return list_item(f->list.next, struct wpvf, list);
}

/*
 * Get a wpv field by it index.
 *
 * @param r   - a wpv record
 * @param idx - a field index (starting from 0)
 *
 * @return field - if ok
 * @return NULL - if field with such index
 */
struct wpvf*
wpv_fget(struct wpvr *r, int idx)
{
	struct list_item_head *lh;

	if (idx < 0)
		return NULL;	
	list_for_each(lh, &r->fields.first->list) {
		if (idx == 0)
			return list_item(lh, struct wpvf, list);
		idx--;
	}
	
	return NULL;
}

/**
 * @brief Return text message for specified error code.
 *
 * @param ecode - a error code returned by one of wpv_* function
 *
 * @return text error message
 */
const char*
wpv_strerror(int ecode)
{
	switch (ecode) {
	case 0:
		return "success";
	case -1:
		return "no memory";
	case -2:
		return "no free field in the pool";
	case -3:
		return "parse error: record format error";
	case -4:
		return "parse error: record is incompelete";
	case -5:
		return "format error: no enough space in fmtbuf";
	case -6:
		return "format error: no enough iovec structs";
	case -7:
		return "parse error: field width is negative";
	case -8:
		return "parse error: field width is too big";
	default:
		break;
	}
	return "unknown error";
}
