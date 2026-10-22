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

ကိန်းပြည့် cmd_upload_archive_writer(ကိန်းပြည့် argc,
			      const var_na **argv,
			      const var_na *prefix,
			      sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa strvec sent_argv = STRVEC_INIT;
	const var_na *arg_cmd = "argument ";
	ကိန်းပြည့် ret;

	show_usage_if_asked(argc, argv, upload_archive_usage);
	အကယ်၍ (argc != 2)
		usage(upload_archive_usage);

	အကယ်၍ (!enter_repo(the_repository, argv[1], 0))
		die("'%s' does not appear to be a git repository", argv[1]);

	init_archivers();

	/* put received options in sent_argv[] */
	strvec_push(&sent_argv, "git-upload-archive");
	အတွက် (;;) {
		var_na *buf = packet_read_line(0, NULL);
		အကယ်၍ (!buf)
			bha_nga;	/* got a flush */
		အကယ်၍ (sent_argv.nr > MAX_ARGS)
			die("Too many options (>%d)", MAX_ARGS - 1);

		အကယ်၍ (!starts_with(buf, arg_cmd))
			die("'argument' token or flush expected");
		strvec_push(&sent_argv, buf + strlen(arg_cmd));
	}

	/* parse all options sent by the client */
	ret = write_archive(sent_argv.nr, sent_argv.v, prefix,
			    the_repository, NULL, 1);

	strvec_clear(&sent_argv);
	ပြန်ပေး ret;
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

static ssize_t process_input(ကိန်းပြည့် child_fd, ကိန်းပြည့် band)
{
	var_na buf[16384];
	ssize_t sz = read(child_fd, buf, parimaa_na(buf));
	အကယ်၍ (sz < 0) {
		အကယ်၍ (errno != EAGAIN && errno != EINTR)
			error_clnt("read error: %s\n", strerror(errno));
		ပြန်ပေး sz;
	}
	send_sideband(1, band, buf, sz, LARGE_PACKET_MAX);
	ပြန်ပေး sz;
}

ကိန်းပြည့် cmd_upload_archive(ကိန်းပြည့် argc,
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
	အကယ်၍ (start_command(&writer)) {
		ကိန်းပြည့် err = errno;
		packet_write_fmt(1, "NACK unable to spawn subprocess\n");
		die("upload-archive: %s", strerror(err));
	}

	packet_write_fmt(1, "ACK\n");
	packet_flush(1);

	နေစဉ် (1) {
		sa_nracanaa pollfd pfd[2];

		pfd[0].fd = writer.out;
		pfd[0].events = POLLIN;
		pfd[1].fd = writer.err;
		pfd[1].events = POLLIN;
		အကယ်၍ (poll(pfd, 2, -1) < 0) {
			အကယ်၍ (errno != EINTR) {
				error_errno("poll failed resuming");
				sleep(1);
			}
			continue;
		}
		အကယ်၍ (pfd[1].revents & POLLIN)
			/* Status stream ready */
			အကယ်၍ (process_input(pfd[1].fd, 2))
				continue;
		အကယ်၍ (pfd[0].revents & POLLIN)
			/* Data stream ready */
			အကယ်၍ (process_input(pfd[0].fd, 1))
				continue;

		အကယ်၍ (finish_command(&writer))
			error_clnt("%s", deadchild);
		packet_flush(1);
		bha_nga;
	}
	ပြန်ပေး 0;
}
