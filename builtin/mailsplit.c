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

static จำนวนเต็ม is_from_line(const var_na *line, จำนวนเต็ม len)
{
	const var_na *colon;

	ถ้า (len < 20 || memcmp("From ", line, 5))
		คืน 0;

	colon = line + len - 2;
	line += 5;
	สำหรับ (;;) {
		ถ้า (colon < line)
			คืน 0;
		ถ้า (*--colon == ':')
			bha_nga;
	}

	ถ้า (!isdigit(colon[-4]) ||
	    !isdigit(colon[-2]) ||
	    !isdigit(colon[-1]) ||
	    !isdigit(colon[ 1]) ||
	    !isdigit(colon[ 2]))
		คืน 0;

	/* year */
	ถ้า (strtol(colon+3, NULL, 10) <= 90)
		คืน 0;

	/* Ok, close enough */
	คืน 1;
}

static sa_nracanaa strbuf buf = STRBUF_INIT;
static จำนวนเต็ม keep_cr;
static จำนวนเต็ม mboxrd;

static จำนวนเต็ม is_gtfrom(const sa_nracanaa strbuf *buf)
{
	size_t min = strlen(">From ");
	size_t ngt;

	ถ้า (buf->len < min)
		คืน 0;

	ngt = strspn(buf->buf, ">");
	คืน ngt && starts_with(buf->buf + ngt, "From ");
}

/* Called with the first line (potentially partial)
 * already in buf[] -- normally that should begin with
 * the Unix "From " line.  Write it into the specified
 * file.
 */
