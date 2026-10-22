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

स्थिर पूर्णाङ्क is_from_पंक्ति(ध्रुव वर्ण *पंक्ति, पूर्णाङ्क len)
{
	ध्रुव वर्ण *colon;

	यदि (len < 20 || स_भेद("From ", पंक्ति, 5))
		प्रत्यागम 0;

	colon = पंक्ति + len - 2;
	पंक्ति += 5;
	प्रति (;;) {
		यदि (colon < पंक्ति)
			प्रत्यागम 0;
		यदि (*--colon == ':')
			भङ्ग;
	}

	यदि (!है_अंक(colon[-4]) ||
	    !है_अंक(colon[-2]) ||
	    !है_अंक(colon[-1]) ||
	    !है_अंक(colon[ 1]) ||
	    !है_अंक(colon[ 2]))
		प्रत्यागम 0;

	/* year */
	यदि (म_से_दीर्घ(colon+3, रिक्त, 10) <= 90)
		प्रत्यागम 0;

	/* Ok, close enough */
	प्रत्यागम 1;
}

स्थिर संरचना strbuf buf = STRBUF_INIT;
स्थिर पूर्णाङ्क keep_cr;
स्थिर पूर्णाङ्क mboxrd;

स्थिर पूर्णाङ्क is_gtfrom(ध्रुव संरचना strbuf *buf)
{
	माप_प्रकार min = म_माप(">From ");
	माप_प्रकार ngt;

	यदि (buf->len < min)
		प्रत्यागम 0;

	ngt = म_अखोज(buf->buf, ">");
	प्रत्यागम ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
स्थिर पूर्णाङ्क split_one(खाता *mbox, ध्रुव वर्ण *name, पूर्णाङ्क allow_bare)
{
	खाता *output;
	पूर्णाङ्क fd;
	पूर्णाङ्क status = 0;
	पूर्णाङ्क is_bare = !is_from_पंक्ति(buf.buf, buf.len);

	यदि (is_bare && !allow_bare) {
		ख_लिखो(मानक_त्रुटि, "corrupt mailbox\n");
		निकास(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	प्रति (;;) {
		यदि (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		यदि (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		यदि (ख_डालो(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		यदि (strbuf_getwholeline(&buf, mbox, '\n')) {
			यदि (ख_पूर्ण(mbox)) {
				status = 1;
				भङ्ग;
			}
			die_errno("cannot read mbox");
		}
		यदि (!is_bare && is_from_पंक्ति(buf.buf, buf.len))
			भङ्ग; /* done with one message */
	}
	ख_बंद(output);
	प्रत्यागम status;
}

स्थिर पूर्णाङ्क populate_maildir_list(संरचना string_list *list, ध्रुव वर्ण *path)
{
	सूची *dir;
	संरचना dirent *dent;
	वर्ण *name = रिक्त;
	ध्रुव वर्ण *subs[] = { "cur", "new", रिक्त };
	ध्रुव वर्ण **sub;
	पूर्णाङ्क ret = -1;

	प्रति (sub = subs; *sub; ++sub) {
		मुक्त(name);
		name = xstrfmt("%s/%s", path, *sub);
		यदि (!(dir = सूची_खोलो(name))) {
			यदि (त्रुटि_सं == ENOENT)
				अनुवर्त;
			error_errno("cannot opendir %s", name);
			गच्छ out;
		}

		यावत् ((dent = सूची_पढ़ो(dir)) != रिक्त) {
			यदि (dent->d_name[0] == '.')
				अनुवर्त;
			मुक्त(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_जोड़ो(list, name);
		}

		बंद_सूची(dir);
	}

	ret = 0;

out:
	मुक्त(name);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क maildir_filename_cmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
	यावत् (*a && *b) {
		यदि (है_अंक(*a) && है_अंक(*b)) {
			दीर्घ पूर्णाङ्क na, nb;
			na = म_से_दीर्घ(a, (वर्ण **)&a, 10);
			nb = म_से_दीर्घ(b, (वर्ण **)&b, 10);
			यदि (na != nb)
				प्रत्यागम na - nb;
			/* strtol advanced our pointers */
		}
		अन्यथा {
			यदि (*a != *b)
				प्रत्यागम (अचिन्हित वर्ण)*a - (अचिन्हित वर्ण)*b;
			a++;
			b++;
		}
	}
	प्रत्यागम (अचिन्हित वर्ण)*a - (अचिन्हित वर्ण)*b;
}

स्थिर पूर्णाङ्क split_maildir(ध्रुव वर्ण *maildir, ध्रुव वर्ण *dir,
	पूर्णाङ्क nr_prec, पूर्णाङ्क skip)
{
	वर्ण *file = रिक्त;
	खाता *f = रिक्त;
	पूर्णाङ्क ret = -1;
	संरचना string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	यदि (populate_maildir_list(&list, maildir) < 0)
		गच्छ out;

	प्रति (माप_प्रकार i = 0; i < list.nr; i++) {
		वर्ण *name;

		मुक्त(file);
		file = xstrfmt("%s/%s", maildir, list.वस्तुएँ[i].string);

		f = ख_खोलो(file, "r");
		यदि (!f) {
			error_errno("cannot open mail %s", file);
			गच्छ out;
		}

		यदि (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			गच्छ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		मुक्त(name);

		ख_बंद(f);
		f = रिक्त;
	}

	ret = skip;
out:
	यदि (f)
		ख_बंद(f);
	मुक्त(file);
	string_list_clear(&list, 1);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क split_mbox(ध्रुव वर्ण *file, ध्रुव वर्ण *dir, पूर्णाङ्क allow_bare,
		      पूर्णाङ्क nr_prec, पूर्णाङ्क skip)
{
	पूर्णाङ्क ret = -1;
	पूर्णाङ्क peek;

	खाता *f = !म_भेद(file, "-") ? मानक_निवेश : ख_खोलो(file, "r");
	पूर्णाङ्क file_done = 0;

	यदि (!f) {
		error_errno("cannot open mbox %s", file);
		गच्छ out;
	}

	यदि (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	कुरु {
		peek = ख_अक्षर_लो(f);
		यदि (peek == खातापूर्ण) {
			यदि (f == मानक_निवेश)
				/* empty stdin is OK */
				ret = skip;
			अन्यथा {
				ख_बंद(f);
				error(_("empty mbox: '%s'"), file);
			}
			गच्छ out;
		}
	} यावत् (है_खाली(peek));
	अक्षर_वापस(peek, f);

	यदि (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		यदि (f != मानक_निवेश) {
			error("cannot read mbox %s", file);
			गच्छ out;
		}
		file_done = 1;
	}

	यावत् (!file_done) {
		वर्ण *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		मुक्त(name);
	}

	यदि (f != मानक_निवेश)
		ख_बंद(f);

	ret = skip;
out:
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_mailsplit(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णाङ्क nr = 0, nr_prec = 4, अंक = 0;
	पूर्णाङ्क allow_bare = 0;
	ध्रुव वर्ण *dir = रिक्त;
	ध्रुव वर्ण **argp;
	स्थिर ध्रुव वर्ण *stdin_only[] = { "-", रिक्त };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	प्रति (argp = argv+1; *argp; argp++) {
		ध्रुव वर्ण *arg = *argp;

		यदि (arg[0] != '-')
			भङ्ग;
		/* do flags here */
		यदि ( arg[1] == 'd' ) {
			nr_prec = म_से_दीर्घ(arg+2, रिक्त, 10);
			यदि (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			अनुवर्त;
		} अन्यथा यदि ( arg[1] == 'f' ) {
			nr = म_से_दीर्घ(arg+2, रिक्त, 10);
		} अन्यथा यदि ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} अन्यथा यदि (!म_भेद(arg, "--keep-cr")) {
			keep_cr = 1;
		} अन्यथा यदि ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} अन्यथा यदि (!म_भेद(arg, "--mboxrd")) {
			mboxrd = 1;
		} अन्यथा यदि ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			भङ्ग;
		} अन्यथा {
			die("unknown option: %s", arg);
		}
	}

	यदि ( !dir ) {
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
	} अन्यथा {
		/* New usage: if no more argument, parse stdin */
		यदि ( !*argp )
			argp = stdin_only;
	}

	यावत् (*argp) {
		ध्रुव वर्ण *arg = *argp++;
		संरचना stat argstat;
		पूर्णाङ्क ret = 0;

		यदि (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			यदि (ret < 0) {
				error("cannot split patches from stdin");
				प्रत्यागम 1;
			}
			अंक += (ret - nr);
			nr = ret;
			अनुवर्त;
		}

		यदि (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			प्रत्यागम 1;
		}

		यदि (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		अन्यथा
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		यदि (ret < 0) {
			error("cannot split patches from %s", arg);
			प्रत्यागम 1;
		}
		अंक += (ret - nr);
		nr = ret;
	}

	म_लिखो("%d\n", अंक);

	प्रत्यागम 0;
}
