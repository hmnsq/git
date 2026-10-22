#define USE_THE_REPOSITORY_VARIABLE

#ضمن "git-compat-util.h"
#ضمن "credential.h"
#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "config.h"

ساكن ثابت حرف usage_msg[] =
	"git credential (fill|approve|reject)";

صحيح cmd_credential(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix UNUSED,
		   بنية repository *repo UNUSED)
{
	ثابت حرف *op;
	بنية credential c = CREDENTIAL_INIT;

	repo_config(the_repository, git_default_config, عدم);

	show_usage_if_asked(argc, argv, usage_msg);
	אם (argc != 2)
		usage(usage_msg);
	op = argv[1];

	אם (!strcmp(op, "capability")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_INITIAL);
		credential_announce_capabilities(&c, stdout);
		החזר 0;
	}

	אם (credential_read(&c, stdin, CREDENTIAL_OP_INITIAL) < 0)
		die("unable to read credential from stdin");

	אם (!strcmp(op, "fill")) {
		credential_fill(the_repository, &c, 0);
		credential_next_state(&c);
		credential_write(&c, stdout, CREDENTIAL_OP_RESPONSE);
	} אחרת אם (!strcmp(op, "approve")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_approve(the_repository, &c);
	} אחרת אם (!strcmp(op, "reject")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_reject(the_repository, &c);
	} אחרת {
		usage(usage_msg);
	}

	credential_clear(&c);
	החזר 0;
}
