#define USE_THE_REPOSITORY_VARIABLE

#include "git-compat-util.h"
#include "credential.h"
#include "builtin.h"
#include "environment.h"
#include "config.h"

static const char usage_msg[] =
	"git credential (fill|approve|reject)";

정수 cmd_credential(정수 argc,
		   const char **argv,
		   const char *prefix UNUSED,
		   struct repository *repo UNUSED)
{
	const char *op;
	struct credential c = CREDENTIAL_INIT;

	repo_config(the_repository, git_default_config, NULL);

	show_usage_if_asked(argc, argv, usage_msg);
	만약 (argc != 2)
		usage(usage_msg);
	op = argv[1];

	만약 (!strcmp(op, "capability")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_INITIAL);
		credential_announce_capabilities(&c, stdout);
		반환 0;
	}

	만약 (credential_read(&c, stdin, CREDENTIAL_OP_INITIAL) < 0)
		die("unable to read credential from stdin");

	만약 (!strcmp(op, "fill")) {
		credential_fill(the_repository, &c, 0);
		credential_next_state(&c);
		credential_write(&c, stdout, CREDENTIAL_OP_RESPONSE);
	} 아니면 만약 (!strcmp(op, "approve")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_approve(the_repository, &c);
	} 아니면 만약 (!strcmp(op, "reject")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_reject(the_repository, &c);
	} 아니면 {
		usage(usage_msg);
	}

	credential_clear(&c);
	반환 0;
}
