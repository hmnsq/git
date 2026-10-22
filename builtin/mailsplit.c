/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "strbuf.h"

స్థిర స్థిరరాశి అక్షరం git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

స్థిర పూర్ణాంకం is_from_పంక్తి(స్థిరరాశి అక్షరం *పంక్తి, పూర్ణాంకం len)
{
	స్థిరరాశి అక్షరం *colon;

	ఒకవేళ (len < 20 || memcmp("From ", పంక్తి, 5))
		తిరిగి 0;

	colon = పంక్తి + len - 2;
	పంక్తి += 5;
	కొరకు (;;) {
		ఒకవేళ (colon < పంక్తి)
			తిరిగి 0;
		ఒకవేళ (*--colon == ':')
			ఆపు;
	}

	ఒకవేళ (!అంకెనా(colon[-4]) ||
	    !అంకెనా(colon[-2]) ||
	    !అంకెనా(colon[-1]) ||
	    !అంకెనా(colon[ 1]) ||
	    !అంకెనా(colon[ 2]))
		తిరిగి 0;

	/* year */
	ఒకవేళ (strtol(colon+3, ఖాళీ, 10) <= 90)
		తిరిగి 0;

	/* Ok, close enough */
	తిరిగి 1;
}

స్థిర నిర్మాణం strbuf buf = STRBUF_INIT;
స్థిర పూర్ణాంకం keep_cr;
స్థిర పూర్ణాంకం mboxrd;

