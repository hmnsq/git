/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "strbuf.h"

સ્થિર અચલ અક્ષર git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

સ્થિર પૂર્ણાંક is_from_લીટી(અચલ અક્ષર *લીટી, પૂર્ણાંક len)
{
	અચલ અક્ષર *colon;

	જો (len < 20 || memcmp("From ", લીટી, 5))
		પાછા 0;

	colon = લીટી + len - 2;
	લીટી += 5;
	માટે (;;) {
		જો (colon < લીટી)
			પાછા 0;
		જો (*--colon == ':')
			તોડો;
	}

	જો (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		પાછા 0;

	/* year */
	જો (strtol(colon+3, ખાલી, 10) <= 90)
		પાછા 0;

	/* Ok, close enough */
	પાછા 1;
}

સ્થિર સંરચના strbuf buf = STRBUF_INIT;
સ્થિર પૂર્ણાંક keep_cr;
સ્થિર પૂર્ણાંક mboxrd;

સ્થિર પૂર્ણાંક is_gtfrom(અચલ સંરચના strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	જો (buf->len < min)
		પાછા 0;

	ngt = strspn(buf->buf, ">");
	પાછા ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
સ્થિર પૂર્ણાંક split_one(FILE *mbox, અચલ અક્ષર *name, પૂર્ણાંક allow_bare)
{
	FILE *output;
	પૂર્ણાંક fd;
	પૂર્ણાંક status = 0;
	પૂર્ણાંક is_bare = !is_from_લીટી(buf.buf, buf.len);

	જો (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	માટે (;;) {
		જો (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		જો (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		જો (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		જો (strbuf_getwholeline(&buf, mbox, '\n')) {
			જો (feof(mbox)) {
				status = 1;
				તોડો;
			}
			die_errno("cannot read mbox");
		}
		જો (!is_bare && is_from_લીટી(buf.buf, buf.len))
			તોડો; /* done with one message */
	}
	fclose(output);
	પાછા status;
}

સ્થિર પૂર્ણાંક populate_maildir_list(સંરચના string_list *list, અચલ અક્ષર *path)
{
	DIR *dir;
	સંરચના dirent *dent;
	અક્ષર *name = ખાલી;
	અચલ અક્ષર *subs[] = { "cur", "new", ખાલી };
	અચલ અક્ષર **sub;
	પૂર્ણાંક ret = -1;

	માટે (sub = subs; *sub; ++sub) {
		મુક્ત(name);
		name = xstrfmt("%s/%s", path, *sub);
		જો (!(dir = opendir(name))) {
			જો (errno == ENOENT)
				ચાલુ;
			error_errno("cannot opendir %s", name);
			જાઓ out;
		}

		જ્યારે ((dent = readdir(dir)) != ખાલી) {
			જો (dent->d_name[0] == '.')
				ચાલુ;
			મુક્ત(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_ઉમેરો(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	મુક્ત(name);
	પાછા ret;
}

સ્થિર પૂર્ણાંક maildir_filename_cmp(અચલ અક્ષર *a, અચલ અક્ષર *b)
{
	જ્યારે (*a && *b) {
		જો (isdigit(*a) && isdigit(*b)) {
			long પૂર્ણાંક na, nb;
			na = strtol(a, (અક્ષર **)&a, 10);
			nb = strtol(b, (અક્ષર **)&b, 10);
			જો (na != nb)
				પાછા na - nb;
			/* strtol advanced our pointers */
		}
		નહીંતર {
			જો (*a != *b)
				પાછા (unsigned અક્ષર)*a - (unsigned અક્ષર)*b;
			a++;
			b++;
		}
	}
	પાછા (unsigned અક્ષર)*a - (unsigned અક્ષર)*b;
}

સ્થિર પૂર્ણાંક split_maildir(અચલ અક્ષર *maildir, અચલ અક્ષર *dir,
	પૂર્ણાંક nr_prec, પૂર્ણાંક skip)
{
	અક્ષર *file = ખાલી;
	FILE *f = ખાલી;
	પૂર્ણાંક ret = -1;
	સંરચના string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	જો (populate_maildir_list(&list, maildir) < 0)
		જાઓ out;

	માટે (size_t i = 0; i < list.nr; i++) {
		અક્ષર *name;

		મુક્ત(file);
		file = xstrfmt("%s/%s", maildir, list.વસ્તુઓ[i].string);

		f = fopen(file, "r");
		જો (!f) {
			error_errno("cannot open mail %s", file);
			જાઓ out;
		}

		જો (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			જાઓ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		મુક્ત(name);

		fclose(f);
		f = ખાલી;
	}

	ret = skip;
out:
	જો (f)
		fclose(f);
	મુક્ત(file);
	string_list_clear(&list, 1);
	પાછા ret;
}

સ્થિર પૂર્ણાંક split_mbox(અચલ અક્ષર *file, અચલ અક્ષર *dir, પૂર્ણાંક allow_bare,
		      પૂર્ણાંક nr_prec, પૂર્ણાંક skip)
{
	પૂર્ણાંક ret = -1;
	પૂર્ણાંક peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	પૂર્ણાંક file_done = 0;

	જો (!f) {
		error_errno("cannot open mbox %s", file);
		જાઓ out;
	}

	જો (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	કરો {
		peek = fgetc(f);
		જો (peek == EOF) {
			જો (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			નહીંતર {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			જાઓ out;
		}
	} જ્યારે (isspace(peek));
	ungetc(peek, f);

	જો (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		જો (f != stdin) {
			error("cannot read mbox %s", file);
			જાઓ out;
		}
		file_done = 1;
	}

	જ્યારે (!file_done) {
		અક્ષર *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		મુક્ત(name);
	}

	જો (f != stdin)
		fclose(f);

	ret = skip;
out:
	પાછા ret;
}

પૂર્ણાંક cmd_mailsplit(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક nr = 0, nr_prec = 4, અંક = 0;
	પૂર્ણાંક allow_bare = 0;
	અચલ અક્ષર *dir = ખાલી;
	અચલ અક્ષર **argp;
	સ્થિર અચલ અક્ષર *stdin_only[] = { "-", ખાલી };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	માટે (argp = argv+1; *argp; argp++) {
		અચલ અક્ષર *arg = *argp;

		જો (arg[0] != '-')
			તોડો;
		/* do flags here */
		જો ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, ખાલી, 10);
			જો (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			ચાલુ;
		} નહીંતર જો ( arg[1] == 'f' ) {
			nr = strtol(arg+2, ખાલી, 10);
		} નહીંતર જો ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} નહીંતર જો (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} નહીંતર જો ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} નહીંતર જો (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} નહીંતર જો ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			તોડો;
		} નહીંતર {
			die("unknown option: %s", arg);
		}
	}

	જો ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		પસંદગી (argc - (argp-argv)) {
		સ્થિતિ 1:
			dir = argp[0];
			argp = stdin_only;
			તોડો;
		સ્થિતિ 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			તોડો;
		મૂળભૂત:
			usage(git_mailsplit_usage);
		}
	} નહીંતર {
		/* New usage: if no more argument, parse stdin */
		જો ( !*argp )
			argp = stdin_only;
	}

	જ્યારે (*argp) {
		અચલ અક્ષર *arg = *argp++;
		સંરચના stat argstat;
		પૂર્ણાંક ret = 0;

		જો (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			જો (ret < 0) {
				error("cannot split patches from stdin");
				પાછા 1;
			}
			અંક += (ret - nr);
			nr = ret;
			ચાલુ;
		}

		જો (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			પાછા 1;
		}

		જો (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		નહીંતર
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		જો (ret < 0) {
			error("cannot split patches from %s", arg);
			પાછા 1;
		}
		અંક += (ret - nr);
		nr = ret;
	}

	printf("%d\n", અંક);

	પાછા 0;
}
