#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"
#include "hlparser.h"


enum hlstate_step {
	HL_INVALID = 0,
	HL_SCHEME,
	HL_HOST_OR_USER,
	HL_PASSWORD_OR_PORT,
	HL_HOST,
	HL_PORT,
	HL_PORT_BRACKETED,
	HL_PATH,
	HL_QUERY,
	HL_ANCHOR,
	HL_MAX = HL_ANCHOR
};

const char *hlstate_step_name[] = {
	"INVALID",
	"SCHEME",
	"HOST_OR_USER",
	"PASSWORD_OR_PORT",
	"HOST",
	"PORT",
	"PORT_BRACKETED",
	"PATH",
	"QUERY",
	"ANCHOR",
	NULL
};

struct hlstate {
	hlhandler	hs_handler;
	enum	hlstate_step hs_step;
	bool	hs_is_https;
	bool	hs_is_pct;
	size_t	hs_buflen;
	size_t	hs_pctlen;	// includes '%' itself, so >0 means we're decoding a percent-encoded character
	char	hs_buf[10240];
};

#define lastch(n)	state->hs_buf[state->hs_buflen - (n)]
#define curch		lastch(1)

static void	proceed_hyperlinks_invalid(struct hlstate *state);
static void	proceed_hyperlinks_scheme(struct hlstate *state);
static void	proceed_hyperlinks_host_or_user(struct hlstate *state);
static void	proceed_hyperlinks_password_or_port(struct hlstate *state);
static void	proceed_hyperlinks_host(struct hlstate *state);
static void	proceed_hyperlinks_port(struct hlstate *state);
static void	proceed_hyperlinks_port_bracketed(struct hlstate *state);
static void	proceed_hyperlinks_path(struct hlstate *state);
static void	proceed_hyperlinks_query(struct hlstate *state);
static void	proceed_hyperlinks_anchor(struct hlstate *state);

static void	proceed_hyperlinks_pct(struct hlstate *state);
static void	abort_hyperlink(struct hlstate *state);
static void	finish_hyperlink(struct hlstate *state);
static bool	is_hyperlink_valid(const struct hlstate *state);

static struct hlstate* pop_last_char(struct hlstate *state);

typedef void (*hyperlink_handler)(struct hlstate *);

const hyperlink_handler handlers[] = {
	proceed_hyperlinks_invalid,
	proceed_hyperlinks_scheme,
	proceed_hyperlinks_host_or_user,
	proceed_hyperlinks_password_or_port,
	proceed_hyperlinks_host,
	proceed_hyperlinks_port,
	proceed_hyperlinks_port_bracketed,
	proceed_hyperlinks_path,
	proceed_hyperlinks_query,
	proceed_hyperlinks_anchor,

	// Handlers above must match corresponding HL_* constants.
	// Handlers below may be listed in any order

	proceed_hyperlinks_pct
};


static void
proceed_hyperlinks_invalid(struct hlstate *state) {
	// try to start parsing scheme again after next word delimiter
	if (isblank(curch) || ispunct(curch) || iscntrl(curch))
		abort_hyperlink(state);
	state->hs_step = HL_SCHEME;
}

static void
proceed_hyperlinks_scheme(struct hlstate *state) {
	switch (state->hs_buflen) {
	case 8:
		dbgprint("scheme: 8: %s\n", state->hs_buf);
		if (strcmp("https://", state->hs_buf) == 0) {
			state->hs_is_https = 1;
			state->hs_step = HL_HOST_OR_USER;
		} else
			abort_hyperlink(state);
		break;

	case 7:
		dbgprint("scheme: 7: %s\n", state->hs_buf);
		if (strcmp("http://", state->hs_buf) == 0) {
			state->hs_is_https = 0;
			state->hs_step = HL_HOST_OR_USER;
		} else if (strcmp("https:/", state->hs_buf) != 0)
			abort_hyperlink(state);
		break;

	default:
		dbgprint("scheme: default: %s\n", state->hs_buf);
	}
}

