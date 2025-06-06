#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "hlparser.h"

#define	SELF_NAME	"hldetector"
#define STDIN_DISP_PATH	"(stdin)"


void
printer(const char *uri) {
	puts(uri);
}

int
main(int argc, char **argv) {
	FILE		*f;
	const char	*path;
	int		 rc = 0;

	if (argc > 1) {
		for (int i = 0; i < argc - 1; i++) {
			path = argv[i+1];
			if (strcmp(path, "-") == 0) {
				rc |= proceed_hyperlinks(stdin, STDIN_DISP_PATH, printer);
			} else {
				if ((f = fopen(path, "r")) == NULL) {
					fprintf(stderr, "%s: could not open %s: %s\n",
					    SELF_NAME, path, strerror(errno));
					rc = 1;
					continue;
				}
				rc |= proceed_hyperlinks(f, path, printer);
				fclose(f);
			}
		}
	} else
		rc = proceed_hyperlinks(stdin, STDIN_DISP_PATH, printer);

	return rc;
}
