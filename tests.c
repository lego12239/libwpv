#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "wpv.h"


#define TEST_NULL {NULL, NULL}
struct test {
	int (*test)(void);
	char *title;
};

int
test0_0(void)
{
	struct wpvr *r;
	int ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf._size != 1) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	if (r->fields.first != NULL)
		goto out1;
	if (r->fields.last != NULL)
		goto out1;
	if (r->fields._pool != NULL)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test0_1(void)
{
	struct wpvr *r;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 10, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf._size != 1) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	ret = 0;
	if (!r->fields._pool)
		goto out1;
	list_for_each(lh, &r->fields._pool->list) {
		ret++;
	}
	if (ret != 10)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test0_2(void)
{
	struct wpvr *r;
	int ecode = 0;
	
	r = wpv_rmake(0, 0, 10);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf._size != 10) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	if (r->fields.first != NULL)
		goto out1;
	if (r->fields.last != NULL)
		goto out1;
	if (r->fields._pool != NULL)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test0_3(void)
{
	struct wpvr *r;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 10, 23);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf._size != 23) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	ret = 0;
	if (!r->fields._pool)
		goto out1;
	list_for_each(lh, &r->fields._pool->list) {
		ret++;
	}
	if (ret != 10)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_0_0(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *res = "<4>This<2>is<1>a<6>record\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->size != 4) {
		fprintf(stderr, "first field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 5) != f->val) {
		fprintf(stderr, "first field val point to wrong place\n");
		goto out1;
	}

	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 2) {
		fprintf(stderr, "second field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 3) != f->val) {
		fprintf(stderr, "second field val point to wrong place\n");
		goto out1;
	}

	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1) {
		fprintf(stderr, "third field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 2) != f->val) {
		fprintf(stderr, "third field val point to wrong place\n");
		goto out1;
	}

	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 6) {
		fprintf(stderr, "fourth field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 7) != f->val) {
		fprintf(stderr, "fourth field val point to wrong place\n");
		goto out1;
	}

	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_0_1(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *res = "<4>this<128>cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e<1>a<6>record\n";
	char *buf;
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "this", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	buf = r->fmtbuf.buf;
	ret = wpv_raddf(r, "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e", 128);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (buf == r->fmtbuf.buf) {
		fprintf(stderr, "fmtbuf is pointed to the same place - "
		  "use bigger fields\n");
		goto out1;
	}

	f = r->fields.first;
	if (f->size != 4) {
		fprintf(stderr, "first field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + 3) != f->val) {
		fprintf(stderr, "first field val point to wrong place %p %p\n",
		  r->fmtbuf.buf, f->val);
		goto out1;
	}

	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 128) {
		fprintf(stderr, "second field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + 3 + 4 + 5) != f->val) {
		fprintf(stderr, "second field val point to wrong place\n");
		goto out1;
	}

	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1) {
		fprintf(stderr, "third field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + 3 + 4 + 5 + 128 + 3) != f->val) {
		fprintf(stderr, "third field val point to wrong place\n");
		goto out1;
	}

	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 6) {
		fprintf(stderr, "fourth field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + 3 + 4 + 5 + 128 + 3 + 1 + 3) != f->val) {
		fprintf(stderr, "fourth field val point to wrong place\n");
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_0_2(void)
{
	struct wpvr *r;
	char *res = "\n";
	int ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_0_3(void)
{
	struct wpvr *r;
	char *res = "\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	wpv_rreset(r);
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_1_0(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *f1, *f2, *f3, *f4;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (r->fields.first->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f1 = r->fields.first->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = r->fields.first->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = r->fields.first->list.next->next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = r->fields.first->list.next->next->next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f4 = f->val;
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_1_1(void)
{
	struct wpvr *r;
	struct wpvf *f, *f1_, *f2_, *f3_, *f4_;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *f1, *f2, *f3, *f4;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f1_ = f;
	f1 = f->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f2_ = f;
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f3_ = f;
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f4_ = f;
	f4 = f->val;
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	wpv_rreset(r);
	
	f = r->fields._pool;
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	if (f->list.next) {
		fprintf(stderr, "wpv fields pool contains too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res)) {
		fprintf(stderr, "fmtbuf length is wrong: %d != %ld\n", r->fmtbuf.len,
		  strlen(res));
		goto out1;
	}
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0) {
		fprintf(stderr, "fmtbuf isn't equal to standard\n");
		goto out1;
	}
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_1_2(void)
{
	struct wpvr *r;
	struct wpvf *f, *f1_, *f2_, *f3_, *f4_;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *res1 = "<4>This<2>is<1>a<6>record<2>Hm\n";
	char *f1, *f2, *f3, *f4, *f5;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f1_ = f;
	f1 = f->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f2_ = f;
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f3_ = f;
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	f4_ = f;
	f4 = f->val;
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	wpv_rreset(r);
	
	f = r->fields._pool;
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	if (f->list.next) {
		fprintf(stderr, "wpv fields pool contains too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f5 ="Hm";
	ret = wpv_raddf(r, f5, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f5) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res1))
		goto out1;
	if (memcmp(res1, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_3_0(void)
{
	struct wpvr *r;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_FIELD, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_3_1_0(void)
{
	struct wpvr *r;
	struct wpvf *f, *f1_, *f2_, *f3_, *f4_;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *f1, *f2, *f3, *f4;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_FIELD, 4, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	f1_ = r->fields._pool;
	lh = f1_->list.next;
	f2_ = list_item(lh, struct wpvf, list);
	lh = f2_->list.next;
	f3_ = list_item(lh, struct wpvf, list);
	lh = f3_->list.next;
	f4_ = list_item(lh, struct wpvf, list);
	if (f4_->list.next) {
		fprintf(stderr, "wpv_rallocf() allocate too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f1 = r->fields.first->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f4 = f->val;
	
	ret = wpv_raddf(r, "extra", 5);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_3_1_1(void)
{
	struct wpvr *r;
	struct wpvf *f, *f1_, *f2_, *f3_, *f4_;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *f1, *f2, *f3, *f4;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_FIELD, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_ralloc_fields(r, 4);
	if (ret != 4) {
		fprintf(stderr, "wpv_rallocf() ret is %d\n", ret);
		goto out1;
	}
	f1_ = r->fields._pool;
	lh = f1_->list.next;
	f2_ = list_item(lh, struct wpvf, list);
	lh = f2_->list.next;
	f3_ = list_item(lh, struct wpvf, list);
	lh = f3_->list.next;
	f4_ = list_item(lh, struct wpvf, list);
	if (f4_->list.next) {
		fprintf(stderr, "wpv_rallocf() allocate too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f1 = r->fields.first->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f4 = f->val;
	
	ret = wpv_raddf(r, "extra", 5);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_3_2_0(void)
{
	struct wpvr *r;
	struct wpvf *f, *f1_, *f2_, *f3_, *f4_;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *f1, *f2, *f3, *f4;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_FIELD, 4, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	f1_ = r->fields._pool;
	lh = f1_->list.next;
	f2_ = list_item(lh, struct wpvf, list);
	lh = f2_->list.next;
	f3_ = list_item(lh, struct wpvf, list);
	lh = f3_->list.next;
	f4_ = list_item(lh, struct wpvf, list);
	if (f4_->list.next) {
		fprintf(stderr, "wpv_rallocf() allocate too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f1 = r->fields.first->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f4 = f->val;
	
	ret = wpv_raddf(r, "extra", 5);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	wpv_rreset(r);
	
	f = r->fields._pool;
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	if (f->list.next) {
		fprintf(stderr, "wpv fields pool contains too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	ret = wpv_raddf(r, "extra", 5);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_3_2_1(void)
{
	struct wpvr *r;
	struct wpvf *f, *f1_, *f2_, *f3_, *f4_;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *f1, *f2, *f3, *f4;
	struct list_item_head *lh;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_FIELD, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_ralloc_fields(r, 4);
	if (ret != 4) {
		fprintf(stderr, "wpv_rallocf() ret is %d\n", ret);
		goto out1;
	}
	f1_ = r->fields._pool;
	lh = f1_->list.next;
	f2_ = list_item(lh, struct wpvf, list);
	lh = f2_->list.next;
	f3_ = list_item(lh, struct wpvf, list);
	lh = f3_->list.next;
	f4_ = list_item(lh, struct wpvf, list);
	if (f4_->list.next) {
		fprintf(stderr, "wpv_rallocf() allocate too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f1 = r->fields.first->val;

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f2 = f->val;
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f3 = f->val;
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	f4 = f->val;
	
	ret = wpv_raddf(r, "extra", 5);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	wpv_rreset(r);
	
	f = r->fields._pool;
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is misused\n");
		goto out1;
	}
	if (f->val) {
		fprintf(stderr, "wpv fields pool val isn't NULL\n");
		goto out1;
	}
	if (f->list.next) {
		fprintf(stderr, "wpv fields pool contains too many fields\n");
		goto out1;
	}
	
	f1 = "This";
	ret = wpv_raddf(r, f1, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->val == f1) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f4_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}

	f2 = "is";
	ret = wpv_raddf(r, f2, 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f2) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f3_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f3 = "a";
	ret = wpv_raddf(r, f3, 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f3) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f2_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	f4 ="record";
	ret = wpv_raddf(r, f4, 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	lh = f->list.next;
	f = list_item(lh, struct wpvf, list);
	if (f->val == f4) {
		fprintf(stderr, "wpv field isn't copied\n");
		goto out1;
	}
	if (f != f1_) {
		fprintf(stderr, "wpv fields pool is unused\n");
		goto out1;
	}
	
	ret = wpv_raddf(r, "extra", 5);
	if (ret != -2) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_5_0_0(void)
{
	struct wpvr *r;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *ptr;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_BUF, 0, strlen(res));
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf._size != strlen(res)) {
		fprintf(stderr, "fmtbuf._size is not equal to %ld\n", strlen(res));
		goto out1;
	}
	ptr = r->fmtbuf.buf;
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (ptr != r->fmtbuf.buf)
		goto out1;
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_5_0_1(void)
{
	struct wpvr *r;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *ptr;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_BUF, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_ralloc_fmtbuf(r, strlen(res));
	if (ret < 0) {
		fprintf(stderr, "wpv_ralloc_fmtbuf() error: %s\n",
		  wpv_strerror(ret));
		goto out1;
	}
	if (r->fmtbuf._size != strlen(res)) {
		fprintf(stderr, "fmtbuf._size is not equal to %ld\n", strlen(res));
		goto out1;
	}
	ptr = r->fmtbuf.buf;
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (ptr != r->fmtbuf.buf)
		goto out1;
	if (r->fmtbuf.len != strlen(res))
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_5_1_0(void)
{
	struct wpvr *r;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *ptr;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_BUF, 0, strlen(res));
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	if (r->fmtbuf._size != strlen(res)) {
		fprintf(stderr, "fmtbuf._size is not equal to %ld\n", strlen(res));
		goto out1;
	}
	ptr = r->fmtbuf.buf;
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "Hm", 2);
	if (ret != -5) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (ptr != r->fmtbuf.buf)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test1_5_1_1(void)
{
	struct wpvr *r;
	char *res = "<4>This<2>is<1>a<6>record\n";
	char *ptr;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOALLOC_BUF, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_ralloc_fmtbuf(r, strlen(res));
	if (ret < 0) {
		fprintf(stderr, "wpv_ralloc_fmtbuf() error: %s\n",
		  wpv_strerror(ret));
		goto out1;
	}
	if (r->fmtbuf._size != strlen(res)) {
		fprintf(stderr, "fmtbuf._size is not equal to %ld\n", strlen(res));
		goto out1;
	}
	ptr = r->fmtbuf.buf;
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "Hm", 2);
	if (ret != -5) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (ptr != r->fmtbuf.buf)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test2_0(void)
{
	struct wpvr *r;
	char *res = "<5>\0This<3>i\0s<2>a\0<8>\0record\0\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "\0This", 5);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "i\0s", 3);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a\0", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "\0record\0", 8);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtbuf.len != 31)
		goto out1;
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0)
		goto out1;

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test3_0(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *res = "<4>This<0><1>a<0>\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = r->fields.first;
	if (f->size != 4) {
		fprintf(stderr, "first field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 5) != f->val) {
		fprintf(stderr, "first field val point to wrong place\n");
		goto out1;
	}

	ret = wpv_raddf(r, "", 0);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 0) {
		fprintf(stderr, "second field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 1) != f->val) {
		fprintf(stderr, "second field val point to wrong place\n");
		goto out1;
	}
	
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1) {
		fprintf(stderr, "third field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 2) != f->val) {
		fprintf(stderr, "third field val point to wrong place\n");
		goto out1;
	}
	
	ret = wpv_raddf(r, "", 0);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 0) {
		fprintf(stderr, "fourth field size is wrong: %d\n", f->size);
		goto out1;
	}
	if ((r->fmtbuf.buf + r->fmtbuf.len - 1) != f->val) {
		fprintf(stderr, "fourth field val point to wrong place\n");
		goto out1;
	}
	
	if (r->fmtbuf.len != strlen(res)) {
		fprintf(stderr, "fmtbuf.len is not equal to %ld: %d\n", strlen(res),
		  r->fmtbuf.len);
		goto out1;
	}
	if (memcmp(res, r->fmtbuf.buf, r->fmtbuf.len) != 0) {
		fprintf(stderr, "fmtbuf is wrong: %.*s\n", r->fmtbuf.len, r->fmtbuf.buf);
		goto out1;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_0_0(void)
{
	struct wpvr *r;
	int ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	if (r->fmtbuf._size != 1) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	if (r->fields.first != NULL)
		goto out1;
	if (r->fields.last != NULL)
		goto out1;
	if (r->fields._pool != NULL)
		goto out1;
	if (r->fmtiov._total != 1) {
		fprintf(stderr, "fmtiov._total is not equal to 1: %d\n", r->fmtiov._total);
		goto out1;
	}
	if (r->fmtiov.cnt != 1) {
		fprintf(stderr, "fmtiov.cnt is not equal to 1: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	if (r->fmtiov.iov[0].iov_base != r->fmtbuf.buf) {
		fprintf(stderr, "fmtiov.iov[0].iov_base is not equal to fmtbuf.buf:"
		  "%p != %p\n", r->fmtiov.iov[0].iov_base, r->fmtbuf.buf);
		goto out1;
	}
	if (r->fmtiov.iov[0].iov_len != 1) {
		fprintf(stderr, "fmtiov.iov[0].iov_len is not equal 1: %ld\n",
		  r->fmtiov.iov[0].iov_len);
		goto out1;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_0(void)
{
	struct wpvr *r;
	char *res[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "\n"};
	struct list_item_head *lh;
	struct wpvf *f;
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (r->fmtiov.cnt != 3) {
		fprintf(stderr, "fmtiov.cnt isn't equal to 3: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (r->fmtiov.cnt != 5) {
		fprintf(stderr, "fmtiov.cnt isn't equal to 5: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (r->fmtiov.cnt != 7) {
		fprintf(stderr, "fmtiov.cnt isn't equal to 7: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (r->fmtiov.cnt != 9) {
		fprintf(stderr, "fmtiov.cnt isn't equal to 9: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_00(void)
{
	struct wpvr *r;
	char *res[] = {"<4>", "This", "<128>", "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e", "<1>", "a", "<6>", "record", "<6>", "record", "<6>", "record",
	  "<6>", "record", "<6>", "record", "<6>", "record", "<6>", "record", "<6>",
	  "record", "\n"};
	char *buf;
	struct list_item_head *lh;
	struct wpvf *f;
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}

	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	if (r->fmtiov.cnt != 3) {
		fprintf(stderr, "fmtiov.cnt isn't equal to 3: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	buf = r->fmtbuf.buf;
	
	for(i = 3; i < 22; i += 2) {
		ret = wpv_raddf(r, res[i], strlen(res[i]));
		if (ret < 0) {
			fprintf(stderr, "wpv_raddf() ret is %d for %d field\n", ret, i);
			goto out1;
		}
		if (r->fmtiov.cnt != (i + 2)) {
			fprintf(stderr, "fmtiov.cnt isn't equal to %d for %d: %d\n",
			  i + 2, i, r->fmtiov.cnt);
			goto out1;
		}
	}
	if (r->fmtiov.cnt != 23) {
		fprintf(stderr, "fmtiov.cnt isn't equal to 23: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	
	if (buf == r->fmtbuf.buf) {
		fprintf(stderr, "fmtbuf.buf isn't reallocated!\n");
		goto out1;
	}
	
	for(i = 0; i < 23; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_1(void)
{
	struct wpvr *r;
	void *ptr;
	char *res[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "\n"};
	struct list_item_head *lh;
	struct wpvf *f;
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtiov.cnt != 9)
		goto out1;
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}
	ptr = r->fmtiov.iov;
	
	wpv_rreset(r);
	
	if (r->fmtiov.cnt != 1)
		goto out1;
	if (r->fmtiov.iov[0].iov_len != 1)
		goto out1;
	if (memcmp(r->fmtiov.iov[0].iov_base, "\n", 1) != 0)
		goto out1;
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtiov.cnt != 9)
		goto out1;
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}

	if (ptr != r->fmtiov.iov) {
		fprintf(stderr, "fmtiov is allocated again\n");
		goto out1;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_2(void)
{
	struct wpvr *r;
	void *ptr;
	char *res[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "\n"};
	char *res1[] = {"<4>", "This", "<2>", "is", "<1>", "a", "\n"};
	struct list_item_head *lh;
	struct wpvf *f;
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtiov.cnt != 9)
		goto out1;
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}
	ptr = r->fmtiov.iov;
	
	wpv_rreset(r);
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtiov.cnt != 7)
		goto out1;
	if (r->fields.cnt != 3)
		goto out1;
	
	for(i = 0; i < 7; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res1[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res1[i]), i);
			goto out1;
		}
		if (memcmp(res1[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}
	
	if (ptr != r->fmtiov.iov) {
		fprintf(stderr, "fmtiov is allocated again\n");
		goto out1;
	}

	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_3(void)
{
	struct wpvr *r;
	void *ptr;
	char *res[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "\n"};
	char *res1[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "<2>", "Hm", "<2>", "Hm", "<2>", "Hm", "<2>", "Hm", "<2>", "Hm", "\n"};
	struct list_item_head *lh;
	struct wpvf *f;
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (r->fmtiov.cnt != 9)
		goto out1;
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}
	ptr = r->fmtiov.iov;
	
	wpv_rreset(r);
	
	for(i = 1; i < 19; i += 2) {
		ret = wpv_raddf(r, res1[i], strlen(res1[i]));
		if (ret < 0) {
			fprintf(stderr, "wpv_raddf() ret is %d for %d field\n", ret, i);
			goto out1;
		}
	}
	
	if (r->fmtiov.cnt != 19)
		goto out1;
	if (ptr == r->fmtiov.iov) {
		fprintf(stderr, "fmtiov isn't reallocated\n");
		goto out1;
	}

	for(i = 0; i < 19; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res1[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res1[i]), i);
			goto out1;
		}
		if (memcmp(res1[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	i = 1;
	list_for_each(lh, &r->fields.first->list) {
		f = list_item(lh, struct wpvf, list);
		if (f->size != strlen(res1[i])) {
			fprintf(stderr, "size isn't equal to %ld for %d field\n",
			  strlen(res1[i]), (i-1)/2);
			goto out1;
		}
		if (memcmp(res1[i], f->val, f->size) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
		i += 2;
	}
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_4_0(void)
{
	struct wpvr *r;
	void *ptr;
	char *res[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "\n"};
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV | WPV_FLAG_NOALLOC_BUF, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	ret = wpv_ralloc_fmtbuf(r, 13);
	if (ret < 0) {
		fprintf(stderr, "wpv_ralloc_fmtbuf() error: %s\n",
		  wpv_strerror(ret));
		goto out1;
	}
	if (r->fmtbuf._size != 13) {
		fprintf(stderr, "fmtbuf._size is not 13: %d\n", r->fmtbuf._size);
		goto out1;
	}
	ret = wpv_ralloc_fields(r, 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_ralloc_fmtbuf() error: %s\n",
		  wpv_strerror(ret));
		goto out1;
	}
	if (r->fmtiov._total != 9) {
		fprintf(stderr, "fmtiov._total is not 9: %d\n", r->fmtiov._total);
		goto out1;
	}
	ptr = r->fmtiov.iov;
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (ptr != r->fmtiov.iov) {
		fprintf(stderr, "fmtiov is reallocated\n");
		goto out1;
	}
	if (r->fmtiov.cnt != 9)
		goto out1;
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_4_1(void)
{
	struct wpvr *r;
	void *ptr, *buf;
	char *res[] = {"<4>", "This", "<2>", "is", "<1>", "a", "<6>", "record",
	  "\n"};
	int ret, ecode = 0, i;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV | WPV_FLAG_NOALLOC_BUF, 4, 13);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	buf = r->fmtbuf.buf;
	ptr = r->fmtiov.iov;
	if (r->fmtbuf._size != 13)
		goto out1;
	if (r->fmtiov._total != 9)
		goto out1;
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "is", 2);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "a", 1);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	ret = wpv_raddf(r, "record", 6);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	if (buf != r->fmtbuf.buf) {
		fprintf(stderr, "fmtbuf is reallocated\n");
		goto out1;
	}
	if (ptr != r->fmtiov.iov) {
		fprintf(stderr, "fmtiov is reallocated\n");
		goto out1;
	}
	if (r->fmtiov.cnt != 9)
		goto out1;
	
	for(i = 0; i < 9; i++) {
		if (r->fmtiov.iov[i].iov_len != strlen(res[i])) {
			fprintf(stderr, "iov_len isn't equal to %ld for %d iov\n",
			  strlen(res[i]), i);
			goto out1;
		}
		if (memcmp(res[i], r->fmtiov.iov[i].iov_base,
		  r->fmtiov.iov[i].iov_len) != 0) {
			fprintf(stderr, "iov_base isn't equal for %d iov\n", i);
			goto out1;
		}
	}
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_4_2(void)
{
	struct wpvr *r;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV | WPV_FLAG_NOALLOC_BUF, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	ret = wpv_raddf(r, "This", 4);
	if (ret != -5) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_4_3(void)
{
	struct wpvr *r;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV | WPV_FLAG_NOALLOC_BUF, 0, 4);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	ret = wpv_raddf(r, "This", 4);
	if (ret != -6) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test4_8(void)
{
	struct wpvr *r;
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_FMTIOV, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}
	
	wpv_rreset(r);
	
	if (r->fmtbuf._size != 4) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	if (r->fields.first != NULL)
		goto out1;
	if (r->fields.last != NULL)
		goto out1;
	if (r->fields._pool == NULL)
		goto out1;
	if (r->fmtiov._total != 3) {
		fprintf(stderr, "fmtiov._total is not equal to 1: %d\n", r->fmtiov._total);
		goto out1;
	}
	if (r->fmtiov.cnt != 1) {
		fprintf(stderr, "fmtiov.cnt is not equal to 1: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	if (r->fmtiov.iov[0].iov_base != r->fmtbuf.buf) {
		fprintf(stderr, "fmtiov.iov[0].iov_base is not equal to fmtbuf.buf:"
		  "%p != %p\n", r->fmtiov.iov[0].iov_base, r->fmtbuf.buf);
		goto out1;
	}
	if (r->fmtiov.iov[0].iov_len != 1) {
		fprintf(stderr, "fmtiov.iov[0].iov_len is not equal 1: %ld\n",
		  r->fmtiov.iov[0].iov_len);
		goto out1;
	}
	
	wpv_rreset(r);
	
	ret = wpv_raddf(r, "This", 4);
	if (ret < 0) {
		fprintf(stderr, "wpv_raddf() ret is %d\n", ret);
		goto out1;
	}

	wpv_rreset(r);
	
	if (r->fmtbuf._size != 4) {
		fprintf(stderr, "_fmtbuf_size isn't equal to 1: %d\n", r->fmtbuf._size);
		goto out1;
	}
	if (r->fmtbuf.len != 1)
		goto out1;
	if (r->fmtbuf.buf[0] != '\n')
		goto out1;
	if (r->fields.first != NULL)
		goto out1;
	if (r->fields.last != NULL)
		goto out1;
	if (r->fields._pool == NULL)
		goto out1;
	if (r->fmtiov._total != 3) {
		fprintf(stderr, "fmtiov._total is not equal to 1: %d\n", r->fmtiov._total);
		goto out1;
	}
	if (r->fmtiov.cnt != 1) {
		fprintf(stderr, "fmtiov.cnt is not equal to 1: %d\n", r->fmtiov.cnt);
		goto out1;
	}
	if (r->fmtiov.iov[0].iov_base != r->fmtbuf.buf) {
		fprintf(stderr, "fmtiov.iov[0].iov_base is not equal to fmtbuf.buf:"
		  "%p != %p\n", r->fmtiov.iov[0].iov_base, r->fmtbuf.buf);
		goto out1;
	}
	if (r->fmtiov.iov[0].iov_len != 1) {
		fprintf(stderr, "fmtiov.iov[0].iov_len is not equal 1: %ld\n",
		  r->fmtiov.iov[0].iov_len);
		goto out1;
	}
	
	
	ecode = 1;
out1:
	wpv_rfree(r);
	return ecode;
}

int
test5(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<4>This<2>is<1>a<6>record\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	if (ret < 0) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = r->fields.first;
	if (f->size != 4)
		goto out;
	if (memcmp("This", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 2)
		goto out;
	if (memcmp("is", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1)
		goto out;
	if (memcmp("a", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 6)
		goto out;
	if (memcmp("record", f->val, f->size) != 0)
		goto out;
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

int
test5a(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<4>This<2>is<1>a<6>record\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	if (ret != strlen(src)) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = r->fields.first;
	if (f->size != 4)
		goto out;
	if (memcmp("This", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 2)
		goto out;
	if (memcmp("is", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1)
		goto out;
	if (memcmp("a", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 6)
		goto out;
	if (memcmp("record", f->val, f->size) != 0)
		goto out;
	
	if (r->fmtbuf.len != strlen(src)) {
		fprintf(stderr, "fmtbuf.len isn't equal to %ld: %d\n", strlen(src),
		  r->fmtbuf.len);
		goto out;
	}
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

int
test5b(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<4>This<2>is<1>a<6>record\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(WPV_FLAG_NOCOPY_ON_PARSE, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	if (ret != strlen(src)) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = r->fields.first;
	if (f->size != 4)
		goto out;
	if (f->val != (src + 3))
		goto out;
	if (memcmp("This", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 2)
		goto out;
	if (f->val != (src + 10))
		goto out;
	if (memcmp("is", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1)
		goto out;
	if (f->val != (src + 15))
		goto out;
	if (memcmp("a", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 6)
		goto out;
	if (f->val != (src + 19))
		goto out;
	if (memcmp("record", f->val, f->size) != 0)
		goto out;
	
	if (r->fmtbuf.len != 1) {
		fprintf(stderr, "fmtbuf.len isn't equal to %d: %d\n", 1,
		  r->fmtbuf.len);
		goto out;
	}
	if (r->fmtbuf.buf[0] != '\n')
		goto out;
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

int
test6(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<5>\0This<3>i\0s<2>a\0<8>\0record\0\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, 31);
	if (ret != 31) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = r->fields.first;
	if (f->size != 5)
		goto out;
	if (memcmp("\0This", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 3)
		goto out;
	if (memcmp("i\0s", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 2)
		goto out;
	if (memcmp("a\0", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 8)
		goto out;
	if (memcmp("\0record\0", f->val, f->size) != 0)
		goto out;
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

int
test7(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<4>This<0><1>a<0>\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	if (ret != strlen(src)) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = r->fields.first;
	if (f->size != 4)
		goto out;
	if (memcmp("This", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 0)
		goto out;
	if (memcmp("is", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 1)
		goto out;
	if (memcmp("a", f->val, f->size) != 0)
		goto out;
	
	if (!f->list.next)
		goto out;
	f = list_item(f->list.next, struct wpvf, list);
	if (f->size != 0)
		goto out;
	if (memcmp("record", f->val, f->size) != 0)
		goto out;
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

int
test11(void)
{
	struct wpvr *r;
	char *src = "4>This<2>is<1>a<6>record\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -3)
		return 0;
	return 1;
}

int
test12(void)
{
	struct wpvr *r;
	char *src = "<4This<2>is<1>a<6>record\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -3)
		return 0;
	return 1;
}

int
test13(void)
{
	struct wpvr *r;
	char *src = "<4>This2>is<1>a<6>record\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -3)
		return 0;
	return 1;
}

int
test14(void)
{
	struct wpvr *r;
	char *src = "<4>This<2is<1>a<6>record\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -3)
		return 0;
	return 1;
}

int
test15(void)
{
	struct wpvr *r;
	char *src = "<4>This<2>is<1>a<6>record";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -4)
		return 0;
	return 1;
}

int
test16(void)
{
	struct wpvr *r;
	char *src = "<5>This<2>is<1>a<6>record\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -3)
		return 0;
	return 1;
}

int
test17(void)
{
	struct wpvr *r;
	char *src = "<4>This<2>is<1>a<6>\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -4)
		return 0;
	return 1;
}

int
test18(void)
{
	struct wpvr *r;
	char *src = "<4>This<2>is<1>a<6>r\n";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -4)
		return 0;
	return 1;
}

int
test19(void)
{
	struct wpvr *r;
	char *src = "<4>This<2>is<1>a<6>";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -4)
		return 0;
	return 1;
}

int
test20(void)
{
	struct wpvr *r;
	char *src = "<4>This<2>is<1>a<6>r";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -4)
		return 0;
	return 1;
}

int
test21(void)
{
	struct wpvr *r;
	char *src = "<4>This<-1><2>aa";
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	wpv_rfree(r);
	if (ret != -7)
		return 0;
	return 1;
}

int
test22(void)
{
	struct wpvr *r;
#if INT_MAX == 2147483647
	char *src1 = "<4>This<2147483647>aaaaaaa";
	char *src2 = "<4>This<2147483648>aaaaaaa";
	char *src3 = "<4>This<214748364799>aaaaaaa";
#endif
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src1, strlen(src1));
	if (ret != -8)
		return 0;
	ret = wpv_rparse(r, src2, strlen(src2));
	if (ret != -8)
		return 0;
	ret = wpv_rparse(r, src3, strlen(src3));
	if (ret != -8)
		return 0;
	wpv_rfree(r);
	return 1;
}

int
test23_0(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<4>This<2>is<1>a<6>record\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	if (ret != strlen(src)) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = r->fields.first;
	if (f->size != 4)
		goto out;
	if (memcmp("This", f->val, f->size) != 0)
		goto out;
	
	f = wpv_fget_next(f);
	if (!f)
		goto out;
	if (f->size != 2)
		goto out;
	if (memcmp("is", f->val, f->size) != 0)
		goto out;
	
	f = wpv_fget_next(f);
	if (!f)
		goto out;
	if (f->size != 1)
		goto out;
	if (memcmp("a", f->val, f->size) != 0)
		goto out;
	
	f = wpv_fget_next(f);
	if (!f)
		goto out;
	if (f->size != 6)
		goto out;
	if (memcmp("record", f->val, f->size) != 0)
		goto out;
	
	if (wpv_fget_next(f) != NULL)
		goto out;
	
	if (r->fmtbuf.len != strlen(src)) {
		fprintf(stderr, "fmtbuf.len isn't equal to %ld: %d\n", strlen(src),
		  r->fmtbuf.len);
		goto out;
	}
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

int
test23_1(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *src = "<4>This<2>is<1>a<6>record\n";
	int ret, ecode = 0;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "wpv_rmake() ret NULL\n");
		return 0;
	}
	
	ret = wpv_rparse(r, src, strlen(src));
	if (ret != strlen(src)) {
		fprintf(stderr, "wpv_rparse() ret is %d\n", ret);
		goto out;
	}
	
	if (r->fields.cnt != 4)
		goto out;
	f = wpv_fget(r, 0);
	if (f->size != 4)
		goto out;
	if (memcmp("This", f->val, f->size) != 0)
		goto out;
	
	f = wpv_fget(r, 1);
	if (!f)
		goto out;
	if (f->size != 2)
		goto out;
	if (memcmp("is", f->val, f->size) != 0)
		goto out;
	
	f = wpv_fget(r, 2);
	if (!f)
		goto out;
	if (f->size != 1)
		goto out;
	if (memcmp("a", f->val, f->size) != 0)
		goto out;
	
	f = wpv_fget(r, 3);
	if (!f)
		goto out;
	if (f->size != 6)
		goto out;
	if (memcmp("record", f->val, f->size) != 0)
		goto out;
	
	if (wpv_fget(r, 4) != NULL)
		goto out;
	if (r->fmtbuf.len != strlen(src)) {
		fprintf(stderr, "fmtbuf.len isn't equal to %ld: %d\n", strlen(src),
		  r->fmtbuf.len);
		goto out;
	}
	
	ecode = 1;
out:
	wpv_rfree(r);
	return ecode;
}

struct test tests[] = {
	{test0_0, "wpv rmake with 0, 0, 0"},
	{test0_1, "wpv rmake with 0, 10, 0"},
	{test0_2, "wpv rmake with 0, 0, 10"},
	{test0_3, "wpv rmake with 0, 10, 23"},
	{test1_0_0, "wpv addf 1"},
	{test1_0_1, "wpv addf 2"},
	{test1_0_2, "wpv fmtbuf without fields"},
	{test1_0_3, "wpv fmtbuf without fields after wpv_rreset()"},
	{test1_1_0, "wpv addf (flags=0)"},
	{test1_1_1, "wpv addf, reset, format (flags=0) 1"},
	{test1_1_2, "wpv addf, reset, format (flags=0) 2"},
	{test1_3_0, "wpv addf (flags=WPV_FLAG_NOALLOC_FIELD)"},
	{test1_3_1_0, "wpv addf, reset, format (flags=WPV_FLAG_NOALLOC_FIELD) 1"},
	{test1_3_1_1, "wpv addf, reset, format (flags=WPV_FLAG_NOALLOC_FIELD) 2"},
	{test1_3_2_0, "wpv addf, reset, format (flags=WPV_FLAG_NOALLOC_FIELD) 3"},
	{test1_3_2_1, "wpv addf, reset, format (flags=WPV_FLAG_NOALLOC_FIELD) 4"},
	{test1_5_0_0, "wpv format (flags=WPV_FLAG_NOALLOC_BUF) 1"},
	{test1_5_0_1, "wpv format (flags=WPV_FLAG_NOALLOC_BUF) 2"},
	{test1_5_1_0, "wpv format (flags=WPV_FLAG_NOALLOC_BUF) 3"},
	{test1_5_1_1, "wpv format (flags=WPV_FLAG_NOALLOC_BUF) 4"},
	{test2_0, "wpv addf with \\0"},
	{test3_0, "wpv addf with empty fields"},
	{test4_0_0, "wpv rmake with WPV_FLAG_FMTIOV, 0, 0"},
	{test4_0, "wpv addf iovec 1"},
	{test4_00, "wpv addf iovec 2"},
	{test4_1, "wpv addf, reset, addf iovec 1"},
	{test4_2, "wpv addf, reset, addf iovec 2"},
	{test4_3, "wpv addf, reset, addf iovec 3"},
	{test4_4_0, "wpv addf iovec (flags=WPV_FLAG_NOALLOC_BUF) 1"},
	{test4_4_1, "wpv addf iovec (flags=WPV_FLAG_NOALLOC_BUF) 2"},
	{test4_4_2, "wpv addf iovec (flags=WPV_FLAG_NOALLOC_BUF) 3"},
	{test4_4_3, "wpv addf iovec (flags=WPV_FLAG_NOALLOC_BUF) 4"},
	{test4_8, "wpv addf iovec, empty record (after wpv_rreset())"},
	{test5, "wpv parse (without end checking)"},
	{test5a, "wpv parse"},
	{test5b, "wpv parse (flags=WPV_FLAG_NOCOPY_ON_PARSE)"},
	{test6, "wpv parse with \\0"},
	{test7, "wpv parse with empty fields"},
	{test11, "wpv parse broken buf at start 1"},
	{test12, "wpv parse broken buf at start 2"},
	{test13, "wpv parse broken buf in middle 1"},
	{test14, "wpv parse broken buf in middle 2"},
	{test15, "wpv parse broken buf at end"},
	{test16, "wpv parse broken buf with wrong field size"},
	{test17, "wpv parse incomplete buf 1"},
	{test18, "wpv parse incomplete buf 2"},
	{test19, "wpv parse incomplete buf 3"},
	{test20, "wpv parse incomplete buf 4"},
	{test21, "wpv parse with negative field width"},
	{test22, "wpv parse with too big field width"},
	{test23_0, "wpv get_field_next"},
	{test23_1, "wpv get_field"},
	TEST_NULL
};

int
main(void)
{
	int i, cnt, ret;
	
	for(i = 0, cnt = 0; tests[i].test; i++) {
		printf("%s... ", tests[i].title);
		ret = tests[i].test();
		printf("%s\n", ret ? "ok" : "fail");
		cnt += ret;
	}
	
	printf("TOTAL %d/%d\n", cnt, i);
	return 0;
}
