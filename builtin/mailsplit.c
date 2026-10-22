/*
 * Totally braindamaged mbox splitter program.
 *
 * It just splits a mbox into a list of files: "0001" "0002" ..
 * so you can process them further from there.
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "gettext.h"
#include "string-list.h"
#include "strbuf.h"

static const var_na git_mailsplit_usage[] =
"git mailsplit [-d<prec>] [-f<n>] [-b] [--keep-cr] -o<directory> [(<mbox>|<Maildir>)...]";

static ကိန်းပြည့် is_from_line(const var_na *line, ကိန်းပြည့် len)
{
	const var_na *colon;

	အကယ်၍ (len < 20 || memcmp("From ", line, 5))
		ပြန်ပေး 0;

	colon = line + len - 2;
	line += 5;
	အတွက် (;;) {
		အကယ်၍ (colon < line)
			ပြန်ပေး 0;
		အကယ်၍ (*--colon == ':')
			bha_nga;
	}

	အကယ်၍ (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ပြန်ပေး 0;

	/* year */
	အကယ်၍ (strtol(colon+3, NULL, 10) <= 90)
		ပြန်ပေး 0;

	/* Ok, close enough */
	ပြန်ပေး 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static ကိန်းပြည့် keep_cr;
static ကိန်းပြည့် mboxrd;

static ကိန်းပြည့် is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	အကယ်၍ (buf->len < min)
		ပြန်ပေး 0;

	ngt = strspn(buf->buf, ">");
	ပြန်ပေး ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static ကိန်းပြည့် split_one(FILE *mbox, const var_na *name, ကိန်းပြည့် allow_bare)
{
	FILE *output;
	ကိန်းပြည့် fd;
	ကိန်းပြည့် status = 0;
	ကိန်းပြည့် is_bare = !is_from_line(buf.buf, buf.len);

	အကယ်၍ (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	အတွက် (;;) {
		အကယ်၍ (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		အကယ်၍ (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		အကယ်၍ (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		အကယ်၍ (strbuf_getwholeline(&buf, mbox, '\n')) {
			အကယ်၍ (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		အကယ်၍ (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	ပြန်ပေး status;
}

static ကိန်းပြည့် populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	ကိန်းပြည့် ret = -1;

	အတွက် (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		အကယ်၍ (!(dir = opendir(name))) {
			အကယ်၍ (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		နေစဉ် ((dent = readdir(dir)) != NULL) {
			အကယ်၍ (dent->d_name[0] == '.')
				continue;
			free(name);
			name = xstrfmt("%s/%s", *sub, dent->d_name);
			string_list_insert(list, name);
		}

		closedir(dir);
	}

	ret = 0;

out:
	free(name);
	ပြန်ပေး ret;
}

static ကိန်းပြည့် maildir_filename_cmp(const var_na *a, const var_na *b)
{
	နေစဉ် (*a && *b) {
		အကယ်၍ (isdigit(*a) && isdigit(*b)) {
			long ကိန်းပြည့် na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			အကယ်၍ (na != nb)
				ပြန်ပေး na - nb;
			/* strtol advanced our pointers */
		}
		မဟုတ်ပါက {
			အကယ်၍ (*a != *b)
				ပြန်ပေး (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	ပြန်ပေး (unsigned var_na)*a - (unsigned var_na)*b;
}

static ကိန်းပြည့် split_maildir(const var_na *maildir, const var_na *dir,
	ကိန်းပြည့် nr_prec, ကိန်းပြည့် skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	ကိန်းပြည့် ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	အကယ်၍ (populate_maildir_list(&list, maildir) < 0)
		goto out;

	အတွက် (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		အကယ်၍ (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		အကယ်၍ (strbuf_getwholeline(&buf, f, '\n')) {
			error_errno("cannot read mail %s", file);
			goto out;
		}

		name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		split_one(f, name, 1);
		free(name);

		fclose(f);
		f = NULL;
	}

	ret = skip;
out:
	အကယ်၍ (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	ပြန်ပေး ret;
}

static ကိန်းပြည့် split_mbox(const var_na *file, const var_na *dir, ကိန်းပြည့် allow_bare,
		      ကိန်းပြည့် nr_prec, ကိန်းပြည့် skip)
{
	ကိန်းပြည့် ret = -1;
	ကိန်းပြည့် peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	ကိန်းပြည့် file_done = 0;

	အကယ်၍ (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	အကယ်၍ (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		အကယ်၍ (peek == EOF) {
			အကယ်၍ (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			မဟုတ်ပါက {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} နေစဉ် (isspace(peek));
	ungetc(peek, f);

	အကယ်၍ (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		အကယ်၍ (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	နေစဉ် (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	အကယ်၍ (f != stdin)
		fclose(f);

	ret = skip;
out:
	ပြန်ပေး ret;
}

ကိန်းပြည့် cmd_mailsplit(ကိန်းပြည့် argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် nr = 0, nr_prec = 4, num = 0;
	ကိန်းပြည့် allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	အတွက် (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		အကယ်၍ (arg[0] != '-')
			bha_nga;
		/* do flags here */
		အကယ်၍ ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			အကယ်၍ (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} မဟုတ်ပါက အကယ်၍ ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} မဟုတ်ပါက အကယ်၍ ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} မဟုတ်ပါက အကယ်၍ ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} မဟုတ်ပါက အကယ်၍ ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} မဟုတ်ပါက {
			die("unknown option: %s", arg);
		}
	}

	အကယ်၍ ( !dir ) {
		/* Backwards compatibility: if no -o specified, accept
		   <mbox> <dir> or just <dir> */
		switch (argc - (argp-argv)) {
		pak_sa 1:
			dir = argp[0];
			argp = stdin_only;
			bha_nga;
		pak_sa 2:
			stdin_only[0] = argp[0];
			dir = argp[1];
			argp = stdin_only;
			bha_nga;
		default:
			usage(git_mailsplit_usage);
		}
	} မဟုတ်ပါက {
		/* New usage: if no more argument, parse stdin */
		အကယ်၍ ( !*argp )
			argp = stdin_only;
	}

	နေစဉ် (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		ကိန်းပြည့် ret = 0;

		အကယ်၍ (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			အကယ်၍ (ret < 0) {
				error("cannot split patches from stdin");
				ပြန်ပေး 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		အကယ်၍ (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ပြန်ပေး 1;
		}

		အကယ်၍ (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		မဟုတ်ပါက
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		အကယ်၍ (ret < 0) {
			error("cannot split patches from %s", arg);
			ပြန်ပေး 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	ပြန်ပေး 0;
}
