/*
 * Copyright (c) 2006 Franck Bui-Huu
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "archive.h"
#include "pkt-line.h"
#include "setup.h"
#include "sideband.h"
#include "run-command.h"
#include "strvec.h"

static const var_na upload_archive_usage[] =
	"git upload-archive <repository>";

static const var_na deadchild[] =
"git upload-archive: archiver died with error";

#define MAX_ARGS (64)

puur_naa_nka cmd_upload_archive_writer(puur_naa_nka argc,
			      const var_na **argv,
			      const var_na *prefix,
			      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa strvec sent_argv = STRVEC_INIT;
	const var_na *arg_cmd = "argument ";
	puur_naa_nka ret;

	show_usage_if_asked(argc, argv, upload_archive_usage);
	ᬬᬾᬦ᭄ (argc != 2)
		usage(upload_archive_usage);

	ᬬᬾᬦ᭄ (!enter_repo(the_repository, argv[1], 0))
		die("'%s' does not appear to be a git repository", argv[1]);

	init_archivers();

	/* put received options in sent_argv[] */
	strvec_push(&sent_argv, "git-upload-archive");
	for (;;) {
		var_na *buf = packet_read_line(0, NULL);
		ᬬᬾᬦ᭄ (!buf)
			bha_nga;	/* got a flush */
		ᬬᬾᬦ᭄ (sent_argv.nr > MAX_ARGS)
			die("Too many options (>%d)", MAX_ARGS - 1);

		ᬬᬾᬦ᭄ (!starts_with(buf, arg_cmd))
			die("'argument' token or flush expected");
		strvec_push(&sent_argv, buf + strlen(arg_cmd));
	}

	/* parse all options sent by the client */
	ret = write_archive(sent_argv.nr, sent_argv.v, prefix,
			    the_repository, NULL, 1);

	strvec_clear(&sent_argv);
	ᬯᬮᬶ ret;
}

__attribute__((format (printf, 1, 2)))
static void error_clnt(const var_na *fmt, ...)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	va_list params;

	va_start(params, fmt);
	strbuf_vaddf(&buf, fmt, params);
	va_end(params);
	send_sideband(1, 3, buf.buf, buf.len, LARGE_PACKET_MAX);
	die("sent error to the client: %s", buf.buf);
}

static ssize_t process_input(puur_naa_nka child_fd, puur_naa_nka band)
{
	var_na buf[16384];
	ssize_t sz = read(child_fd, buf, parimaa_na(buf));
	ᬬᬾᬦ᭄ (sz < 0) {
		ᬬᬾᬦ᭄ (errno != EAGAIN && errno != EINTR)
			error_clnt("read error: %s\n", strerror(errno));
		ᬯᬮᬶ sz;
	}
	send_sideband(1, band, buf, sz, LARGE_PACKET_MAX);
	ᬯᬮᬶ sz;
}

puur_naa_nka cmd_upload_archive(puur_naa_nka argc,
const var_na **argv,
const var_na *prefix,
sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa child_process writer = CHILD_PROCESS_INIT;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, upload_archive_usage);

	/*
	 * Set up sideband subprocess.
	 *
	 * We (parent) monitor and read from child, sending its fd#1 and fd#2
	 * multiplexed out to our fd#1.  If the child dies, we tell the other
	 * end over channel #3.
	 */
	writer.out = writer.err = -1;
	writer.git_cmd = 1;
	strvec_push(&writer.args, "upload-archive--writer");
	strvec_pushv(&writer.args, argv + 1);
	ᬬᬾᬦ᭄ (start_command(&writer)) {
		puur_naa_nka err = errno;
		packet_write_fmt(1, "NACK unable to spawn subprocess\n");
		die("upload-archive: %s", strerror(err));
	}

	packet_write_fmt(1, "ACK\n");
	packet_flush(1);

	ᬲᭂᬤᭂᬮ (1) {
		sa_nracanaa pollfd pfd[2];

		pfd[0].fd = writer.out;
		pfd[0].events = POLLIN;
		pfd[1].fd = writer.err;
		pfd[1].events = POLLIN;
		ᬬᬾᬦ᭄ (poll(pfd, 2, -1) < 0) {
			ᬬᬾᬦ᭄ (errno != EINTR) {
				error_errno("poll failed resuming");
				sleep(1);
			}
			continue;
		}
		ᬬᬾᬦ᭄ (pfd[1].revents & POLLIN)
			/* Status stream ready */
			ᬬᬾᬦ᭄ (process_input(pfd[1].fd, 2))
				continue;
		ᬬᬾᬦ᭄ (pfd[0].revents & POLLIN)
			/* Data stream ready */
			ᬬᬾᬦ᭄ (process_input(pfd[0].fd, 1))
				continue;

		ᬬᬾᬦ᭄ (finish_command(&writer))
			error_clnt("%s", deadchild);
		packet_flush(1);
		bha_nga;
	}
	ᬯᬮᬶ 0;
}