static void
proceed_hyperlinks_host_or_user(struct hlstate *state) {
	switch (curch) {
	case '@':
		dbgprint("host_or_user: @: %s\n", state->hs_buf);
		state->hs_step = HL_HOST;
		return;

	case ':':
		dbgprint("host_or_user: :: %s\n", state->hs_buf);
		if (lastch(2) == '/') {
			abort_hyperlink(state);
			return;
		}
		state->hs_step = HL_PASSWORD_OR_PORT;
		return;

	case '/':
		dbgprint("host_or_user: /: %s\n", state->hs_buf);
		if (lastch(2) == '/') {
			abort_hyperlink(state);
			return;
		}
		state->hs_step = HL_PATH;
		return;

	case '?':
		dbgprint("host_or_user: ?: %s\n", state->hs_buf);
		state->hs_step = HL_QUERY;
		return;

	case '#':
		dbgprint("host_or_user: #: %s\n", state->hs_buf);
		state->hs_step = HL_ANCHOR;
		return;

	default:
		dbgprint("host_or_user: default: %s\n", state->hs_buf);
		if (!isalnum(curch) && curch != '-')
			abort_hyperlink(state);
	}
}

static void
proceed_hyperlinks_password_or_port(struct hlstate *state) {
	switch (curch) {
	case '@':
		dbgprint("password_or_port: @: %s\n", state->hs_buf);
		state->hs_step = HL_HOST;
		return;

	case '/':
		dbgprint("password_or_port: /: %s\n", state->hs_buf);
		state->hs_step = HL_PATH;
		return;

	default:
		dbgprint("password_or_port: default: %s\n", state->hs_buf);
	}
}

static void
proceed_hyperlinks_host(struct hlstate *state) {
	switch (curch) {
	case ':':
		dbgprint("host: :: %s\n", state->hs_buf);
		state->hs_step = HL_PORT;
		return;

	case '/':
		dbgprint("host: /: %s\n", state->hs_buf);
		state->hs_step = HL_PATH;
		return;

	case '?':
		dbgprint("host: ?: %s\n", state->hs_buf);
		state->hs_step = HL_QUERY;
		return;

	case '#':
		dbgprint("host: #: %s\n", state->hs_buf);
		state->hs_step = HL_ANCHOR;
		return;

	default:
		dbgprint("host: default: %s\n", state->hs_buf);
		if (!isalnum(curch) && curch != '-')
			abort_hyperlink(state);
	}
}

static void
proceed_hyperlinks_port(struct hlstate *state) {
	switch (curch) {
	case '/':
		dbgprint("port: /: %s\n", state->hs_buf);
		state->hs_step = HL_PATH;
		return;

	case '[':
		dbgprint("port: [: %s\n", state->hs_buf);
		if (lastch(2) != ':')
			abort_hyperlink(state);
		else
			state->hs_step = HL_PORT_BRACKETED;
		return;

	default:
		dbgprint("port: default: %s\n", state->hs_buf);
	}

	if (!isdigit(curch))
		finish_hyperlink(pop_last_char(state));
}

static void
proceed_hyperlinks_port_bracketed(struct hlstate *state) {
	switch (curch) {
	case ']':
		dbgprint("port_bracketed: ]: %s\n", state->hs_buf);
		state->hs_step = HL_PATH;
		return;

	default:
		dbgprint("port_bracketed: default: %s\n", state->hs_buf);
	}

	if (!isdigit(curch)) {
		abort_hyperlink(state);
		return;
	}

	if (isspace(curch) || iscntrl(curch))
		finish_hyperlink(pop_last_char(state));
}

