/*
 * "git push"
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refspec.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "remote.h"
#অন্তর্ভাব "transport.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "submodule-config.h"
#অন্তর্ভাব "send-pack.h"
#অন্তর্ভাব "trace2.h"
#অন্তর্ভাব "color.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক ঠেলো_usage[] = {
	N_("git push [<options>] [<repository> [<refspec>...]]"),
	ফাঁকা,
};

স্থির গণনা git_colorbool ঠেলো_use_color = GIT_COLOR_UNKNOWN;
স্থির অক্ষর ঠেলো_colors[][COLOR_MAXLEN] = {
	GIT_COLOR_RESET,
	GIT_COLOR_RED,	/* ERROR */
};

গণনা color_ঠেলো {
	ঠেলো_COLOR_RESET = 0,
	ঠেলো_COLOR_ERROR = 1
};

স্থির পূর্ণসংখ্যা parse_ঠেলো_color_slot(ধ্রুবক অক্ষর *slot)
{
	যদি (!strcasecmp(slot, "reset"))
		ফেরত ঠেলো_COLOR_RESET;
	যদি (!strcasecmp(slot, "error"))
		ফেরত ঠেলো_COLOR_ERROR;
	ফেরত -1;
}

স্থির ধ্রুবক অক্ষর *ঠেলো_get_color(গণনা color_ঠেলো ix)
{
	যদি (want_color_stderr(ঠেলো_use_color))
		ফেরত ঠেলো_colors[ix];
	ফেরত "";
}

স্থির পূর্ণসংখ্যা thin = 1;
স্থির পূর্ণসংখ্যা deleterefs;
স্থির ধ্রুবক অক্ষর *receivepack;
স্থির পূর্ণসংখ্যা verbosity;
স্থির পূর্ণসংখ্যা progress = -1;
স্থির পূর্ণসংখ্যা recurse_submodules = RECURSE_SUBMODULES_DEFAULT;
স্থির গণনা transport_family family;

স্থির কাঠামো ঠেলো_cas_option cas;

স্থির কাঠামো refspec rs;

স্থির কাঠামো string_list ঠেলো_options_config = STRING_LIST_INIT_DUP;

স্থির শূন্য refspec_append_mapped(কাঠামো refspec *refspec, ধ্রুবক অক্ষর *ref,
				  কাঠামো remote *remote, কাঠামো ref *matched)
{
	ধ্রুবক অক্ষর *branch_name;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	যদি (remote->ঠেলো.nr) {
		কাঠামো refspec_item query = {
			.src = matched->name,
		};

		যদি (!refspec_find_match(&remote->ঠেলো, &query) && query.dst) {
			refspec_appendf(refspec, "%s%s:%s",
					query.force ? "+" : "",
					query.src, query.dst);
			মুক্ত(query.dst);
			ফেরত;
		}
	}

	যদি (cfg->ঠেলো_default == ঠেলো_DEFAULT_UPSTREAM &&
	    skip_prefix(matched->name, "refs/heads/", &branch_name)) {
		কাঠামো branch *branch = branch_get(branch_name);
		যদি (branch->merge_nr == 1 && branch->merge[0]->src) {
			refspec_appendf(refspec, "%s:%s",
					ref, branch->merge[0]->src);
			ফেরত;
		}
	}

	refspec_append(refspec, ref);
}

স্থির শূন্য set_refspecs(ধ্রুবক অক্ষর **refs, পূর্ণসংখ্যা nr, কাঠামো remote *remote)
{
	কাঠামো ref *local_refs = ফাঁকা;
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < nr; i++) {
		ধ্রুবক অক্ষর *ref = refs[i];
		যদি (!strcmp("tag", ref)) {
			যদি (nr <= ++i)
				die(_("tag shorthand without <tag>"));
			ref = refs[i];
			যদি (deleterefs)
				refspec_appendf(&rs, ":refs/tags/%s", ref);
			অন্যথা
				refspec_appendf(&rs, "refs/tags/%s", ref);
		} অন্যথা যদি (deleterefs) {
			যদি (strchr(ref, ':') || !*ref)
				die(_("--delete only accepts plain target ref names"));
			refspec_appendf(&rs, ":%s", ref);
		} অন্যথা যদি (!strchr(ref, ':')) {
			কাঠামো ref *matched = ফাঁকা;

			/* lazily grab local_refs */
			যদি (!local_refs)
				local_refs = get_local_heads();

			/* Does "ref" uniquely name our ref? */
			যদি (গুনতি_refspec_match(ref, local_refs, &matched) != 1)
				refspec_append(&rs, ref);
			অন্যথা
				refspec_append_mapped(&rs, ref, remote, matched);
		} অন্যথা
			refspec_append(&rs, ref);
	}
	free_refs(local_refs);
}

