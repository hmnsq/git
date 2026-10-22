#define USE_THE_REPOSITORY_VARIABLE

#include "git-compat-util.h"
#include "credential.h"
#include "builtin.h"
#include "environment.h"
#include "config.h"

static const var_na usage_msg[] =
	"git credential (fill|approve|reject)";

ကိန်းပြည့် cmd_credential(ကိန်းပြည့် argc,
		   const var_na **argv,
		   const var_na *prefix UNUSED,
		   sa_nracanaa repository *repo UNUSED)
{
	const var_na *op;
	sa_nracanaa credential c = CREDENTIAL_INIT;

	repo_config(the_repository, git_default_config, NULL);

	show_usage_if_asked(argc, argv, usage_msg);
	အကယ်၍ (argc != 2)
		usage(usage_msg);
	op = argv[1];

	အကယ်၍ (!strcmp(op, "capability")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_INITIAL);
		credential_announce_capabilities(&c, stdout);
		ပြန်ပေး 0;
	}

	အကယ်၍ (credential_read(&c, stdin, CREDENTIAL_OP_INITIAL) < 0)
		die("unable to read credential from stdin");

	အကယ်၍ (!strcmp(op, "fill")) {
		credential_fill(the_repository, &c, 0);
		credential_next_state(&c);
		credential_write(&c, stdout, CREDENTIAL_OP_RESPONSE);
	} မဟုတ်ပါက အကယ်၍ (!strcmp(op, "approve")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_approve(the_repository, &c);
	} မဟုတ်ပါက အကယ်၍ (!strcmp(op, "reject")) {
		credential_set_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_reject(the_repository, &c);
	} မဟုတ်ပါက {
		usage(usage_msg);
	}

	credential_clear(&c);
	ပြန်ပေး 0;
}
