#include <stdio.h>
#include <stdlib.h>
#include "../wpv.h"

void
main(void)
{
	struct wpvr *r;
	int ret;
	
	r = wpv_rmake(0, 0, 0);
	if (!r) {
		fprintf(stderr, "no memory\n");
		exit(1);
	}
	
	ret = wpv_raddf(r, "Field1", strlen("Field1"));
	if (ret < 0) {
		fprintf(stderr, "field 1 add error: %s\n", wpv_strerror(ret));
		exit(1);
	}
	ret = wpv_raddf(r, "Field2", strlen("Field2"));
	if (ret < 0) {
		fprintf(stderr, "field 2 add error: %s\n", wpv_strerror(ret));
		exit(1);
	}
	ret = wpv_raddf(r, "Field3", strlen("Field3"));
	if (ret < 0) {
		fprintf(stderr, "field 3 add error: %s\n", wpv_strerror(ret));
		exit(1);
	}
	
	printf("The formatted record is:\n'%.*s'", r->fmtbuf.len, r->fmtbuf.buf);
	
	wpv_rfree(r);
}
