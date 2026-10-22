/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "string-list.h"
#ضمن "strbuf.h"

ساكن ثابت حرف git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

ساكن صحيح is_from_سطر(ثابت حرف *سطر, صحيح len)
{
	ثابت حرف *colon;

	إذا (len < 20 || memcmp("From ", سطر, 5))
		إرجاع 0;

	colon = سطر + len - 2;
	سطر += 5;
	لكل (;;) {
		إذا (colon < سطر)
			إرجاع 0;
		إذا (*--colon == ':')
			اكسر;
	}

	إذا (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		إرجاع 0;

	/* year */
	إذا (strtol(colon+3, عدم, 10) <= 90)
		إرجاع 0;

	/* Ok, close enough */
	إرجاع 1;
}

ساكن بنية strbuf buf = STRBUF_INIT;
ساكن صحيح keep_cr;
ساكن صحيح mboxrd;

ساكن صحيح is_gtfrom(ثابت بنية strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	إذا (buf->len < min)
		إرجاع 0;

	ngt = strspn(buf->buf, ">");
	إرجاع ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
ساكن صحيح split_one(FILE *mbox, ثابت حرف *name, صحيح allow_bare)
{
	FILE *output;
	صحيح fd;
	صحيح status = 0;
	صحيح is_bare = !is_from_سطر(buf.buf, buf.len);

	إذا (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	لكل (;;) {
		إذا (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		إذا (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		إذا (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		إذا (strbuf_getwholeline(&buf, mbox, '\n')) {
			إذا (feof(mbox)) {
				status = 1;
				اكسر;
			}
			die_errno("cannot read mbox");
		}
		إذا (!is_bare && is_from_سطر(buf.buf, buf.len))
			اكسر; /* done with one message */
	}
	fclose(output);
	إرجاع status;
}

ساكن صحيح populate_maildir_list(بنية string_list *list, ثابت حرف *path)
{
	DIR *dir;
	بنية dirent *dent;
	حرف *name = عدم;
	ثابت حرف *subs[] = { "cur", "new", عدم };
	ثابت حرف **sub;
	صحيح ret = -1;

	لكل (sub = subs; *sub; ++sub) {
		حرر(name);
		name = xstrfmt("%s/%s", path, *sub);
		إذا (!(dir = opendir(name))) {
			إذا (errno == ENOENT)
				استمر;
			error_errno("cannot opendir %s", name);
			اذهب out;
		}

		بينما ((dent = readdir(dir)) != عدم) {
			إذا (dent->d_name[0] == '.')
				استمر;
			حرر(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_أدرج(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	حرر(name);
	إرجاع ret;
}

ساكن صحيح maildir_filename_cmp(ثابت حرف *a, ثابت حرف *b)
{
	بينما (*a && *b) {
		إذا (isdigit(*a) && isdigit(*b)) {
			long صحيح na, nb;
			na = strtol(a, (حرف **)&a, 10);
			nb = strtol(b, (حرف **)&b, 10);
			إذا (na != nb)
				إرجاع na - nb;
			/* strtol advanced our pointers */
		}
		وإلا {
			إذا (*a != *b)
				إرجاع (unsigned حرف)*a - (unsigned حرف)*b;
			a++;
			b++;
		}
	}
	إرجاع (unsigned حرف)*a - (unsigned حرف)*b;
}

ساكن صحيح split_maildir(ثابت حرف *maildir, ثابت حرف *dir,
	صحيح nr_prec, صحيح skip)
{
	حرف *file = عدم;
	FILE *f = عدم;
	صحيح ret = -1;
	بنية string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	إذا (populate_maildir_list(&list, maildir) < 0)
		اذهب out;

	لكل (size_t i = 0; i < list.nr; i++) {
		حرف *name;

		حرر(file);
		file = xstrfmt("%s/%s", maildir, list.عناصر[i].string);

		f = fopen(file, "r");
		إذا (!f) {
			error_errno("cannot open mail %s", file);
			اذهب out;
		}

		إذا (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			اذهب out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		حرر(name);

		fclose(f);
		f = عدم;
	}

	ret = skip;
out:
	إذا (f)
		fclose(f);
	حرر(file);
	string_list_clear(&list, 1);
	إرجاع ret;
}

ساكن صحيح split_mbox(ثابت حرف *file, ثابت حرف *dir, صحيح allow_bare,
		      صحيح nr_prec, صحيح skip)
{
	صحيح ret = -1;
	صحيح peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	صحيح file_done = 0;

	إذا (!f) {
		error_errno("cannot open mbox %s", file);
		اذهب out;
	}

	إذا (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	افعل {
		peek = fgetc(f);
		إذا (peek == EOF) {
			إذا (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			وإلا {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			اذهب out;
		}
	} بينما (isspace(peek));
	ungetc(peek, f);

	إذا (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		إذا (f != stdin) {
			error("cannot read mbox %s", file);
			اذهب out;
		}
		file_done = 1;
	}

	بينما (!file_done) {
		حرف *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		حرر(name);
	}

	إذا (f != stdin)
		fclose(f);

	ret = skip;
out:
	إرجاع ret;
}

صحيح cmd_mailsplit(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	صحيح nr = 0, nr_prec = 4, رقم = 0;
	صحيح allow_bare = 0;
	ثابت حرف *dir = عدم;
	ثابت حرف **argp;
	ساكن ثابت حرف *stdin_only[] = { "-", عدم };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	لكل (argp = argv+1; *argp; argp++) {
		ثابت حرف *arg = *argp;

		إذا (arg[0] != '-')
			اكسر;
		/* do flags here */
		إذا ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, عدم, 10);
			إذا (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			استمر;
		} وإلا إذا ( arg[1] == 'f' ) {
			nr = strtol(arg+2, عدم, 10);
		} وإلا إذا ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} وإلا إذا (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} وإلا إذا ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} وإلا إذا (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} وإلا إذا ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			اكسر;
		} وإلا {
			die("unknown option: %s", arg);
		}
	}

	إذا ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		بدل (argc - (argp-argv)) {
		حالة 1:
			dir = argp[0];
			argp = stdin_only;
			اكسر;
		حالة 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			اكسر;
		افتراضي:
			usage(git_mailsplit_usage);
		}
	} وإلا {
		/* New usage: if no more argument, parse stdin */
		إذا ( !*argp )
			argp = stdin_only;
	}

	بينما (*argp) {
		ثابت حرف *arg = *argp++;
		بنية stat argstat;
		صحيح ret = 0;

		إذا (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			إذا (ret < 0) {
				error("cannot split patches from stdin");
				إرجاع 1;
			}
			رقم += (ret - nr);
			nr = ret;
			استمر;
		}

		إذا (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			إرجاع 1;
		}

		إذا (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		وإلا
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		إذا (ret < 0) {
			error("cannot split patches from %s", arg);
			إرجاع 1;
		}
		رقم += (ret - nr);
		nr = ret;
	}

	printf("%d\n", رقم);

	إرجاع 0;
}
