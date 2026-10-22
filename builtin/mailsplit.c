/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "strbuf.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

സ്ഥിതം പൂർണ്ണസംഖ്യ is_from_വരി(സ്ഥിരം അക്ഷരം *വരി, പൂർണ്ണസംഖ്യ len)
{
	സ്ഥിരം അക്ഷരം *colon;

	എങ്കിൽ (len < 20 || memcmp("From ", വരി, 5))
		തിരികെ 0;

	colon = വരി + len - 2;
	വരി += 5;
	വേണ്ടി (;;) {
		എങ്കിൽ (colon < വരി)
			തിരികെ 0;
		എങ്കിൽ (*--colon == ':')
			നിർത്തുക;
	}

	എങ്കിൽ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		തിരികെ 0;

	/* year */
	എങ്കിൽ (strtol(colon+3, ശൂന്യ, 10) <= 90)
		തിരികെ 0;

	/* Ok, close enough */
	തിരികെ 1;
}

സ്ഥിതം ഘടന strbuf buf = STRBUF_INIT;
സ്ഥിതം പൂർണ്ണസംഖ്യ keep_cr;
സ്ഥിതം പൂർണ്ണസംഖ്യ mboxrd;

സ്ഥിതം പൂർണ്ണസംഖ്യ is_gtfrom(സ്ഥിരം ഘടന strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	എങ്കിൽ (buf->len < min)
		തിരികെ 0;

	ngt = strspn(buf->buf, ">");
	തിരികെ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ split_one(FILE *mbox, സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ allow_bare)
{
	FILE *output;
	പൂർണ്ണസംഖ്യ fd;
	പൂർണ്ണസംഖ്യ status = 0;
	പൂർണ്ണസംഖ്യ is_bare = !is_from_വരി(buf.buf, buf.len);

	എങ്കിൽ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	വേണ്ടി (;;) {
		എങ്കിൽ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		എങ്കിൽ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		എങ്കിൽ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		എങ്കിൽ (strbuf_getwholeline(&buf, mbox, '\n')) {
			എങ്കിൽ (feof(mbox)) {
				status = 1;
				നിർത്തുക;
			}
			die_errno("cannot read mbox");
		}
		എങ്കിൽ (!is_bare && is_from_വരി(buf.buf, buf.len))
			നിർത്തുക; /* done with one message */
	}
	fclose(output);
	തിരികെ status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ populate_maildir_list(ഘടന string_list *list, സ്ഥിരം അക്ഷരം *path)
{
	DIR *dir;
	ഘടന dirent *dent;
	അക്ഷരം *name = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *subs[] = { "cur", "new", ശൂന്യ };
	സ്ഥിരം അക്ഷരം **sub;
	പൂർണ്ണസംഖ്യ ret = -1;

	വേണ്ടി (sub = subs; *sub; ++sub) {
		സ്വതന്ത്രം(name);
		name = xstrfmt("%s/%s", path, *sub);
		എങ്കിൽ (!(dir = opendir(name))) {
			എങ്കിൽ (errno == ENOENT)
				തുടരുക;
			error_errno("cannot opendir %s", name);
			പോകുക out;
		}

		ആകുമ്പോൾ ((dent = readdir(dir)) != ശൂന്യ) {
			എങ്കിൽ (dent->d_name[0] == '.')
				തുടരുക;
			സ്വതന്ത്രം(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_തിരുകുക(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	സ്വതന്ത്രം(name);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ maildir_filename_cmp(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b)
{
	ആകുമ്പോൾ (*a && *b) {
		എങ്കിൽ (isdigit(*a) && isdigit(*b)) {
			long പൂർണ്ണസംഖ്യ na, nb;
			na = strtol(a, (അക്ഷരം **)&a, 10);
			nb = strtol(b, (അക്ഷരം **)&b, 10);
			എങ്കിൽ (na != nb)
				തിരികെ na - nb;
			/* strtol advanced our pointers */
		}
		അല്ലെങ്കിൽ {
			എങ്കിൽ (*a != *b)
				തിരികെ (unsigned അക്ഷരം)*a - (unsigned അക്ഷരം)*b;
			a++;
			b++;
		}
	}
	തിരികെ (unsigned അക്ഷരം)*a - (unsigned അക്ഷരം)*b;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ split_maildir(സ്ഥിരം അക്ഷരം *maildir, സ്ഥിരം അക്ഷരം *dir,
	പൂർണ്ണസംഖ്യ nr_prec, പൂർണ്ണസംഖ്യ skip)
{
	അക്ഷരം *file = ശൂന്യ;
	FILE *f = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = -1;
	ഘടന string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	എങ്കിൽ (populate_maildir_list(&list, maildir) < 0)
		പോകുക out;

	വേണ്ടി (size_t i = 0; i < list.nr; i++) {
		അക്ഷരം *name;

		സ്വതന്ത്രം(file);
		file = xstrfmt("%s/%s", maildir, list.ഇനങ്ങൾ[i].string);

		f = fopen(file, "r");
		എങ്കിൽ (!f) {
			error_errno("cannot open mail %s", file);
			പോകുക out;
		}

		എങ്കിൽ (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			പോകുക out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		സ്വതന്ത്രം(name);

		fclose(f);
		f = ശൂന്യ;
	}

	ret = skip;
out:
	എങ്കിൽ (f)
		fclose(f);
	സ്വതന്ത്രം(file);
	string_list_clear(&list, 1);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ split_mbox(സ്ഥിരം അക്ഷരം *file, സ്ഥിരം അക്ഷരം *dir, പൂർണ്ണസംഖ്യ allow_bare,
		      പൂർണ്ണസംഖ്യ nr_prec, പൂർണ്ണസംഖ്യ skip)
{
	പൂർണ്ണസംഖ്യ ret = -1;
	പൂർണ്ണസംഖ്യ peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	പൂർണ്ണസംഖ്യ file_done = 0;

	എങ്കിൽ (!f) {
		error_errno("cannot open mbox %s", file);
		പോകുക out;
	}

	എങ്കിൽ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	ചെയ്യുക {
		peek = fgetc(f);
		എങ്കിൽ (peek == EOF) {
			എങ്കിൽ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			അല്ലെങ്കിൽ {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			പോകുക out;
		}
	} ആകുമ്പോൾ (isspace(peek));
	ungetc(peek, f);

	എങ്കിൽ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		എങ്കിൽ (f != stdin) {
			error("cannot read mbox %s", file);
			പോകുക out;
		}
		file_done = 1;
	}

	ആകുമ്പോൾ (!file_done) {
		അക്ഷരം *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		സ്വതന്ത്രം(name);
	}

	എങ്കിൽ (f != stdin)
		fclose(f);

	ret = skip;
out:
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_mailsplit(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ nr = 0, nr_prec = 4, അക്കം = 0;
	പൂർണ്ണസംഖ്യ allow_bare = 0;
	സ്ഥിരം അക്ഷരം *dir = ശൂന്യ;
	സ്ഥിരം അക്ഷരം **argp;
	സ്ഥിതം സ്ഥിരം അക്ഷരം *stdin_only[] = { "-", ശൂന്യ };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	വേണ്ടി (argp = argv+1; *argp; argp++) {
		സ്ഥിരം അക്ഷരം *arg = *argp;

		എങ്കിൽ (arg[0] != '-')
			നിർത്തുക;
		/* do flags here */
		എങ്കിൽ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, ശൂന്യ, 10);
			എങ്കിൽ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			തുടരുക;
		} അല്ലെങ്കിൽ എങ്കിൽ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, ശൂന്യ, 10);
		} അല്ലെങ്കിൽ എങ്കിൽ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			നിർത്തുക;
		} അല്ലെങ്കിൽ {
			die("unknown option: %s", arg);
		}
	}

	എങ്കിൽ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		തിരഞ്ഞെടുപ്പ് (argc - (argp-argv)) {
		അവസ്ഥ 1:
			dir = argp[0];
			argp = stdin_only;
			നിർത്തുക;
		അവസ്ഥ 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			usage(git_mailsplit_usage);
		}
	} അല്ലെങ്കിൽ {
		/* New usage: if no more argument, parse stdin */
		എങ്കിൽ ( !*argp )
			argp = stdin_only;
	}

	ആകുമ്പോൾ (*argp) {
		സ്ഥിരം അക്ഷരം *arg = *argp++;
		ഘടന stat argstat;
		പൂർണ്ണസംഖ്യ ret = 0;

		എങ്കിൽ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			എങ്കിൽ (ret < 0) {
				error("cannot split patches from stdin");
				തിരികെ 1;
			}
			അക്കം += (ret - nr);
			nr = ret;
			തുടരുക;
		}

		എങ്കിൽ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			തിരികെ 1;
		}

		എങ്കിൽ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		അല്ലെങ്കിൽ
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		എങ്കിൽ (ret < 0) {
			error("cannot split patches from %s", arg);
			തിരികെ 1;
		}
		അക്കം += (ret - nr);
		nr = ret;
	}

	printf("%d\n", അക്കം);

	തിരികെ 0;
}
