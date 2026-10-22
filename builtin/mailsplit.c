/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "strbuf.h"

நிலையான மாறிலி எழுத்து git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

நிலையான முழு is_from_வரி(மாறிலி எழுத்து *வரி, முழு len)
{
	மாறிலி எழுத்து *colon;

	எனில் (len < 20 || memcmp("From ", வரி, 5))
		திரும்பு 0;

	colon = வரி + len - 2;
	வரி += 5;
	க்கு (;;) {
		எனில் (colon < வரி)
			திரும்பு 0;
		எனில் (*--colon == ':')
			நிறுத்து;
	}

	எனில் (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		திரும்பு 0;

	/* year */
	எனில் (strtol(colon+3, வெற்று, 10) <= 90)
		திரும்பு 0;

	/* Ok, close enough */
	திரும்பு 1;
}

நிலையான கட்டமைப்பு strbuf buf = STRBUF_INIT;
நிலையான முழு keep_cr;
நிலையான முழு mboxrd;

நிலையான முழு is_gtfrom(மாறிலி கட்டமைப்பு strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	எனில் (buf->len < min)
		திரும்பு 0;

	ngt = strspn(buf->buf, ">");
	திரும்பு ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
நிலையான முழு split_one(FILE *mbox, மாறிலி எழுத்து *name, முழு allow_bare)
{
	FILE *output;
	முழு fd;
	முழு status = 0;
	முழு is_bare = !is_from_வரி(buf.buf, buf.len);

	எனில் (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	க்கு (;;) {
		எனில் (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		எனில் (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		எனில் (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		எனில் (strbuf_getwholeline(&buf, mbox, '\n')) {
			எனில் (feof(mbox)) {
				status = 1;
				நிறுத்து;
			}
			die_errno("cannot read mbox");
		}
		எனில் (!is_bare && is_from_வரி(buf.buf, buf.len))
			நிறுத்து; /* done with one message */
	}
	fclose(output);
	திரும்பு status;
}

நிலையான முழு populate_maildir_list(கட்டமைப்பு string_list *list, மாறிலி எழுத்து *path)
{
	DIR *dir;
	கட்டமைப்பு dirent *dent;
	எழுத்து *name = வெற்று;
	மாறிலி எழுத்து *subs[] = { "cur", "new", வெற்று };
	மாறிலி எழுத்து **sub;
	முழு ret = -1;

	க்கு (sub = subs; *sub; ++sub) {
		விடுவி(name);
		name = xstrfmt("%s/%s", path, *sub);
		எனில் (!(dir = opendir(name))) {
			எனில் (errno == ENOENT)
				தொடர்;
			error_errno("cannot opendir %s", name);
			செல் out;
		}

		போது ((dent = readdir(dir)) != வெற்று) {
			எனில் (dent->d_name[0] == '.')
				தொடர்;
			விடுவி(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_செருகு(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	விடுவி(name);
	திரும்பு ret;
}

நிலையான முழு maildir_filename_cmp(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b)
{
	போது (*a && *b) {
		எனில் (isdigit(*a) && isdigit(*b)) {
			long முழு na, nb;
			na = strtol(a, (எழுத்து **)&a, 10);
			nb = strtol(b, (எழுத்து **)&b, 10);
			எனில் (na != nb)
				திரும்பு na - nb;
			/* strtol advanced our pointers */
		}
		இல்லையெனில் {
			எனில் (*a != *b)
				திரும்பு (unsigned எழுத்து)*a - (unsigned எழுத்து)*b;
			a++;
			b++;
		}
	}
	திரும்பு (unsigned எழுத்து)*a - (unsigned எழுத்து)*b;
}

நிலையான முழு split_maildir(மாறிலி எழுத்து *maildir, மாறிலி எழுத்து *dir,
	முழு nr_prec, முழு skip)
{
	எழுத்து *file = வெற்று;
	FILE *f = வெற்று;
	முழு ret = -1;
	கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	எனில் (populate_maildir_list(&list, maildir) < 0)
		செல் out;

	க்கு (size_t i = 0; i < list.nr; i++) {
		எழுத்து *name;

		விடுவி(file);
		file = xstrfmt("%s/%s", maildir, list.பொருட்கள்[i].string);

		f = fopen(file, "r");
		எனில் (!f) {
			error_errno("cannot open mail %s", file);
			செல் out;
		}

		எனில் (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			செல் out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		விடுவி(name);

		fclose(f);
		f = வெற்று;
	}

	ret = skip;
out:
	எனில் (f)
		fclose(f);
	விடுவி(file);
	string_list_clear(&list, 1);
	திரும்பு ret;
}

நிலையான முழு split_mbox(மாறிலி எழுத்து *file, மாறிலி எழுத்து *dir, முழு allow_bare,
		      முழு nr_prec, முழு skip)
{
	முழு ret = -1;
	முழு peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	முழு file_done = 0;

	எனில் (!f) {
		error_errno("cannot open mbox %s", file);
		செல் out;
	}

	எனில் (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	செய் {
		peek = fgetc(f);
		எனில் (peek == EOF) {
			எனில் (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			இல்லையெனில் {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			செல் out;
		}
	} போது (isspace(peek));
	ungetc(peek, f);

	எனில் (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		எனில் (f != stdin) {
			error("cannot read mbox %s", file);
			செல் out;
		}
		file_done = 1;
	}

	போது (!file_done) {
		எழுத்து *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		விடுவி(name);
	}

	எனில் (f != stdin)
		fclose(f);

	ret = skip;
out:
	திரும்பு ret;
}

முழு cmd_mailsplit(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	முழு nr = 0, nr_prec = 4, இலக்கம் = 0;
	முழு allow_bare = 0;
	மாறிலி எழுத்து *dir = வெற்று;
	மாறிலி எழுத்து **argp;
	நிலையான மாறிலி எழுத்து *stdin_only[] = { "-", வெற்று };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	க்கு (argp = argv+1; *argp; argp++) {
		மாறிலி எழுத்து *arg = *argp;

		எனில் (arg[0] != '-')
			நிறுத்து;
		/* do flags here */
		எனில் ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, வெற்று, 10);
			எனில் (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			தொடர்;
		} இல்லையெனில் எனில் ( arg[1] == 'f' ) {
			nr = strtol(arg+2, வெற்று, 10);
		} இல்லையெனில் எனில் ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} இல்லையெனில் எனில் (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} இல்லையெனில் எனில் ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} இல்லையெனில் எனில் (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} இல்லையெனில் எனில் ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			நிறுத்து;
		} இல்லையெனில் {
			die("unknown option: %s", arg);
		}
	}

	எனில் ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		தேர்வு (argc - (argp-argv)) {
		நிலை 1:
			dir = argp[0];
			argp = stdin_only;
			நிறுத்து;
		நிலை 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			நிறுத்து;
		இயல்பு:
			usage(git_mailsplit_usage);
		}
	} இல்லையெனில் {
		/* New usage: if no more argument, parse stdin */
		எனில் ( !*argp )
			argp = stdin_only;
	}

	போது (*argp) {
		மாறிலி எழுத்து *arg = *argp++;
		கட்டமைப்பு stat argstat;
		முழு ret = 0;

		எனில் (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			எனில் (ret < 0) {
				error("cannot split patches from stdin");
				திரும்பு 1;
			}
			இலக்கம் += (ret - nr);
			nr = ret;
			தொடர்;
		}

		எனில் (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			திரும்பு 1;
		}

		எனில் (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		இல்லையெனில்
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		எனில் (ret < 0) {
			error("cannot split patches from %s", arg);
			திரும்பு 1;
		}
		இலக்கம் += (ret - nr);
		nr = ret;
	}

	printf("%d\n", இலக்கம்);

	திரும்பு 0;
}
