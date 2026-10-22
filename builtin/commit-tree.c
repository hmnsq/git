/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"

#అన్తర్భావ "commit.h"
#అన్తర్భావ "parse-options.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి commit_వృక్షం_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *sign_commit;

స్థిర శూన్య new_మాతృ(నిర్మాణం commit *మాతృ, నిర్మాణం commit_list **parents_p)
{
	నిర్మాణం object_id *oid = &మాతృ->object.oid;
	నిర్మాణం commit_list *parents;
	కొరకు (parents = *parents_p; parents; parents = parents->తదుపరి) {
		ఒకవేళ (parents->అంశం == మాతృ) {
			error(_("duplicate parent %s ignored"), oid_గమ్యం_hex(oid));
			తిరిగి;
		}
		parents_p = &parents->తదుపరి;
	}
	commit_list_చొప్పించు(మాతృ, parents_p);
}

స్థిర పూర్ణాంకం parse_మాతృ_arg_callback(స్థిరరాశి నిర్మాణం option *opt,
		స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం object_id oid;
	నిర్మాణం commit_list **parents = opt->విలువ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ఒకవేళ (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_మాతృ(lookup_commit(the_repository, &oid), parents);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_సందేశం_arg_callback(స్థిరరాశి నిర్మాణం option *opt,
		స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం strbuf *buf = opt->విలువ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ఒకవేళ (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_పంక్తి(buf);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_file_arg_callback(స్థిరరాశి నిర్మాణం option *opt,
		స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం fd;
	నిర్మాణం strbuf *buf = opt->విలువ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ఒకవేళ (buf->len)
		strbuf_addch(buf, '\n');
	ఒకవేళ (!strcmp(arg, "-"))
		fd = 0;
	లేకపోతే {
		fd = xopen(arg, O_RDONLY);
	}
	ఒకవేళ (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	ఒకవేళ (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	తిరిగి 0;
}

పూర్ణాంకం cmd_commit_వృక్షం(పూర్ణాంకం argc,
		    స్థిరరాశి అక్షరం **argv,
		    స్థిరరాశి అక్షరం *ఉపసర్గ,
		    నిర్మాణం repository *repo వాడని)
{
	స్థిర నిర్మాణం strbuf బఫరు = STRBUF_INIT;
	నిర్మాణం commit_list *parents = ఖాళీ;
	నిర్మాణం object_id వృక్షం_oid;
	నిర్మాణం object_id commit_oid;

	నిర్మాణం option options[] = {
		OPT_CALLBACK_F('p', ఖాళీ, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_మాతృ_arg_callback),
		OPT_CALLBACK_F('m', ఖాళీ, &బఫరు, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_సందేశం_arg_callback),
		OPT_CALLBACK_F('F', ఖాళీ, &బఫరు, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.విలువ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_చివర()
	};
	పూర్ణాంకం ret;

	repo_config(the_repository, git_default_config, ఖాళీ);

	show_usage_with_options_if_asked(argc, argv,
					 commit_వృక్షం_usage, options);

	argc = parse_options(argc, argv, ఉపసర్గ, options, commit_వృక్షం_usage, 0);

	ఒకవేళ (argc != 1)
		die(_("must give exactly one tree"));

	ఒకవేళ (repo_get_oid_వృక్షం(the_repository, argv[0], &వృక్షం_oid))
		die(_("not a valid object name %s"), argv[0]);

	ఒకవేళ (!బఫరు.len) {
		ఒకవేళ (strbuf_read(&బఫరు, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	ఒకవేళ (commit_వృక్షం(బఫరు.buf, బఫరు.len, &వృక్షం_oid, parents, &commit_oid,
			ఖాళీ, sign_commit)) {
		ret = 1;
		వెళ్ళు బహిర్గతం;
	}

	రాయి("%s\n", oid_గమ్యం_hex(&commit_oid));
	ret = 0;

బహిర్గతం:
	commit_list_free(parents);
	strbuf_release(&బఫరు);
	తిరిగి ret;
}
