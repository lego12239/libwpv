#include <stdio.h>
#include <stdlib.h>
#include "../wpv.h"

void
main(void)
{
	struct wpvr *r;
	struct wpvf *f;
	char *buf = "<6>Field1<8>Field2-_<6>Field3\n";
	int ret, i;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "no memory\n");
		exit(1);
	}
	
	printf("Try to parse a record: '%s'\n", buf);
	
	ret = wpv_rparse(r, buf, strlen(buf));
	if (ret < 0) {
		fprintf(stderr, "parse error: %s\n", wpv_strerror(ret));
		exit(1);
	}
	
	for(i = 0; i < r->fields.cnt; i++) {
		f = wpv_fget(r, i);
		printf("%d: '%.*s'\n", i, f->size, f->val);
	}
	
	wpv_rfree(r);
}
