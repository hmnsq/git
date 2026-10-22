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

static පූර්ණ_සංඛ්‍යා is_from_line(const var_na *line, පූර්ණ_සංඛ්‍යා len)
{
	const var_na *colon;

	නම් (len < 20 || memcmp("From ", line, 5))
		ආපසු 0;

	colon = line + len - 2;
	line += 5;
	සඳහා (;;) {
		නම් (colon < line)
			ආපසු 0;
		නම් (*--colon == ':')
			bha_nga;
	}

	නම් (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		ආපසු 0;

	/* year */
	නම් (strtol(colon+3, NULL, 10) <= 90)
		ආපසු 0;

	/* Ok, close enough */
	ආපසු 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static පූර්ණ_සංඛ්‍යා keep_cr;
static පූර්ණ_සංඛ්‍යා mboxrd;

static පූර්ණ_සංඛ්‍යා is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	නම් (buf->len < min)
		ආපසු 0;

	ngt = strspn(buf->buf, ">");
	ආපසු ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static පූර්ණ_සංඛ්‍යා split_one(FILE *mbox, const var_na *name, පූර්ණ_සංඛ්‍යා allow_bare)
{
	FILE *output;
	පූර්ණ_සංඛ්‍යා fd;
	පූර්ණ_සංඛ්‍යා status = 0;
	පූර්ණ_සංඛ්‍යා is_bare = !is_from_line(buf.buf, buf.len);

	නම් (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	සඳහා (;;) {
		නම් (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		නම් (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		නම් (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		නම් (strbuf_getwholeline(&buf, mbox, '\n')) {
			නම් (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		නම් (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	ආපසු status;
}

static පූර්ණ_සංඛ්‍යා populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	පූර්ණ_සංඛ්‍යා ret = -1;

	සඳහා (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		නම් (!(dir = opendir(name))) {
			නම් (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		අතරතුර ((dent = readdir(dir)) != NULL) {
			නම් (dent->d_name[0] == '.')
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
	ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා maildir_filename_cmp(const var_na *a, const var_na *b)
{
	අතරතුර (*a && *b) {
		නම් (isdigit(*a) && isdigit(*b)) {
			long පූර්ණ_සංඛ්‍යා na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			නම් (na != nb)
				ආපසු na - nb;
			/* strtol advanced our pointers */
		}
		නැතහොත් {
			නම් (*a != *b)
				ආපසු (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	ආපසු (unsigned var_na)*a - (unsigned var_na)*b;
}

static පූර්ණ_සංඛ්‍යා split_maildir(const var_na *maildir, const var_na *dir,
	පූර්ණ_සංඛ්‍යා nr_prec, පූර්ණ_සංඛ්‍යා skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	පූර්ණ_සංඛ්‍යා ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	නම් (populate_maildir_list(&list, maildir) < 0)
		goto out;

	සඳහා (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		නම් (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		නම් (strbuf_getwholeline(&buf, f, '\n')) {
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
	නම් (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා split_mbox(const var_na *file, const var_na *dir, පූර්ණ_සංඛ්‍යා allow_bare,
		      පූර්ණ_සංඛ්‍යා nr_prec, පූර්ණ_සංඛ්‍යා skip)
{
	පූර්ණ_සංඛ්‍යා ret = -1;
	පූර්ණ_සංඛ්‍යා peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	පූර්ණ_සංඛ්‍යා file_done = 0;

	නම් (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	නම් (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		නම් (peek == EOF) {
			නම් (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			නැතහොත් {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} අතරතුර (isspace(peek));
	ungetc(peek, f);

	නම් (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		නම් (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	අතරතුර (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	නම් (f != stdin)
		fclose(f);

	ret = skip;
out:
	ආපසු ret;
}

පූර්ණ_සංඛ්‍යා cmd_mailsplit(පූර්ණ_සංඛ්‍යා argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	පූර්ණ_සංඛ්‍යා nr = 0, nr_prec = 4, num = 0;
	පූර්ණ_සංඛ්‍යා allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	සඳහා (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		නම් (arg[0] != '-')
			bha_nga;
		/* do flags here */
		නම් ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			නම් (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} නැතහොත් නම් ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} නැතහොත් නම් ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} නැතහොත් නම් (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} නැතහොත් නම් ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} නැතහොත් නම් (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} නැතහොත් නම් ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} නැතහොත් {
			die("unknown option: %s", arg);
		}
	}

	නම් ( !dir ) {
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
	} නැතහොත් {
		/* New usage: if no more argument, parse stdin */
		නම් ( !*argp )
			argp = stdin_only;
	}

	අතරතුර (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		පූර්ණ_සංඛ්‍යා ret = 0;

		නම් (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			නම් (ret < 0) {
				error("cannot split patches from stdin");
				ආපසු 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		නම් (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			ආපසු 1;
		}

		නම් (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		නැතහොත්
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		නම් (ret < 0) {
			error("cannot split patches from %s", arg);
			ආපසු 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	ආපසු 0;
}