স্থির NORETURN শূন্য die_ঠেলো_simple(কাঠামো branch *branch,
				     কাঠামো remote *remote)
{
	/*
	 * There's no point in using shorten_unambiguous_ref here,
	 * as the ambiguity would be on the remote side, not what
	 * we have locally. Plus, this is supposed to be the simple
	 * mode. If the user is doing something crazy like setting
	 * upstream to a non-branch, we should probably be showing
	 * them the big ugly fully qualified ref.
	 */
	ধ্রুবক অক্ষর *advice_pushdefault_maybe = "";
	ধ্রুবক অক্ষর *advice_automergesimple_maybe = "";
	ধ্রুবক অক্ষর *short_upstream = branch->merge[0]->src;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	skip_prefix(short_upstream, "refs/heads/", &short_upstream);

	/*
	 * Don't show advice for people who explicitly set
	 * push.default.
	 */
	যদি (cfg->ঠেলো_default == ঠেলো_DEFAULT_UNSPECIFIED)
		advice_pushdefault_maybe = _("\n"
				 "To choose either option permanently, "
				 "see push.default in 'git help config'.\n");
	যদি (cfg->branch_track != BRANCH_TRACK_SIMPLE)
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

স্থির ধ্রুবক অক্ষর message_detached_শীর্ষ_die[] =
	N_("You are not currently on a branch.\n"
	   "To push the history leading to the current (detached HEAD)\n"
	   "state now, use\n"
	   "\n"
	   "    git push %s HEAD:<name-of-remote-branch>\n");

স্থির ধ্রুবক অক্ষর *get_upstream_ref(পূর্ণসংখ্যা flags, কাঠামো branch *branch, ধ্রুবক অক্ষর *remote_name)
{
	যদি (branch->merge_nr == 0 && (flags & TRANSPORT_ঠেলো_AUTO_UPSTREAM)) {
		/* if missing, assume same; set_upstream will be defined later */
		ফেরত branch->refname;
	}

	যদি (!branch->merge_nr || !branch->merge || !branch->remote_name) {
		ধ্রুবক অক্ষর *advice_autosetup_maybe = "";
		যদি (!(flags & TRANSPORT_ঠেলো_AUTO_UPSTREAM)) {
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
	যদি (branch->merge_nr != 1)
		die(_("The current branch %s has multiple upstream branches, "
		    "refusing to push."), branch->name);

	ফেরত branch->merge[0]->src;
}

স্থির শূন্য setup_default_ঠেলো_refspecs(পূর্ণসংখ্যা *flags, কাঠামো remote *remote)
{
	কাঠামো branch *branch;
	ধ্রুবক অক্ষর *dst;
	পূর্ণসংখ্যা same_remote;
	কাঠামো repo_config_মানসমূহ *cfg = repo_config_মানসমূহ(the_repository);

	নির্বাচন (cfg->ঠেলো_default) {
	ক্ষেত্র ঠেলো_DEFAULT_MATCHING:
		refspec_append(&rs, ":");
		ফেরত;

	ক্ষেত্র ঠেলো_DEFAULT_NOTHING:
		die(_("You didn't specify any refspecs to push, and "
		    "push.default is \"nothing\"."));
		ফেরত;
	পূর্বনির্ধারিত:
		বিরতি;
	}

	branch = branch_get(ফাঁকা);
	যদি (!branch)
		die(_(message_detached_শীর্ষ_die), remote->name);

	dst = branch->refname;
	same_remote = !strcmp(remote->name, remote_for_branch(branch, ফাঁকা));

	নির্বাচন (cfg->ঠেলো_default) {
	পূর্বনির্ধারিত:
	ক্ষেত্র ঠেলো_DEFAULT_UNSPECIFIED:
	ক্ষেত্র ঠেলো_DEFAULT_SIMPLE:
		যদি (!same_remote)
			বিরতি;
		যদি (strcmp(branch->refname, get_upstream_ref(*flags, branch, remote->name)))
			die_ঠেলো_simple(branch, remote);
		বিরতি;

	ক্ষেত্র ঠেলো_DEFAULT_UPSTREAM:
		যদি (!same_remote)
			die(_("You are pushing to remote '%s', which is not the upstream of\n"
			      "your current branch '%s', without telling me what to push\n"
			      "to update which remote branch."),
			    remote->name, branch->name);
		dst = get_upstream_ref(*flags, branch, remote->name);
		বিরতি;

	ক্ষেত্র ঠেলো_DEFAULT_CURRENT:
		বিরতি;
	}

	/*
	 * this is a default push - if auto-upstream is enabled and there is
	 * no upstream defined, then set it (with options 'simple', 'upstream',
	 * and 'current').
	 */
	যদি ((*flags & TRANSPORT_ঠেলো_AUTO_UPSTREAM) && branch->merge_nr == 0)
		*flags |= TRANSPORT_ঠেলো_SET_UPSTREAM;

	refspec_appendf(&rs, "%s:%s", branch->refname, dst);
}

স্থির ধ্রুবক অক্ষর message_advice_pull_before_ঠেলো[] =
	N_("Updates were rejected because the tip of your current branch is behind\n"
	   "its remote counterpart. If you want to integrate the remote changes,\n"
	   "use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

স্থির ধ্রুবক অক্ষর message_advice_checkout_pull_ঠেলো[] =
	N_("Updates were rejected because a pushed branch tip is behind its remote\n"
	   "counterpart. If you want to integrate the remote changes, use 'git pull'\n"
	   "before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

স্থির ধ্রুবক অক্ষর message_advice_ref_fetch_first[] =
	N_("Updates were rejected because the remote contains work that you do not\n"
	   "have locally. This is usually caused by another repository pushing to\n"
	   "the same ref. If you want to integrate the remote changes, use\n"
	   "'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

স্থির ধ্রুবক অক্ষর message_advice_ref_already_exists[] =
	N_("Updates were rejected because the tag already exists in the remote.");

স্থির ধ্রুবক অক্ষর message_advice_ref_needs_force[] =
	N_("You cannot update a remote ref that points at a non-commit object,\n"
	   "or update a remote ref to make it point at a non-commit object,\n"
	   "without using the '--force' option.\n");

স্থির ধ্রুবক অক্ষর message_advice_ref_needs_update[] =
	N_("Updates were rejected because the tip of the remote-tracking branch has\n"
	   "been updated since the last checkout. If you want to integrate the\n"
	   "remote changes, use 'git pull' before pushing again.\n"
	   "See the 'Note about fast-forwards' in 'git push --help' for details.");

স্থির শূন্য advise_pull_before_ঠেলো(শূন্য)
{
	যদি (!advice_enabled(ADVICE_ঠেলো_NON_FF_CURRENT) || !advice_enabled(ADVICE_ঠেলো_UPDATE_REJECTED))
		ফেরত;
	advise(_(message_advice_pull_before_ঠেলো));
}

স্থির শূন্য advise_checkout_pull_ঠেলো(শূন্য)
{
	যদি (!advice_enabled(ADVICE_ঠেলো_NON_FF_MATCHING) || !advice_enabled(ADVICE_ঠেলো_UPDATE_REJECTED))
		ফেরত;
	advise(_(message_advice_checkout_pull_ঠেলো));
}

স্থির শূন্য advise_ref_already_exists(শূন্য)
{
	যদি (!advice_enabled(ADVICE_ঠেলো_ALREADY_EXISTS) || !advice_enabled(ADVICE_ঠেলো_UPDATE_REJECTED))
		ফেরত;
	advise(_(message_advice_ref_already_exists));
}

স্থির শূন্য advise_ref_fetch_first(শূন্য)
{
	যদি (!advice_enabled(ADVICE_ঠেলো_FETCH_FIRST) || !advice_enabled(ADVICE_ঠেলো_UPDATE_REJECTED))
		ফেরত;
	advise(_(message_advice_ref_fetch_first));
}

স্থির শূন্য advise_ref_needs_force(শূন্য)
{
	যদি (!advice_enabled(ADVICE_ঠেলো_NEEDS_FORCE) || !advice_enabled(ADVICE_ঠেলো_UPDATE_REJECTED))
		ফেরত;
	advise(_(message_advice_ref_needs_force));
}

স্থির শূন্য advise_ref_needs_update(শূন্য)
{
	যদি (!advice_enabled(ADVICE_ঠেলো_REF_NEEDS_UPDATE) || !advice_enabled(ADVICE_ঠেলো_UPDATE_REJECTED))
		ফেরত;
	advise(_(message_advice_ref_needs_update));
}

স্থির পূর্ণসংখ্যা ঠেলো_with_options(কাঠামো transport *transport, কাঠামো refspec *rs,
			     পূর্ণসংখ্যা flags)
{
	পূর্ণসংখ্যা err;
	unsigned পূর্ণসংখ্যা reject_reasons;
	অক্ষর *anon_url = transport_anonymize_url(transport->url);

	transport_set_verbosity(transport, verbosity, progress);
	transport->family = family;

	যদি (receivepack)
		transport_set_option(transport,
				     TRANS_OPT_RECEIVEPACK, receivepack);
	transport_set_option(transport, TRANS_OPT_THIN, thin ? "yes" : ফাঁকা);

	যদি (!is_empty_cas(&cas)) {
		যদি (!transport->smart_options)
			die("underlying transport does not support --%s option",
			    "force-with-lease");
		transport->smart_options->cas = &cas;
	}

	যদি (verbosity > 0)
		fprintf(stderr, _("Pushing to %s\n"), anon_url);
	trace2_region_enter("push", "transport_push", the_repository);
	err = transport_ঠেলো(the_repository, transport,
			     rs, flags, &reject_reasons);
	trace2_region_leave("push", "transport_push", the_repository);
	যদি (err != 0) {
		fprintf(stderr, "%s", ঠেলো_get_color(ঠেলো_COLOR_ERROR));
		error(_("failed to push some refs to '%s'"), anon_url);
		fprintf(stderr, "%s", ঠেলো_get_color(ঠেলো_COLOR_RESET));
	}

	err |= transport_disconnect(transport);
	মুক্ত(anon_url);
	যদি (!err)
		ফেরত 0;

	যদি (reject_reasons & REJECT_NON_FF_শীর্ষ) {
		advise_pull_before_ঠেলো();
	} অন্যথা যদি (reject_reasons & REJECT_NON_FF_OTHER) {
		advise_checkout_pull_ঠেলো();
	} অন্যথা যদি (reject_reasons & REJECT_ALREADY_EXISTS) {
		advise_ref_already_exists();
	} অন্যথা যদি (reject_reasons & REJECT_FETCH_FIRST) {
		advise_ref_fetch_first();
	} অন্যথা যদি (reject_reasons & REJECT_NEEDS_FORCE) {
		advise_ref_needs_force();
	} অন্যথা যদি (reject_reasons & REJECT_REF_NEEDS_UPDATE) {
		advise_ref_needs_update();
	}

	ফেরত 1;
}

স্থির পূর্ণসংখ্যা do_ঠেলো(পূর্ণসংখ্যা flags,
		   ধ্রুবক কাঠামো string_list *ঠেলো_options,
		   কাঠামো remote *remote)
{
	পূর্ণসংখ্যা errs;
	কাঠামো strvec *url;
	কাঠামো refspec *ঠেলো_refspec = &rs;

	যদি (ঠেলো_options->nr)
		flags |= TRANSPORT_ঠেলো_OPTIONS;

	যদি (!ঠেলো_refspec->nr && !(flags & TRANSPORT_ঠেলো_ALL)) {
		যদি (remote->ঠেলো.nr) {
			ঠেলো_refspec = &remote->ঠেলো;
		} অন্যথা যদি (!(flags & TRANSPORT_ঠেলো_MIRROR))
			setup_default_ঠেলো_refspecs(&flags, remote);
	}
	errs = 0;
	url = ঠেলো_url_of_remote(remote);
	জন্য (আকার_t i = 0; i < url->nr; i++) {
		কাঠামো transport *transport =
			transport_get(remote, url->v[i]);
		যদি (flags & TRANSPORT_ঠেলো_OPTIONS)
			transport->ঠেলো_options = ঠেলো_options;
		যদি (ঠেলো_with_options(transport, ঠেলো_refspec, flags))
			errs++;
	}
	ফেরত !!errs;
}

স্থির পূর্ণসংখ্যা option_parse_recurse_submodules(ধ্রুবক কাঠামো option *opt,
				   ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *recurse_submodules = opt->মান;

	যদি (unset)
		*recurse_submodules = RECURSE_SUBMODULES_OFF;
	অন্যথা {
		যদি (!strcmp(arg, "only-is-on-demand")) {
			যদি (*recurse_submodules == RECURSE_SUBMODULES_ONLY) {
				warning(_("recursing into submodule with push.recurseSubmodules=only; using on-demand instead"));
				*recurse_submodules = RECURSE_SUBMODULES_ON_DEMAND;
			}
		} অন্যথা {
			*recurse_submodules = parse_ঠেলো_recurse_submodules_arg(opt->long_name, arg);
		}
	}

	ফেরত 0;
}

স্থির শূন্য set_ঠেলো_cert_flags(পূর্ণসংখ্যা *flags, পূর্ণসংখ্যা v)
{
	নির্বাচন (v) {
	ক্ষেত্র SEND_PACK_ঠেলো_CERT_NEVER:
		*flags &= ~(TRANSPORT_ঠেলো_CERT_ALWAYS | TRANSPORT_ঠেলো_CERT_IF_ASKED);
		বিরতি;
	ক্ষেত্র SEND_PACK_ঠেলো_CERT_ALWAYS:
		*flags |= TRANSPORT_ঠেলো_CERT_ALWAYS;
		*flags &= ~TRANSPORT_ঠেলো_CERT_IF_ASKED;
		বিরতি;
	ক্ষেত্র SEND_PACK_ঠেলো_CERT_IF_ASKED:
		*flags |= TRANSPORT_ঠেলো_CERT_IF_ASKED;
		*flags &= ~TRANSPORT_ঠেলো_CERT_ALWAYS;
		বিরতি;
	}
}


স্থির পূর্ণসংখ্যা git_ঠেলো_config(ধ্রুবক অক্ষর *k, ধ্রুবক অক্ষর *v,
			   ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	ধ্রুবক অক্ষর *slot_name;
	পূর্ণসংখ্যা *flags = cb;

	যদি (!strcmp(k, "push.followtags")) {
		যদি (git_config_bool(k, v))
			*flags |= TRANSPORT_ঠেলো_FOLLOW_TAGS;
		অন্যথা
			*flags &= ~TRANSPORT_ঠেলো_FOLLOW_TAGS;
		ফেরত 0;
	} অন্যথা যদি (!strcmp(k, "push.autosetupremote")) {
		যদি (git_config_bool(k, v))
			*flags |= TRANSPORT_ঠেলো_AUTO_UPSTREAM;
		ফেরত 0;
	} অন্যথা যদি (!strcmp(k, "push.gpgsign")) {
		নির্বাচন (git_parse_maybe_bool(v)) {
		ক্ষেত্র 0:
			set_ঠেলো_cert_flags(flags, SEND_PACK_ঠেলো_CERT_NEVER);
			বিরতি;
		ক্ষেত্র 1:
			set_ঠেলো_cert_flags(flags, SEND_PACK_ঠেলো_CERT_ALWAYS);
			বিরতি;
		পূর্বনির্ধারিত:
			যদি (!strcasecmp(v, "if-asked"))
				set_ঠেলো_cert_flags(flags, SEND_PACK_ঠেলো_CERT_IF_ASKED);
			অন্যথা
				ফেরত error(_("invalid value for '%s'"), k);
		}
	} অন্যথা যদি (!strcmp(k, "push.recursesubmodules")) {
		recurse_submodules = parse_ঠেলো_recurse_submodules_arg(k, v);
	} অন্যথা যদি (!strcmp(k, "submodule.recurse")) {
		পূর্ণসংখ্যা val = git_config_bool(k, v) ?
			RECURSE_SUBMODULES_ON_DEMAND : RECURSE_SUBMODULES_OFF;
		recurse_submodules = val;
	} অন্যথা যদি (!strcmp(k, "push.pushoption")) {
		ফেরত parse_transport_option(k, v, &ঠেলো_options_config);
	} অন্যথা যদি (!strcmp(k, "color.push")) {
		ঠেলো_use_color = git_config_colorbool(k, v);
		ফেরত 0;
	} অন্যথা যদি (skip_prefix(k, "color.push.", &slot_name)) {
		পূর্ণসংখ্যা slot = parse_ঠেলো_color_slot(slot_name);
		যদি (slot < 0)
			ফেরত 0;
		যদি (!v)
			ফেরত config_error_nonbool(k);
		ফেরত color_parse(v, ঠেলো_colors[slot]);
	} অন্যথা যদি (!strcmp(k, "push.useforceifincludes")) {
		যদি (git_config_bool(k, v))
			*flags |= TRANSPORT_ঠেলো_FORCE_IF_INCLUDES;
		অন্যথা
			*flags &= ~TRANSPORT_ঠেলো_FORCE_IF_INCLUDES;
		ফেরত 0;
	}

	ফেরত git_default_config(k, v, ctx, ফাঁকা);
}

স্থির পূর্ণসংখ্যা ঠেলো_multiple(কাঠামো string_list *list,
			 ধ্রুবক কাঠামো string_list *ঠেলো_options,
			 পূর্ণসংখ্যা flags,
			 পূর্ণসংখ্যা tags,
			 ধ্রুবক অক্ষর **refspecs,
			 পূর্ণসংখ্যা refspec_nr)
{
	পূর্ণসংখ্যা ফলাফল = 0;
	আকার_t i;
	কাঠামো strvec argv = STRVEC_INIT;

	strvec_ঠেলো(&argv, "push");

	যদি (flags & TRANSPORT_ঠেলো_FORCE)
		strvec_ঠেলো(&argv, "--force");
	যদি (flags & TRANSPORT_ঠেলো_DRY_RUN)
		strvec_ঠেলো(&argv, "--dry-run");
	যদি (flags & TRANSPORT_ঠেলো_PORCELAIN)
		strvec_ঠেলো(&argv, "--porcelain");
	যদি (flags & TRANSPORT_ঠেলো_PRUNE)
		strvec_ঠেলো(&argv, "--prune");
	যদি (flags & TRANSPORT_ঠেলো_NO_HOOK)
		strvec_ঠেলো(&argv, "--no-verify");
	যদি (flags & TRANSPORT_ঠেলো_FOLLOW_TAGS)
		strvec_ঠেলো(&argv, "--follow-tags");
	যদি (flags & TRANSPORT_ঠেলো_SET_UPSTREAM)
		strvec_ঠেলো(&argv, "--set-upstream");
	যদি (flags & TRANSPORT_ঠেলো_FORCE_IF_INCLUDES)
		strvec_ঠেলো(&argv, "--force-if-includes");
	যদি (flags & TRANSPORT_ঠেলো_ALL)
		strvec_ঠেলো(&argv, "--all");
	যদি (flags & TRANSPORT_ঠেলো_MIRROR)
		strvec_ঠেলো(&argv, "--mirror");

	যদি (flags & TRANSPORT_ঠেলো_CERT_ALWAYS)
		strvec_ঠেলো(&argv, "--signed=yes");
	অন্যথা যদি (flags & TRANSPORT_ঠেলো_CERT_IF_ASKED)
		strvec_ঠেলো(&argv, "--signed=if-asked");
	যদি (!thin)
		strvec_ঠেলো(&argv, "--no-thin");

	যদি (deleterefs)
		strvec_ঠেলো(&argv, "--delete");

	যদি (receivepack)
		strvec_pushf(&argv, "--receive-pack=%s", receivepack);
	যদি (verbosity >= 2)
		strvec_ঠেলো(&argv, "-v");
	যদি (verbosity >= 1)
		strvec_ঠেলো(&argv, "-v");
	অন্যথা যদি (verbosity < 0)
		strvec_ঠেলো(&argv, "-q");
	যদি (progress > 0)
		strvec_ঠেলো(&argv, "--progress");
	অন্যথা যদি (progress == 0)
		strvec_ঠেলো(&argv, "--no-progress");

	যদি (family == TRANSPORT_FAMILY_IPV4)
		strvec_ঠেলো(&argv, "--ipv4");
	অন্যথা যদি (family == TRANSPORT_FAMILY_IPV6)
		strvec_ঠেলো(&argv, "--ipv6");

	যদি (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		strvec_ঠেলো(&argv, "--recurse-submodules=check");
	অন্যথা যদি (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		strvec_ঠেলো(&argv, "--recurse-submodules=on-demand");
	অন্যথা যদি (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		strvec_ঠেলো(&argv, "--recurse-submodules=only");
	অন্যথা যদি (recurse_submodules == RECURSE_SUBMODULES_OFF)
		strvec_ঠেলো(&argv, "--recurse-submodules=no");


	যদি (tags)
		strvec_ঠেলো(&argv, "--tags");

	জন্য (i = 0; i < ঠেলো_options->nr; i++)
		strvec_pushf(&argv, "--push-option=%s",
			     ঠেলো_options->বস্তুসমূহ[i].string);

	জন্য (i = 0; i < cas.nr; i++) {
		যদি (cas.entry[i].use_tracking) {
			strvec_pushf(&argv, "--force-with-lease=%s",
				     cas.entry[i].refname);
		} অন্যথা যদি (!is_null_oid(&cas.entry[i].expect)) {
			strvec_pushf(&argv, "--force-with-lease=%s:%s",
				     cas.entry[i].refname,
				     oid_to_hex(&cas.entry[i].expect));
		} অন্যথা {
			strvec_ঠেলো(&argv, "--force-with-lease");
		}
	}

	জন্য (i = 0; i < list->nr; i++) {
		ধ্রুবক অক্ষর *name = list->বস্তুসমূহ[i].string;
		কাঠামো child_process cmd = CHILD_PROCESS_INIT;
		পূর্ণসংখ্যা j;

		strvec_pushv(&cmd.args, argv.v);
		strvec_ঠেলো(&cmd.args, name);

		জন্য (j = 0; j < refspec_nr; j++)
			strvec_ঠেলো(&cmd.args, refspecs[j]);

		যদি (verbosity >= 0)
			printf(_("Pushing to %s\n"), name);

		cmd.git_cmd = 1;
		যদি (run_command(&cmd)) {
			error(_("could not push to %s"), name);
			ফলাফল = 1;
		}
	}

	strvec_clear(&argv);
	ফেরত ফলাফল;
}

স্থির শূন্য die_if_repo_looks_like_ref(ধ্রুবক অক্ষর *repo)
{
	ধ্রুবক অক্ষর *slash = strchr(repo, '/');
	কাঠামো strbuf name = STRBUF_INIT;
	পূর্ণসংখ্যা code;

	যদি (!slash || !slash[1] || file_exists(repo))
		ফেরত;

	strbuf_add(&name, repo, slash - repo);
	যদি (!remote_is_configured(remote_get(name.buf), 0)) {
		strbuf_release(&name);
		ফেরত;
	}

	code = die_message(_("'%s' is not a valid push target"), repo);
	advise_if_enabled(ADVICE_ঠেলো_REPO_LOOKS_LIKE_REF,
			  _("Did you mean to use: git push %s %s?"),
			  name.buf, slash + 1);
	strbuf_release(&name);
	exit(code);
}

পূর্ণসংখ্যা cmd_ঠেলো(পূর্ণসংখ্যা argc,
	     ধ্রুবক অক্ষর **argv,
	     ধ্রুবক অক্ষর *prefix,
	     কাঠামো repository *repository UNUSED)
{
	পূর্ণসংখ্যা flags = 0;
	পূর্ণসংখ্যা tags = 0;
	পূর্ণসংখ্যা ঠেলো_cert = -1;
	পূর্ণসংখ্যা rc = 0;
	পূর্ণসংখ্যা base_flags;
	ধ্রুবক অক্ষর *repo = ফাঁকা;	/* default repository */
	কাঠামো string_list ঠেলো_options_cmdline = STRING_LIST_INIT_DUP;
	কাঠামো string_list remote_group = STRING_LIST_INIT_DUP;
	কাঠামো string_list *ঠেলো_options;
	ধ্রুবক কাঠামো string_list_item *item;

	কাঠামো option options[] = {
		OPT__VERBOSITY(&verbosity),
		OPT_STRING( 0 , "repo", &repo, N_("repository"), N_("repository")),
		OPT_BIT( 0 , "all", &flags, N_("push all branches"), TRANSPORT_ঠেলো_ALL),
		OPT_ALIAS( 0 , "branches", "all"),
		OPT_BIT( 0 , "mirror", &flags, N_("mirror all refs"),
			    (TRANSPORT_ঠেলো_MIRROR|TRANSPORT_ঠেলো_FORCE)),
		OPT_BOOL('d', "delete", &deleterefs, N_("delete refs")),
		OPT_BOOL( 0 , "tags", &tags, N_("push tags (can't be used with --all or --branches or --mirror)")),
		OPT_BIT('n' , "dry-run", &flags, N_("dry run"), TRANSPORT_ঠেলো_DRY_RUN),
		OPT_BIT( 0,  "porcelain", &flags, N_("machine-readable output"), TRANSPORT_ঠেলো_PORCELAIN),
		OPT_BIT('f', "force", &flags, N_("force updates"), TRANSPORT_ঠেলো_FORCE),
		OPT_CALLBACK_F(0, "force-with-lease", &cas, N_("<refname>:<expect>"),
			       N_("require old value of ref to be at this value"),
			       PARSE_OPT_OPTARG | PARSE_OPT_LITERAL_ARGHELP, parseopt_ঠেলো_cas_option),
		OPT_BIT(0, TRANS_OPT_FORCE_IF_INCLUDES, &flags,
			N_("require remote updates to be integrated locally"),
			TRANSPORT_ঠেলো_FORCE_IF_INCLUDES),
		OPT_CALLBACK(0, "recurse-submodules", &recurse_submodules, "(check|on-demand|no)",
			     N_("control recursive pushing of submodules"), option_parse_recurse_submodules),
		OPT_BOOL_F( 0 , "thin", &thin, N_("use thin pack"), PARSE_OPT_NOCOMPLETE),
		OPT_STRING( 0 , "receive-pack", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_STRING( 0 , "exec", &receivepack, "receive-pack", N_("receive pack program")),
		OPT_BIT('u', "set-upstream", &flags, N_("set upstream for git pull/status"),
			TRANSPORT_ঠেলো_SET_UPSTREAM),
		OPT_BOOL(0, "progress", &progress, N_("force progress reporting")),
		OPT_BIT(0, "prune", &flags, N_("prune locally removed refs"),
			TRANSPORT_ঠেলো_PRUNE),
		OPT_BIT(0, "no-verify", &flags, N_("bypass pre-push hook"), TRANSPORT_ঠেলো_NO_HOOK),
		OPT_BIT(0, "follow-tags", &flags, N_("push missing but relevant tags"),
			TRANSPORT_ঠেলো_FOLLOW_TAGS),
		OPT_CALLBACK_F(0, "signed", &ঠেলো_cert, "(yes|no|if-asked)", N_("GPG sign the push"),
				PARSE_OPT_OPTARG, option_parse_ঠেলো_signed),
		OPT_BIT(0, "atomic", &flags, N_("request atomic transaction on remote side"), TRANSPORT_ঠেলো_ATOMIC),
		OPT_STRING_LIST('o', "push-option", &ঠেলো_options_cmdline, N_("server-specific"), N_("option to transmit")),
		OPT_IPVERSION(&family),
		OPT_END()
	};

	packet_trace_identity("push");
	repo_config(the_repository, git_ঠেলো_config, &flags);
	argc = parse_options(argc, argv, prefix, options, ঠেলো_usage, 0);
	ঠেলো_options = (ঠেলো_options_cmdline.nr
		? &ঠেলো_options_cmdline
		: &ঠেলো_options_config);
	set_ঠেলো_cert_flags(&flags, ঠেলো_cert);

	refspec_init_ঠেলো(&rs, the_hash_algo);

	die_for_incompatible_opt4(deleterefs, "--delete",
				  tags, "--tags",
				  flags & TRANSPORT_ঠেলো_ALL, "--all/--branches",
				  flags & TRANSPORT_ঠেলো_MIRROR, "--mirror");
	যদি (deleterefs && argc < 2)
		die(_("--delete doesn't make sense without any refs"));

	যদি (recurse_submodules == RECURSE_SUBMODULES_CHECK)
		flags |= TRANSPORT_RECURSE_SUBMODULES_CHECK;
	অন্যথা যদি (recurse_submodules == RECURSE_SUBMODULES_ON_DEMAND)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ON_DEMAND;
	অন্যথা যদি (recurse_submodules == RECURSE_SUBMODULES_ONLY)
		flags |= TRANSPORT_RECURSE_SUBMODULES_ONLY;

	যদি (argc > 0)
		repo = argv[0];

	যদি (repo) {
		যদি (!add_remote_or_group(repo, &remote_group)) {
			কাঠামো remote *r;

			/*
			 * Check the advice up front to avoid the remote
			 * lookup when the hint is off. The helper still
			 * calls advise_if_enabled() so the hint carries the
			 * standard "disable this message" instructions.
			 */
			যদি (advice_enabled(ADVICE_ঠেলো_REPO_LOOKS_LIKE_REF))
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
			যদি (!r)
				die(_("bad repository '%s'"), repo);
			string_list_append(&remote_group, r->name);
		}
	} অন্যথা {
		কাঠামো remote *r = pushremote_get(ফাঁকা);
		যদি (!r)
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

	যদি (!is_empty_cas(&cas) && (flags & TRANSPORT_ঠেলো_FORCE_IF_INCLUDES))
		cas.use_force_if_includes = 1;

	for_each_string_list_item(item, ঠেলো_options)
		যদি (strchr(item->string, '\n'))
			die(_("push options must not have new line characters"));

	যদি (remote_group.nr == 1) {
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
			পূর্ণসংখ্যা inner_flags = base_flags;
			কাঠামো remote *r = pushremote_get(remote_group.বস্তুসমূহ[0].string);
			যদি (!r)
				die(_("no such remote or remote group: %s"),
				    remote_group.বস্তুসমূহ[0].string);

			যদি (r->mirror)
				inner_flags |= (TRANSPORT_ঠেলো_MIRROR|TRANSPORT_ঠেলো_FORCE);

			যদি (inner_flags & TRANSPORT_ঠেলো_ALL) {
				যদি (argc >= 2)
					die(_("--all can't be combined with refspecs"));
			}
			যদি (inner_flags & TRANSPORT_ঠেলো_MIRROR) {
				যদি (argc >= 2)
					die(_("--mirror can't be combined with refspecs"));
			}

			refspec_clear(&rs);
			rs = (কাঠামো refspec) REFSPEC_INIT_ঠেলো(the_hash_algo);

			যদি (tags)
				refspec_append(&rs, "refs/tags/*");
			যদি (argc > 0)
				set_refspecs(argv + 1, argc - 1, r);

			rc = do_ঠেলো(inner_flags, ঠেলো_options, r);
		}
	} অন্যথা {
		/*
		 * Multiple remotes: spawn one "git push <remote> [<refspecs>]"
		 * subprocess per remote, sequentially.
		 *
		 * Options that only make sense for a single transport connection
		 * are rejected here.
		 */
		যদি (flags & TRANSPORT_ঠেলো_ATOMIC)
			die(_("--atomic can only be used when pushing to one remote"));

		rc = ঠেলো_multiple(&remote_group, ঠেলো_options, flags,
				   tags,
				   argc > 1 ? argv + 1 : ফাঁকা,
				   argc > 1 ? argc - 1 : 0);
	}

	string_list_clear(&ঠেলো_options_cmdline, 0);
	string_list_clear(&ঠেলো_options_config, 0);
	string_list_clear(&remote_group, 0);
	clear_cas_option(&cas);

	যদি (rc == -1)
		usage_with_options(ঠেলো_usage, options);
	অন্যথা
		ফেরত rc;
}
