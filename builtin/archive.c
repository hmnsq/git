/*
 * Copyright (c) 2006 Franck Bui-Huu
 * Copyright (c) 2006 Rene Scharfe
 */
#ضمن "builtin.h"
#ضمن "archive.h"
#ضمن "gettext.h"
#ضمن "transport.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"

ساكن فراغ create_output_file(ثابت حرف *output_file)
{
	صحيح output_fd = xopen(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	إذا (output_fd != 1) {
		إذا (dup2(output_fd, 1) < 0)
			die_errno(_("could not redirect output"));
		وإلا
			close(output_fd);
	}
}

ساكن صحيح run_remote_archiver(صحيح argc, ثابت حرف **argv,
			       ثابت حرف *remote, ثابت حرف *exec,
			       ثابت حرف *name_hint)
{
	صحيح fd[2], i, rv;
	بنية transport *transport;
	بنية remote *_remote;
	بنية packet_reader reader;

	_remote = remote_get(remote);
	transport = transport_get(_remote, _remote->url.v[0]);
	transport_connect(transport, GIT_CONNECT_UPLOAD_ARCHIVE, exec, fd);

	/*
	 * Inject a fake --format field at the beginning of the
	 * arguments, with the format inferred from our output
	 * filename. This way explicit --format options can override
	 * it.
	 */
	إذا (name_hint) {
		ثابت حرف *format = archive_format_from_filename(name_hint);
		إذا (format)
			packet_write_fmt(fd[1], "argument --format=%s\n", format);
	}
	لكل (i = 1; i < argc; i++)
		packet_write_fmt(fd[1], "argument %s\n", argv[i]);
	packet_flush(fd[1]);

	packet_reader_init(&reader, fd[0], عدم, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	إذا (packet_reader_read(&reader) != PACKET_READ_NORMAL)
		die(_("git archive: expected ACK/NAK, got a flush packet"));
	إذا (strcmp(reader.لي, "ACK")) {
		إذا (starts_with(reader.لي, "NACK "))
			die(_("git archive: NACK %s"), reader.لي + 5);
		die(_("git archive: protocol error"));
	}

	إذا (packet_reader_read(&reader) != PACKET_READ_FLUSH)
		die(_("git archive: expected a flush"));

	/* Now, start reading from fd[0] and spit it out to stdout */
	rv = recv_sideband("archive", fd[0], 1);
	rv |= transport_disconnect(transport);

	أرجع !!rv;
}

#define PARSE_OPT_KEEP_ALL ( PARSE_OPT_KEEP_DASHDASH | 	\
			     PARSE_OPT_KEEP_ARGV0 | 	\
			     PARSE_OPT_KEEP_UNKNOWN_OPT |	\
			     PARSE_OPT_NO_INTERNAL_HELP	)

صحيح cmd_archive(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *prefix,
		بنية repository *repo)
{
	ثابت حرف *exec = "git-upload-archive";
	حرف *output = عدم;
	ثابت حرف *remote = عدم;
	بنية option local_opts[] = {
		OPT_FILENAME('o', "output", &output,
			     N_("write the archive to this file")),
		OPT_STRING(0, "remote", &remote, N_("repo"),
			N_("retrieve the archive from remote repository <repo>")),
		OPT_STRING(0, "exec", &exec, N_("command"),
			N_("path to the remote git-upload-archive command")),
		OPT_END()
	};
	صحيح ret;

	argc = parse_options(argc, argv, prefix, local_opts, عدم,
			     PARSE_OPT_KEEP_ALL);

	init_archivers();

	إذا (output)
		create_output_file(output);

	إذا (remote) {
		ret = run_remote_archiver(argc, argv, remote, exec, output);
		اذهب out;
	}

	setvbuf(stderr, عدم, _IOLBF, BUFSIZ);

	ret = write_archive(argc, argv, prefix, repo, output, 0);

out:
	حرر(output);
	أرجع ret;
}
