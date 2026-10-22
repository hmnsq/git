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

static целое is_from_line(const char *line, целое len)
{
	const char *colon;

	если (len < 20 || memcmp("From ", line, 5))
		вернуть 0;

	colon = line + len - 2;
	line += 5;
	для (;;) {
		если (colon < line)
			вернуть 0;
		если (*--colon == ':')
			break;
	}

	если (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		вернуть 0;

	/* year */
	если (strtol(colon+3, NULL, 10) <= 90)
		вернуть 0;

	/* Ok, close enough */
	вернуть 1;
}

static struct strbuf buf = STRBUF_INIT;
static целое keep_cr;
static целое mboxrd;

static целое is_gtfrom(const struct strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	если (buf->len < min)
		вернуть 0;

	ngt = strspn(buf->buf, ">");
	вернуть ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static целое split_one(FILE *mbox, const char *name, целое allow_bare)
{
	FILE *output;
	целое fd;
	целое status = 0;
	целое is_bare = !is_from_line(buf.buf, buf.len);

	если (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	для (;;) {
		если (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		если (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		если (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		если (strbuf_getwholeline(&buf, mbox, '\n')) {
			если (feof(mbox)) {
				status = 1;
				break;
			}
			die_errno("cannot read mbox");
		}
		если (!is_bare && is_from_line(buf.buf, buf.len))
			break; /* done with one message */
	}
	fclose(output);
	вернуть status;
}

static целое populate_maildir_list(struct string_list *list, const char *path)
{
	DIR *dir;
	struct dirent *dent;
	char *name = NULL;
	const char *subs[] = { "cur", "new", NULL };
	const char **sub;
	целое ret = -1;

	для (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		если (!(dir = opendir(name))) {
			если (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		пока ((dent = readdir(dir)) != NULL) {
			если (dent->d_name[0] == '.')
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
	вернуть ret;
}

static целое maildir_filename_cmp(const char *a, const char *b)
{
	пока (*a && *b) {
		если (isdigit(*a) && isdigit(*b)) {
			long целое na, nb;
			na = strtol(a, (char **)&a, 10);
			nb = strtol(b, (char **)&b, 10);
			если (na != nb)
				вернуть na - nb;
			/* strtol advanced our pointers */
		}
		иначе {
			если (*a != *b)
				вернуть (unsigned char)*a - (unsigned char)*b;
			a++;
			b++;
		}
	}
	вернуть (unsigned char)*a - (unsigned char)*b;
}

static целое split_maildir(const char *maildir, const char *dir,
	целое nr_prec, целое skip)
{
	char *file = NULL;
	FILE *f = NULL;
	целое ret = -1;
	struct string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	если (populate_maildir_list(&list, maildir) < 0)
		goto out;

	для (size_t i = 0; i < list.nr; i++) {
		char *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		если (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		если (strbuf_getwholeline(&buf, f, '\n')) {
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
	если (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	вернуть ret;
}

static целое split_mbox(const char *file, const char *dir, целое allow_bare,
		      целое nr_prec, целое skip)
{
	целое ret = -1;
	целое peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	целое file_done = 0;

	если (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	если (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		если (peek == EOF) {
			если (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			иначе {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} пока (isspace(peek));
	ungetc(peek, f);

	если (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		если (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	пока (!file_done) {
		char *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	если (f != stdin)
		fclose(f);

	ret = skip;
out:
	вернуть ret;
}

целое cmd_mailsplit(целое argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	целое nr = 0, nr_prec = 4, num = 0;
	целое allow_bare = 0;
	const char *dir = NULL;
	const char **argp;
	static const char *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	для (argp = argv+1; *argp; argp++) {
		const char *arg = *argp;

		если (arg[0] != '-')
			break;
		/* do flags here */
		если ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			если (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} иначе если ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} иначе если ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} иначе если (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} иначе если ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} иначе если (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} иначе если ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			break;
		} иначе {
			die("unknown option: %s", arg);
		}
	}

	если ( !dir ) {
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
	} иначе {
		/* New usage: if no more argument, parse stdin */
		если ( !*argp )
			argp = stdin_only;
	}

	пока (*argp) {
		const char *arg = *argp++;
		struct stat argstat;
		целое ret = 0;

		если (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			если (ret < 0) {
				error("cannot split patches from stdin");
				вернуть 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		если (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			вернуть 1;
		}

		если (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		иначе
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		если (ret < 0) {
			error("cannot split patches from %s", arg);
			вернуть 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	вернуть 0;
}
