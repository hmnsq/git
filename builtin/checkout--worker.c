#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "entry.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parallel-checkout.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "read-cache-ll.h"

ساكن فراغ packet_منزل_pc_شے(ثابت حرف *بفر, صحيح len,
			      بنية parallel_checkout_شے *pc_شے)
{
	ثابت بنية pc_شے_fixed_portion *fixed_portion;
	ثابت حرف *variant;
	حرف *encoding;

	إذا (len < حجم(بنية pc_شے_fixed_portion))
		BUG("checkout worker received too short item (got %dB, exp %dB)",
		    len, (صحيح)حجم(بنية pc_شے_fixed_portion));

	fixed_portion = (بنية pc_شے_fixed_portion *)بفر;

	إذا (len - حجم(بنية pc_شے_fixed_portion) !=
		fixed_portion->name_len + fixed_portion->working_درخت_encoding_len)
		BUG("checkout worker received corrupted item");

	variant = بفر + حجم(بنية pc_شے_fixed_portion);

	/*
	 * Note: the main process uses zero length to communicate that the
	 * encoding is NULL. There is no use case that requires sending an
	 * actual empty string, since convert_attrs() never sets
	 * ca.working_tree_enconding to "".
	 */
	إذا (fixed_portion->working_درخت_encoding_len) {
		encoding = xmemdupz(variant,
				    fixed_portion->working_درخت_encoding_len);
		variant += fixed_portion->working_درخت_encoding_len;
	} وإلا {
		encoding = عدم;
	}

	memset(pc_شے, 0, حجم(*pc_شے));
	pc_شے->ce = make_خالی_تھاواں_transient_cache_اندراج(fixed_portion->name_len, عدم);
	pc_شے->ce->ce_namelen = fixed_portion->name_len;
	pc_شے->ce->ce_منوال = fixed_portion->ce_منوال;
	حافظہ_نقل(pc_شے->ce->name, variant, pc_شے->ce->ce_namelen);
	oidcpy(&pc_شے->ce->oid, &fixed_portion->oid);

	pc_شے->id = fixed_portion->id;
	pc_شے->ca.crlf_action = fixed_portion->crlf_action;
	pc_شے->ca.ident = fixed_portion->ident;
	pc_شے->ca.working_درخت_encoding = encoding;
}

ساكن فراغ report_نتیجہ(بنية parallel_checkout_شے *pc_شے)
{
	بنية pc_شے_نتیجہ res = { 0 };
	ناپ_t ناپ;

	res.id = pc_شے->id;
	res.حالت = pc_شے->حالت;

	إذا (pc_شے->حالت == PC_شے_WRITTEN) {
		res.st = pc_شے->st;
		ناپ = حجم(res);
	} وإلا {
		ناپ = PC_شے_نتیجہ_ادھار_ناپ;
	}

	packet_write(1, (ثابت حرف *)&res, ناپ);
}

/* Free the worker-side malloced data, but not pc_item itself. */
ساكن فراغ release_pc_شے_انکڑے(بنية parallel_checkout_شے *pc_شے)
{
	حرر((حرف *)pc_شے->ca.working_درخت_encoding);
	discard_cache_اندراج(pc_شے->ce);
}

ساكن فراغ کامہ_loop(بنية checkout *state)
{
	بنية parallel_checkout_شے *اشیاء = عدم;
	ناپ_t i, nr = 0, alloc = 0;

	طالما (1) {
		صحيح len = packet_read(0, packet_بفر, حجم(packet_بفر),
				      0);

		إذا (len < 0)
			BUG("packet_read() returned negative value");
		وإلا إذا (!len)
			اكسر;

		ALLOC_GROW(اشیاء, nr + 1, alloc);
		packet_منزل_pc_شے(packet_بفر, len, &اشیاء[nr++]);
	}

	لكل (i = 0; i < nr; i++) {
		بنية parallel_checkout_شے *pc_شے = &اشیاء[i];
		write_pc_شے(pc_شے, state);
		report_نتیجہ(pc_شے);
		release_pc_شے_انکڑے(pc_شے);
	}

	packet_flush(1);

	حرر(اشیاء);
}

ساكن ثابت حرف * ثابت checkout_کامہ_usage[] = {
	N_("git checkout--worker [<options>]"),
	عدم
};

صحيح cmd_checkout_کامہ(صحيح argc,
			 ثابت حرف **argv,
			 ثابت حرف *اگیتر,
			 بنية repository *repo بن_ورتیا)
{
	بنية checkout state = CHECKOUT_INIT;
	بنية option checkout_کامہ_options[] = {
		OPT_STRING(0, "prefix", &state.ادھار_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_سرا()
	};

	show_usage_with_options_if_asked(argc, argv,
					 checkout_کامہ_usage,
					 checkout_کامہ_options);

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اگیتر, checkout_کامہ_options,
			     checkout_کامہ_usage, 0);
	إذا (argc > 0)
		usage_with_options(checkout_کامہ_usage, checkout_کامہ_options);

	إذا (state.ادھار_dir)
		state.ادھار_dir_len = لڑی_لمبائی(state.ادھار_dir);

	/*
	 * Setting this on a worker won't actually update the index. We just
	 * need to tell the checkout machinery to lstat() the written entries,
	 * so that we can send this data back to the main process.
	 */
	state.refresh_cache = 1;

	کامہ_loop(&state);
	أرجع 0;
}
