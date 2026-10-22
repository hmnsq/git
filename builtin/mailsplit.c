/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "string-list.h"
#অন্তর্ভাৱ "strbuf.h"

স্থির ধ্রুৱ ৱর্ণ git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

স্থির পূর্ণাঙ্ক is_from_সাৰি(ধ্রুৱ ৱর্ণ *সাৰি, পূর্ণাঙ্ক len)
{
	ধ্রুৱ ৱর্ণ *colon;

	যদি (len < 20 || memcmp("From ", সাৰি, 5))
		ঘূৰাই 0;

	colon = সাৰি + len - 2;
	সাৰি += 5;
	বাবে (;;) {
		যদি (colon < সাৰি)
			ঘূৰাই 0;
		যদি (*--colon == ':')
			ভঙ্গ;
	}

	যদি (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ঘূৰাই 0;

	/* year */
	যদি (strtol(colon+3, রিক্ত, 10) <= 90)
		ঘূৰাই 0;

	/* Ok, close enough */
	ঘূৰাই 1;
}

স্থির সংরচনা strbuf buf = STRBUF_INIT;
স্থির পূর্ণাঙ্ক keep_cr;
স্থির পূর্ণাঙ্ক mboxrd;

স্থির পূর্ণাঙ্ক is_gtfrom(ধ্রুৱ সংরচনা strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	যদি (buf->len < min)
		ঘূৰাই 0;

	ngt = strspn(buf->buf, ">");
	ঘূৰাই ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
স্থির পূর্ণাঙ্ক split_one(FILE *mbox, ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক allow_bare)
{
	FILE *output;
	পূর্ণাঙ্ক fd;
	পূর্ণাঙ্ক status = 0;
	পূর্ণাঙ্ক is_bare = !is_from_সাৰি(buf.buf, buf.len);

	যদি (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	বাবে (;;) {
		যদি (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		যদি (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		যদি (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		যদি (strbuf_getwholeline(&buf, mbox, '\n')) {
			যদি (feof(mbox)) {
				status = 1;
				ভঙ্গ;
			}
			die_errno("cannot read mbox");
		}
		যদি (!is_bare && is_from_সাৰি(buf.buf, buf.len))
			ভঙ্গ; /* done with one message */
	}
	fclose(output);
	ঘূৰাই status;
}

স্থির পূর্ণাঙ্ক populate_maildir_list(সংরচনা string_list *list, ধ্রুৱ ৱর্ণ *path)
{
	DIR *dir;
	সংরচনা dirent *dent;
	ৱর্ণ *name = রিক্ত;
	ধ্রুৱ ৱর্ণ *subs[] = { "cur", "new", রিক্ত };
	ধ্রুৱ ৱর্ণ **sub;
	পূর্ণাঙ্ক ret = -1;

	বাবে (sub = subs; *sub; ++sub) {
		মুক্ত(name);
		name = xstrfmt("%s/%s", path, *sub);
		যদি (!(dir = opendir(name))) {
			যদি (errno == ENOENT)
				অনুৱর্ত;
			error_errno("cannot opendir %s", name);
			গচ্ছ out;
		}

		যেতিয়ালৈ ((dent = readdir(dir)) != রিক্ত) {
			যদি (dent->d_name[0] == '.')
				অনুৱর্ত;
			মুক্ত(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_ঢোকাও(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	মুক্ত(name);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক maildir_filename_cmp(ধ্রুৱ ৱর্ণ *a, ধ্রুৱ ৱর্ণ *b)
{
	যেতিয়ালৈ (*a && *b) {
		যদি (isdigit(*a) && isdigit(*b)) {
			long পূর্ণাঙ্ক na, nb;
			na = strtol(a, (ৱর্ণ **)&a, 10);
			nb = strtol(b, (ৱর্ণ **)&b, 10);
			যদি (na != nb)
				ঘূৰাই na - nb;
			/* strtol advanced our pointers */
		}
		অন্যথা {
			যদি (*a != *b)
				ঘূৰাই (unsigned ৱর্ণ)*a - (unsigned ৱর্ণ)*b;
			a++;
			b++;
		}
	}
	ঘূৰাই (unsigned ৱর্ণ)*a - (unsigned ৱর্ণ)*b;
}

স্থির পূর্ণাঙ্ক split_maildir(ধ্রুৱ ৱর্ণ *maildir, ধ্রুৱ ৱর্ণ *dir,
	পূর্ণাঙ্ক nr_prec, পূর্ণাঙ্ক skip)
{
	ৱর্ণ *file = রিক্ত;
	FILE *f = রিক্ত;
	পূর্ণাঙ্ক ret = -1;
	সংরচনা string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	যদি (populate_maildir_list(&list, maildir) < 0)
		গচ্ছ out;

	বাবে (size_t i = 0; i < list.nr; i++) {
		ৱর্ণ *name;

		মুক্ত(file);
		file = xstrfmt("%s/%s", maildir, list.বস্তুসমূহ[i].string);

		f = fopen(file, "r");
		যদি (!f) {
			error_errno("cannot open mail %s", file);
			গচ্ছ out;
		}

		যদি (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			গচ্ছ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		মুক্ত(name);

		fclose(f);
		f = রিক্ত;
	}

	ret = skip;
out:
	যদি (f)
		fclose(f);
	মুক্ত(file);
	string_list_clear(&list, 1);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক split_mbox(ধ্রুৱ ৱর্ণ *file, ধ্রুৱ ৱর্ণ *dir, পূর্ণাঙ্ক allow_bare,
		      পূর্ণাঙ্ক nr_prec, পূর্ণাঙ্ক skip)
{
	পূর্ণাঙ্ক ret = -1;
	পূর্ণাঙ্ক peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	পূর্ণাঙ্ক file_done = 0;

	যদি (!f) {
		error_errno("cannot open mbox %s", file);
		গচ্ছ out;
	}

	যদি (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	কৰা {
		peek = fgetc(f);
		যদি (peek == EOF) {
			যদি (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			অন্যথা {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			গচ্ছ out;
		}
	} যেতিয়ালৈ (isspace(peek));
	ungetc(peek, f);

	যদি (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		যদি (f != stdin) {
			error("cannot read mbox %s", file);
			গচ্ছ out;
		}
		file_done = 1;
	}

	যেতিয়ালৈ (!file_done) {
		ৱর্ণ *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		মুক্ত(name);
	}

	যদি (f != stdin)
		fclose(f);

	ret = skip;
out:
	ঘূৰাই ret;
}

পূর্ণাঙ্ক cmd_mailsplit(পূর্ণাঙ্ক argc,
		  ধ্রুৱ ৱর্ণ **argv,
		  ধ্রুৱ ৱর্ণ *prefix,
		  সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক nr = 0, nr_prec = 4, অঙ্ক = 0;
	পূর্ণাঙ্ক allow_bare = 0;
	ধ্রুৱ ৱর্ণ *dir = রিক্ত;
	ধ্রুৱ ৱর্ণ **argp;
	স্থির ধ্রুৱ ৱর্ণ *stdin_only[] = { "-", রিক্ত };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	বাবে (argp = argv+1; *argp; argp++) {
		ধ্রুৱ ৱর্ণ *arg = *argp;

		যদি (arg[0] != '-')
			ভঙ্গ;
		/* do flags here */
		যদি ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, রিক্ত, 10);
			যদি (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			অনুৱর্ত;
		} অন্যথা যদি ( arg[1] == 'f' ) {
			nr = strtol(arg+2, রিক্ত, 10);
		} অন্যথা যদি ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} অন্যথা যদি (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} অন্যথা যদি ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} অন্যথা যদি (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} অন্যথা যদি ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			ভঙ্গ;
		} অন্যথা {
			die("unknown option: %s", arg);
		}
	}

	যদি ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		ৱিকল্প (argc - (argp-argv)) {
		পক্ষ 1:
			dir = argp[0];
			argp = stdin_only;
			ভঙ্গ;
		পক্ষ 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			ভঙ্গ;
		স্ৱভাৱ:
			usage(git_mailsplit_usage);
		}
	} অন্যথা {
		/* New usage: if no more argument, parse stdin */
		যদি ( !*argp )
			argp = stdin_only;
	}

	যেতিয়ালৈ (*argp) {
		ধ্রুৱ ৱর্ণ *arg = *argp++;
		সংরচনা stat argstat;
		পূর্ণাঙ্ক ret = 0;

		যদি (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			যদি (ret < 0) {
				error("cannot split patches from stdin");
				ঘূৰাই 1;
			}
			অঙ্ক += (ret - nr);
			nr = ret;
			অনুৱর্ত;
		}

		যদি (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ঘূৰাই 1;
		}

		যদি (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		অন্যথা
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		যদি (ret < 0) {
			error("cannot split patches from %s", arg);
			ঘূৰাই 1;
		}
		অঙ্ক += (ret - nr);
		nr = ret;
	}

	printf("%d\n", অঙ্ক);

	ঘূৰাই 0;
}
