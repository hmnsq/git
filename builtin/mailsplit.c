/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

ਸਥਿਰ ਪੂਰਨ is_from_ਸਤਰ(ਅਚਲ ਅੱਖਰ *ਸਤਰ, ਪੂਰਨ len)
{
	ਅਚਲ ਅੱਖਰ *colon;

	ਜੇ (len < 20 || memcmp("From ", ਸਤਰ, 5))
		ਵਾਪਸ 0;

	colon = ਸਤਰ + len - 2;
	ਸਤਰ += 5;
	ਲਈ (;;) {
		ਜੇ (colon < ਸਤਰ)
			ਵਾਪਸ 0;
		ਜੇ (*--colon == ':')
			ਰੋਕੋ;
	}

	ਜੇ (!ਅੰਕ_ਹੈ(colon[-4]) ||
	    !ਅੰਕ_ਹੈ(colon[-2]) ||
	    !ਅੰਕ_ਹੈ(colon[-1]) ||
	    !ਅੰਕ_ਹੈ(colon[ 1]) ||
	    !ਅੰਕ_ਹੈ(colon[ 2]))
		ਵਾਪਸ 0;

	/* year */
	ਜੇ (strtol(colon+3, ਨਿਰਾ, 10) <= 90)
		ਵਾਪਸ 0;

	/* Ok, close enough */
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਬਣਤਰ strbuf buf = STRBUF_INIT;
ਸਥਿਰ ਪੂਰਨ keep_cr;
ਸਥਿਰ ਪੂਰਨ mboxrd;

ਸਥਿਰ ਪੂਰਨ is_gtfrom(ਅਚਲ ਬਣਤਰ strbuf *buf)
{
	ਮਾਪ_t min = ਲੜੀ_ਲੰਬਾਈ(">From ");
	ਮਾਪ_t ngt;

	ਜੇ (buf->len < min)
		ਵਾਪਸ 0;

	ngt = strspn(buf->buf, ">");
	ਵਾਪਸ ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_one(FILE *mbox, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ allow_bare)
{
	FILE *output;
	ਪੂਰਨ fd;
	ਪੂਰਨ ਸਥਿਤੀ = 0;
	ਪੂਰਨ is_bare = !is_from_ਸਤਰ(buf.buf, buf.len);

	ਜੇ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	ਲਈ (;;) {
		ਜੇ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ਜੇ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ਜੇ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ਜੇ (strbuf_getwholeline(&buf, mbox, '\n')) {
			ਜੇ (feof(mbox)) {
				ਸਥਿਤੀ = 1;
				ਰੋਕੋ;
			}
			die_errno("cannot read mbox");
		}
		ਜੇ (!is_bare && is_from_ਸਤਰ(buf.buf, buf.len))
			ਰੋਕੋ; /* done with one message */
	}
	fclose(output);
	ਵਾਪਸ ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ populate_maildir_list(ਬਣਤਰ string_list *list, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	DIR *dir;
	ਬਣਤਰ dirent *dent;
	ਅੱਖਰ *name = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *subs[] = { "cur", "new", ਨਿਰਾ };
	ਅਚਲ ਅੱਖਰ **sub;
	ਪੂਰਨ ret = -1;

	ਲਈ (sub = subs; *sub; ++sub) {
		ਮੁਕਤ(name);
		name = xstrfmt("%s/%s", ਰਸਤਾ, *sub);
		ਜੇ (!(dir = opendir(name))) {
			ਜੇ (errno == ENOENT)
				ਜਾਰੀ;
			error_errno("cannot opendir %s", name);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜਦੋਂ ((dent = readdir(dir)) != ਨਿਰਾ) {
			ਜੇ (dent->d_name[0] == '.')
				ਜਾਰੀ;
			ਮੁਕਤ(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_ਪਾਓ(list, name);
		}

		closedir(dir);
	}

	ret = 0;

ਬਾਹਰਲਾ:
	ਮੁਕਤ(name);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ maildir_filename_cmp(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b)
{
	ਜਦੋਂ (*a && *b) {
		ਜੇ (ਅੰਕ_ਹੈ(*a) && ਅੰਕ_ਹੈ(*b)) {
			ਲੰਮਾ ਪੂਰਨ na, nb;
			na = strtol(a, (ਅੱਖਰ **)&a, 10);
			nb = strtol(b, (ਅੱਖਰ **)&b, 10);
			ਜੇ (na != nb)
				ਵਾਪਸ na - nb;
			/* strtol advanced our pointers */
		}
		ਨਹੀਂਤਾਂ {
			ਜੇ (*a != *b)
				ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*a - (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*b;
			a++;
			b++;
		}
	}
	ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*a - (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*b;
}

ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_maildir(ਅਚਲ ਅੱਖਰ *maildir, ਅਚਲ ਅੱਖਰ *dir,
	ਪੂਰਨ nr_prec, ਪੂਰਨ skip)
{
	ਅੱਖਰ *file = ਨਿਰਾ;
	FILE *f = ਨਿਰਾ;
	ਪੂਰਨ ret = -1;
	ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ਜੇ (populate_maildir_list(&list, maildir) < 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਲਈ (ਮਾਪ_t i = 0; i < list.nr; i++) {
		ਅੱਖਰ *name;

		ਮੁਕਤ(file);
		file = xstrfmt("%s/%s", maildir, list.ਚੀਜ਼ਾਂ[i].string);

		f = fopen(file, "r");
		ਜੇ (!f) {
			error_errno("cannot open mail %s", file);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		ਵੰਡੋ_one(f, name, 1);
		ਮੁਕਤ(name);

		fclose(f);
		f = ਨਿਰਾ;
	}

	ret = skip;
ਬਾਹਰਲਾ:
	ਜੇ (f)
		fclose(f);
	ਮੁਕਤ(file);
	string_list_clear(&list, 1);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_mbox(ਅਚਲ ਅੱਖਰ *file, ਅਚਲ ਅੱਖਰ *dir, ਪੂਰਨ allow_bare,
		      ਪੂਰਨ nr_prec, ਪੂਰਨ skip)
{
	ਪੂਰਨ ret = -1;
	ਪੂਰਨ peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ਪੂਰਨ file_ਪੂਰਾ = 0;

	ਜੇ (!f) {
		error_errno("cannot open mbox %s", file);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	ਕਰੋ {
		peek = fgetc(f);
		ਜੇ (peek == EOF) {
			ਜੇ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			ਨਹੀਂਤਾਂ {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	} ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ(peek));
	ungetc(peek, f);

	ਜੇ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ਜੇ (f != stdin) {
			error("cannot read mbox %s", file);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		file_ਪੂਰਾ = 1;
	}

	ਜਦੋਂ (!file_ਪੂਰਾ) {
		ਅੱਖਰ *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_ਪੂਰਾ = ਵੰਡੋ_one(f, name, allow_bare);
		ਮੁਕਤ(name);
	}

	ਜੇ (f != stdin)
		fclose(f);

	ret = skip;
ਬਾਹਰਲਾ:
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_mailsplit(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ nr = 0, nr_prec = 4, ਅੰਕ = 0;
	ਪੂਰਨ allow_bare = 0;
	ਅਚਲ ਅੱਖਰ *dir = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ **argp;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *stdin_only[] = { "-", ਨਿਰਾ };

	BUG_ON_NON_ਖਾਲੀ_ਥਾਵਾਂ_ਅਗੇਤਰ(ਅਗੇਤਰ);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	ਲਈ (argp = argv+1; *argp; argp++) {
		ਅਚਲ ਅੱਖਰ *arg = *argp;

		ਜੇ (arg[0] != '-')
			ਰੋਕੋ;
		/* do flags here */
		ਜੇ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, ਨਿਰਾ, 10);
			ਜੇ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ ਜੇ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, ਨਿਰਾ, 10);
		} ਨਹੀਂਤਾਂ ਜੇ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} ਨਹੀਂਤਾਂ ਜੇ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} ਨਹੀਂਤਾਂ ਜੇ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			ਰੋਕੋ;
		} ਨਹੀਂਤਾਂ {
			die("unknown option: %s", arg);
		}
	}

	ਜੇ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		ਚੋਣ (argc - (argp-argv)) {
		ਹਾਲਤ 1:
			dir = argp[0];
			argp = stdin_only;
			ਰੋਕੋ;
		ਹਾਲਤ 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			ਰੋਕੋ;
		ਮੂਲ:
			usage(git_mailsplit_usage);
		}
	} ਨਹੀਂਤਾਂ {
		/* New usage: if no more argument, parse stdin */
		ਜੇ ( !*argp )
			argp = stdin_only;
	}

	ਜਦੋਂ (*argp) {
		ਅਚਲ ਅੱਖਰ *arg = *argp++;
		ਬਣਤਰ stat argstat;
		ਪੂਰਨ ret = 0;

		ਜੇ (arg[0] == '-' && arg[1] == 0) {
			ret = ਵੰਡੋ_mbox(arg, dir, allow_bare, nr_prec, nr);
			ਜੇ (ret < 0) {
				error("cannot split patches from stdin");
				ਵਾਪਸ 1;
			}
			ਅੰਕ += (ret - nr);
			nr = ret;
			ਜਾਰੀ;
		}

		ਜੇ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ਵਾਪਸ 1;
		}

		ਜੇ (S_ISDIR(argstat.st_ਬਹੁਲਕ))
			ret = ਵੰਡੋ_maildir(arg, dir, nr_prec, nr);
		ਨਹੀਂਤਾਂ
			ret = ਵੰਡੋ_mbox(arg, dir, allow_bare, nr_prec, nr);

		ਜੇ (ret < 0) {
			error("cannot split patches from %s", arg);
			ਵਾਪਸ 1;
		}
		ਅੰਕ += (ret - nr);
		nr = ret;
	}

	ਲਿਖੋ("%d\n", ਅੰਕ);

	ਵਾਪਸ 0;
}
