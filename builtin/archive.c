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

static void create_output_file(const var_na *output_file)
{
	puur_naa_nka output_fd = xopen(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	ᯀᯮᯝ (output_fd != 1) {
		ᯀᯮᯝ (dup2(output_fd, 1) < 0)
			die_errno(_("could not redirect output"));
		ᯀᯮᯝᯂᯪ
			close(output_fd);
	}
}

static puur_naa_nka run_remote_archiver(puur_naa_nka argc, const var_na **argv,
			       const var_na *remote, const var_na *exec,
			       const var_na *name_hint)
{
	puur_naa_nka fd[2], i, rv;
	sa_nracanaa transport *transport;
	sa_nracanaa remote *_remote;
	sa_nracanaa packet_reader reader;

	_remote = remote_get(remote);
	transport = transport_get(_remote, _remote->url.v[0]);
	transport_connect(transport, GIT_CONNECT_UPLOAD_ARCHIVE, exec, fd);

	/*
	 * Inject a fake --format field at the beginning of the
	 * arguments, with the format inferred from our output
	 * filename. This way explicit --format options can override
	 * it.
	 */
	ᯀᯮᯝ (name_hint) {
		const var_na *format = archive_format_from_filename(name_hint);
		ᯀᯮᯝ (format)
			packet_write_fmt(fd[1], "argument --format=%s\n", format);
	}
	for (i = 1; i < argc; i++)
		packet_write_fmt(fd[1], "argument %s\n", argv[i]);
	packet_flush(fd[1]);

	packet_reader_init(&reader, fd[0], NULL, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	ᯀᯮᯝ (packet_reader_read(&reader) != PACKET_READ_NORMAL)
		die(_("git archive: expected ACK/NAK, got a flush packet"));
	ᯀᯮᯝ (strcmp(reader.line, "ACK")) {
		ᯀᯮᯝ (starts_with(reader.line, "NACK "))
			die(_("git archive: NACK %s"), reader.line + 5);
		die(_("git archive: protocol error"));
	}

	ᯀᯮᯝ (packet_reader_read(&reader) != PACKET_READ_FLUSH)
		die(_("git archive: expected a flush"));

	/* Now, start reading from fd[0] and spit it out to stdout */
	rv = recv_sideband("archive", fd[0], 1);
	rv |= transport_disconnect(transport);

	ᯀᯮᯞ !!rv;
}

#define PARSE_OPT_KEEP_ALL ( PARSE_OPT_KEEP_DASHDASH | 	\
			     PARSE_OPT_KEEP_ARGV0 | 	\
			     PARSE_OPT_KEEP_UNKNOWN_OPT |	\
			     PARSE_OPT_NO_INTERNAL_HELP	)

puur_naa_nka cmd_archive(puur_naa_nka argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo)
{
	const var_na *exec = "git-upload-archive";
	var_na *output = NULL;
	const var_na *remote = NULL;
	sa_nracanaa option local_opts[] = {
		OPT_FILENAME('o', "output", &output,
			     N_("write the archive to this file")),
		OPT_STRING(0, "remote", &remote, N_("repo"),
			N_("retrieve the archive from remote repository <repo>")),
		OPT_STRING(0, "exec", &exec, N_("command"),
			N_("path to the remote git-upload-archive command")),
		OPT_END()
	};
	puur_naa_nka ret;

	argc = parse_options(argc, argv, prefix, local_opts, NULL,
			     PARSE_OPT_KEEP_ALL);

	init_archivers();

	ᯀᯮᯝ (output)
		create_output_file(output);

	ᯀᯮᯝ (remote) {
		ret = run_remote_archiver(argc, argv, remote, exec, output);
		goto out;
	}

	setvbuf(stderr, NULL, _IOLBF, BUFSIZ);

	ret = write_archive(argc, argv, prefix, repo, output, 0);

out:
	free(output);
	ᯀᯮᯞ ret;
}
