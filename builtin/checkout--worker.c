#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "entry.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parallel-checkout.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "read-cache-ll.h"

ساکن خالی packet_مقصد_pc_شے(ثابت حرف *بفر, صحیح len,
			      ساخت parallel_checkout_شے *pc_شے)
{
	ثابت ساخت pc_شے_fixed_portion *fixed_portion;
	ثابت حرف *variant;
	حرف *encoding;

	اگر (len < حجم(ساخت pc_شے_fixed_portion))
		BUG("checkout worker received too short item (got %dB, exp %dB)",
		    len, (صحیح)حجم(ساخت pc_شے_fixed_portion));

	fixed_portion = (ساخت pc_شے_fixed_portion *)بفر;

	اگر (len - حجم(ساخت pc_شے_fixed_portion) !=
		fixed_portion->name_len + fixed_portion->working_درخت_encoding_len)
		BUG("checkout worker received corrupted item");

	variant = بفر + حجم(ساخت pc_شے_fixed_portion);

	/*
	 * Note: the main process uses zero length to communicate that the
	 * encoding is NULL. There is no use case that requires sending an
	 * actual empty string, since convert_attrs() never sets
	 * ca.working_tree_enconding to "".
	 */
	اگر (fixed_portion->working_درخت_encoding_len) {
		encoding = xmemdupz(variant,
				    fixed_portion->working_درخت_encoding_len);
		variant += fixed_portion->working_درخت_encoding_len;
	} ورنہ {
		encoding = تہی;
	}

	memset(pc_شے, 0, حجم(*pc_شے));
	pc_شے->ce = make_خالی_خانے_transient_cache_اندراج(fixed_portion->name_len, تہی);
	pc_شے->ce->ce_namelen = fixed_portion->name_len;
	pc_شے->ce->ce_منوال = fixed_portion->ce_منوال;
	حافظہ_نقل(pc_شے->ce->name, variant, pc_شے->ce->ce_namelen);
	oidcpy(&pc_شے->ce->oid, &fixed_portion->oid);

	pc_شے->id = fixed_portion->id;
	pc_شے->ca.crlf_action = fixed_portion->crlf_action;
	pc_شے->ca.ident = fixed_portion->ident;
	pc_شے->ca.working_درخت_encoding = encoding;
}

ساکن خالی report_نتیجہ(ساخت parallel_checkout_شے *pc_شے)
{
	ساخت pc_شے_نتیجہ res = { 0 };
	سائز_t سائز;

	res.id = pc_شے->id;
	res.حالت_2 = pc_شے->حالت_2;

	اگر (pc_شے->حالت_2 == PC_شے_WRITTEN) {
		res.st = pc_شے->st;
		سائز = حجم(res);
	} ورنہ {
		سائز = PC_شے_نتیجہ_بنیاد_سائز;
	}

	packet_write(1, (ثابت حرف *)&res, سائز);
}

/* Free the worker-side malloced data, but not pc_item itself. */
ساکن خالی release_pc_شے_ڈیٹا(ساخت parallel_checkout_شے *pc_شے)
{
	آزاد((حرف *)pc_شے->ca.working_درخت_encoding);
	discard_cache_اندراج(pc_شے->ce);
}

ساکن خالی کارکن_loop(ساخت checkout *state)
{
	ساخت parallel_checkout_شے *اشیاء = تہی;
	سائز_t i, nr = 0, alloc = 0;

	جب (1) {
		صحیح len = packet_read(0, packet_بفر, حجم(packet_بفر),
				      0);

		اگر (len < 0)
			BUG("packet_read() returned negative value");
		ورنہ اگر (!len)
			توڑو;

		ALLOC_GROW(اشیاء, nr + 1, alloc);
		packet_مقصد_pc_شے(packet_بفر, len, &اشیاء[nr++]);
	}

	برائے (i = 0; i < nr; i++) {
		ساخت parallel_checkout_شے *pc_شے = &اشیاء[i];
		write_pc_شے(pc_شے, state);
		report_نتیجہ(pc_شے);
		release_pc_شے_ڈیٹا(pc_شے);
	}

	packet_flush(1);

	آزاد(اشیاء);
}

ساکن ثابت حرف * ثابت checkout_کارکن_usage[] = {
	N_("git checkout--worker [<options>]"),
	تہی
};

صحیح cmd_checkout_کارکن(صحیح argc,
			 ثابت حرف **argv,
			 ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	ساخت checkout state = CHECKOUT_INIT;
	ساخت option checkout_کارکن_options[] = {
		OPT_STRING(0, "prefix", &state.بنیاد_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_انتہا()
	};

	show_usage_with_options_if_asked(argc, argv,
					 checkout_کارکن_usage,
					 checkout_کارکن_options);

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, checkout_کارکن_options,
			     checkout_کارکن_usage, 0);
	اگر (argc > 0)
		usage_with_options(checkout_کارکن_usage, checkout_کارکن_options);

	اگر (state.بنیاد_dir)
		state.بنیاد_dir_len = لڑی_لمبائی(state.بنیاد_dir);

	/*
	 * Setting this on a worker won't actually update the index. We just
	 * need to tell the checkout machinery to lstat() the written entries,
	 * so that we can send this data back to the main process.
	 */
	state.refresh_cache = 1;

	کارکن_loop(&state);
	واپس 0;
}
