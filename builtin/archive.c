/*
 * Copyright (c) 2006 Franck Bui-Huu
 * Copyright (c) 2006 Rene Scharfe
 */
#समावेश "builtin.h"
#समावेश "archive.h"
#समावेश "gettext.h"
#समावेश "transport.h"
#समावेश "parse-options.h"
#समावेश "pkt-line.h"

अटल व्योम create_output_file(स्थिर अक्षर *output_file)
{
	पूर्णांक output_fd = xopen(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	अगर (output_fd != 1) {
		अगर (dup2(output_fd, 1) < 0)
			die_errno(_("could not redirect output"));
		अन्यथा
			बंद(output_fd);
	}
}

अटल पूर्णांक run_remote_archiver(पूर्णांक argc, स्थिर अक्षर **argv,
			       स्थिर अक्षर *remote, स्थिर अक्षर *exec,
			       स्थिर अक्षर *name_hint)
{
	पूर्णांक fd[2], i, rv;
	काष्ठा transport *transport;
	काष्ठा remote *_remote;
	काष्ठा packet_reader reader;

	_remote = remote_get(remote);
	transport = transport_get(_remote, _remote->url.v[0]);
	transport_connect(transport, GIT_CONNECT_UPLOAD_ARCHIVE, exec, fd);

	/*
	 * Inject a fake --format field at the beginning of the
	 * arguments, with the format inferred from our output
	 * filename. This way explicit --format options can override
	 * it.
	 */
	अगर (name_hint) {
		स्थिर अक्षर *format = archive_format_from_filename(name_hint);
		अगर (format)
			packet_write_fmt(fd[1], "argument --format=%s\n", format);
	}
	क्रम (i = 1; i < argc; i++)
		packet_write_fmt(fd[1], "argument %s\n", argv[i]);
	packet_flush(fd[1]);

	packet_reader_init(&reader, fd[0], शून्य, 0,
			   PACKET_READ_CHOMP_NEWLINE |
			   PACKET_READ_DIE_ON_ERR_PACKET);

	अगर (packet_reader_read(&reader) != PACKET_READ_NORMAL)
		die(_("git archive: expected ACK/NAK, got a flush packet"));
	अगर (म_भेद(reader.पंक्ति, "ACK")) {
		अगर (starts_with(reader.पंक्ति, "NACK "))
			die(_("git archive: NACK %s"), reader.पंक्ति + 5);
		die(_("git archive: protocol error"));
	}

	अगर (packet_reader_read(&reader) != PACKET_READ_FLUSH)
		die(_("git archive: expected a flush"));

	/* Now, start reading from fd[0] and spit it out to stdout */
	rv = recv_sideband("archive", fd[0], 1);
	rv |= transport_disconnect(transport);

	वापस !!rv;
}

#घोषणा PARSE_OPT_KEEP_ALL ( PARSE_OPT_KEEP_DASHDASH | 	\
			     PARSE_OPT_KEEP_ARGV0 | 	\
			     PARSE_OPT_KEEP_UNKNOWN_OPT |	\
			     PARSE_OPT_NO_INTERNAL_HELP	)

पूर्णांक cmd_archive(पूर्णांक argc,
		स्थिर अक्षर **argv,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo)
{
	स्थिर अक्षर *exec = "git-upload-archive";
	अक्षर *output = शून्य;
	स्थिर अक्षर *remote = शून्य;
	काष्ठा option local_opts[] = {
		OPT_FILENAME('o', "output", &output,
			     N_("write the archive to this file")),
		OPT_STRING(0, "remote", &remote, N_("repo"),
			N_("retrieve the archive from remote repository <repo>")),
		OPT_STRING(0, "exec", &exec, N_("command"),
			N_("path to the remote git-upload-archive command")),
		OPT_END()
	};
	पूर्णांक ret;

	argc = parse_options(argc, argv, prefix, local_opts, शून्य,
			     PARSE_OPT_KEEP_ALL);

	init_archivers();

	अगर (output)
		create_output_file(output);

	अगर (remote) {
		ret = run_remote_archiver(argc, argv, remote, exec, output);
		जाओ out;
	}

	रखो_भबफ(मानक_त्रुटि, शून्य, _IOLBF, बफ_मान);

	ret = write_archive(argc, argv, prefix, repo, output, 0);

out:
	मुक्त(output);
	वापस ret;
}