స్థిర పూర్ణాంకం is_gtfrom(స్థిరరాశి నిర్మాణం strbuf *buf)
{
	కొలత_t min = అక్షరశ్రేణి_పొడవు(">From ");
	కొలత_t ngt;

	ఒకవేళ (buf->len < min)
		తిరిగి 0;

	ngt = strspn(buf->buf, ">");
	తిరిగి ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
స్థిర పూర్ణాంకం విభజించు_one(FILE *mbox, స్థిరరాశి అక్షరం *name, పూర్ణాంకం allow_bare)
{
	FILE *output;
	పూర్ణాంకం fd;
	పూర్ణాంకం స్థితి_2 = 0;
	పూర్ణాంకం is_bare = !is_from_పంక్తి(buf.buf, buf.len);

	ఒకవేళ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		నిష్క్రమించు(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	కొరకు (;;) {
		ఒకవేళ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ఒకవేళ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ఒకవేళ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ఒకవేళ (strbuf_getwholeline(&buf, mbox, '\n')) {
			ఒకవేళ (feof(mbox)) {
				స్థితి_2 = 1;
				ఆపు;
			}
			die_errno("cannot read mbox");
		}
		ఒకవేళ (!is_bare && is_from_పంక్తి(buf.buf, buf.len))
			ఆపు; /* done with one message */
	}
	fclose(output);
	తిరిగి స్థితి_2;
}

స్థిర పూర్ణాంకం populate_maildir_list(నిర్మాణం string_list *list, స్థిరరాశి అక్షరం *మార్గం)
{
	DIR *dir;
	నిర్మాణం dirent *dent;
	అక్షరం *name = ఖాళీ;
	స్థిరరాశి అక్షరం *subs[] = { "cur", "new", ఖాళీ };
	స్థిరరాశి అక్షరం **sub;
	పూర్ణాంకం ret = -1;

	కొరకు (sub = subs; *sub; ++sub) {
		విడుదల(name);
		name = xstrfmt("%s/%s", మార్గం, *sub);
		ఒకవేళ (!(dir = opendir(name))) {
			ఒకవేళ (errno == ENOENT)
				కొనసాగు;
			error_errno("cannot opendir %s", name);
			వెళ్ళు బహిర్గతం;
		}

		అయితే ((dent = readdir(dir)) != ఖాళీ) {
			ఒకవేళ (dent->d_name[0] == '.')
				కొనసాగు;
			విడుదల(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_చొప్పించు(list, name);
		}

		closedir(dir);
	}

	ret = 0;

బహిర్గతం:
	విడుదల(name);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం maildir_filename_cmp(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b)
{
	అయితే (*a && *b) {
		ఒకవేళ (అంకెనా(*a) && అంకెనా(*b)) {
			దీర్ఘ పూర్ణాంకం na, nb;
			na = strtol(a, (అక్షరం **)&a, 10);
			nb = strtol(b, (అక్షరం **)&b, 10);
			ఒకవేళ (na != nb)
				తిరిగి na - nb;
			/* strtol advanced our pointers */
		}
		లేకపోతే {
			ఒకవేళ (*a != *b)
				తిరిగి (గుర్తులేని అక్షరం)*a - (గుర్తులేని అక్షరం)*b;
			a++;
			b++;
		}
	}
	తిరిగి (గుర్తులేని అక్షరం)*a - (గుర్తులేని అక్షరం)*b;
}

స్థిర పూర్ణాంకం విభజించు_maildir(స్థిరరాశి అక్షరం *maildir, స్థిరరాశి అక్షరం *dir,
	పూర్ణాంకం nr_prec, పూర్ణాంకం skip)
{
	అక్షరం *file = ఖాళీ;
	FILE *f = ఖాళీ;
	పూర్ణాంకం ret = -1;
	నిర్మాణం string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ఒకవేళ (populate_maildir_list(&list, maildir) < 0)
		వెళ్ళు బహిర్గతం;

	కొరకు (కొలత_t i = 0; i < list.nr; i++) {
		అక్షరం *name;

		విడుదల(file);
		file = xstrfmt("%s/%s", maildir, list.వస్తువులు[i].string);

		f = fopen(file, "r");
		ఒకవేళ (!f) {
			error_errno("cannot open mail %s", file);
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			వెళ్ళు బహిర్గతం;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		విభజించు_one(f, name, 1);
		విడుదల(name);

		fclose(f);
		f = ఖాళీ;
	}

	ret = skip;
బహిర్గతం:
	ఒకవేళ (f)
		fclose(f);
	విడుదల(file);
	string_list_clear(&list, 1);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం విభజించు_mbox(స్థిరరాశి అక్షరం *file, స్థిరరాశి అక్షరం *dir, పూర్ణాంకం allow_bare,
		      పూర్ణాంకం nr_prec, పూర్ణాంకం skip)
{
	పూర్ణాంకం ret = -1;
	పూర్ణాంకం peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	పూర్ణాంకం file_పూర్తి = 0;

	ఒకవేళ (!f) {
		error_errno("cannot open mbox %s", file);
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	చేయి {
		peek = fgetc(f);
		ఒకవేళ (peek == EOF) {
			ఒకవేళ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			లేకపోతే {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			వెళ్ళు బహిర్గతం;
		}
	} అయితే (ఖాళీనా(peek));
	ungetc(peek, f);

	ఒకవేళ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ఒకవేళ (f != stdin) {
			error("cannot read mbox %s", file);
			వెళ్ళు బహిర్గతం;
		}
		file_పూర్తి = 1;
	}

	అయితే (!file_పూర్తి) {
		అక్షరం *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_పూర్తి = విభజించు_one(f, name, allow_bare);
		విడుదల(name);
	}

	ఒకవేళ (f != stdin)
		fclose(f);

	ret = skip;
బహిర్గతం:
	తిరిగి ret;
}

పూర్ణాంకం cmd_mailsplit(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం nr = 0, nr_prec = 4, అంకె = 0;
	పూర్ణాంకం allow_bare = 0;
	స్థిరరాశి అక్షరం *dir = ఖాళీ;
	స్థిరరాశి అక్షరం **argp;
	స్థిర స్థిరరాశి అక్షరం *stdin_only[] = { "-", ఖాళీ };

	BUG_ON_NON_ఖాళీలు_ఉపసర్గ(ఉపసర్గ);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	కొరకు (argp = argv+1; *argp; argp++) {
		స్థిరరాశి అక్షరం *arg = *argp;

		ఒకవేళ (arg[0] != '-')
			ఆపు;
		/* do flags here */
		ఒకవేళ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, ఖాళీ, 10);
			ఒకవేళ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			కొనసాగు;
		} లేకపోతే ఒకవేళ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, ఖాళీ, 10);
		} లేకపోతే ఒకవేళ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} లేకపోతే ఒకవేళ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} లేకపోతే ఒకవేళ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			ఆపు;
		} లేకపోతే {
			die("unknown option: %s", arg);
		}
	}

	ఒకవేళ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		ఎంపిక (argc - (argp-argv)) {
		స్థితి 1:
			dir = argp[0];
			argp = stdin_only;
			ఆపు;
		స్థితి 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			ఆపు;
		అప్రమేయ:
			usage(git_mailsplit_usage);
		}
	} లేకపోతే {
		/* New usage: if no more argument, parse stdin */
		ఒకవేళ ( !*argp )
			argp = stdin_only;
	}

	అయితే (*argp) {
		స్థిరరాశి అక్షరం *arg = *argp++;
		నిర్మాణం stat argstat;
		పూర్ణాంకం ret = 0;

		ఒకవేళ (arg[0] == '-' && arg[1] == 0) {
			ret = విభజించు_mbox(arg, dir, allow_bare, nr_prec, nr);
			ఒకవేళ (ret < 0) {
				error("cannot split patches from stdin");
				తిరిగి 1;
			}
			అంకె += (ret - nr);
			nr = ret;
			కొనసాగు;
		}

		ఒకవేళ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			తిరిగి 1;
		}

		ఒకవేళ (S_ISDIR(argstat.st_బహుళకం))
			ret = విభజించు_maildir(arg, dir, nr_prec, nr);
		లేకపోతే
			ret = విభజించు_mbox(arg, dir, allow_bare, nr_prec, nr);

		ఒకవేళ (ret < 0) {
			error("cannot split patches from %s", arg);
			తిరిగి 1;
		}
		అంకె += (ret - nr);
		nr = ret;
	}

	రాయి("%d\n", అంకె);

	తిరిగి 0;
}