static จำนวนเต็ม split_one(FILE *mbox, const var_na *name, จำนวนเต็ม allow_bare)
{
	FILE *output;
	จำนวนเต็ม fd;
	จำนวนเต็ม status = 0;
	จำนวนเต็ม is_bare = !is_from_line(buf.buf, buf.len);

	ถ้า (is_bare && !allow_bare) {
		fprintf(stderr, "corrupt mailbox\n");
		exit(1);
	}
	fd = xopen(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
	output = xfdopen(fd, "w");

	/* Copy it out, while searching for a line that begins with
	 * "From " and having something that looks like a date format.
	 */
	สำหรับ (;;) {
		ถ้า (!keep_cr && buf.len > 1 && buf.buf[buf.len-1] == '\n' &&
			buf.buf[buf.len-2] == '\r') {
			strbuf_setlen(&buf, buf.len-2);
			strbuf_addch(&buf, '\n');
		}

		ถ้า (mboxrd && is_gtfrom(&buf))
			strbuf_remove(&buf, 0, 1);

		ถ้า (fwrite(buf.buf, 1, buf.len, output) != buf.len)
			die_errno("cannot write output");

		ถ้า (strbuf_getwholeline(&buf, mbox, '\n')) {
			ถ้า (feof(mbox)) {
				status = 1;
				bha_nga;
			}
			die_errno("cannot read mbox");
		}
		ถ้า (!is_bare && is_from_line(buf.buf, buf.len))
			bha_nga; /* done with one message */
	}
	fclose(output);
	คืน status;
}

static จำนวนเต็ม populate_maildir_list(sa_nracanaa string_list *list, const var_na *path)
{
	DIR *dir;
	sa_nracanaa dirent *dent;
	var_na *name = NULL;
	const var_na *subs[] = { "cur", "new", NULL };
	const var_na **sub;
	จำนวนเต็ม ret = -1;

	สำหรับ (sub = subs; *sub; ++sub) {
		free(name);
		name = xstrfmt("%s/%s", path, *sub);
		ถ้า (!(dir = opendir(name))) {
			ถ้า (errno == ENOENT)
				continue;
			error_errno("cannot opendir %s", name);
			goto out;
		}

		ขณะ ((dent = readdir(dir)) != NULL) {
			ถ้า (dent->d_name[0] == '.')
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
	คืน ret;
}

static จำนวนเต็ม maildir_filename_cmp(const var_na *a, const var_na *b)
{
	ขณะ (*a && *b) {
		ถ้า (isdigit(*a) && isdigit(*b)) {
			long จำนวนเต็ม na, nb;
			na = strtol(a, (var_na **)&a, 10);
			nb = strtol(b, (var_na **)&b, 10);
			ถ้า (na != nb)
				คืน na - nb;
			/* strtol advanced our pointers */
		}
		มิฉะนั้น {
			ถ้า (*a != *b)
				คืน (unsigned var_na)*a - (unsigned var_na)*b;
			a++;
			b++;
		}
	}
	คืน (unsigned var_na)*a - (unsigned var_na)*b;
}

static จำนวนเต็ม split_maildir(const var_na *maildir, const var_na *dir,
	จำนวนเต็ม nr_prec, จำนวนเต็ม skip)
{
	var_na *file = NULL;
	FILE *f = NULL;
	จำนวนเต็ม ret = -1;
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

	list.cmp = maildir_filename_cmp;

	ถ้า (populate_maildir_list(&list, maildir) < 0)
		goto out;

	สำหรับ (size_t i = 0; i < list.nr; i++) {
		var_na *name;

		free(file);
		file = xstrfmt("%s/%s", maildir, list.items[i].string);

		f = fopen(file, "r");
		ถ้า (!f) {
			error_errno("cannot open mail %s", file);
			goto out;
		}

		ถ้า (strbuf_getwholeline(&buf, f, '\n')) {
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
	ถ้า (f)
		fclose(f);
	free(file);
	string_list_clear(&list, 1);
	คืน ret;
}

static จำนวนเต็ม split_mbox(const var_na *file, const var_na *dir, จำนวนเต็ม allow_bare,
		      จำนวนเต็ม nr_prec, จำนวนเต็ม skip)
{
	จำนวนเต็ม ret = -1;
	จำนวนเต็ม peek;

	FILE *f = !strcmp(file, "-") ? stdin : fopen(file, "r");
	จำนวนเต็ม file_done = 0;

	ถ้า (!f) {
		error_errno("cannot open mbox %s", file);
		goto out;
	}

	ถ้า (isatty(fileno(f)))
		warning(_("reading patches from stdin/tty..."));

	do {
		peek = fgetc(f);
		ถ้า (peek == EOF) {
			ถ้า (f == stdin)
				/* empty stdin is OK */
				ret = skip;
			มิฉะนั้น {
				fclose(f);
				error(_("empty mbox: '%s'"), file);
			}
			goto out;
		}
	} ขณะ (isspace(peek));
	ungetc(peek, f);

	ถ้า (strbuf_getwholeline(&buf, f, '\n')) {
		/* empty stdin is OK */
		ถ้า (f != stdin) {
			error("cannot read mbox %s", file);
			goto out;
		}
		file_done = 1;
	}

	ขณะ (!file_done) {
		var_na *name = xstrfmt("%s/%0*d", dir, nr_prec, ++skip);
		file_done = split_one(f, name, allow_bare);
		free(name);
	}

	ถ้า (f != stdin)
		fclose(f);

	ret = skip;
out:
	คืน ret;
}

จำนวนเต็ม cmd_mailsplit(จำนวนเต็ม argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม nr = 0, nr_prec = 4, num = 0;
	จำนวนเต็ม allow_bare = 0;
	const var_na *dir = NULL;
	const var_na **argp;
	static const var_na *stdin_only[] = { "-", NULL };

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, git_mailsplit_usage);

	สำหรับ (argp = argv+1; *argp; argp++) {
		const var_na *arg = *argp;

		ถ้า (arg[0] != '-')
			bha_nga;
		/* do flags here */
		ถ้า ( arg[1] == 'd' ) {
			nr_prec = strtol(arg+2, NULL, 10);
			ถ้า (nr_prec < 3 || 10 <= nr_prec)
				usage(git_mailsplit_usage);
			continue;
		} มิฉะนั้น ถ้า ( arg[1] == 'f' ) {
			nr = strtol(arg+2, NULL, 10);
		} มิฉะนั้น ถ้า ( arg[1] == 'b' && !arg[2] ) {
			allow_bare = 1;
		} มิฉะนั้น ถ้า (!strcmp(arg, "--keep-cr")) {
			keep_cr = 1;
		} มิฉะนั้น ถ้า ( arg[1] == 'o' && arg[2] ) {
			dir = arg+2;
		} มิฉะนั้น ถ้า (!strcmp(arg, "--mboxrd")) {
			mboxrd = 1;
		} มิฉะนั้น ถ้า ( arg[1] == '-' && !arg[2] ) {
			argp++;	/* -- marks end of options */
			bha_nga;
		} มิฉะนั้น {
			die("unknown option: %s", arg);
		}
	}

	ถ้า ( !dir ) {
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
	} มิฉะนั้น {
		/* New usage: if no more argument, parse stdin */
		ถ้า ( !*argp )
			argp = stdin_only;
	}

	ขณะ (*argp) {
		const var_na *arg = *argp++;
		sa_nracanaa stat argstat;
		จำนวนเต็ม ret = 0;

		ถ้า (arg[0] == '-' && arg[1] == 0) {
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);
			ถ้า (ret < 0) {
				error("cannot split patches from stdin");
				คืน 1;
			}
			num += (ret - nr);
			nr = ret;
			continue;
		}

		ถ้า (stat(arg, &argstat) == -1) {
			error_errno("cannot stat %s", arg);
			คืน 1;
		}

		ถ้า (S_ISDIR(argstat.st_mode))
			ret = split_maildir(arg, dir, nr_prec, nr);
		มิฉะนั้น
			ret = split_mbox(arg, dir, allow_bare, nr_prec, nr);

		ถ้า (ret < 0) {
			error("cannot split patches from %s", arg);
			คืน 1;
		}
		num += (ret - nr);
		nr = ret;
	}

	printf("%d\n", num);

	คืน 0;
}
