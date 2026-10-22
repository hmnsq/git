/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_from_ಸಾಲು(ಧ್ರುವ ವರ್ಣ *ಸಾಲು, ಪೂರ್ಣಾಙ್ಕ len)
{
	ಧ್ರುವ ವರ್ಣ *colon;

	ಯದಿ (len < 20 || memcmp("From ", ಸಾಲು, 5))
		ಪ್ರತ್ಯಾಗಮ 0;

	colon = ಸಾಲು + len - 2;
	ಸಾಲು += 5;
	ಪ್ರತಿ (;;) {
		ಯದಿ (colon < ಸಾಲು)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (*--colon == ':')
			ಭಙ್ಗ;
	}

	ಯದಿ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ಪ್ರತ್ಯಾಗಮ 0;

	/* year */
	ಯದಿ (strtol(colon+3, ರಿಕ್ತ, 10) <= 90)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Ok, close enough */
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ keep_cr;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mboxrd;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_gtfrom(ಧ್ರುವ ಸಂರಚನಾ strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ಯದಿ (buf->len < min)
		ಪ್ರತ್ಯಾಗಮ 0;

	ngt = strspn(buf->buf, ">");
	ಪ್ರತ್ಯಾಗಮ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_one(FILE *mbox, ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ allow_bare)
{
	FILE *output;
	ಪೂರ್ಣಾಙ್ಕ fd;
	ಪೂರ್ಣಾಙ್ಕ status = 0;
	ಪೂರ್ಣಾಙ್ಕ is_bare = !is_from_ಸಾಲು(buf.buf, buf.len);

	ಯದಿ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	ಪ್ರತಿ (;;) {
		ಯದಿ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ಯದಿ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ಯದಿ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ಯದಿ (strbuf_getwholeline(&buf, mbox, '\n')) {
			ಯದಿ (feof(mbox)) {
				status = 1;
				ಭಙ್ಗ;
			}
			die_errno("cannot read mbox");
		}
		ಯದಿ (!is_bare && is_from_ಸಾಲು(buf.buf, buf.len))
			ಭಙ್ಗ; /* done with one message */
	}
	fclose(output);
	ಪ್ರತ್ಯಾಗಮ status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ populate_maildir_list(ಸಂರಚನಾ string_list *list, ಧ್ರುವ ವರ್ಣ *path)
{
	DIR *dir;
	ಸಂರಚನಾ dirent *dent;
	ವರ್ಣ *name = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *subs[] = { "cur", "new", ರಿಕ್ತ };
	ಧ್ರುವ ವರ್ಣ **sub;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	ಪ್ರತಿ (sub = subs; *sub; ++sub) {
		ಮುಕ್ತ(name);
		name = xstrfmt("%s/%s", path, *sub);
		ಯದಿ (!(dir = opendir(name))) {
			ಯದಿ (errno == ENOENT)
				ಅನುವರ್ತ;
			error_errno("cannot opendir %s", name);
			ಗಚ್ಛ out;
		}

		ಯಾವತ್ ((dent = readdir(dir)) != ರಿಕ್ತ) {
			ಯದಿ (dent->d_name[0] == '.')
				ಅನುವರ್ತ;
			ಮುಕ್ತ(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_ಸೇರಿಸು(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	ಮುಕ್ತ(name);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ maildir_filename_cmp(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b)
{
	ಯಾವತ್ (*a && *b) {
		ಯದಿ (isdigit(*a) && isdigit(*b)) {
			long ಪೂರ್ಣಾಙ್ಕ na, nb;
			na = strtol(a, (ವರ್ಣ **)&a, 10);
			nb = strtol(b, (ವರ್ಣ **)&b, 10);
			ಯದಿ (na != nb)
				ಪ್ರತ್ಯಾಗಮ na - nb;
			/* strtol advanced our pointers */
		}
		ಅನ್ಯಥಾ {
			ಯದಿ (*a != *b)
				ಪ್ರತ್ಯಾಗಮ (unsigned ವರ್ಣ)*a - (unsigned ವರ್ಣ)*b;
			a++;
			b++;
		}
	}
	ಪ್ರತ್ಯಾಗಮ (unsigned ವರ್ಣ)*a - (unsigned ವರ್ಣ)*b;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_maildir(ಧ್ರುವ ವರ್ಣ *maildir, ಧ್ರುವ ವರ್ಣ *dir,
	ಪೂರ್ಣಾಙ್ಕ nr_prec, ಪೂರ್ಣಾಙ್ಕ skip)
{
	ವರ್ಣ *file = ರಿಕ್ತ;
	FILE *f = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ಯದಿ (populate_maildir_list(&list, maildir) < 0)
		ಗಚ್ಛ out;

	ಪ್ರತಿ (size_t i = 0; i < list.nr; i++) {
		ವರ್ಣ *name;

		ಮುಕ್ತ(file);
		file = xstrfmt("%s/%s", maildir, list.ವಸ್ತುಗಳು[i].string);

		f = fopen(file, "r");
		ಯದಿ (!f) {
			error_errno("cannot open mail %s", file);
			ಗಚ್ಛ out;
		}

		ಯದಿ (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			ಗಚ್ಛ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		ಮುಕ್ತ(name);

		fclose(f);
		f = ರಿಕ್ತ;
	}

	ret = skip;
out:
	ಯದಿ (f)
		fclose(f);
	ಮುಕ್ತ(file);
	string_list_clear(&list, 1);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_mbox(ಧ್ರುವ ವರ್ಣ *file, ಧ್ರುವ ವರ್ಣ *dir, ಪೂರ್ಣಾಙ್ಕ allow_bare,
		      ಪೂರ್ಣಾಙ್ಕ nr_prec, ಪೂರ್ಣಾಙ್ಕ skip)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -1;
	ಪೂರ್ಣಾಙ್ಕ peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ಪೂರ್ಣಾಙ್ಕ file_done = 0;

	ಯದಿ (!f) {
		error_errno("cannot open mbox %s", file);
		ಗಚ್ಛ out;
	}

	ಯದಿ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	ಕುರು {
		peek = fgetc(f);
		ಯದಿ (peek == EOF) {
			ಯದಿ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			ಅನ್ಯಥಾ {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			ಗಚ್ಛ out;
		}
	} ಯಾವತ್ (isspace(peek));
	ungetc(peek, f);

	ಯದಿ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ಯದಿ (f != stdin) {
			error("cannot read mbox %s", file);
			ಗಚ್ಛ out;
		}
		file_done = 1;
	}

	ಯಾವತ್ (!file_done) {
		ವರ್ಣ *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		ಮುಕ್ತ(name);
	}

	ಯದಿ (f != stdin)
		fclose(f);

	ret = skip;
out:
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_mailsplit(ಪೂರ್ಣಾಙ್ಕ argc,
		  ಧ್ರುವ ವರ್ಣ **argv,
		  ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ nr = 0, nr_prec = 4, ಅಂಕೆ = 0;
	ಪೂರ್ಣಾಙ್ಕ allow_bare = 0;
	ಧ್ರುವ ವರ್ಣ *dir = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ **argp;
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *stdin_only[] = { "-", ರಿಕ್ತ };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	ಪ್ರತಿ (argp = argv+1; *argp; argp++) {
		ಧ್ರುವ ವರ್ಣ *arg = *argp;

		ಯದಿ (arg[0] != '-')
			ಭಙ್ಗ;
		/* do flags here */
		ಯದಿ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, ರಿಕ್ತ, 10);
			ಯದಿ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ ಯದಿ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, ರಿಕ್ತ, 10);
		} ಅನ್ಯಥಾ ಯದಿ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} ಅನ್ಯಥಾ ಯದಿ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} ಅನ್ಯಥಾ ಯದಿ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			ಭಙ್ಗ;
		} ಅನ್ಯಥಾ {
			die("unknown option: %s", arg);
		}
	}

	ಯದಿ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		ವಿಕಲ್ಪ (argc - (argp-argv)) {
		ಪಕ್ಷ 1:
			dir = argp[0];
			argp = stdin_only;
			ಭಙ್ಗ;
		ಪಕ್ಷ 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			usage(git_mailsplit_usage);
		}
	} ಅನ್ಯಥಾ {
		/* New usage: if no more argument, parse stdin */
		ಯದಿ ( !*argp )
			argp = stdin_only;
	}

	ಯಾವತ್ (*argp) {
		ಧ್ರುವ ವರ್ಣ *arg = *argp++;
		ಸಂರಚನಾ stat argstat;
		ಪೂರ್ಣಾಙ್ಕ ret = 0;

		ಯದಿ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ಯದಿ (ret < 0) {
				error("cannot split patches from stdin");
				ಪ್ರತ್ಯಾಗಮ 1;
			}
			ಅಂಕೆ += (ret - nr);
			nr = ret;
			ಅನುವರ್ತ;
		}

		ಯದಿ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ಪ್ರತ್ಯಾಗಮ 1;
		}

		ಯದಿ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		ಅನ್ಯಥಾ
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ಯದಿ (ret < 0) {
			error("cannot split patches from %s", arg);
			ಪ್ರತ್ಯಾಗಮ 1;
		}
		ಅಂಕೆ += (ret - nr);
		nr = ret;
	}

	printf("%d\n", ಅಂಕೆ);

	ಪ್ರತ್ಯಾಗಮ 0;
}
