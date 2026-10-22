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

static ຈຳນວນເຕັມ is_from_line(const var_na *line, ຈຳນວນເຕັມ len)
{
	const var_na *colon;

	ຖ້າ (len < 20 || memcmp("From ", line, 5))
		ສົ່ງຄືນ 0;

	colon = line + len - 2;
	line += 5;
	ສຳລັບ (;;) {
		ຖ້າ (colon < line)
			ສົ່ງຄືນ 0;
		ຖ້າ (*--colon == ':')
			bha_nga;
	}

	ຖ້າ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ສົ່ງຄືນ 0;

	/* year */
	ຖ້າ (strtol(colon+3, NULL, 10) <= 90)
		ສົ່ງຄືນ 0;

	/* Ok, close enough */
	ສົ່ງຄືນ 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static ຈຳນວນເຕັມ keep_cr;
static ຈຳນວນເຕັມ mboxrd;

static ຈຳນວນເຕັມ is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ຖ້າ (buf->len < min)
		ສົ່ງຄືນ 0;

	ngt = strspn(buf->buf, ">");
	ສົ່ງຄືນ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static ຈຳນວນເຕັມ split_one(FILE *mbox, const var_na *name, ຈຳນວນເຕັມ allow_bare)
{
	FILE *output;
	ຈຳນວນເຕັມ fd;
	ຈຳນວນເຕັມ status = 0;
	ຈຳນວນເຕັມ is_bare = !is_from_line(buf.buf, buf.len);

	ຖ້າ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	ສຳລັບ (;;) {
		ຖ້າ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ຖ້າ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ຖ້າ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ຖ້າ (strbuf_getwholeline(&buf, mbox, '\n')) {
			ຖ້າ (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		ຖ້າ (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	ສົ່ງຄືນ status;
}

static ຈຳນວນເຕັມ populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	ຈຳນວນເຕັມ ret = -1;

	ສຳລັບ (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		ຖ້າ (!(dir = opendir(name))) {
			ຖ້າ (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		ໃນຂະນະທີ່ ((dent = readdir(dir)) != NULL) {
			ຖ້າ (dent->d_name[0] == '.')
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
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ maildir_filename_cmp(const var_na *a, const var_na *b)
{
	ໃນຂະນະທີ່ (*a && *b) {
		ຖ້າ (isdigit(*a) && isdigit(*b)) {
			long ຈຳນວນເຕັມ na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			ຖ້າ (na != nb)
				ສົ່ງຄືນ na - nb;
			/* strtol advanced our pointers */
		}
		ບໍ່ດັ່ງນັ້ນ {
			ຖ້າ (*a != *b)
				ສົ່ງຄືນ (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	ສົ່ງຄືນ (unsigned var_na)*a - (unsigned var_na)*b;
}

static ຈຳນວນເຕັມ split_maildir(const var_na *maildir, const var_na *dir,
	ຈຳນວນເຕັມ nr_prec, ຈຳນວນເຕັມ skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	ຈຳນວນເຕັມ ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ຖ້າ (populate_maildir_list(&list, maildir) < 0)
		goto out;

	ສຳລັບ (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		ຖ້າ (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		ຖ້າ (strbuf_getwholeline(&buf, f, '\n')) {
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
	ຖ້າ (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ split_mbox(const var_na *file, const var_na *dir, ຈຳນວນເຕັມ allow_bare,
		      ຈຳນວນເຕັມ nr_prec, ຈຳນວນເຕັມ skip)
{
	ຈຳນວນເຕັມ ret = -1;
	ຈຳນວນເຕັມ peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ຈຳນວນເຕັມ file_done = 0;

	ຖ້າ (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	ຖ້າ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		ຖ້າ (peek == EOF) {
			ຖ້າ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			ບໍ່ດັ່ງນັ້ນ {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} ໃນຂະນະທີ່ (isspace(peek));
	ungetc(peek, f);

	ຖ້າ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ຖ້າ (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	ໃນຂະນະທີ່ (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	ຖ້າ (f != stdin)
		fclose(f);

	ret = skip;
out:
	ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ cmd_mailsplit(ຈຳນວນເຕັມ argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ nr = 0, nr_prec = 4, num = 0;
	ຈຳນວນເຕັມ allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	ສຳລັບ (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		ຖ້າ (arg[0] != '-')
			bha_nga;
		/* do flags here */
		ຖ້າ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			ຖ້າ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} ບໍ່ດັ່ງນັ້ນ {
			die("unknown option: %s", arg);
		}
	}

	ຖ້າ ( !dir ) {
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
	} ບໍ່ດັ່ງນັ້ນ {
		/* New usage: if no more argument, parse stdin */
		ຖ້າ ( !*argp )
			argp = stdin_only;
	}

	ໃນຂະນະທີ່ (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		ຈຳນວນເຕັມ ret = 0;

		ຖ້າ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ຖ້າ (ret < 0) {
				error("cannot split patches from stdin");
				ສົ່ງຄືນ 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		ຖ້າ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ສົ່ງຄືນ 1;
		}

		ຖ້າ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		ບໍ່ດັ່ງນັ້ນ
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ຖ້າ (ret < 0) {
			error("cannot split patches from %s", arg);
			ສົ່ງຄືນ 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	ສົ່ງຄືນ 0;
}
