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

static số_nguyên is_from_line(const char *line, số_nguyên len)
{
	const char *colon;

	nếu (len < 20 || memcmp("From ", line, 5))
		trả_về 0;

	colon = line + len - 2;
	line += 5;
	cho (;;) {
		nếu (colon < line)
			trả_về 0;
		nếu (*--colon == ':')
			break;
	}

	nếu (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		trả_về 0;

	/* year */
	nếu (strtol(colon+3, NULL, 10) <= 90)
		trả_về 0;

	/* Ok, close enough */
	trả_về 1;
}

static struct strbuf buf = STRBUF_INIT;
static số_nguyên keep_cr;
static số_nguyên mboxrd;

static số_nguyên is_gtfrom(const struct strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	nếu (buf->len < min)
		trả_về 0;

	ngt = strspn(buf->buf, ">");
	trả_về ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static số_nguyên split_one(FILE *mbox, const char *name, số_nguyên allow_bare)
{
	FILE *output;
	số_nguyên fd;
	số_nguyên status = 0;
	số_nguyên is_bare = !is_from_line(buf.buf, buf.len);

	nếu (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	cho (;;) {
		nếu (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		nếu (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		nếu (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		nếu (strbuf_getwholeline(&buf, mbox, '\n')) {
			nếu (feof(mbox)) {
				status = 1;
				break;
			}
			die_errno("cannot read mbox");
		}
		nếu (!is_bare && is_from_line(buf.buf, buf.len))
			break; /* done with one message */
	}
	fclose(output);
	trả_về status;
}

static số_nguyên populate_maildir_list(struct string_list *list, const char *path)
{
	DIR *dir;
	struct dirent *dent;
	char *name = NULL;
	const char *subs[] = { "cur", "new", NULL };
	const char **sub;
	số_nguyên ret = -1;

	cho (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		nếu (!(dir = opendir(name))) {
			nếu (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		trong_khi ((dent = readdir(dir)) != NULL) {
			nếu (dent->d_name[0] == '.')
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
	trả_về ret;
}

static số_nguyên maildir_filename_cmp(const char *a, const char *b)
{
	trong_khi (*a && *b) {
		nếu (isdigit(*a) && isdigit(*b)) {
			long số_nguyên na, nb;
			na = strtol(a, (char **)&a, 10);
			nb = strtol(b, (char **)&b, 10);
			nếu (na != nb)
				trả_về na - nb;
			/* strtol advanced our pointers */
		}
		không_thì {
			nếu (*a != *b)
				trả_về (unsigned char)*a - (unsigned char)*b;
			a++;
			b++;
		}
	}
	trả_về (unsigned char)*a - (unsigned char)*b;
}

static số_nguyên split_maildir(const char *maildir, const char *dir,
	số_nguyên nr_prec, số_nguyên skip)
{
	char *file = NULL;
	FILE *f = NULL;
	số_nguyên ret = -1;
	struct string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	nếu (populate_maildir_list(&list, maildir) < 0)
		goto out;

	cho (size_t i = 0; i < list.nr; i++) {
		char *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		nếu (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		nếu (strbuf_getwholeline(&buf, f, '\n')) {
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
	nếu (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	trả_về ret;
}

static số_nguyên split_mbox(const char *file, const char *dir, số_nguyên allow_bare,
		      số_nguyên nr_prec, số_nguyên skip)
{
	số_nguyên ret = -1;
	số_nguyên peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	số_nguyên file_done = 0;

	nếu (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	nếu (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		nếu (peek == EOF) {
			nếu (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			không_thì {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} trong_khi (isspace(peek));
	ungetc(peek, f);

	nếu (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		nếu (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	trong_khi (!file_done) {
		char *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	nếu (f != stdin)
		fclose(f);

	ret = skip;
out:
	trả_về ret;
}

số_nguyên cmd_mailsplit(số_nguyên argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	số_nguyên nr = 0, nr_prec = 4, num = 0;
	số_nguyên allow_bare = 0;
	const char *dir = NULL;
	const char **argp;
	static const char *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	cho (argp = argv+1; *argp; argp++) {
		const char *arg = *argp;

		nếu (arg[0] != '-')
			break;
		/* do flags here */
		nếu ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			nếu (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} không_thì nếu ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} không_thì nếu ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} không_thì nếu (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} không_thì nếu ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} không_thì nếu (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} không_thì nếu ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			break;
		} không_thì {
			die("unknown option: %s", arg);
		}
	}

	nếu ( !dir ) {
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
	} không_thì {
		/* New usage: if no more argument, parse stdin */
		nếu ( !*argp )
			argp = stdin_only;
	}

	trong_khi (*argp) {
		const char *arg = *argp++;
		struct stat argstat;
		số_nguyên ret = 0;

		nếu (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			nếu (ret < 0) {
				error("cannot split patches from stdin");
				trả_về 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		nếu (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			trả_về 1;
		}

		nếu (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		không_thì
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		nếu (ret < 0) {
			error("cannot split patches from %s", arg);
			trả_về 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	trả_về 0;
}
