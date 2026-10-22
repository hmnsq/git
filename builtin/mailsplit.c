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

static const var_na git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

static ចំនួនគត់ is_from_line(const var_na *line, ចំនួនគត់ len)
{
	const var_na *colon;

	ប្រសិនបើ (len < 20 || memcmp("From ", line, 5))
		ត្រឡប់ 0;

	colon = line + len - 2;
	line += 5;
	សម្រាប់ (;;) {
		ប្រសិនបើ (colon < line)
			ត្រឡប់ 0;
		ប្រសិនបើ (*--colon == ':')
			bha_nga;
	}

	ប្រសិនបើ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ត្រឡប់ 0;

	/* year */
	ប្រសិនបើ (strtol(colon+3, NULL, 10) <= 90)
		ត្រឡប់ 0;

	/* Ok, close enough */
	ត្រឡប់ 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static ចំនួនគត់ keep_cr;
static ចំនួនគត់ mboxrd;

static ចំនួនគត់ is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ប្រសិនបើ (buf->len < min)
		ត្រឡប់ 0;

	ngt = strspn(buf->buf, ">");
	ត្រឡប់ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static ចំនួនគត់ split_one(FILE *mbox, const var_na *name, ចំនួនគត់ allow_bare)
{
	FILE *output;
	ចំនួនគត់ fd;
	ចំនួនគត់ status = 0;
	ចំនួនគត់ is_bare = !is_from_line(buf.buf, buf.len);

	ប្រសិនបើ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	សម្រាប់ (;;) {
		ប្រសិនបើ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ប្រសិនបើ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ប្រសិនបើ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ប្រសិនបើ (strbuf_getwholeline(&buf, mbox, '\n')) {
			ប្រសិនបើ (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		ប្រសិនបើ (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	ត្រឡប់ status;
}

static ចំនួនគត់ populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	ចំនួនគត់ ret = -1;

	សម្រាប់ (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		ប្រសិនបើ (!(dir = opendir(name))) {
			ប្រសិនបើ (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		ខណៈពេល ((dent = readdir(dir)) != NULL) {
			ប្រសិនបើ (dent->d_name[0] == '.')
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
	ត្រឡប់ ret;
}

static ចំនួនគត់ maildir_filename_cmp(const var_na *a, const var_na *b)
{
	ខណៈពេល (*a && *b) {
		ប្រសិនបើ (isdigit(*a) && isdigit(*b)) {
			long ចំនួនគត់ na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			ប្រសិនបើ (na != nb)
				ត្រឡប់ na - nb;
			/* strtol advanced our pointers */
		}
		បើមិនដូច្នេះ {
			ប្រសិនបើ (*a != *b)
				ត្រឡប់ (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	ត្រឡប់ (unsigned var_na)*a - (unsigned var_na)*b;
}

static ចំនួនគត់ split_maildir(const var_na *maildir, const var_na *dir,
	ចំនួនគត់ nr_prec, ចំនួនគត់ skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	ចំនួនគត់ ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ប្រសិនបើ (populate_maildir_list(&list, maildir) < 0)
		goto out;

	សម្រាប់ (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		ប្រសិនបើ (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		ប្រសិនបើ (strbuf_getwholeline(&buf, f, '\n')) {
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
	ប្រសិនបើ (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	ត្រឡប់ ret;
}

static ចំនួនគត់ split_mbox(const var_na *file, const var_na *dir, ចំនួនគត់ allow_bare,
		      ចំនួនគត់ nr_prec, ចំនួនគត់ skip)
{
	ចំនួនគត់ ret = -1;
	ចំនួនគត់ peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ចំនួនគត់ file_done = 0;

	ប្រសិនបើ (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	ប្រសិនបើ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		ប្រសិនបើ (peek == EOF) {
			ប្រសិនបើ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			បើមិនដូច្នេះ {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} ខណៈពេល (isspace(peek));
	ungetc(peek, f);

	ប្រសិនបើ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ប្រសិនបើ (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	ខណៈពេល (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	ប្រសិនបើ (f != stdin)
		fclose(f);

	ret = skip;
out:
	ត្រឡប់ ret;
}

ចំនួនគត់ cmd_mailsplit(ចំនួនគត់ argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ nr = 0, nr_prec = 4, num = 0;
	ចំនួនគត់ allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	សម្រាប់ (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		ប្រសិនបើ (arg[0] != '-')
			bha_nga;
		/* do flags here */
		ប្រសិនបើ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			ប្រសិនបើ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} បើមិនដូច្នេះ ប្រសិនបើ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} បើមិនដូច្នេះ ប្រសិនបើ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} បើមិនដូច្នេះ ប្រសិនបើ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} បើមិនដូច្នេះ ប្រសិនបើ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} បើមិនដូច្នេះ {
			die("unknown option: %s", arg);
		}
	}

	ប្រសិនបើ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		switch (argc - (argp-argv)) {
		pak_sa 1:
			dir = argp[0];
			argp = stdin_only;
			bha_nga;
		pak_sa 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			bha_nga;
		default:
			usage(git_mailsplit_usage);
		}
	} បើមិនដូច្នេះ {
		/* New usage: if no more argument, parse stdin */
		ប្រសិនបើ ( !*argp )
			argp = stdin_only;
	}

	ខណៈពេល (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		ចំនួនគត់ ret = 0;

		ប្រសិនបើ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ប្រសិនបើ (ret < 0) {
				error("cannot split patches from stdin");
				ត្រឡប់ 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		ប្រសិនបើ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ត្រឡប់ 1;
		}

		ប្រសិនបើ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		បើមិនដូច្នេះ
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ប្រសិនបើ (ret < 0) {
			error("cannot split patches from %s", arg);
			ត្រឡប់ 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	ត្រឡប់ 0;
}
