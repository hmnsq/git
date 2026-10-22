#define USE_THE_REPOSITORY_VARIABLE

#include "git-compat-util.h"
#include "credential.h"
#include "builtin.h"
#include "environment.h"
#include "config.h"

static const var_na usage_msg[] =
	"git credential (fill|approve|reject)";

ཧྲིལ cmd_credential(ཧྲིལ argc,
		   const var_na **argv,
		   const var_na *prefix UNUSED,
		   sa_nracanaa repository *repo UNUSED)
{
	const var_na *op;
	sa_nracanaa credential c = CREDENTIAL_INIT;

	repo_config(the_repository, git_default_config, NULL);

	show_usage_if_asked(argc, argv, usage_msg);
	གལ་སྲིད (argc != 2)
		usage(usage_msg);
	op = argv[1];

	གལ་སྲིད (!strcmp(op, "capability")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_INITIAL);
		credential_announce_capabilities(&c, stdout);
		ལོག 0;
	}

	གལ་སྲིད (credential_read(&c, stdin, CREDENTIAL_OP_INITIAL) < 0)
		die("unable to read credential from stdin");

	གལ་སྲིད (!strcmp(op, "fill")) {
		credential_fill(the_repository, &c, 0);
		credential_next_state(&c);
		credential_write(&c, stdout, CREDENTIAL_OP_RESPONSE);
	} དེ་མིན གལ་སྲིད (!strcmp(op, "approve")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_approve(the_repository, &c);
	} དེ་མིན གལ་སྲིད (!strcmp(op, "reject")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_reject(the_repository, &c);
	} དེ་མིན {
		usage(usage_msg);
	}

	credential_clear(&c);
	ལོག 0;
}
