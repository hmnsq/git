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

static ཧྲིལ is_from_line(const var_na *line, ཧྲིལ len)
{
	const var_na *colon;

	གལ་ཏེ (len < 20 || memcmp("From ", line, 5))
		ཕྱིར 0;

	colon = line + len - 2;
	line += 5;
	ཆེད་དུ (;;) {
		གལ་ཏེ (colon < line)
			ཕྱིར 0;
		གལ་ཏེ (*--colon == ':')
			bha_nga;
	}

	གལ་ཏེ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ཕྱིར 0;

	/* year */
	གལ་ཏེ (strtol(colon+3, NULL, 10) <= 90)
		ཕྱིར 0;

	/* Ok, close enough */
	ཕྱིར 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static ཧྲིལ keep_cr;
static ཧྲིལ mboxrd;

static ཧྲིལ is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	གལ་ཏེ (buf->len < min)
		ཕྱིར 0;

	ngt = strspn(buf->buf, ">");
	ཕྱིར ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static ཧྲིལ split_one(FILE *mbox, const var_na *name, ཧྲིལ allow_bare)
{
	FILE *output;
	ཧྲིལ fd;
	ཧྲིལ status = 0;
	ཧྲིལ is_bare = !is_from_line(buf.buf, buf.len);

	གལ་ཏེ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	ཆེད་དུ (;;) {
		གལ་ཏེ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		གལ་ཏེ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		གལ་ཏེ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		གལ་ཏེ (strbuf_getwholeline(&buf, mbox, '\n')) {
			གལ་ཏེ (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		གལ་ཏེ (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	ཕྱིར status;
}

static ཧྲིལ populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	ཧྲིལ ret = -1;

	ཆེད་དུ (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		གལ་ཏེ (!(dir = opendir(name))) {
			གལ་ཏེ (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		སྐབས ((dent = readdir(dir)) != NULL) {
			གལ་ཏེ (dent->d_name[0] == '.')
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
	ཕྱིར ret;
}

static ཧྲིལ maildir_filename_cmp(const var_na *a, const var_na *b)
{
	སྐབས (*a && *b) {
		གལ་ཏེ (isdigit(*a) && isdigit(*b)) {
			long ཧྲིལ na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			གལ་ཏེ (na != nb)
				ཕྱིར na - nb;
			/* strtol advanced our pointers */
		}
		དེ་མིན {
			གལ་ཏེ (*a != *b)
				ཕྱིར (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	ཕྱིར (unsigned var_na)*a - (unsigned var_na)*b;
}

static ཧྲིལ split_maildir(const var_na *maildir, const var_na *dir,
	ཧྲིལ nr_prec, ཧྲིལ skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	ཧྲིལ ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	གལ་ཏེ (populate_maildir_list(&list, maildir) < 0)
		goto out;

	ཆེད་དུ (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		གལ་ཏེ (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		གལ་ཏེ (strbuf_getwholeline(&buf, f, '\n')) {
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
	གལ་ཏེ (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	ཕྱིར ret;
}

static ཧྲིལ split_mbox(const var_na *file, const var_na *dir, ཧྲིལ allow_bare,
		      ཧྲིལ nr_prec, ཧྲིལ skip)
{
	ཧྲིལ ret = -1;
	ཧྲིལ peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ཧྲིལ file_done = 0;

	གལ་ཏེ (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	གལ་ཏེ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		གལ་ཏེ (peek == EOF) {
			གལ་ཏེ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			དེ་མིན {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} སྐབས (isspace(peek));
	ungetc(peek, f);

	གལ་ཏེ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		གལ་ཏེ (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	སྐབས (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	གལ་ཏེ (f != stdin)
		fclose(f);

	ret = skip;
out:
	ཕྱིར ret;
}

ཧྲིལ cmd_mailsplit(ཧྲིལ argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ nr = 0, nr_prec = 4, num = 0;
	ཧྲིལ allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	ཆེད་དུ (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		གལ་ཏེ (arg[0] != '-')
			bha_nga;
		/* do flags here */
		གལ་ཏེ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			གལ་ཏེ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} དེ་མིན གལ་ཏེ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} དེ་མིན གལ་ཏེ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} དེ་མིན གལ་ཏེ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} དེ་མིན གལ་ཏེ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} དེ་མིན གལ་ཏེ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} དེ་མིན གལ་ཏེ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} དེ་མིན {
			die("unknown option: %s", arg);
		}
	}

	གལ་ཏེ ( !dir ) {
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
	} དེ་མིན {
		/* New usage: if no more argument, parse stdin */
		གལ་ཏེ ( !*argp )
			argp = stdin_only;
	}

	སྐབས (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		ཧྲིལ ret = 0;

		གལ་ཏེ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			གལ་ཏེ (ret < 0) {
				error("cannot split patches from stdin");
				ཕྱིར 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		གལ་ཏེ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ཕྱིར 1;
		}

		གལ་ཏེ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		དེ་མིན
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		གལ་ཏེ (ret < 0) {
			error("cannot split patches from %s", arg);
			ཕྱིར 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	ཕྱིར 0;
}
