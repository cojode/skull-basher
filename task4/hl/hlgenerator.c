#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

#ifndef countof
#define countof(x)	(sizeof( x ) / sizeof( x[0] ))
#endif

#define	SELF_NAME	"hlgenerator"


const char *schemes[] = {
	"http://",
	"https://",
	"ftp://",
	"news://",
	"foobar://",
	"data:",
	"foo:",
	"",
};

const char *users[] = {
	"john:doe@",
	"smith@",
	"123456@",
	""
};

const char *domains[] = {
	"gosuslugi.ru",
	"ietf.org",
	"москва.рф",
	""
};

const char *ports[] = {
	":8080",
	":[80]",
	"[:80]",
	":",
	""
};

const char *paths[] = {
	"/",
	"/foo",
	"/foo/",
	"/foo/bar",
	"/foo/bar/",
	""
};

const char *queries[] = {
	"?foo=bar&bar=buz",
	"?foo=bar&",
	"?foo=bar",
	"?foo=",
	"?foo",
	"?",
	""
};

const char *anchors[] = {
	"#foo%20bar",
	"#foo%20",
	"#%20foo",
	"#foo",
	"#",
	""
};

const char **sources[] = {
	schemes,
	users,
	domains,
	ports,
	paths,
	queries,
	anchors
};

int
main(int argc, char **argv) {
	size_t	idx[countof(sources)], i, j, t;
	size_t	nvariants = 1;
	char	buf[1024];

	for (size_t i = 0; i < countof(sources); i++)
		nvariants *= arrsz(sources[i]);

	for (size_t i = 0; i < nvariants; i++) {
		t = i;

		buf[0] = '\0';
		for (j = 0; j < countof(sources); j++) {
			idx[j] = t % arrsz(sources[j]);
			if (strlcat(buf, sources[j][idx[j]], sizeof(buf)) >= sizeof(buf)) {
				fprintf(stderr, "internal buffer was too small\n");
				return 1;
			}

			t /= arrsz(sources[j]);
		}

		puts(buf);
	}

	return 0;
}
