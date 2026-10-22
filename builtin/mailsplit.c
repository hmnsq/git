/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "gettext.h"
#شامل "string-list.h"
#شامل "strbuf.h"

ایستا ثابت نویسه git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

ایستا صحیح is_from_سطر(ثابت نویسه *سطر, صحیح len)
{
	ثابت نویسه *colon;

	اگر (len < 20 || memcmp("From ", سطر, 5))
		بازگشت 0;

	colon = سطر + len - 2;
	سطر += 5;
	برای (;;) {
		اگر (colon < سطر)
			بازگشت 0;
		اگر (*--colon == ':')
			بشکن;
	}

	اگر (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		بازگشت 0;

	/* year */
	اگر (strtol(colon+3, پوچ, 10) <= 90)
		بازگشت 0;

	/* Ok, close enough */
	بازگشت 1;
}

ایستا ساختار strbuf buf = STRBUF_INIT;
ایستا صحیح keep_cr;
ایستا صحیح mboxrd;

ایستا صحیح is_gtfrom(ثابت ساختار strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	اگر (buf->len < min)
		بازگشت 0;

	ngt = strspn(buf->buf, ">");
	بازگشت ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
ایستا صحیح split_one(FILE *mbox, ثابت نویسه *name, صحیح allow_bare)
{
	FILE *output;
	صحیح fd;
	صحیح status = 0;
	صحیح is_bare = !is_from_سطر(buf.buf, buf.len);

	اگر (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	برای (;;) {
		اگر (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		اگر (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		اگر (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		اگر (strbuf_getwholeline(&buf, mbox, '\n')) {
			اگر (feof(mbox)) {
				status = 1;
				بشکن;
			}
			die_errno("cannot read mbox");
		}
		اگر (!is_bare && is_from_سطر(buf.buf, buf.len))
			بشکن; /* done with one message */
	}
	fclose(output);
	بازگشت status;
}

ایستا صحیح populate_maildir_list(ساختار string_list *list, ثابت نویسه *path)
{
	DIR *dir;
	ساختار dirent *dent;
	نویسه *name = پوچ;
	ثابت نویسه *subs[] = { "cur", "new", پوچ };
	ثابت نویسه **sub;
	صحیح ret = -1;

	برای (sub = subs; *sub; ++sub) {
		آزاد(name);
		name = xstrfmt("%s/%s", path, *sub);
		اگر (!(dir = opendir(name))) {
			اگر (errno == ENOENT)
				ادامه;
			error_errno("cannot opendir %s", name);
			برو out;
		}

		وقتی ((dent = readdir(dir)) != پوچ) {
			اگر (dent->d_name[0] == '.')
				ادامه;
			آزاد(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_درج(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	آزاد(name);
	بازگشت ret;
}

ایستا صحیح maildir_filename_cmp(ثابت نویسه *a, ثابت نویسه *b)
{
	وقتی (*a && *b) {
		اگر (isdigit(*a) && isdigit(*b)) {
			long صحیح na, nb;
			na = strtol(a, (نویسه **)&a, 10);
			nb = strtol(b, (نویسه **)&b, 10);
			اگر (na != nb)
				بازگشت na - nb;
			/* strtol advanced our pointers */
		}
		وگرنه {
			اگر (*a != *b)
				بازگشت (unsigned نویسه)*a - (unsigned نویسه)*b;
			a++;
			b++;
		}
	}
	بازگشت (unsigned نویسه)*a - (unsigned نویسه)*b;
}

ایستا صحیح split_maildir(ثابت نویسه *maildir, ثابت نویسه *dir,
	صحیح nr_prec, صحیح skip)
{
	نویسه *file = پوچ;
	FILE *f = پوچ;
	صحیح ret = -1;
	ساختار string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	اگر (populate_maildir_list(&list, maildir) < 0)
		برو out;

	برای (size_t i = 0; i < list.nr; i++) {
		نویسه *name;

		آزاد(file);
		file = xstrfmt("%s/%s", maildir, list.اقلام[i].string);

		f = fopen(file, "r");
		اگر (!f) {
			error_errno("cannot open mail %s", file);
			برو out;
		}

		اگر (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			برو out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		آزاد(name);

		fclose(f);
		f = پوچ;
	}

	ret = skip;
out:
	اگر (f)
		fclose(f);
	آزاد(file);
	string_list_clear(&list, 1);
	بازگشت ret;
}

ایستا صحیح split_mbox(ثابت نویسه *file, ثابت نویسه *dir, صحیح allow_bare,
		      صحیح nr_prec, صحیح skip)
{
	صحیح ret = -1;
	صحیح peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	صحیح file_done = 0;

	اگر (!f) {
		error_errno("cannot open mbox %s", file);
		برو out;
	}

	اگر (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	انجامبده {
		peek = fgetc(f);
		اگر (peek == EOF) {
			اگر (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			وگرنه {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			برو out;
		}
	} وقتی (isspace(peek));
	ungetc(peek, f);

	اگر (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		اگر (f != stdin) {
			error("cannot read mbox %s", file);
			برو out;
		}
		file_done = 1;
	}

	وقتی (!file_done) {
		نویسه *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		آزاد(name);
	}

	اگر (f != stdin)
		fclose(f);

	ret = skip;
out:
	بازگشت ret;
}

صحیح cmd_mailsplit(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	صحیح nr = 0, nr_prec = 4, رقم = 0;
	صحیح allow_bare = 0;
	ثابت نویسه *dir = پوچ;
	ثابت نویسه **argp;
	ایستا ثابت نویسه *stdin_only[] = { "-", پوچ };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	برای (argp = argv+1; *argp; argp++) {
		ثابت نویسه *arg = *argp;

		اگر (arg[0] != '-')
			بشکن;
		/* do flags here */
		اگر ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, پوچ, 10);
			اگر (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			ادامه;
		} وگرنه اگر ( arg[1] == 'f' ) {
			nr = strtol(arg+2, پوچ, 10);
		} وگرنه اگر ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} وگرنه اگر (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} وگرنه اگر ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} وگرنه اگر (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} وگرنه اگر ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			بشکن;
		} وگرنه {
			die("unknown option: %s", arg);
		}
	}

	اگر ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		انتخاب (argc - (argp-argv)) {
		حالت 1:
			dir = argp[0];
			argp = stdin_only;
			بشکن;
		حالت 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			بشکن;
		پیشفرض:
			usage(git_mailsplit_usage);
		}
	} وگرنه {
		/* New usage: if no more argument, parse stdin */
		اگر ( !*argp )
			argp = stdin_only;
	}

	وقتی (*argp) {
		ثابت نویسه *arg = *argp++;
		ساختار stat argstat;
		صحیح ret = 0;

		اگر (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			اگر (ret < 0) {
				error("cannot split patches from stdin");
				بازگشت 1;
			}
			رقم += (ret - nr);
			nr = ret;
			ادامه;
		}

		اگر (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			بازگشت 1;
		}

		اگر (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		وگرنه
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		اگر (ret < 0) {
			error("cannot split patches from %s", arg);
			بازگشت 1;
		}
		رقم += (ret - nr);
		nr = ret;
	}

	printf("%d\n", رقم);

	بازگشت 0;
}
