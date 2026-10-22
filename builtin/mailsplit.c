/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "string-list.h"
#समावेश "strbuf.h"

अटल स्थिर अक्षर git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

अटल पूर्णांक is_from_पंक्ति(स्थिर अक्षर *पंक्ति, पूर्णांक len)
{
	स्थिर अक्षर *colon;

	अगर (len < 20 || स_भेद("From ", पंक्ति, 5))
		वापस 0;

	colon = पंक्ति + len - 2;
	पंक्ति += 5;
	क्रम (;;) {
		अगर (colon < पंक्ति)
			वापस 0;
		अगर (*--colon == ':')
			अवरोध;
	}

	अगर (!है_अंक(colon[-4]) ||
	    !है_अंक(colon[-2]) ||
	    !है_अंक(colon[-1]) ||
	    !है_अंक(colon[ 1]) ||
	    !है_अंक(colon[ 2]))
		वापस 0;

	/* year */
	अगर (म_से_दीर्घ(colon+3, शून्य, 10) <= 90)
		वापस 0;

	/* Ok, close enough */
	वापस 1;
}

अटल काष्ठा strbuf buf = STRBUF_INIT;
अटल पूर्णांक keep_cr;
अटल पूर्णांक mboxrd;

अटल पूर्णांक is_gtfrom(स्थिर काष्ठा strbuf *buf)
{
	माप_प्रकार min = म_माप(">From ");
	माप_प्रकार ngt;

	अगर (buf->len < min)
		वापस 0;

	ngt = म_अखोज(buf->buf, ">");
	वापस ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
अटल पूर्णांक split_one(खाता *mbox, स्थिर अक्षर *name, पूर्णांक allow_bare)
{
	खाता *output;
	पूर्णांक fd;
	पूर्णांक status = 0;
	पूर्णांक is_bare = !is_from_पंक्ति(buf.buf, buf.len);

	अगर (is_bare && !allow_bare) {
		ख_लिखो(मानक_त्रुटि, "corrupt mailbox\n");
		निकास(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	क्रम (;;) {
		अगर (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		अगर (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		अगर (ख_डालो(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		अगर (strbuf_getwholeline(&buf, mbox, '\n')) {
			अगर (ख_पूर्ण(mbox)) {
				status = 1;
				अवरोध;
			}
			die_errno("cannot read mbox");
		}
		अगर (!is_bare && is_from_पंक्ति(buf.buf, buf.len))
			अवरोध; /* done with one message */
	}
	ख_बंद(output);
	वापस status;
}

अटल पूर्णांक populate_maildir_list(काष्ठा string_list *list, स्थिर अक्षर *path)
{
	सूची *dir;
	काष्ठा dirent *dent;
	अक्षर *name = शून्य;
	स्थिर अक्षर *subs[] = { "cur", "new", शून्य };
	स्थिर अक्षर **sub;
	पूर्णांक ret = -1;

	क्रम (sub = subs; *sub; ++sub) {
		मुक्त(name);
		name = xstrfmt("%s/%s", path, *sub);
		अगर (!(dir = सूची_खोलो(name))) {
			अगर (त्रुटि_सं == ENOENT)
				जारी;
			error_errno("cannot opendir %s", name);
			जाओ out;
		}

		जबतक ((dent = सूची_पढ़ो(dir)) != शून्य) {
			अगर (dent->d_name[0] == '.')
				जारी;
			मुक्त(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_जोड़ो(list, name);
		}

		बंद_सूची(dir);
	}

	ret = 0;

out:
	मुक्त(name);
	वापस ret;
}

अटल पूर्णांक maildir_filename_cmp(स्थिर अक्षर *a, स्थिर अक्षर *b)
{
	जबतक (*a && *b) {
		अगर (है_अंक(*a) && है_अंक(*b)) {
			दीर्घ पूर्णांक na, nb;
			na = म_से_दीर्घ(a, (अक्षर **)&a, 10);
			nb = म_से_दीर्घ(b, (अक्षर **)&b, 10);
			अगर (na != nb)
				वापस na - nb;
			/* strtol advanced our pointers */
		}
		अन्यथा {
			अगर (*a != *b)
				वापस (अचिन्हित अक्षर)*a - (अचिन्हित अक्षर)*b;
			a++;
			b++;
		}
	}
	वापस (अचिन्हित अक्षर)*a - (अचिन्हित अक्षर)*b;
}

अटल पूर्णांक split_maildir(स्थिर अक्षर *maildir, स्थिर अक्षर *dir,
	पूर्णांक nr_prec, पूर्णांक skip)
{
	अक्षर *file = शून्य;
	खाता *f = शून्य;
	पूर्णांक ret = -1;
	काष्ठा string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	अगर (populate_maildir_list(&list, maildir) < 0)
		जाओ out;

	क्रम (माप_प्रकार i = 0; i < list.nr; i++) {
		अक्षर *name;

		मुक्त(file);
		file = xstrfmt("%s/%s", maildir, list.वस्तुएँ[i].string);

		f = ख_खोलो(file, "r");
		अगर (!f) {
			error_errno("cannot open mail %s", file);
			जाओ out;
		}

		अगर (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			जाओ out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		मुक्त(name);

		ख_बंद(f);
		f = शून्य;
	}

	ret = skip;
out:
	अगर (f)
		ख_बंद(f);
	मुक्त(file);
	string_list_clear(&list, 1);
	वापस ret;
}

अटल पूर्णांक split_mbox(स्थिर अक्षर *file, स्थिर अक्षर *dir, पूर्णांक allow_bare,
		      पूर्णांक nr_prec, पूर्णांक skip)
{
	पूर्णांक ret = -1;
	पूर्णांक peek;

	खाता *f = !म_भेद(file, "-") ? मानक_निवेश : ख_खोलो(file, "r");
	पूर्णांक file_done = 0;

	अगर (!f) {
		error_errno("cannot open mbox %s", file);
		जाओ out;
	}

	अगर (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	करो {
		peek = ख_अक्षर_लो(f);
		अगर (peek == खातापूर्ण) {
			अगर (f == मानक_निवेश)
				/* empty stdin is OK */
				ret = skip;
			अन्यथा {
				ख_बंद(f);
				error(_("empty mbox: '%s'"), file);
			}
			जाओ out;
		}
	} जबतक (है_खाली(peek));
	अक्षर_वापस(peek, f);

	अगर (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		अगर (f != मानक_निवेश) {
			error("cannot read mbox %s", file);
			जाओ out;
		}
		file_done = 1;
	}

	जबतक (!file_done) {
		अक्षर *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		मुक्त(name);
	}

	अगर (f != मानक_निवेश)
		ख_बंद(f);

	ret = skip;
out:
	वापस ret;
}

पूर्णांक cmd_mailsplit(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	पूर्णांक nr = 0, nr_prec = 4, अंक = 0;
	पूर्णांक allow_bare = 0;
	स्थिर अक्षर *dir = शून्य;
	स्थिर अक्षर **argp;
	अटल स्थिर अक्षर *stdin_only[] = { "-", शून्य };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	क्रम (argp = argv+1; *argp; argp++) {
		स्थिर अक्षर *arg = *argp;

		अगर (arg[0] != '-')
			अवरोध;
		/* do flags here */
		अगर ( arg[1] == 'd' ) {
			nr_prec = म_से_दीर्घ(arg+2, शून्य, 10);
			अगर (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			जारी;
		} अन्यथा अगर ( arg[1] == 'f' ) {
			nr = म_से_दीर्घ(arg+2, शून्य, 10);
		} अन्यथा अगर ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} अन्यथा अगर (!म_भेद(arg, "--keep-cr")) {
			keep_cr = 1;
		} अन्यथा अगर ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} अन्यथा अगर (!म_भेद(arg, "--mboxrd")) {
			mboxrd = 1;
		} अन्यथा अगर ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			अवरोध;
		} अन्यथा {
			die("unknown option: %s", arg);
		}
	}

	अगर ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		चयन (argc - (argp-argv)) {
		हाल 1:
			dir = argp[0];
			argp = stdin_only;
			अवरोध;
		हाल 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			अवरोध;
		शेष:
			usage(git_mailsplit_usage);
		}
	} अन्यथा {
		/* New usage: if no more argument, parse stdin */
		अगर ( !*argp )
			argp = stdin_only;
	}

	जबतक (*argp) {
		स्थिर अक्षर *arg = *argp++;
		काष्ठा stat argstat;
		पूर्णांक ret = 0;

		अगर (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			अगर (ret < 0) {
				error("cannot split patches from stdin");
				वापस 1;
			}
			अंक += (ret - nr);
			nr = ret;
			जारी;
		}

		अगर (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			वापस 1;
		}

		अगर (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		अन्यथा
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		अगर (ret < 0) {
			error("cannot split patches from %s", arg);
			वापस 1;
		}
		अंक += (ret - nr);
		nr = ret;
	}

	म_लिखो("%d\n", अंक);

	वापस 0;
}
