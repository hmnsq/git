/*
 * Copyright (c) 2006 Franck Bui-Huu
 * Copyright (c) 2006 Rene Scharfe
 */
#include "builtin.h"
#include "archive.h"
#include "gettext.h"
#include "transport.h"
#include "parse-options.h"
#include "pkt-line.h"

static void create_output_file(const char *output_file)
{
	정수 output_fd = xopen(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	만약 (output_fd != 1) {
		만약 (dup2(output_fd, 1) < 0)
			die_errno(_("could not redirect output"));
		아니면
			close(output_fd);
	}
}

static 정수 run_remote_archiver(정수 argc, const char **argv,
			       const char *remote, const char *exec,
			       const char *name_hint)
{
	정수 fd[2], i, rv;
	struct transport *transport;
	struct remote *_remote;
	struct packet_reader reader;

	_remote = remote_get(remote);
	transport = transport_get(_remote, _remote->url.v[0]);
	transport_connect(transport, GIT_CONNECT_UPLOAD_ARCHIVE, exec, fd);

	/*
	 * Inject a fake --format field at the beginning of the
	 * arguments, with the format inferred from our output
	 * filename. This way explicit --format options can override
	 * it.
	 */
	만약 (name_hint) {
		const char *format = archive_format_from_filename(name_hint);
		만약 (format)
			packet_write_fmt(fd[1], "argument --format=%s\n", format);
	}
	위해 (i = 1; i < argc; i++)
		packet_write_fmt(fd[1], "argument %s\n", argv[i]);
	packet_flush(fd[1]);

	packet_reader_init(&reader, fd[0], NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	만약 (packet_reader_read(&reader) != PACKET_READ_NORMAL)
		die(_("git archive: expected ACK/NAK, got a flush packet"));
	만약 (strcmp(reader.line, "ACK")) {
		만약 (starts_with(reader.line, "NACK "))
			die(_("git archive: NACK %s"), reader.line + 5);
		die(_("git archive: protocol error"));
	}

	만약 (packet_reader_read(&reader) != PACKET_READ_FLUSH)
		die(_("git archive: expected a flush"));

	/* Now, start reading from fd[0] and spit it out to stdout */
	rv = recv_sideband("archive", fd[0], 1);
	rv |= transport_disconnect(transport);

	반환 !!rv;
}

#define PARSE_OPT_KEEP_ALL ( PARSE_OPT_KEEP_DASHDASH | 	\
			     PARSE_OPT_KEEP_ARGV0 | 	\
			     PARSE_OPT_KEEP_UNKNOWN_OPT |	\
			     PARSE_OPT_NO_INTERNAL_HELP	)

정수 cmd_archive(정수 argc,
		const char **argv,
		const char *prefix,
		struct repository *repo)
{
	const char *exec = "git-upload-archive";
	char *output = NULL;
	const char *remote = NULL;
	struct option local_opts[] = {
		OPT_FILENAME('o', "output", &output,
			     N_("write the archive to this file")),
		OPT_STRING(0, "remote", &remote, N_("repo"),
			N_("retrieve the archive from remote repository <repo>")),
		OPT_STRING(0, "exec", &exec, N_("command"),
			N_("path to the remote git-upload-archive command")),
		OPT_END()
	};
	정수 ret;

	argc = parse_options(argc, argv, prefix, local_opts, NULL,
			     PARSE_OPT_KEEP_ALL);

	init_archivers();

	만약 (output)
		create_output_file(output);

	만약 (remote) {
		ret = run_remote_archiver(argc, argv, remote, exec, output);
		goto out;
	}

	setvbuf(stderr, NULL, _IOLBF, BUFSIZ);

	ret = write_archive(argc, argv, prefix, repo, output, 0);

out:
	free(output);
	반환 ret;
}
