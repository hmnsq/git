#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#تعریف_کړه DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "entry.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parallel-checkout.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "read-cache-ll.h"

ساكن فراغ packet_منزل_pc_توکی(ثابت حرف *بفر, صحيح len,
			      بنية parallel_checkout_توکی *pc_توکی)
{
	ثابت بنية pc_توکی_fixed_portion *fixed_portion;
	ثابت حرف *variant;
	حرف *encoding;

	إذا (len < حجم(بنية pc_توکی_fixed_portion))
		BUG("checkout worker received too short item (got %dB, exp %dB)",
		    len, (صحيح)حجم(بنية pc_توکی_fixed_portion));

	fixed_portion = (بنية pc_توکی_fixed_portion *)بفر;

	إذا (len - حجم(بنية pc_توکی_fixed_portion) !=
		fixed_portion->name_len + fixed_portion->working_ونه_encoding_len)
		BUG("checkout worker received corrupted item");

	variant = بفر + حجم(بنية pc_توکی_fixed_portion);

	/*
	 * Note: the main process uses zero length to communicate that the
	 * encoding is NULL. There is no use case that requires sending an
	 * actual empty string, since convert_attrs() never sets
	 * ca.working_tree_enconding to "".
	 */
	إذا (fixed_portion->working_ونه_encoding_len) {
		encoding = xmemdupz(variant,
				    fixed_portion->working_ونه_encoding_len);
		variant += fixed_portion->working_ونه_encoding_len;
	} وإلا {
		encoding = عدم;
	}

	memset(pc_توکی, 0, حجم(*pc_توکی));
	pc_توکی->ce = make_تشه_transient_cache_ثبت(fixed_portion->name_len, عدم);
	pc_توکی->ce->ce_namelen = fixed_portion->name_len;
	pc_توکی->ce->ce_ډېرپېښ = fixed_portion->ce_ډېرپېښ;
	حافظه_نقل(pc_توکی->ce->name, variant, pc_توکی->ce->ce_namelen);
	oidcpy(&pc_توکی->ce->oid, &fixed_portion->oid);

	pc_توکی->id = fixed_portion->id;
	pc_توکی->ca.crlf_action = fixed_portion->crlf_action;
	pc_توکی->ca.ident = fixed_portion->ident;
	pc_توکی->ca.working_ونه_encoding = encoding;
}

ساكن فراغ report_پایله(بنية parallel_checkout_توکی *pc_توکی)
{
	بنية pc_توکی_پایله res = { 0 };
	اندازه_t اندازه;

	res.id = pc_توکی->id;
	res.حالت = pc_توکی->حالت;

	إذا (pc_توکی->حالت == PC_توکی_WRITTEN) {
		res.st = pc_توکی->st;
		اندازه = حجم(res);
	} وإلا {
		اندازه = PC_توکی_پایله_بنسټ_اندازه;
	}

	packet_write(1, (ثابت حرف *)&res, اندازه);
}

/* Free the worker-side malloced data, but not pc_item itself. */
ساكن فراغ release_pc_توکی_معلومات(بنية parallel_checkout_توکی *pc_توکی)
{
	حرر((حرف *)pc_توکی->ca.working_ونه_encoding);
	discard_cache_ثبت(pc_توکی->ce);
}

ساكن فراغ کارګر_loop(بنية checkout *state)
{
	بنية parallel_checkout_توکی *توکي = عدم;
	اندازه_t i, nr = 0, alloc = 0;

	طالما (1) {
		صحيح len = packet_read(0, packet_بفر, حجم(packet_بفر),
				      0);

		إذا (len < 0)
			BUG("packet_read() returned negative value");
		وإلا إذا (!len)
			اكسر;

		ALLOC_GROW(توکي, nr + 1, alloc);
		packet_منزل_pc_توکی(packet_بفر, len, &توکي[nr++]);
	}

	لكل (i = 0; i < nr; i++) {
		بنية parallel_checkout_توکی *pc_توکی = &توکي[i];
		write_pc_توکی(pc_توکی, state);
		report_پایله(pc_توکی);
		release_pc_توکی_معلومات(pc_توکی);
	}

	packet_flush(1);

	حرر(توکي);
}

ساكن ثابت حرف * ثابت checkout_کارګر_usage[] = {
	N_("git checkout--worker [<options>]"),
	عدم
};

صحيح cmd_checkout_کارګر(صحيح argc,
			 ثابت حرف **argv,
			 ثابت حرف *مختاړی,
			 بنية repository *repo ناکارول_شوی)
{
	بنية checkout state = CHECKOUT_INIT;
	بنية option checkout_کارګر_options[] = {
		OPT_STRING(0, "prefix", &state.بنسټ_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_پای_ټکی()
	};

	show_usage_with_options_if_asked(argc, argv,
					 checkout_کارګر_usage,
					 checkout_کارګر_options);

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, مختاړی, checkout_کارګر_options,
			     checkout_کارګر_usage, 0);
	إذا (argc > 0)
		usage_with_options(checkout_کارګر_usage, checkout_کارګر_options);

	إذا (state.بنسټ_dir)
		state.بنسټ_dir_len = مزي_اوږدوالی(state.بنسټ_dir);

	/*
	 * Setting this on a worker won't actually update the index. We just
	 * need to tell the checkout machinery to lstat() the written entries,
	 * so that we can send this data back to the main process.
	 */
	state.refresh_cache = 1;

	کارګر_loop(&state);
	أرجع 0;
}