static void
proceed_hyperlinks_path(struct hlstate *state) {
	dbgprint("path: step is %s, curch is '%c'\n", hlstate_step_name[state->hs_step], curch);
	switch (curch) {
	case '?':
		dbgprint("path: ?: %s\n", state->hs_buf);
		state->hs_step = HL_QUERY;
		return;

	case '#':
		dbgprint("path: #: %s\n", state->hs_buf);
		state->hs_step = HL_ANCHOR;
		return;

	default:
		dbgprint("path: default: %s\n", state->hs_buf);
	}

	if (isspace(curch) || iscntrl(curch))
		finish_hyperlink(pop_last_char(state));
}

static void
proceed_hyperlinks_query(struct hlstate *state) {
	switch (curch) {
	case '#':
		dbgprint("query: #: %s\n", state->hs_buf);
		state->hs_step = HL_ANCHOR;
		return;

	default:
		dbgprint("query: default: %s\n", state->hs_buf);
	}

	if (isspace(curch) || iscntrl(curch))
		finish_hyperlink(pop_last_char(state));
}

static void
proceed_hyperlinks_anchor(struct hlstate *state) {
	dbgprint("anchor: %s\n", state->hs_buf);
	if (isspace(curch) || iscntrl(curch))
		finish_hyperlink(pop_last_char(state));
}

static void
proceed_hyperlinks_pct(struct hlstate *state) {
	dbgprint("pct: %s\n", state->hs_buf);

	if (curch == '%') {
		state->hs_is_pct = true;
		return;
	}

	if (!isxdigit(curch)) {
		abort_hyperlink(state);
		return;
	}

	if (state->hs_buflen >= 3 && lastch(2) != '%' && lastch(3) == '%') {
		// replace '%' with the decoded character
		lastch(3) = (char)((unsigned int)lastch(2) << 8 | (unsigned int)lastch(1));
		lastch(2) = '\0';
		state->hs_buflen -= 2;
		state->hs_is_pct = false;
	}
}

static void
abort_hyperlink(struct hlstate *state) {
	dbgprint("aborting: %s\n", state->hs_buf);
	*state->hs_buf = '\0';
	state->hs_buflen = 0;
	state->hs_step = HL_INVALID;
}

static void
finish_hyperlink(struct hlstate *state) {
	dbgprint("finishing: %s\n", state->hs_buf);
	state->hs_handler(state->hs_buf);
	*state->hs_buf = '\0';
	state->hs_buflen = 0;
	state->hs_step = HL_INVALID;
}

static bool
is_hyperlink_valid(const struct hlstate *state) {
	if (state->hs_buflen == 0)
		return false;

	switch (state->hs_step) {
	case HL_HOST:
		return isalnum(state->hs_buf[state->hs_buflen-1]);

	case HL_PORT:
		return isdigit(state->hs_buf[state->hs_buflen-1]);

	case HL_PATH:
	case HL_QUERY:
	case HL_ANCHOR:
		return true;

	default:
		return false;
	}
}

struct hlstate*
pop_last_char(struct hlstate *state) {
	state->hs_buf[--state->hs_buflen] = '\0';
	return state;
}

int
proceed_hyperlinks(FILE *f, const char *path, hlhandler handler) {
	struct hlstate	state;
	int		c;

	dbgprint("processing %s...\n", path);
	memset(&state, 0, sizeof(state));
	state.hs_step = HL_SCHEME;
	state.hs_handler = handler;
	while ((c = getc(f)) != EOF) {
		if (state.hs_buflen == sizeof(state.hs_buf) - 1)
			abort_hyperlink(&state);

		state.hs_buf[state.hs_buflen++] = c;
		state.hs_buf[state.hs_buflen] = '\0';

		dbgprint("Proceeding 0x%02x: %s: [%zu]%s\n", c, hlstate_step_name[state.hs_step], state.hs_buflen, state.hs_buf);

		if (c == '%' || state.hs_is_pct)
			proceed_hyperlinks_pct(&state);
		if (!state.hs_is_pct)
			handlers[state.hs_step](&state);
	}
	if (!ferror(f) && is_hyperlink_valid(&state))
		finish_hyperlink(&state);

	return ferror(f);
}
