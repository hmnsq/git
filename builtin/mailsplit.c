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

static 정수 is_from_line(const char *line, 정수 len)
{
	const char *colon;

	만약 (len < 20 || memcmp("From ", line, 5))
		반환 0;

	colon = line + len - 2;
	line += 5;
	위해 (;;) {
		만약 (colon < line)
			반환 0;
		만약 (*--colon == ':')
			break;
	}

	만약 (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		반환 0;

	/* year */
	만약 (strtol(colon+3, NULL, 10) <= 90)
		반환 0;

	/* Ok, close enough */
	반환 1;
}

static struct strbuf buf = STRBUF_INIT;
static 정수 keep_cr;
static 정수 mboxrd;

static 정수 is_gtfrom(const struct strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	만약 (buf->len < min)
		반환 0;

	ngt = strspn(buf->buf, ">");
	반환 ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static 정수 split_one(FILE *mbox, const char *name, 정수 allow_bare)
{
	FILE *output;
	정수 fd;
	정수 status = 0;
	정수 is_bare = !is_from_line(buf.buf, buf.len);

	만약 (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	위해 (;;) {
		만약 (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		만약 (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		만약 (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		만약 (strbuf_getwholeline(&buf, mbox, '\n')) {
			만약 (feof(mbox)) {
				status = 1;
				break;
			}
			die_errno("cannot read mbox");
		}
		만약 (!is_bare && is_from_line(buf.buf, buf.len))
			break; /* done with one message */
	}
	fclose(output);
	반환 status;
}

static 정수 populate_maildir_list(struct string_list *list, const char *path)
{
	DIR *dir;
	struct dirent *dent;
	char *name = NULL;
	const char *subs[] = { "cur", "new", NULL };
	const char **sub;
	정수 ret = -1;

	위해 (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		만약 (!(dir = opendir(name))) {
			만약 (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		동안 ((dent = readdir(dir)) != NULL) {
			만약 (dent->d_name[0] == '.')
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
	반환 ret;
}

static 정수 maildir_filename_cmp(const char *a, const char *b)
{
	동안 (*a && *b) {
		만약 (isdigit(*a) && isdigit(*b)) {
			long 정수 na, nb;
			na = strtol(a, (char **)&a, 10);
			nb = strtol(b, (char **)&b, 10);
			만약 (na != nb)
				반환 na - nb;
			/* strtol advanced our pointers */
		}
		아니면 {
			만약 (*a != *b)
				반환 (unsigned char)*a - (unsigned char)*b;
			a++;
			b++;
		}
	}
	반환 (unsigned char)*a - (unsigned char)*b;
}

static 정수 split_maildir(const char *maildir, const char *dir,
	정수 nr_prec, 정수 skip)
{
	char *file = NULL;
	FILE *f = NULL;
	정수 ret = -1;
	struct string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	만약 (populate_maildir_list(&list, maildir) < 0)
		goto out;

	위해 (size_t i = 0; i < list.nr; i++) {
		char *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		만약 (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		만약 (strbuf_getwholeline(&buf, f, '\n')) {
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
	만약 (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	반환 ret;
}

static 정수 split_mbox(const char *file, const char *dir, 정수 allow_bare,
		      정수 nr_prec, 정수 skip)
{
	정수 ret = -1;
	정수 peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	정수 file_done = 0;

	만약 (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	만약 (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		만약 (peek == EOF) {
			만약 (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			아니면 {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} 동안 (isspace(peek));
	ungetc(peek, f);

	만약 (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		만약 (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	동안 (!file_done) {
		char *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	만약 (f != stdin)
		fclose(f);

	ret = skip;
out:
	반환 ret;
}

정수 cmd_mailsplit(정수 argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	정수 nr = 0, nr_prec = 4, num = 0;
	정수 allow_bare = 0;
	const char *dir = NULL;
	const char **argp;
	static const char *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	위해 (argp = argv+1; *argp; argp++) {
		const char *arg = *argp;

		만약 (arg[0] != '-')
			break;
		/* do flags here */
		만약 ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			만약 (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} 아니면 만약 ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} 아니면 만약 ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} 아니면 만약 (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} 아니면 만약 ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} 아니면 만약 (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} 아니면 만약 ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			break;
		} 아니면 {
			die("unknown option: %s", arg);
		}
	}

	만약 ( !dir ) {
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
	} 아니면 {
		/* New usage: if no more argument, parse stdin */
		만약 ( !*argp )
			argp = stdin_only;
	}

	동안 (*argp) {
		const char *arg = *argp++;
		struct stat argstat;
		정수 ret = 0;

		만약 (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			만약 (ret < 0) {
				error("cannot split patches from stdin");
				반환 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		만약 (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			반환 1;
		}

		만약 (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		아니면
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		만약 (ret < 0) {
			error("cannot split patches from %s", arg);
			반환 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	반환 0;
}
