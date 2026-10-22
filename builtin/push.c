/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refspec.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "remote.h"
#ଅନ୍ତର୍ଭାଵ "transport.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "submodule-config.h"
#ଅନ୍ତର୍ଭାଵ "send-pack.h"
#ଅନ୍ତର୍ଭାଵ "trace2.h"
#ଅନ୍ତର୍ଭାଵ "color.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ ଠେଲ_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ଖାଲି,
};

ସ୍ଥିର ଗଣନା git_colorbool ଠେଲ_use_color = GIT_COLOR_UNKNOWN;
ସ୍ଥିର ଅକ୍ଷର ଠେଲ_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

ଗଣନା color_ଠେଲ {
	ଠେଲ_COLOR_RESET = 0,
	ଠେଲ_COLOR_ERROR = 1
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_ଠେଲ_color_slot(ଅଚଳ ଅକ୍ଷର *slot)
{
	ଯଦି (!strcasecmp(slot, "reset"))
		ଫେରନ୍ତୁ ଠେଲ_COLOR_RESET;
	ଯଦି (!strcasecmp(slot, "error"))
		ଫେରନ୍ତୁ ଠେଲ_COLOR_ERROR;
	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଠେଲ_get_color(ଗଣନା color_ଠେଲ ix)
{
	ଯଦି (want_color_stderr(ଠେଲ_use_color))
		ଫେରନ୍ତୁ ଠେଲ_colors[ix];
	ଫେରନ୍ତୁ "";
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା thin = 1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା deleterefs;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *receivepack;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
ସ୍ଥିର ଗଣନା transport_family family;

ସ୍ଥିର ସଂରଚନା ଠେଲ_cas_option cas;

ସ୍ଥିର ସଂରଚନା refspec rs;

ସ୍ଥିର ସଂରଚନା string_list ଠେଲ_options_config = STRING_LIST_INIT_DUP;

ସ୍ଥିର ଶୂନ୍ୟ refspec_append_mapped(ସଂରଚନା refspec *refspec, ଅଚଳ ଅକ୍ଷର *ref,
				  ସଂରଚନା remote *remote, ସଂରଚନା ref *matched)
{
	ଅଚଳ ଅକ୍ଷର *branch_name;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ଯଦି (remote->ଠେଲ.nr) {
		ସଂରଚନା refspec_item query = {
			.src = matched->name,
		};

		ଯଦି (!refspec_find_match(&remote->ଠେଲ, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			ମୁକ୍ତ(query.dst);
			ଫେରନ୍ତୁ;
		}
	}

	ଯଦି (cfg->ଠେଲ_default == ଠେଲ_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		ସଂରଚନା branch *branch = branch_get(branch_name);
		ଯଦି (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			ଫେରନ୍ତୁ;
		}
	}

	refspec_append(refspec, ref);
}

ସ୍ଥିର ଶୂନ୍ୟ set_refspecs(ଅଚଳ ଅକ୍ଷର **refs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr, ସଂରଚନା remote *remote)
{
	ସଂରଚନା ref *local_refs = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < nr; i++) {
		ଅଚଳ ଅକ୍ଷର *ref = refs[i];
		ଯଦି (!strcmp("tag", ref)) {
			ଯଦି (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			ଯଦି (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			ଅନ୍ୟଥା
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} ଅନ୍ୟଥା ଯଦି (deleterefs) {
			ଯଦି (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} ଅନ୍ୟଥା ଯଦି (!strchr(ref, ':')) {
			ସଂରଚନା ref *matched = ଖାଲି;

			/* lazily grab local_refs */
			ଯଦି (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			ଯଦି (ଗଣନା_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			ଅନ୍ୟଥା
				refspec_append_mapped(&rs, ref, remote, matched);
		} ଅନ୍ୟଥା
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

ସ୍ଥିର NORETURN ଶୂନ୍ୟ die_ଠେଲ_simple(ସଂରଚନା branch *branch,
				     ସଂରଚନା remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ଅଚଳ ଅକ୍ଷର *advice_pushdefault_maybe = "";
	ଅଚଳ ଅକ୍ଷର *advice_automergesimple_maybe = "";
	ଅଚଳ ଅକ୍ଷର *short_upstream = branch->merge[0]->src;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	ଯଦି (cfg->ଠେଲ_default == ଠେଲ_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	ଯଦି (cfg->branch_track != BRANCH_TRACK_SIMPLE)
		advice_automergesimple_maybe = _("\n"
				 "To avoid automatically configuring "
				 "an upstream branch when its name\n"
				 "won't match the local branch, see option "
				 "'simple' of branch.autoSetupMerge\n"
				 "in 'git help config'.\n");
	die(_("The upstream branch of your current branch does not match\n"
	      "the name of your current branch.  To push to the upstream branch\n"
	      "on the remote, use\n"
	      "\n"
	      "    git push %s HEAD:%s\n"
	      "\n"
	      "To push to the branch of the same name on the remote, use\n"
	      "\n"
	      "    git push %s HEAD\n"
	      "%s%s"),
	    remote->name, short_upstream,
	    remote->name, advice_pushdefault_maybe,
	    advice_automergesimple_maybe);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_detached_head_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_upstream_ref(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ସଂରଚନା branch *branch, ଅଚଳ ଅକ୍ଷର *remote_name)
{
	ଯଦି (branch->merge_nr == 0 && (flags & TRANSPORT_ଠେଲ_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		ଫେରନ୍ତୁ branch->refname;
	}

	ଯଦି (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		ଅଚଳ ଅକ୍ଷର *advice_autosetup_maybe = "";
		ଯଦି (!(flags & TRANSPORT_ଠେଲ_AUTO_UPSTREAM)) {
			advice_autosetup_maybe = _("\n"
					   "To have this happen automatically for "
					   "branches without a tracking\n"
					   "upstream, see 'push.autoSetupRemote' "
					   "in 'git help config'.\n");
		}
		die(_("The current branch %s has no upstream branch.\n"
		    "To push the current branch and set the remote as upstream, use\n"
		    "\n"
		    "    git push --set-upstream %s %s\n"
		    "%s"),
		    branch->name,
		    remote_name,
		    branch->name,
		    advice_autosetup_maybe);
	}
	ଯଦି (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	ଫେରନ୍ତୁ branch->merge[0]->src;
}

ସ୍ଥିର ଶୂନ୍ୟ setup_default_ଠେଲ_refspecs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags, ସଂରଚନା remote *remote)
{
	ସଂରଚନା branch *branch;
	ଅଚଳ ଅକ୍ଷର *dst;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା same_remote;
	ସଂରଚନା repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ *cfg = repo_config_ମୂଲ୍ୟଗୁଡ଼ିକ(the_repository);

	ବାଛ (cfg->ଠେଲ_default) {
	ସ୍ଥିତି ଠେଲ_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		ଫେରନ୍ତୁ;

	ସ୍ଥିତି ଠେଲ_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		ଫେରନ୍ତୁ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଭାଙ୍ଗ;
	}

	branch = branch_get(ଖାଲି);
	ଯଦି (!branch)
		die(_(message_detached_head_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ଖାଲି));

	ବାଛ (cfg->ଠେଲ_default) {
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	ସ୍ଥିତି ଠେଲ_DEFAULT_UNSPECIFIED:
	ସ୍ଥିତି ଠେଲ_DEFAULT_SIMPLE:
		ଯଦି (!same_remote)
			ଭାଙ୍ଗ;
		ଯଦି (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ଠେଲ_simple(branch, remote);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି ଠେଲ_DEFAULT_UPSTREAM:
		ଯଦି (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		ଭାଙ୍ଗ;

	ସ୍ଥିତି ଠେଲ_DEFAULT_CURRENT:
		ଭାଙ୍ଗ;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	ଯଦି ((*flags & TRANSPORT_ଠେଲ_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_ଠେଲ_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_pull_before_ଠେଲ[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_checkout_pull_ଠେଲ[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

ସ୍ଥିର ଶୂନ୍ୟ advise_pull_before_ଠେଲ(ଶୂନ୍ୟ)
{
	ଯଦି (!advice_enabled(ADVICE_ଠେଲ_NON_FF_CURRENT) || !advice_enabled(ADVICE_ଠେଲ_UPDATE_REJECTED))
		ଫେରନ୍ତୁ;
	advise(_(message_advice_pull_before_ଠେଲ));
}

ସ୍ଥିର ଶୂନ୍ୟ advise_checkout_pull_ଠେଲ(ଶୂନ୍ୟ)
{
	ଯଦି (!advice_enabled(ADVICE_ଠେଲ_NON_FF_MATCHING) || !advice_enabled(ADVICE_ଠେଲ_UPDATE_REJECTED))
		ଫେରନ୍ତୁ;
	advise(_(message_advice_checkout_pull_ଠେଲ));
}

ସ୍ଥିର ଶୂନ୍ୟ advise_ref_already_exists(ଶୂନ୍ୟ)
{
	ଯଦି (!advice_enabled(ADVICE_ଠେଲ_ALREADY_EXISTS) || !advice_enabled(ADVICE_ଠେଲ_UPDATE_REJECTED))
		ଫେରନ୍ତୁ;
	advise(_(message_advice_ref_already_exists));
}

ସ୍ଥିର ଶୂନ୍ୟ advise_ref_fetch_first(ଶୂନ୍ୟ)
{
	ଯଦି (!advice_enabled(ADVICE_ଠେଲ_FETCH_FIRST) || !advice_enabled(ADVICE_ଠେଲ_UPDATE_REJECTED))
		ଫେରନ୍ତୁ;
	advise(_(message_advice_ref_fetch_first));
}

ସ୍ଥିର ଶୂନ୍ୟ advise_ref_needs_force(ଶୂନ୍ୟ)
{
	ଯଦି (!advice_enabled(ADVICE_ଠେଲ_NEEDS_FORCE) || !advice_enabled(ADVICE_ଠେଲ_UPDATE_REJECTED))
		ଫେରନ୍ତୁ;
	advise(_(message_advice_ref_needs_force));
}

ସ୍ଥିର ଶୂନ୍ୟ advise_ref_needs_update(ଶୂନ୍ୟ)
{
	ଯଦି (!advice_enabled(ADVICE_ଠେଲ_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ଠେଲ_UPDATE_REJECTED))
		ଫେରନ୍ତୁ;
	advise(_(message_advice_ref_needs_update));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_with_options(ସଂରଚନା transport *transport, ସଂରଚନା refspec *rs,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା reject_reasons;
	ଅକ୍ଷର *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	ଯଦି (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : ଖାଲି);

	ଯଦି (!is_empty_cas(&cas)) {
		ଯଦି (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	ଯଦି (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ଠେଲ(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	ଯଦି (err != 0) {
		fprintf(stderr, "%s", ଠେଲ_get_color(ଠେଲ_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ଠେଲ_get_color(ଠେଲ_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	ମୁକ୍ତ(anon_url);
	ଯଦି (!err)
		ଫେରନ୍ତୁ 0;

	ଯଦି (reject_reasons & REJECT_NON_FF_HEAD) {
		advise_pull_before_ଠେଲ();
	} ଅନ୍ୟଥା ଯଦି (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ଠେଲ();
	} ଅନ୍ୟଥା ଯଦି (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} ଅନ୍ୟଥା ଯଦି (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} ଅନ୍ୟଥା ଯଦି (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} ଅନ୍ୟଥା ଯଦି (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_ଠେଲ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
		   ଅଚଳ ସଂରଚନା string_list *ଠେଲ_options,
		   ସଂରଚନା remote *remote)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା errs;
	ସଂରଚନା strvec *url;
	ସଂରଚନା refspec *ଠେଲ_refspec = &rs;

	ଯଦି (ଠେଲ_options->nr)
		flags |= TRANSPORT_ଠେଲ_OPTIONS;

	ଯଦି (!ଠେଲ_refspec->nr && !(flags & TRANSPORT_ଠେଲ_ALL)) {
		ଯଦି (remote->ଠେଲ.nr) {
			ଠେଲ_refspec = &remote->ଠେଲ;
		} ଅନ୍ୟଥା ଯଦି (!(flags & TRANSPORT_ଠେଲ_MIRROR))
			setup_default_ଠେଲ_refspecs(&flags, remote);
	}
	errs = 0;
	url = ଠେଲ_url_of_remote(remote);
	ପାଇଁ (size_t i = 0; i < url->nr; i++) {
		ସଂରଚନା transport *transport =
			transport_get(remote, url->v[i]);
		ଯଦି (flags & TRANSPORT_ଠେଲ_OPTIONS)
			transport->ଠେଲ_options = ଠେଲ_options;
		ଯଦି (ଠେଲ_with_options(transport, ଠେଲ_refspec, flags))
			errs++;
	}
	ଫେରନ୍ତୁ !!errs;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_parse_recurse_submodules(ଅଚଳ ସଂରଚନା option *opt,
				   ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *recurse_submodules = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	ଅନ୍ୟଥା {
		ଯଦି (!strcmp(arg, "only-is-on-demand")) {
			ଯଦି (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} ଅନ୍ୟଥା {
			*recurse_submodules = parse_ଠେଲ_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ set_ଠେଲ_cert_flags(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)
{
	ବାଛ (v) {
	ସ୍ଥିତି SEND_PACK_ଠେଲ_CERT_NEVER:
		*flags &= ~(TRANSPORT_ଠେଲ_CERT_ALWAYS | TRANSPORT_ଠେଲ_CERT_IF_ASKED);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SEND_PACK_ଠେଲ_CERT_ALWAYS:
		*flags |= TRANSPORT_ଠେଲ_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ଠେଲ_CERT_IF_ASKED;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SEND_PACK_ଠେଲ_CERT_IF_ASKED:
		*flags |= TRANSPORT_ଠେଲ_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ଠେଲ_CERT_ALWAYS;
		ଭାଙ୍ଗ;
	}
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_ଠେଲ_config(ଅଚଳ ଅକ୍ଷର *k, ଅଚଳ ଅକ୍ଷର *v,
			   ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଅଚଳ ଅକ୍ଷର *slot_name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags = cb;

	ଯଦି (!strcmp(k, "push.followtags")) {
		ଯଦି (git_config_bool(k, v))
			*flags |= TRANSPORT_ଠେଲ_FOLLOW_TAGS;
		ଅନ୍ୟଥା
			*flags &= ~TRANSPORT_ଠେଲ_FOLLOW_TAGS;
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "push.autosetupremote")) {
		ଯଦି (git_config_bool(k, v))
			*flags |= TRANSPORT_ଠେଲ_AUTO_UPSTREAM;
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "push.gpgsign")) {
		ବାଛ (git_parse_maybe_bool(v)) {
		ସ୍ଥିତି 0:
			set_ଠେଲ_cert_flags(flags, SEND_PACK_ଠେଲ_CERT_NEVER);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 1:
			set_ଠେଲ_cert_flags(flags, SEND_PACK_ଠେଲ_CERT_ALWAYS);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			ଯଦି (!strcasecmp(v, "if-asked"))
				set_ଠେଲ_cert_flags(flags, SEND_PACK_ଠେଲ_CERT_IF_ASKED);
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ error(_("invalid value for '%s'"), k);
		}
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ଠେଲ_recurse_submodules_arg(k, v);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "submodule.recurse")) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "push.pushoption")) {
		ଫେରନ୍ତୁ parse_transport_option(k, v, &ଠେଲ_options_config);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "color.push")) {
		ଠେଲ_use_color = git_config_colorbool(k, v);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (skip_prefix(k, "color.push.", &slot_name)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା slot = parse_ଠେଲ_color_slot(slot_name);
		ଯଦି (slot < 0)
			ଫେରନ୍ତୁ 0;
		ଯଦି (!v)
			ଫେରନ୍ତୁ config_error_nonbool(k);
		ଫେରନ୍ତୁ color_parse(v, ଠେଲ_colors[slot]);
	} ଅନ୍ୟଥା ଯଦି (!strcmp(k, "push.useforceifincludes")) {
		ଯଦି (git_config_bool(k, v))
			*flags |= TRANSPORT_ଠେଲ_FORCE_IF_INCLUDES;
		ଅନ୍ୟଥା
			*flags &= ~TRANSPORT_ଠେଲ_FORCE_IF_INCLUDES;
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ git_default_config(k, v, ctx, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_multiple(ସଂରଚନା string_list *list,
			 ଅଚଳ ସଂରଚନା string_list *ଠେଲ_options,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା tags,
			 ଅଚଳ ଅକ୍ଷର **refspecs,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା refspec_nr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
	size_t i;
	ସଂରଚନା strvec argv = STRVEC_INIT;

	strvec_ଠେଲ(&argv, "push");

	ଯଦି (flags & TRANSPORT_ଠେଲ_FORCE)
		strvec_ଠେଲ(&argv, "--force");
	ଯଦି (flags & TRANSPORT_ଠେଲ_DRY_RUN)
		strvec_ଠେଲ(&argv, "--dry-run");
	ଯଦି (flags & TRANSPORT_ଠେଲ_PORCELAIN)
		strvec_ଠେଲ(&argv, "--porcelain");
	ଯଦି (flags & TRANSPORT_ଠେଲ_PRUNE)
		strvec_ଠେଲ(&argv, "--prune");
	ଯଦି (flags & TRANSPORT_ଠେଲ_NO_HOOK)
		strvec_ଠେଲ(&argv, "--no-verify");
	ଯଦି (flags & TRANSPORT_ଠେଲ_FOLLOW_TAGS)
		strvec_ଠେଲ(&argv, "--follow-tags");
	ଯଦି (flags & TRANSPORT_ଠେଲ_SET_UPSTREAM)
		strvec_ଠେଲ(&argv, "--set-upstream");
	ଯଦି (flags & TRANSPORT_ଠେଲ_FORCE_IF_INCLUDES)
		strvec_ଠେଲ(&argv, "--force-if-includes");
	ଯଦି (flags & TRANSPORT_ଠେଲ_ALL)
		strvec_ଠେଲ(&argv, "--all");
	ଯଦି (flags & TRANSPORT_ଠେଲ_MIRROR)
		strvec_ଠେଲ(&argv, "--mirror");

	ଯଦି (flags & TRANSPORT_ଠେଲ_CERT_ALWAYS)
		strvec_ଠେଲ(&argv, "--signed=yes");
	ଅନ୍ୟଥା ଯଦି (flags & TRANSPORT_ଠେଲ_CERT_IF_ASKED)
		strvec_ଠେଲ(&argv, "--signed=if-asked");
	ଯଦି (!thin)
		strvec_ଠେଲ(&argv, "--no-thin");

	ଯଦି (deleterefs)
		strvec_ଠେଲ(&argv, "--delete");

	ଯଦି (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	ଯଦି (verbosity >= 2)
		strvec_ଠେଲ(&argv, "-v");
	ଯଦି (verbosity >= 1)
		strvec_ଠେଲ(&argv, "-v");
	ଅନ୍ୟଥା ଯଦି (verbosity < 0)
		strvec_ଠେଲ(&argv, "-q");
	ଯଦି (progress > 0)
		strvec_ଠେଲ(&argv, "--progress");
	ଅନ୍ୟଥା ଯଦି (progress == 0)
		strvec_ଠେଲ(&argv, "--no-progress");

	ଯଦି (family == TRANSPORT_FAMILY_IPV4)
		strvec_ଠେଲ(&argv, "--ipv4");
	ଅନ୍ୟଥା ଯଦି (family == TRANSPORT_FAMILY_IPV6)
		strvec_ଠେଲ(&argv, "--ipv6");

	ଯଦି (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ଠେଲ(&argv, "--recurse-submodules=check");
	ଅନ୍ୟଥା ଯଦି (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ଠେଲ(&argv, "--recurse-submodules=on-demand");
	ଅନ୍ୟଥା ଯଦି (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ଠେଲ(&argv, "--recurse-submodules=only");
	ଅନ୍ୟଥା ଯଦି (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ଠେଲ(&argv, "--recurse-submodules=no");


	ଯଦି (tags)
		strvec_ଠେଲ(&argv, "--tags");

	ପାଇଁ (i = 0; i < ଠେଲ_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ଠେଲ_options->ବସ୍ତୁଗୁଡ଼ିକ[i].string);

	ପାଇଁ (i = 0; i < cas.nr; i++) {
		ଯଦି (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} ଅନ୍ୟଥା ଯଦି (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} ଅନ୍ୟଥା {
			strvec_ଠେଲ(&argv, "--force-with-lease");
		}
	}

	ପାଇଁ (i = 0; i < list->nr; i++) {
		ଅଚଳ ଅକ୍ଷର *name = list->ବସ୍ତୁଗୁଡ଼ିକ[i].string;
		ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_ଠେଲ(&cmd.args, name);

		ପାଇଁ (j = 0; j < refspec_nr; j++)
			strvec_ଠେଲ(&cmd.args, refspecs[j]);

		ଯଦି (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		ଯଦି (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			result = 1;
		}
	}

	strvec_clear(&argv);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଶୂନ୍ୟ die_if_repo_looks_like_ref(ଅଚଳ ଅକ୍ଷର *repo)
{
	ଅଚଳ ଅକ୍ଷର *slash = strchr(repo, '/');
	ସଂରଚନା strbuf name = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

	ଯଦି (!slash || !slash[1] || file_exists(repo))
		ଫେରନ୍ତୁ;

	strbuf_add(&name, repo, slash - repo);
	ଯଦି (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		ଫେରନ୍ତୁ;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ଠେଲ_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_ଠେଲ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repository UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tags = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_cert = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_flags;
	ଅଚଳ ଅକ୍ଷର *repo = ଖାଲି;	/* default repository */
	ସଂରଚନା string_list ଠେଲ_options_cmdline = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list remote_group = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list *ଠେଲ_options;
	ଅଚଳ ସଂରଚନା string_list_item *item;

	ସଂରଚନା option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ଠେଲ_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ଠେଲ_MIRROR|TRANSPORT_ଠେଲ_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ଠେଲ_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ଠେଲ_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_ଠେଲ_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ଠେଲ_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ଠେଲ_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ଠେଲ_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ଠେଲ_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ଠେଲ_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ଠେଲ_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &ଠେଲ_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ଠେଲ_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ଠେଲ_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ଠେଲ_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_ଠେଲ_config, &flags);
	argc = parse_options(argc, argv, prefix, options, ଠେଲ_usage, 0);
	ଠେଲ_options = (ଠେଲ_options_cmdline.nr
		? &ଠେଲ_options_cmdline
		: &ଠେଲ_options_config);
	set_ଠେଲ_cert_flags(&flags, ଠେଲ_cert);

	refspec_init_ଠେଲ(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_ଠେଲ_ALL, "--all/--branches",
				  flags & TRANSPORT_ଠେଲ_MIRROR, "--mirror");
	ଯଦି (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	ଯଦି (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	ଅନ୍ୟଥା ଯଦି (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	ଅନ୍ୟଥା ଯଦି (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	ଯଦି (argc > 0)
		repo = argv[0];

	ଯଦି (repo) {
		ଯଦି (!add_remote_or_group(repo, &remote_group)) {
			ସଂରଚନା remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			ଯଦି (advice_enabled(ADVICE_ଠେଲ_REPO_LOOKS_LIKE_REF))
				die_if_repo_looks_like_ref(repo);

			/*
			 * Not a configured remote name or group name.
			 * Try treating it as a direct URL or path, e.g.
			 *   git push /tmp/foo.git
			 *   git push https://github.com/user/repo.git
			 * pushremote_get() creates an anonymous remote
			 * from the URL so the loop below can handle it
			 * identically to a named remote.
			 */
			r = pushremote_get(repo);
			ଯଦି (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} ଅନ୍ୟଥା {
		ସଂରଚନା remote *r = pushremote_get(ଖାଲି);
		ଯଦି (!r)
			die(_("No configured push destination.\n"
			    "Either specify the URL from the command-line or configure a remote repository using\n"
			    "\n"
			    "    git remote add <name> <url>\n"
			    "\n"
			    "and then push using the remote name\n"
			    "\n"
			    "    git push <name>\n"
			    "\n"
			    "To push to multiple remotes at once, configure a remote group using\n"
			    "\n"
			    "    git config remotes.<groupname> \"<remote1> <remote2>\"\n"
			    "\n"
			    "and then push using the group name\n"
			    "\n"
			    "    git push <groupname>\n"));
		string_list_append(&remote_group, r->name);
	}

	ଯଦି (!is_empty_cas(&cas) && (flags & TRANSPORT_ଠେଲ_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, ଠେଲ_options)
		ଯଦି (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	ଯଦି (remote_group.nr == 1) {
		/*
		 * Single remote (the common case): run do_push() directly
		 * in this process.  The loop runs exactly once.
		 *
		 * Mirror detection and the --mirror/--all + refspec conflict
		 * checks are done here.  rs is rebuilt so that per-remote push
		 * mappings (remote.NAME.push config) are resolved against the
		 * correct remote.  inner_flags is a snapshot of flags so that a
		 * mirror remote cannot bleed TRANSPORT_PUSH_FORCE into any
		 * subsequent call.
		 */
		base_flags = flags;
		{
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner_flags = base_flags;
			ସଂରଚନା remote *r = pushremote_get(remote_group.ବସ୍ତୁଗୁଡ଼ିକ[0].string);
			ଯଦି (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.ବସ୍ତୁଗୁଡ଼ିକ[0].string);

			ଯଦି (r->mirror)
				inner_flags |= (TRANSPORT_ଠେଲ_MIRROR|TRANSPORT_ଠେଲ_FORCE);

			ଯଦି (inner_flags & TRANSPORT_ଠେଲ_ALL) {
				ଯଦି (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			ଯଦି (inner_flags & TRANSPORT_ଠେଲ_MIRROR) {
				ଯଦି (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (ସଂରଚନା refspec) REFSPEC_INIT_ଠେଲ(the_hash_algo);

			ଯଦି (tags)
				refspec_append(&rs, "refs/tags/*");
			ଯଦି (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_ଠେଲ(inner_flags, ଠେଲ_options, r);
		}
	} ଅନ୍ୟଥା {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		ଯଦି (flags & TRANSPORT_ଠେଲ_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ଠେଲ_multiple(&remote_group, ଠେଲ_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : ଖାଲି,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ଠେଲ_options_cmdline, 0);
	string_list_clear(&ଠେଲ_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	ଯଦି (rc == -1)
		usage_with_options(ଠେଲ_usage, options);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ rc;
}
