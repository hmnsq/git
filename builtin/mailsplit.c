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

static 整数 is_from_line(const char *line, 整数 len)
{
	const char *colon;

	如果 (len < 20 || memcmp("From ", line, 5))
		返回 0;

	colon = line + len - 2;
	line += 5;
	为 (;;) {
		如果 (colon < line)
			返回 0;
		如果 (*--colon == ':')
			break;
	}

	如果 (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		返回 0;

	/* year */
	如果 (strtol(colon+3, NULL, 10) <= 90)
		返回 0;

	/* Ok, close enough */
	返回 1;
}

static struct strbuf buf = STRBUF_INIT;
static 整数 keep_cr;
static 整数 mboxrd;

static 整数 is_gtfrom(const struct strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	如果 (buf->len < min)
		返回 0;

	ngt = strspn(buf->buf, ">");
	返回 ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static 整数 split_one(FILE *mbox, const char *name, 整数 allow_bare)
{
	FILE *output;
	整数 fd;
	整数 status = 0;
	整数 is_bare = !is_from_line(buf.buf, buf.len);

	如果 (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	为 (;;) {
		如果 (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		如果 (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		如果 (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		如果 (strbuf_getwholeline(&buf, mbox, '\n')) {
			如果 (feof(mbox)) {
				status = 1;
				break;
			}
			die_errno("cannot read mbox");
		}
		如果 (!is_bare && is_from_line(buf.buf, buf.len))
			break; /* done with one message */
	}
	fclose(output);
	返回 status;
}

static 整数 populate_maildir_list(struct string_list *list, const char *path)
{
	DIR *dir;
	struct dirent *dent;
	char *name = NULL;
	const char *subs[] = { "cur", "new", NULL };
	const char **sub;
	整数 ret = -1;

	为 (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		如果 (!(dir = opendir(name))) {
			如果 (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		当 ((dent = readdir(dir)) != NULL) {
			如果 (dent->d_name[0] == '.')
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
	返回 ret;
}

static 整数 maildir_filename_cmp(const char *a, const char *b)
{
	当 (*a && *b) {
		如果 (isdigit(*a) && isdigit(*b)) {
			long 整数 na, nb;
			na = strtol(a, (char **)&a, 10);
			nb = strtol(b, (char **)&b, 10);
			如果 (na != nb)
				返回 na - nb;
			/* strtol advanced our pointers */
		}
		否则 {
			如果 (*a != *b)
				返回 (unsigned char)*a - (unsigned char)*b;
			a++;
			b++;
		}
	}
	返回 (unsigned char)*a - (unsigned char)*b;
}

static 整数 split_maildir(const char *maildir, const char *dir,
	整数 nr_prec, 整数 skip)
{
	char *file = NULL;
	FILE *f = NULL;
	整数 ret = -1;
	struct string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	如果 (populate_maildir_list(&list, maildir) < 0)
		goto out;

	为 (size_t i = 0; i < list.nr; i++) {
		char *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		如果 (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		如果 (strbuf_getwholeline(&buf, f, '\n')) {
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
	如果 (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	返回 ret;
}

static 整数 split_mbox(const char *file, const char *dir, 整数 allow_bare,
		      整数 nr_prec, 整数 skip)
{
	整数 ret = -1;
	整数 peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	整数 file_done = 0;

	如果 (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	如果 (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		如果 (peek == EOF) {
			如果 (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			否则 {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} 当 (isspace(peek));
	ungetc(peek, f);

	如果 (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		如果 (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	当 (!file_done) {
		char *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	如果 (f != stdin)
		fclose(f);

	ret = skip;
out:
	返回 ret;
}

整数 cmd_mailsplit(整数 argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	整数 nr = 0, nr_prec = 4, num = 0;
	整数 allow_bare = 0;
	const char *dir = NULL;
	const char **argp;
	static const char *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	为 (argp = argv+1; *argp; argp++) {
		const char *arg = *argp;

		如果 (arg[0] != '-')
			break;
		/* do flags here */
		如果 ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			如果 (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} 否则 如果 ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} 否则 如果 ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} 否则 如果 (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} 否则 如果 ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} 否则 如果 (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} 否则 如果 ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			break;
		} 否则 {
			die("unknown option: %s", arg);
		}
	}

	如果 ( !dir ) {
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
	} 否则 {
		/* New usage: if no more argument, parse stdin */
		如果 ( !*argp )
			argp = stdin_only;
	}

	当 (*argp) {
		const char *arg = *argp++;
		struct stat argstat;
		整数 ret = 0;

		如果 (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			如果 (ret < 0) {
				error("cannot split patches from stdin");
				返回 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		如果 (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			返回 1;
		}

		如果 (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		否则
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		如果 (ret < 0) {
			error("cannot split patches from %s", arg);
			返回 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	返回 0;
}
