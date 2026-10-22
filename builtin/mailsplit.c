/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "gettext.h"
#include "string-list.h"
#include "strbuf.h"

static const char git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

static цео is_from_line(const char *line, цео len)
{
	const char *colon;

	ако (len < 20 || memcmp("From ", line, 5))
		врати 0;

	colon = line + len - 2;
	line += 5;
	за (;;) {
		ако (colon < line)
			врати 0;
		ако (*--colon == ':')
			break;
	}

	ако (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		врати 0;

	/* year */
	ако (strtol(colon+3, NULL, 10) <= 90)
		врати 0;

	/* Ok, close enough */
	врати 1;
}

static struct strbuf buf = STRBUF_INIT;
static цео keep_cr;
static цео mboxrd;

static цео is_gtfrom(const struct strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ако (buf->len < min)
		врати 0;

	ngt = strspn(buf->buf, ">");
	врати ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static цео split_one(FILE *mbox, const char *name, цео allow_bare)
{
	FILE *output;
	цео fd;
	цео status = 0;
	цео is_bare = !is_from_line(buf.buf, buf.len);

	ако (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	за (;;) {
		ако (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ако (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ако (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ако (strbuf_getwholeline(&buf, mbox, '\n')) {
			ако (feof(mbox)) {
				status = 1;
				break;
			}
			die_errno("cannot read mbox");
		}
		ако (!is_bare && is_from_line(buf.buf, buf.len))
			break; /* done with one message */
	}
	fclose(output);
	врати status;
}

static цео populate_maildir_list(struct string_list *list, const char *path)
{
	DIR *dir;
	struct dirent *dent;
	char *name = NULL;
	const char *subs[] = { "cur", "new", NULL };
	const char **sub;
	цео ret = -1;

	за (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		ако (!(dir = opendir(name))) {
			ако (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		док ((dent = readdir(dir)) != NULL) {
			ако (dent->d_name[0] == '.')
				continue;
			free(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_insert(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	free(name);
	врати ret;
}

static цео maildir_filename_cmp(const char *a, const char *b)
{
	док (*a && *b) {
		ако (isdigit(*a) && isdigit(*b)) {
			long цео na, nb;
			na = strtol(a, (char **)&a, 10);
			nb = strtol(b, (char **)&b, 10);
			ако (na != nb)
				врати na - nb;
			/* strtol advanced our pointers */
		}
		инако {
			ако (*a != *b)
				врати (unsigned char)*a - (unsigned char)*b;
			a++;
			b++;
		}
	}
	врати (unsigned char)*a - (unsigned char)*b;
}

static цео split_maildir(const char *maildir, const char *dir,
	цео nr_prec, цео skip)
{
	char *file = NULL;
	FILE *f = NULL;
	цео ret = -1;
	struct string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ако (populate_maildir_list(&list, maildir) < 0)
		goto out;

	за (size_t i = 0; i < list.nr; i++) {
		char *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		ако (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		ако (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			goto out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		free(name);

		fclose(f);
		f = NULL;
	}

	ret = skip;
out:
	ако (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	врати ret;
}

static цео split_mbox(const char *file, const char *dir, цео allow_bare,
		      цео nr_prec, цео skip)
{
	цео ret = -1;
	цео peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	цео file_done = 0;

	ако (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	ако (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		ако (peek == EOF) {
			ако (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			инако {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} док (isspace(peek));
	ungetc(peek, f);

	ако (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ако (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	док (!file_done) {
		char *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	ако (f != stdin)
		fclose(f);

	ret = skip;
out:
	врати ret;
}

цео cmd_mailsplit(цео argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	цео nr = 0, nr_prec = 4, num = 0;
	цео allow_bare = 0;
	const char *dir = NULL;
	const char **argp;
	static const char *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	за (argp = argv+1; *argp; argp++) {
		const char *arg = *argp;

		ако (arg[0] != '-')
			break;
		/* do flags here */
		ако ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			ако (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} инако ако ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} инако ако ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} инако ако (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} инако ако ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} инако ако (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} инако ако ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			break;
		} инако {
			die("unknown option: %s", arg);
		}
	}

	ако ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		switch (argc - (argp-argv)) {
		case 1:
			dir = argp[0];
			argp = stdin_only;
			break;
		case 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			break;
		default:
			usage(git_mailsplit_usage);
		}
	} инако {
		/* New usage: if no more argument, parse stdin */
		ако ( !*argp )
			argp = stdin_only;
	}

	док (*argp) {
		const char *arg = *argp++;
		struct stat argstat;
		цео ret = 0;

		ако (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ако (ret < 0) {
				error("cannot split patches from stdin");
				врати 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		ако (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			врати 1;
		}

		ако (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		инако
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ако (ret < 0) {
			error("cannot split patches from %s", arg);
			врати 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	врати 0;
}
