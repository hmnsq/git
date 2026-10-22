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

static puur_naa_nka is_from_line(const var_na *line, puur_naa_nka len)
{
	const var_na *colon;

	ᱡᱩᱫᱤ (len < 20 || memcmp("From ", line, 5))
		ᱨᱩᱣᱟᱹᱲ 0;

	colon = line + len - 2;
	line += 5;
	for (;;) {
		ᱡᱩᱫᱤ (colon < line)
			ᱨᱩᱣᱟᱹᱲ 0;
		ᱡᱩᱫᱤ (*--colon == ':')
			bha_nga;
	}

	ᱡᱩᱫᱤ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ᱨᱩᱣᱟᱹᱲ 0;

	/* year */
	ᱡᱩᱫᱤ (strtol(colon+3, NULL, 10) <= 90)
		ᱨᱩᱣᱟᱹᱲ 0;

	/* Ok, close enough */
	ᱨᱩᱣᱟᱹᱲ 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static puur_naa_nka keep_cr;
static puur_naa_nka mboxrd;

static puur_naa_nka is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ᱡᱩᱫᱤ (buf->len < min)
		ᱨᱩᱣᱟᱹᱲ 0;

	ngt = strspn(buf->buf, ">");
	ᱨᱩᱣᱟᱹᱲ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static puur_naa_nka split_one(FILE *mbox, const var_na *name, puur_naa_nka allow_bare)
{
	FILE *output;
	puur_naa_nka fd;
	puur_naa_nka status = 0;
	puur_naa_nka is_bare = !is_from_line(buf.buf, buf.len);

	ᱡᱩᱫᱤ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	for (;;) {
		ᱡᱩᱫᱤ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ᱡᱩᱫᱤ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ᱡᱩᱫᱤ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ᱡᱩᱫᱤ (strbuf_getwholeline(&buf, mbox, '\n')) {
			ᱡᱩᱫᱤ (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		ᱡᱩᱫᱤ (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	ᱨᱩᱣᱟᱹᱲ status;
}

static puur_naa_nka populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	puur_naa_nka ret = -1;

	for (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		ᱡᱩᱫᱤ (!(dir = opendir(name))) {
			ᱡᱩᱫᱤ (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		ᱡᱚᱠᱷᱚᱱ ((dent = readdir(dir)) != NULL) {
			ᱡᱩᱫᱤ (dent->d_name[0] == '.')
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
	ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka maildir_filename_cmp(const var_na *a, const var_na *b)
{
	ᱡᱚᱠᱷᱚᱱ (*a && *b) {
		ᱡᱩᱫᱤ (isdigit(*a) && isdigit(*b)) {
			long puur_naa_nka na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			ᱡᱩᱫᱤ (na != nb)
				ᱨᱩᱣᱟᱹᱲ na - nb;
			/* strtol advanced our pointers */
		}
		ᱵᱟᱝ {
			ᱡᱩᱫᱤ (*a != *b)
				ᱨᱩᱣᱟᱹᱲ (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	ᱨᱩᱣᱟᱹᱲ (unsigned var_na)*a - (unsigned var_na)*b;
}

static puur_naa_nka split_maildir(const var_na *maildir, const var_na *dir,
	puur_naa_nka nr_prec, puur_naa_nka skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	puur_naa_nka ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ᱡᱩᱫᱤ (populate_maildir_list(&list, maildir) < 0)
		goto out;

	for (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		ᱡᱩᱫᱤ (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		ᱡᱩᱫᱤ (strbuf_getwholeline(&buf, f, '\n')) {
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
	ᱡᱩᱫᱤ (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka split_mbox(const var_na *file, const var_na *dir, puur_naa_nka allow_bare,
		      puur_naa_nka nr_prec, puur_naa_nka skip)
{
	puur_naa_nka ret = -1;
	puur_naa_nka peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	puur_naa_nka file_done = 0;

	ᱡᱩᱫᱤ (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	ᱡᱩᱫᱤ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		ᱡᱩᱫᱤ (peek == EOF) {
			ᱡᱩᱫᱤ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			ᱵᱟᱝ {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} ᱡᱚᱠᱷᱚᱱ (isspace(peek));
	ungetc(peek, f);

	ᱡᱩᱫᱤ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ᱡᱩᱫᱤ (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	ᱡᱚᱠᱷᱚᱱ (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	ᱡᱩᱫᱤ (f != stdin)
		fclose(f);

	ret = skip;
out:
	ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka cmd_mailsplit(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka nr = 0, nr_prec = 4, num = 0;
	puur_naa_nka allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	for (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		ᱡᱩᱫᱤ (arg[0] != '-')
			bha_nga;
		/* do flags here */
		ᱡᱩᱫᱤ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			ᱡᱩᱫᱤ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} ᱵᱟᱝ ᱡᱩᱫᱤ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} ᱵᱟᱝ ᱡᱩᱫᱤ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} ᱵᱟᱝ ᱡᱩᱫᱤ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} ᱵᱟᱝ ᱡᱩᱫᱤ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} ᱵᱟᱝ {
			die("unknown option: %s", arg);
		}
	}

	ᱡᱩᱫᱤ ( !dir ) {
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
	} ᱵᱟᱝ {
		/* New usage: if no more argument, parse stdin */
		ᱡᱩᱫᱤ ( !*argp )
			argp = stdin_only;
	}

	ᱡᱚᱠᱷᱚᱱ (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		puur_naa_nka ret = 0;

		ᱡᱩᱫᱤ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ᱡᱩᱫᱤ (ret < 0) {
				error("cannot split patches from stdin");
				ᱨᱩᱣᱟᱹᱲ 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		ᱡᱩᱫᱤ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ᱨᱩᱣᱟᱹᱲ 1;
		}

		ᱡᱩᱫᱤ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		ᱵᱟᱝ
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ᱡᱩᱫᱤ (ret < 0) {
			error("cannot split patches from %s", arg);
			ᱨᱩᱣᱟᱹᱲ 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	ᱨᱩᱣᱟᱹᱲ 0;
}
