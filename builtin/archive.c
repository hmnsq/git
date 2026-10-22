/*
 * Copyright (c) 2006 Franck Bui-Huu
 * Copyright (c) 2006 Rene Scharfe
 */
#شامل "builtin.h"
#شامل "archive.h"
#شامل "gettext.h"
#شامل "transport.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"

ایستا تهی create_output_file(ثابت نویسه *output_file)
{
	صحیح output_fd = xopen(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	اگر (output_fd != 1) {
		اگر (dup2(output_fd, 1) < 0)
			die_errno(_("could not redirect output"));
		وگرنه
			close(output_fd);
	}
}

ایستا صحیح run_remote_archiver(صحیح argc, ثابت نویسه **argv,
			       ثابت نویسه *remote, ثابت نویسه *exec,
			       ثابت نویسه *name_hint)
{
	صحیح fd[2], i, rv;
	ساختار transport *transport;
	ساختار remote *_remote;
	ساختار packet_reader reader;

	_remote = remote_get(remote);
	transport = transport_get(_remote, _remote->url.v[0]);
	transport_connect(transport, GIT_CONNECT_UPLOAD_ARCHIVE, exec, fd);

	/*
	 * Inject a fake --format field at the beginning of the
	 * arguments, with the format inferred from our output
	 * filename. This way explicit --format options can override
	 * it.
	 */
	اگر (name_hint) {
		ثابت نویسه *format = archive_format_from_filename(name_hint);
		اگر (format)
			packet_write_fmt(fd[1], "argument --format=%s\n", format);
	}
	برای (i = 1; i < argc; i++)
		packet_write_fmt(fd[1], "argument %s\n", argv[i]);
	packet_flush(fd[1]);

	packet_reader_init(&reader, fd[0], پوچ, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	اگر (packet_reader_read(&reader) != PACKET_READ_NORMAL)
		die(_("git archive: expected ACK/NAK, got a flush packet"));
	اگر (strcmp(reader.سطر, "ACK")) {
		اگر (starts_with(reader.سطر, "NACK "))
			die(_("git archive: NACK %s"), reader.سطر + 5);
		die(_("git archive: protocol error"));
	}

	اگر (packet_reader_read(&reader) != PACKET_READ_FLUSH)
		die(_("git archive: expected a flush"));

	/* Now, start reading from fd[0] and spit it out to stdout */
	rv = recv_sideband("archive", fd[0], 1);
	rv |= transport_disconnect(transport);

	بازگشت !!rv;
}

#define PARSE_OPT_KEEP_ALL ( PARSE_OPT_KEEP_DASHDASH | 	\
			     PARSE_OPT_KEEP_ARGV0 | 	\
			     PARSE_OPT_KEEP_UNKNOWN_OPT |	\
			     PARSE_OPT_NO_INTERNAL_HELP	)

صحیح cmd_archive(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه *prefix,
		ساختار repository *repo)
{
	ثابت نویسه *exec = "git-upload-archive";
	نویسه *output = پوچ;
	ثابت نویسه *remote = پوچ;
	ساختار option local_opts[] = {
		OPT_FILENAME('o', "output", &output,
			     N_("write the archive to this file")),
		OPT_STRING(0, "remote", &remote, N_("repo"),
			N_("retrieve the archive from remote repository <repo>")),
		OPT_STRING(0, "exec", &exec, N_("command"),
			N_("path to the remote git-upload-archive command")),
		OPT_END()
	};
	صحیح ret;

	argc = parse_options(argc, argv, prefix, local_opts, پوچ,
			     PARSE_OPT_KEEP_ALL);

	init_archivers();

	اگر (output)
		create_output_file(output);

	اگر (remote) {
		ret = run_remote_archiver(argc, argv, remote, exec, output);
		برو out;
	}

	setvbuf(stderr, پوچ, _IOLBF, BUFSIZ);

	ret = write_archive(argc, argv, prefix, repo, output, 0);

out:
	آزاد(output);
	بازگشت ret;
}
