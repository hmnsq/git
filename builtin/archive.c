/*
 * Copyright (c) 2006 Franck Bui-Huu
 * Copyright (c) 2006 Rene Scharfe
 */
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "archive.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "transport.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "pkt-line.h"

સ્થિર શૂન્ય create_output_file(અચલ અક્ષર *output_file)
{
	પૂર્ણાંક output_fd = xopen(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	જો (output_fd != 1) {
		જો (dup2(output_fd, 1) < 0)
			die_errno(_("could not redirect output"));
		નહીંતર
			close(output_fd);
	}
}

સ્થિર પૂર્ણાંક run_remote_archiver(પૂર્ણાંક argc, અચલ અક્ષર **argv,
			       અચલ અક્ષર *remote, અચલ અક્ષર *exec,
			       અચલ અક્ષર *name_hint)
{
	પૂર્ણાંક fd[2], i, rv;
	સંરચના transport *transport;
	સંરચના remote *_remote;
	સંરચના packet_reader reader;

	_remote = remote_get(remote);
	transport = transport_get(_remote, _remote->url.v[0]);
	transport_connect(transport, GIT_CONNECT_UPLOAD_ARCHIVE, exec, fd);

	/*
	 * Inject a fake --format field at the beginning of the
	 * arguments, with the format inferred from our output
	 * filename. This way explicit --format options can override
	 * it.
	 */
	જો (name_hint) {
		અચલ અક્ષર *format = archive_format_from_filename(name_hint);
		જો (format)
			packet_write_fmt(fd[1], "argument --format=%s\n", format);
	}
	માટે (i = 1; i < argc; i++)
		packet_write_fmt(fd[1], "argument %s\n", argv[i]);
	packet_flush(fd[1]);

	packet_reader_init(&reader, fd[0], ખાલી, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	જો (packet_reader_read(&reader) != PACKET_READ_NORMAL)
		die(_("git archive: expected ACK/NAK, got a flush packet"));
	જો (strcmp(reader.લીટી, "ACK")) {
		જો (starts_with(reader.લીટી, "NACK "))
			die(_("git archive: NACK %s"), reader.લીટી + 5);
		die(_("git archive: protocol error"));
	}

	જો (packet_reader_read(&reader) != PACKET_READ_FLUSH)
		die(_("git archive: expected a flush"));

	/* Now, start reading from fd[0] and spit it out to stdout */
	rv = recv_sideband("archive", fd[0], 1);
	rv |= transport_disconnect(transport);

	પાછા !!rv;
}

#define PARSE_OPT_KEEP_ALL ( PARSE_OPT_KEEP_DASHDASH | 	\
			     PARSE_OPT_KEEP_ARGV0 | 	\
			     PARSE_OPT_KEEP_UNKNOWN_OPT |	\
			     PARSE_OPT_NO_INTERNAL_HELP	)

પૂર્ણાંક cmd_archive(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo)
{
	અચલ અક્ષર *exec = "git-upload-archive";
	અક્ષર *output = ખાલી;
	અચલ અક્ષર *remote = ખાલી;
	સંરચના option local_opts[] = {
		OPT_FILENAME('o', "output", &output,
			     N_("write the archive to this file")),
		OPT_STRING(0, "remote", &remote, N_("repo"),
			N_("retrieve the archive from remote repository <repo>")),
		OPT_STRING(0, "exec", &exec, N_("command"),
			N_("path to the remote git-upload-archive command")),
		OPT_END()
	};
	પૂર્ણાંક ret;

	argc = parse_options(argc, argv, prefix, local_opts, ખાલી,
			     PARSE_OPT_KEEP_ALL);

	init_archivers();

	જો (output)
		create_output_file(output);

	જો (remote) {
		ret = run_remote_archiver(argc, argv, remote, exec, output);
		જાઓ out;
	}

	setvbuf(stderr, ખાલી, _IOLBF, BUFSIZ);

	ret = write_archive(argc, argv, prefix, repo, output, 0);

out:
	મુક્ત(output);
	પાછા ret;
}
