/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "strbuf.h"

स्थिर ध्रुव वर्ण git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

स्थिर पूर्णाङ्क is_from_पङ्क्तिः(ध्रुव वर्ण *पङ्क्तिः, पूर्णाङ्क len)
{
	ध्रुव वर्ण *colon;

	जइ (len < 20 || memcmp("From ", पङ्क्तिः, 5))
		प्रत्यागम 0;

	colon = पङ्क्तिः + len - 2;
	पङ्क्तिः += 5;
	प्रति (;;) {
		जइ (colon < पङ्क्तिः)
			प्रत्यागम 0;
		जइ (*--colon == ':')
			भङ्ग;
	}

	जइ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		प्रत्यागम 0;

	/* year */
	जइ (strtol(colon+3, रिक्त, 10) <= 90)
		प्रत्यागम 0;

	/* Ok, close enough */
	प्रत्यागम 1;
}

स्थिर संरचना strbuf buf = STRBUF_INIT;
स्थिर पूर्णाङ्क keep_cr;
स्थिर पूर्णाङ्क mboxrd;

स्थिर पूर्णाङ्क is_gtfrom(ध्रुव संरचना strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	जइ (buf->len < min)
		प्रत्यागम 0;

	ngt = strspn(buf->buf, ">");
	प्रत्यागम ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
स्थिर पूर्णाङ्क split_one(FILE *mbox, ध्रुव वर्ण *name, पूर्णाङ्क allow_bare)
{
	FILE *output;
	पूर्णाङ्क fd;
	पूर्णाङ्क status = 0;
	पूर्णाङ्क is_bare = !is_from_पङ्क्तिः(buf.buf, buf.len);

	जइ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	प्रति (;;) {
		जइ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		जइ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		जइ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		जइ (strbuf_getwholeline(&buf, mbox, '\n')) {
			जइ (feof(mbox)) {
				status = 1;
				भङ्ग;
			}
			die_errno("cannot read mbox");
		}
		जइ (!is_bare && is_from_पङ्क्तिः(buf.buf, buf.len))
			भङ्ग; /* done with one message */
	}
	fclose(output);
	प्रत्यागम status;
}

स्थिर पूर्णाङ्क populate_maildir_list(संरचना string_list *list, ध्रुव वर्ण *path)
{
	DIR *dir;
	संरचना dirent *dent;
	वर्ण *name = रिक्त;
	ध्रुव वर्ण *subs[] = { "cur", "new", रिक्त };
	ध्रुव वर्ण **sub;
	पूर्णाङ्क ret = -1;

	प्रति (sub = subs; *sub; ++sub) {
		मुक्त(name);
		name = xstrfmt("%s/%s", path, *sub);
		जइ (!(dir = opendir(name))) {
			जइ (errno == ENOENT)
				अनुवर्त;
			error_errno("cannot opendir %s", name);
			गच्छ out;
		}

		जाव ((dent = readdir(dir)) != रिक्त) {
			जइ (dent->d_name[0] == '.')
				अनुवर्त;
			मुक्त(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_निवेशय(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	मुक्त(name);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क maildir_filename_cmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
	जाव (*a && *b) {
		जइ (isdigit(*a) && isdigit(*b)) {
			long पूर्णाङ्क na, nb;
			na = strtol(a, (वर्ण **)&a, 10);
			nb = strtol(b, (वर्ण **)&b, 10);
			जइ (na != nb)
				प्रत्यागम na - nb;
			/* strtol advanced our pointers */
		}
		अण्णहा {
			जइ (*a != *b)
				प्रत्यागम (unsigned वर्ण)*a - (unsigned वर्ण)*b;
			a++;
			b++;
		}
	}
	प्रत्यागम (unsigned वर्ण)*a - (unsigned वर्ण)*b;
}

स्थिर पूर्णाङ्क split_maildir(ध्रुव वर्ण *maildir, ध्रुव वर्ण *dir,
	पूर्णाङ्क nr_prec, पूर्णाङ्क skip)
{
	वर्ण *file = रिक्त;
	FILE *f = रिक्त;
	पूर्णाङ्क ret = -1;
	संरचना string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	जइ (populate_maildir_list(&list, maildir) < 0)
		गच्छ out;

	प्रति (size_t i = 0; i < list.nr; i++) {
		वर्ण *name;

		मुक्त(file);
		file = xstrfmt("%s/%s", maildir, list.वस्तूनि[i].string);

		f = fopen(file, "r");
		जइ (!f) {
			error_errno("cannot open mail %s", file);
			गच्छ out;
		}

		जइ (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			गच्छ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		मुक्त(name);

		fclose(f);
		f = रिक्त;
	}

	ret = skip;
out:
	जइ (f)
		fclose(f);
	मुक्त(file);
	string_list_clear(&list, 1);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क split_mbox(ध्रुव वर्ण *file, ध्रुव वर्ण *dir, पूर्णाङ्क allow_bare,
		      पूर्णाङ्क nr_prec, पूर्णाङ्क skip)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	पूर्णाङ्क file_done = 0;

	जइ (!f) {
		error_errno("cannot open mbox %s", file);
		गच्छ out;
	}

	जइ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	करेहि {
		peek = fgetc(f);
		जइ (peek == EOF) {
			जइ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			अण्णहा {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			गच्छ out;
		}
	} जाव (isspace(peek));
	ungetc(peek, f);

	जइ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		जइ (f != stdin) {
			error("cannot read mbox %s", file);
			गच्छ out;
		}
		file_done = 1;
	}

	जाव (!file_done) {
		वर्ण *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		मुक्त(name);
	}

	जइ (f != stdin)
		fclose(f);

	ret = skip;
out:
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_mailsplit(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क nr = 0, nr_prec = 4, अङ्कः = 0;
	पूर्णाङ्क allow_bare = 0;
	ध्रुव वर्ण *dir = रिक्त;
	ध्रुव वर्ण **argp;
	स्थिर ध्रुव वर्ण *stdin_only[] = { "-", रिक्त };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	प्रति (argp = argv+1; *argp; argp++) {
		ध्रुव वर्ण *arg = *argp;

		जइ (arg[0] != '-')
			भङ्ग;
		/* do flags here */
		जइ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, रिक्त, 10);
			जइ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			अनुवर्त;
		} अण्णहा जइ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, रिक्त, 10);
		} अण्णहा जइ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} अण्णहा जइ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} अण्णहा जइ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} अण्णहा जइ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} अण्णहा जइ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			भङ्ग;
		} अण्णहा {
			die("unknown option: %s", arg);
		}
	}

	जइ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		विकल्प (argc - (argp-argv)) {
		पक्ष 1:
			dir = argp[0];
			argp = stdin_only;
			भङ्ग;
		पक्ष 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			भङ्ग;
		स्वभाव:
			usage(git_mailsplit_usage);
		}
	} अण्णहा {
		/* New usage: if no more argument, parse stdin */
		जइ ( !*argp )
			argp = stdin_only;
	}

	जाव (*argp) {
		ध्रुव वर्ण *arg = *argp++;
		संरचना stat argstat;
		पूर्णाङ्क ret = 0;

		जइ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			जइ (ret < 0) {
				error("cannot split patches from stdin");
				प्रत्यागम 1;
			}
			अङ्कः += (ret - nr);
			nr = ret;
			अनुवर्त;
		}

		जइ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			प्रत्यागम 1;
		}

		जइ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		अण्णहा
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		जइ (ret < 0) {
			error("cannot split patches from %s", arg);
			प्रत्यागम 1;
		}
		अङ्कः += (ret - nr);
		nr = ret;
	}

	printf("%d\n", अङ्कः);

	प्रत्यागम 0;
}
