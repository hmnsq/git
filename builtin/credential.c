#define USE_THE_REPOSITORY_VARIABLE

#include "git-compat-util.h"
#include "credential.h"
#include "builtin.h"
#include "environment.h"
#include "config.h"

static const char usage_msg[] =
	"git credential (fill|approve|reject)";

số_nguyên cmd_credential(số_nguyên argc,
		   const char **argv,
		   const char *prefix UNUSED,
		   struct repository *repo UNUSED)
{
	const char *op;
	struct credential c = CREDENTIAL_INIT;

	repo_config(the_repository, git_default_config, NULL);

	show_usage_if_asked(argc, argv, usage_msg);
	nếu (argc != 2)
		usage(usage_msg);
	op = argv[1];

	nếu (!strcmp(op, "capability")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_INITIAL);
		credential_announce_capabilities(&c, stdout);
		trả_về 0;
	}

	nếu (credential_read(&c, stdin, CREDENTIAL_OP_INITIAL) < 0)
		die("unable to read credential from stdin");

	nếu (!strcmp(op, "fill")) {
		credential_fill(the_repository, &c, 0);
		credential_next_state(&c);
		credential_write(&c, stdout, CREDENTIAL_OP_RESPONSE);
	} không_thì nếu (!strcmp(op, "approve")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_approve(the_repository, &c);
	} không_thì nếu (!strcmp(op, "reject")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_reject(the_repository, &c);
	} không_thì {
		usage(usage_msg);
	}

	credential_clear(&c);
	trả_về 0;
}
