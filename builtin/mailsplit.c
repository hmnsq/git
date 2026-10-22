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

स्थिर स्थिरांक अक्षर git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

स्थिर पूर्णांक is_from_ओळ(स्थिरांक अक्षर *ओळ, पूर्णांक len)
{
	स्थिरांक अक्षर *colon;

	जर (len < 20 || स_भेद("From ", ओळ, 5))
		परत 0;

	colon = ओळ + len - 2;
	ओळ += 5;
	साठी (;;) {
		जर (colon < ओळ)
			परत 0;
		जर (*--colon == ':')
			थांबा;
	}

	जर (!है_अंक(colon[-4]) ||
	    !है_अंक(colon[-2]) ||
	    !है_अंक(colon[-1]) ||
	    !है_अंक(colon[ 1]) ||
	    !है_अंक(colon[ 2]))
		परत 0;

	/* year */
	जर (म_से_दीर्घ(colon+3, रिक्त, 10) <= 90)
		परत 0;

	/* Ok, close enough */
	परत 1;
}

स्थिर संरचना strbuf buf = STRBUF_INIT;
स्थिर पूर्णांक keep_cr;
स्थिर पूर्णांक mboxrd;

स्थिर पूर्णांक is_gtfrom(स्थिरांक संरचना strbuf *buf)
{
	माप_प्रकार min = म_माप(">From ");
	माप_प्रकार ngt;

	जर (buf->len < min)
		परत 0;

	ngt = म_अखोज(buf->buf, ">");
	परत ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
स्थिर पूर्णांक split_one(खाता *mbox, स्थिरांक अक्षर *name, पूर्णांक allow_bare)
{
	खाता *output;
	पूर्णांक fd;
	पूर्णांक status = 0;
	पूर्णांक is_bare = !is_from_ओळ(buf.buf, buf.len);

	जर (is_bare && !allow_bare) {
		ख_लिखो(मानक_त्रुटि, "corrupt mailbox\n");
		निकास(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	साठी (;;) {
		जर (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		जर (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		जर (ख_डालो(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		जर (strbuf_getwholeline(&buf, mbox, '\n')) {
			जर (ख_पूर्ण(mbox)) {
				status = 1;
				थांबा;
			}
			die_errno("cannot read mbox");
		}
		जर (!is_bare && is_from_ओळ(buf.buf, buf.len))
			थांबा; /* done with one message */
	}
	ख_बंद(output);
	परत status;
}

स्थिर पूर्णांक populate_maildir_list(संरचना string_list *list, स्थिरांक अक्षर *path)
{
	सूची *dir;
	संरचना dirent *dent;
	अक्षर *name = रिक्त;
	स्थिरांक अक्षर *subs[] = { "cur", "new", रिक्त };
	स्थिरांक अक्षर **sub;
	पूर्णांक ret = -1;

	साठी (sub = subs; *sub; ++sub) {
		मुक्त(name);
		name = xstrfmt("%s/%s", path, *sub);
		जर (!(dir = सूची_खोलो(name))) {
			जर (त्रुटि_सं == ENOENT)
				सुरूठेवा;
			error_errno("cannot opendir %s", name);
			जा out;
		}

		पर्यंत ((dent = सूची_पढ़ो(dir)) != रिक्त) {
			जर (dent->d_name[0] == '.')
				सुरूठेवा;
			मुक्त(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_घाला(list, name);
		}

		बंद_सूची(dir);
	}

	ret = 0;

out:
	मुक्त(name);
	परत ret;
}

स्थिर पूर्णांक maildir_filename_cmp(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b)
{
	पर्यंत (*a && *b) {
		जर (है_अंक(*a) && है_अंक(*b)) {
			दीर्घ पूर्णांक na, nb;
			na = म_से_दीर्घ(a, (अक्षर **)&a, 10);
			nb = म_से_दीर्घ(b, (अक्षर **)&b, 10);
			जर (na != nb)
				परत na - nb;
			/* strtol advanced our pointers */
		}
		अन्यथा {
			जर (*a != *b)
				परत (अचिन्हित अक्षर)*a - (अचिन्हित अक्षर)*b;
			a++;
			b++;
		}
	}
	परत (अचिन्हित अक्षर)*a - (अचिन्हित अक्षर)*b;
}

स्थिर पूर्णांक split_maildir(स्थिरांक अक्षर *maildir, स्थिरांक अक्षर *dir,
	पूर्णांक nr_prec, पूर्णांक skip)
{
	अक्षर *file = रिक्त;
	खाता *f = रिक्त;
	पूर्णांक ret = -1;
	संरचना string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	जर (populate_maildir_list(&list, maildir) < 0)
		जा out;

	साठी (माप_प्रकार i = 0; i < list.nr; i++) {
		अक्षर *name;

		मुक्त(file);
		file = xstrfmt("%s/%s", maildir, list.वस्तुसंच[i].string);

		f = ख_खोलो(file, "r");
		जर (!f) {
			error_errno("cannot open mail %s", file);
			जा out;
		}

		जर (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			जा out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		मुक्त(name);

		ख_बंद(f);
		f = रिक्त;
	}

	ret = skip;
out:
	जर (f)
		ख_बंद(f);
	मुक्त(file);
	string_list_clear(&list, 1);
	परत ret;
}

स्थिर पूर्णांक split_mbox(स्थिरांक अक्षर *file, स्थिरांक अक्षर *dir, पूर्णांक allow_bare,
		      पूर्णांक nr_prec, पूर्णांक skip)
{
	पूर्णांक ret = -1;
	पूर्णांक peek;

	खाता *f = !म_भेद(file, "-") ? मानक_निवेश : ख_खोलो(file, "r");
	पूर्णांक file_done = 0;

	जर (!f) {
		error_errno("cannot open mbox %s", file);
		जा out;
	}

	जर (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	करा {
		peek = ख_अक्षर_लो(f);
		जर (peek == खातापूर्ण) {
			जर (f == मानक_निवेश)
				/* empty stdin is OK */
				ret = skip;
			अन्यथा {
				ख_बंद(f);
				error(_("empty mbox: '%s'"), file);
			}
			जा out;
		}
	} पर्यंत (है_खाली(peek));
	अक्षर_वापस(peek, f);

	जर (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		जर (f != मानक_निवेश) {
			error("cannot read mbox %s", file);
			जा out;
		}
		file_done = 1;
	}

	पर्यंत (!file_done) {
		अक्षर *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		मुक्त(name);
	}

	जर (f != मानक_निवेश)
		ख_बंद(f);

	ret = skip;
out:
	परत ret;
}

पूर्णांक cmd_mailsplit(पूर्णांक argc,
		  स्थिरांक अक्षर **argv,
		  स्थिरांक अक्षर *prefix,
		  संरचना repository *repo UNUSED)
{
	पूर्णांक nr = 0, nr_prec = 4, अंक = 0;
	पूर्णांक allow_bare = 0;
	स्थिरांक अक्षर *dir = रिक्त;
	स्थिरांक अक्षर **argp;
	स्थिर स्थिरांक अक्षर *stdin_only[] = { "-", रिक्त };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	साठी (argp = argv+1; *argp; argp++) {
		स्थिरांक अक्षर *arg = *argp;

		जर (arg[0] != '-')
			थांबा;
		/* do flags here */
		जर ( arg[1] == 'd' ) {
			nr_prec = म_से_दीर्घ(arg+2, रिक्त, 10);
			जर (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			सुरूठेवा;
		} अन्यथा जर ( arg[1] == 'f' ) {
			nr = म_से_दीर्घ(arg+2, रिक्त, 10);
		} अन्यथा जर ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} अन्यथा जर (!म_भेद(arg, "--keep-cr")) {
			keep_cr = 1;
		} अन्यथा जर ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} अन्यथा जर (!म_भेद(arg, "--mboxrd")) {
			mboxrd = 1;
		} अन्यथा जर ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			थांबा;
		} अन्यथा {
			die("unknown option: %s", arg);
		}
	}

	जर ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		निवड (argc - (argp-argv)) {
		स्थिती 1:
			dir = argp[0];
			argp = stdin_only;
			थांबा;
		स्थिती 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			थांबा;
		पूर्वनिर्धारित:
			usage(git_mailsplit_usage);
		}
	} अन्यथा {
		/* New usage: if no more argument, parse stdin */
		जर ( !*argp )
			argp = stdin_only;
	}

	पर्यंत (*argp) {
		स्थिरांक अक्षर *arg = *argp++;
		संरचना stat argstat;
		पूर्णांक ret = 0;

		जर (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			जर (ret < 0) {
				error("cannot split patches from stdin");
				परत 1;
			}
			अंक += (ret - nr);
			nr = ret;
			सुरूठेवा;
		}

		जर (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			परत 1;
		}

		जर (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		अन्यथा
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		जर (ret < 0) {
			error("cannot split patches from %s", arg);
			परत 1;
		}
		अंक += (ret - nr);
		nr = ret;
	}

	म_लिखो("%d\n", अंक);

	परत 0;
}
