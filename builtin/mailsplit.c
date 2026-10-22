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

static ціле is_from_line(const char *line, ціле len)
{
	const char *colon;

	якщо (len < 20 || memcmp("From ", line, 5))
		повернути 0;

	colon = line + len - 2;
	line += 5;
	для (;;) {
		якщо (colon < line)
			повернути 0;
		якщо (*--colon == ':')
			break;
	}

	якщо (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		повернути 0;

	/* year */
	якщо (strtol(colon+3, NULL, 10) <= 90)
		повернути 0;

	/* Ok, close enough */
	повернути 1;
}

static struct strbuf buf = STRBUF_INIT;
static ціле keep_cr;
static ціле mboxrd;

static ціле is_gtfrom(const struct strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	якщо (buf->len < min)
		повернути 0;

	ngt = strspn(buf->buf, ">");
	повернути ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static ціле split_one(FILE *mbox, const char *name, ціле allow_bare)
{
	FILE *output;
	ціле fd;
	ціле status = 0;
	ціле is_bare = !is_from_line(buf.buf, buf.len);

	якщо (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	для (;;) {
		якщо (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		якщо (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		якщо (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		якщо (strbuf_getwholeline(&buf, mbox, '\n')) {
			якщо (feof(mbox)) {
				status = 1;
				break;
			}
			die_errno("cannot read mbox");
		}
		якщо (!is_bare && is_from_line(buf.buf, buf.len))
			break; /* done with one message */
	}
	fclose(output);
	повернути status;
}

static ціле populate_maildir_list(struct string_list *list, const char *path)
{
	DIR *dir;
	struct dirent *dent;
	char *name = NULL;
	const char *subs[] = { "cur", "new", NULL };
	const char **sub;
	ціле ret = -1;

	для (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		якщо (!(dir = opendir(name))) {
			якщо (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		поки ((dent = readdir(dir)) != NULL) {
			якщо (dent->d_name[0] == '.')
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
	повернути ret;
}

static ціле maildir_filename_cmp(const char *a, const char *b)
{
	поки (*a && *b) {
		якщо (isdigit(*a) && isdigit(*b)) {
			long ціле na, nb;
			na = strtol(a, (char **)&a, 10);
			nb = strtol(b, (char **)&b, 10);
			якщо (na != nb)
				повернути na - nb;
			/* strtol advanced our pointers */
		}
		інакше {
			якщо (*a != *b)
				повернути (unsigned char)*a - (unsigned char)*b;
			a++;
			b++;
		}
	}
	повернути (unsigned char)*a - (unsigned char)*b;
}

static ціле split_maildir(const char *maildir, const char *dir,
	ціле nr_prec, ціле skip)
{
	char *file = NULL;
	FILE *f = NULL;
	ціле ret = -1;
	struct string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	якщо (populate_maildir_list(&list, maildir) < 0)
		goto out;

	для (size_t i = 0; i < list.nr; i++) {
		char *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		якщо (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		якщо (strbuf_getwholeline(&buf, f, '\n')) {
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
	якщо (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	повернути ret;
}

static ціле split_mbox(const char *file, const char *dir, ціле allow_bare,
		      ціле nr_prec, ціле skip)
{
	ціле ret = -1;
	ціле peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ціле file_done = 0;

	якщо (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	якщо (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		якщо (peek == EOF) {
			якщо (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			інакше {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} поки (isspace(peek));
	ungetc(peek, f);

	якщо (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		якщо (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	поки (!file_done) {
		char *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	якщо (f != stdin)
		fclose(f);

	ret = skip;
out:
	повернути ret;
}

ціле cmd_mailsplit(ціле argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	ціле nr = 0, nr_prec = 4, num = 0;
	ціле allow_bare = 0;
	const char *dir = NULL;
	const char **argp;
	static const char *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	для (argp = argv+1; *argp; argp++) {
		const char *arg = *argp;

		якщо (arg[0] != '-')
			break;
		/* do flags here */
		якщо ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			якщо (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} інакше якщо ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} інакше якщо ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} інакше якщо (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} інакше якщо ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} інакше якщо (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} інакше якщо ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			break;
		} інакше {
			die("unknown option: %s", arg);
		}
	}

	якщо ( !dir ) {
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
	} інакше {
		/* New usage: if no more argument, parse stdin */
		якщо ( !*argp )
			argp = stdin_only;
	}

	поки (*argp) {
		const char *arg = *argp++;
		struct stat argstat;
		ціле ret = 0;

		якщо (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			якщо (ret < 0) {
				error("cannot split patches from stdin");
				повернути 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		якщо (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			повернути 1;
		}

		якщо (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		інакше
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		якщо (ret < 0) {
			error("cannot split patches from %s", arg);
			повернути 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	повернути 0;
}
