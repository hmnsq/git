#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "entry.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parallel-checkout.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "read-cache-ll.h"

ساكن فراغ packet_مقصد_pc_قلم(ثابت حرف *میانگیر, صحيح len,
			      بنية parallel_checkout_قلم *pc_قلم)
{
	ثابت بنية pc_قلم_fixed_portion *fixed_portion;
	ثابت حرف *variant;
	حرف *encoding;

	إذا (len < حجم(بنية pc_قلم_fixed_portion))
		BUG("checkout worker received too short item (got %dB, exp %dB)",
		    len, (صحيح)حجم(بنية pc_قلم_fixed_portion));

	fixed_portion = (بنية pc_قلم_fixed_portion *)میانگیر;

	إذا (len - حجم(بنية pc_قلم_fixed_portion) !=
		fixed_portion->name_len + fixed_portion->working_درخت_encoding_len)
		BUG("checkout worker received corrupted item");

	variant = میانگیر + حجم(بنية pc_قلم_fixed_portion);

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

	memset(pc_قلم, 0, حجم(*pc_قلم));
	pc_قلم->ce = make_خالی_transient_cache_مدخل(fixed_portion->name_len, عدم);
	pc_قلم->ce->ce_namelen = fixed_portion->name_len;
	pc_قلم->ce->ce_نما = fixed_portion->ce_نما;
	کپی_حافظه(pc_قلم->ce->name, variant, pc_قلم->ce->ce_namelen);
	oidcpy(&pc_قلم->ce->oid, &fixed_portion->oid);

	pc_قلم->id = fixed_portion->id;
	pc_قلم->ca.crlf_action = fixed_portion->crlf_action;
	pc_قلم->ca.ident = fixed_portion->ident;
	pc_قلم->ca.working_درخت_encoding = encoding;
}

ساكن فراغ report_نتیجه(بنية parallel_checkout_قلم *pc_قلم)
{
	بنية pc_قلم_نتیجه res = { 0 };
	اندازه_t اندازه;

	res.id = pc_قلم->id;
	res.وضعیت = pc_قلم->وضعیت;

	إذا (pc_قلم->وضعیت == PC_قلم_WRITTEN) {
		res.st = pc_قلم->st;
		اندازه = حجم(res);
	} وإلا {
		اندازه = PC_قلم_نتیجه_پایه_اندازه;
	}

	packet_write(1, (ثابت حرف *)&res, اندازه);
}

/* Free the worker-side malloced data, but not pc_item itself. */
ساكن فراغ release_pc_قلم_داده(بنية parallel_checkout_قلم *pc_قلم)
{
	حرر((حرف *)pc_قلم->ca.working_درخت_encoding);
	discard_cache_مدخل(pc_قلم->ce);
}

ساكن فراغ کارگر_loop(بنية checkout *state)
{
	بنية parallel_checkout_قلم *اقلام = عدم;
	اندازه_t i, nr = 0, alloc = 0;

	طالما (1) {
		صحيح len = packet_read(0, packet_میانگیر, حجم(packet_میانگیر),
				      0);

		إذا (len < 0)
			BUG("packet_read() returned negative value");
		وإلا إذا (!len)
			اكسر;

		ALLOC_GROW(اقلام, nr + 1, alloc);
		packet_مقصد_pc_قلم(packet_میانگیر, len, &اقلام[nr++]);
	}

	لكل (i = 0; i < nr; i++) {
		بنية parallel_checkout_قلم *pc_قلم = &اقلام[i];
		write_pc_قلم(pc_قلم, state);
		report_نتیجه(pc_قلم);
		release_pc_قلم_داده(pc_قلم);
	}

	packet_flush(1);

	حرر(اقلام);
}

ساكن ثابت حرف * ثابت checkout_کارگر_usage[] = {
	N_("git checkout--worker [<options>]"),
	عدم
};

صحيح cmd_checkout_کارگر(صحيح argc,
			 ثابت حرف **argv,
			 ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
{
	بنية checkout state = CHECKOUT_INIT;
	بنية option checkout_کارگر_options[] = {
		OPT_STRING(0, "prefix", &state.پایه_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_انتها()
	};

	show_usage_with_options_if_asked(argc, argv,
					 checkout_کارگر_usage,
					 checkout_کارگر_options);

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, پیشوند, checkout_کارگر_options,
			     checkout_کارگر_usage, 0);
	إذا (argc > 0)
		usage_with_options(checkout_کارگر_usage, checkout_کارگر_options);

	إذا (state.پایه_dir)
		state.پایه_dir_len = طول_رشته(state.پایه_dir);

	/*
	 * Setting this on a worker won't actually update the index. We just
	 * need to tell the checkout machinery to lstat() the written entries,
	 * so that we can send this data back to the main process.
	 */
	state.refresh_cache = 1;

	کارگر_loop(&state);
	أرجع 0;
}
