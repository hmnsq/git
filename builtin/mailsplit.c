/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_from_ଧାଡ଼ି(ଅଚଳ ଅକ୍ଷର *ଧାଡ଼ି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ଅଚଳ ଅକ୍ଷର *colon;

	ଯଦି (len < 20 || memcmp("From ", ଧାଡ଼ି, 5))
		ଫେରନ୍ତୁ 0;

	colon = ଧାଡ଼ି + len - 2;
	ଧାଡ଼ି += 5;
	ପାଇଁ (;;) {
		ଯଦି (colon < ଧାଡ଼ି)
			ଫେରନ୍ତୁ 0;
		ଯଦି (*--colon == ':')
			ଭାଙ୍ଗ;
	}

	ଯଦି (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ଫେରନ୍ତୁ 0;

	/* year */
	ଯଦି (strtol(colon+3, ଖାଲି, 10) <= 90)
		ଫେରନ୍ତୁ 0;

	/* Ok, close enough */
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ସଂରଚନା strbuf buf = STRBUF_INIT;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mboxrd;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_gtfrom(ଅଚଳ ସଂରଚନା strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ଯଦି (buf->len < min)
		ଫେରନ୍ତୁ 0;

	ngt = strspn(buf->buf, ">");
	ଫେରନ୍ତୁ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_one(FILE *mbox, ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_bare)
{
	FILE *output;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bare = !is_from_ଧାଡ଼ି(buf.buf, buf.len);

	ଯଦି (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	ପାଇଁ (;;) {
		ଯଦି (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ଯଦି (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ଯଦି (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ଯଦି (strbuf_getwholeline(&buf, mbox, '\n')) {
			ଯଦି (feof(mbox)) {
				status = 1;
				ଭାଙ୍ଗ;
			}
			die_errno("cannot read mbox");
		}
		ଯଦି (!is_bare && is_from_ଧାଡ଼ି(buf.buf, buf.len))
			ଭାଙ୍ଗ; /* done with one message */
	}
	fclose(output);
	ଫେରନ୍ତୁ status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା populate_maildir_list(ସଂରଚନା string_list *list, ଅଚଳ ଅକ୍ଷର *path)
{
	DIR *dir;
	ସଂରଚନା dirent *dent;
	ଅକ୍ଷର *name = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *subs[] = { "cur", "new", ଖାଲି };
	ଅଚଳ ଅକ୍ଷର **sub;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;

	ପାଇଁ (sub = subs; *sub; ++sub) {
		ମୁକ୍ତ(name);
		name = xstrfmt("%s/%s", path, *sub);
		ଯଦି (!(dir = opendir(name))) {
			ଯଦି (errno == ENOENT)
				ଜାରି;
			error_errno("cannot opendir %s", name);
			ଯାଅ out;
		}

		ଯେତେବେଳେ ((dent = readdir(dir)) != ଖାଲି) {
			ଯଦି (dent->d_name[0] == '.')
				ଜାରି;
			ମୁକ୍ତ(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_ଭର୍ତ୍ତି(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	ମୁକ୍ତ(name);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା maildir_filename_cmp(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b)
{
	ଯେତେବେଳେ (*a && *b) {
		ଯଦି (isdigit(*a) && isdigit(*b)) {
			long ପୂର୍ଣ୍ଣସଂଖ୍ୟା na, nb;
			na = strtol(a, (ଅକ୍ଷର **)&a, 10);
			nb = strtol(b, (ଅକ୍ଷର **)&b, 10);
			ଯଦି (na != nb)
				ଫେରନ୍ତୁ na - nb;
			/* strtol advanced our pointers */
		}
		ଅନ୍ୟଥା {
			ଯଦି (*a != *b)
				ଫେରନ୍ତୁ (unsigned ଅକ୍ଷର)*a - (unsigned ଅକ୍ଷର)*b;
			a++;
			b++;
		}
	}
	ଫେରନ୍ତୁ (unsigned ଅକ୍ଷର)*a - (unsigned ଅକ୍ଷର)*b;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_maildir(ଅଚଳ ଅକ୍ଷର *maildir, ଅଚଳ ଅକ୍ଷର *dir,
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_prec, ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip)
{
	ଅକ୍ଷର *file = ଖାଲି;
	FILE *f = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ଯଦି (populate_maildir_list(&list, maildir) < 0)
		ଯାଅ out;

	ପାଇଁ (size_t i = 0; i < list.nr; i++) {
		ଅକ୍ଷର *name;

		ମୁକ୍ତ(file);
		file = xstrfmt("%s/%s", maildir, list.ବସ୍ତୁଗୁଡ଼ିକ[i].string);

		f = fopen(file, "r");
		ଯଦି (!f) {
			error_errno("cannot open mail %s", file);
			ଯାଅ out;
		}

		ଯଦି (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			ଯାଅ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		ମୁକ୍ତ(name);

		fclose(f);
		f = ଖାଲି;
	}

	ret = skip;
out:
	ଯଦି (f)
		fclose(f);
	ମୁକ୍ତ(file);
	string_list_clear(&list, 1);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_mbox(ଅଚଳ ଅକ୍ଷର *file, ଅଚଳ ଅକ୍ଷର *dir, ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_bare,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_prec, ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_done = 0;

	ଯଦି (!f) {
		error_errno("cannot open mbox %s", file);
		ଯାଅ out;
	}

	ଯଦି (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	କର {
		peek = fgetc(f);
		ଯଦି (peek == EOF) {
			ଯଦି (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			ଅନ୍ୟଥା {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			ଯାଅ out;
		}
	} ଯେତେବେଳେ (isspace(peek));
	ungetc(peek, f);

	ଯଦି (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ଯଦି (f != stdin) {
			error("cannot read mbox %s", file);
			ଯାଅ out;
		}
		file_done = 1;
	}

	ଯେତେବେଳେ (!file_done) {
		ଅକ୍ଷର *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		ମୁକ୍ତ(name);
	}

	ଯଦି (f != stdin)
		fclose(f);

	ret = skip;
out:
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_mailsplit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		  ଅଚଳ ଅକ୍ଷର **argv,
		  ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr = 0, nr_prec = 4, ଅଙ୍କ = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_bare = 0;
	ଅଚଳ ଅକ୍ଷର *dir = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର **argp;
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *stdin_only[] = { "-", ଖାଲି };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	ପାଇଁ (argp = argv+1; *argp; argp++) {
		ଅଚଳ ଅକ୍ଷର *arg = *argp;

		ଯଦି (arg[0] != '-')
			ଭାଙ୍ଗ;
		/* do flags here */
		ଯଦି ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, ଖାଲି, 10);
			ଯଦି (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			ଜାରି;
		} ଅନ୍ୟଥା ଯଦି ( arg[1] == 'f' ) {
			nr = strtol(arg+2, ଖାଲି, 10);
		} ଅନ୍ୟଥା ଯଦି ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} ଅନ୍ୟଥା ଯଦି ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} ଅନ୍ୟଥା ଯଦି ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			ଭାଙ୍ଗ;
		} ଅନ୍ୟଥା {
			die("unknown option: %s", arg);
		}
	}

	ଯଦି ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		ବାଛ (argc - (argp-argv)) {
		ସ୍ଥିତି 1:
			dir = argp[0];
			argp = stdin_only;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			usage(git_mailsplit_usage);
		}
	} ଅନ୍ୟଥା {
		/* New usage: if no more argument, parse stdin */
		ଯଦି ( !*argp )
			argp = stdin_only;
	}

	ଯେତେବେଳେ (*argp) {
		ଅଚଳ ଅକ୍ଷର *arg = *argp++;
		ସଂରଚନା stat argstat;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

		ଯଦି (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ଯଦି (ret < 0) {
				error("cannot split patches from stdin");
				ଫେରନ୍ତୁ 1;
			}
			ଅଙ୍କ += (ret - nr);
			nr = ret;
			ଜାରି;
		}

		ଯଦି (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ଫେରନ୍ତୁ 1;
		}

		ଯଦି (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		ଅନ୍ୟଥା
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ଯଦି (ret < 0) {
			error("cannot split patches from %s", arg);
			ଫେରନ୍ତୁ 1;
		}
		ଅଙ୍କ += (ret - nr);
		nr = ret;
	}

	printf("%d\n", ଅଙ୍କ);

	ଫେରନ୍ତୁ 0;
}
