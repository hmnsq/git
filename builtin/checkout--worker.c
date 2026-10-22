#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "entry.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parallel-checkout.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"

ਸਥਿਰ ਖਾਲੀ packet_ਮੰਜ਼ਿਲ_pc_ਵਸਤੂ(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਪੂਰਨ len,
			      ਬਣਤਰ parallel_checkout_ਵਸਤੂ *pc_ਵਸਤੂ)
{
	ਅਚਲ ਬਣਤਰ pc_ਵਸਤੂ_fixed_portion *fixed_portion;
	ਅਚਲ ਅੱਖਰ *variant;
	ਅੱਖਰ *encoding;

	ਜੇ (len < ਆਕਾਰ(ਬਣਤਰ pc_ਵਸਤੂ_fixed_portion))
		BUG("checkout worker received too short item (got %dB, exp %dB)",
		    len, (ਪੂਰਨ)ਆਕਾਰ(ਬਣਤਰ pc_ਵਸਤੂ_fixed_portion));

	fixed_portion = (ਬਣਤਰ pc_ਵਸਤੂ_fixed_portion *)ਬਫਰ;

	ਜੇ (len - ਆਕਾਰ(ਬਣਤਰ pc_ਵਸਤੂ_fixed_portion) !=
		fixed_portion->name_len + fixed_portion->working_ਰੁੱਖ_encoding_len)
		BUG("checkout worker received corrupted item");

	variant = ਬਫਰ + ਆਕਾਰ(ਬਣਤਰ pc_ਵਸਤੂ_fixed_portion);

	/*
	 * Note: the main process uses zero length to communicate that the
	 * encoding is NULL. There is no use case that requires sending an
	 * actual empty string, since convert_attrs() never sets
	 * ca.working_tree_enconding to "".
	 */
	ਜੇ (fixed_portion->working_ਰੁੱਖ_encoding_len) {
		encoding = xmemdupz(variant,
				    fixed_portion->working_ਰੁੱਖ_encoding_len);
		variant += fixed_portion->working_ਰੁੱਖ_encoding_len;
	} ਨਹੀਂਤਾਂ {
		encoding = ਨਿਰਾ;
	}

	memset(pc_ਵਸਤੂ, 0, ਆਕਾਰ(*pc_ਵਸਤੂ));
	pc_ਵਸਤੂ->ce = make_ਖਾਲੀ_ਥਾਵਾਂ_transient_cache_ਇੰਦਰਾਜ(fixed_portion->name_len, ਨਿਰਾ);
	pc_ਵਸਤੂ->ce->ce_namelen = fixed_portion->name_len;
	pc_ਵਸਤੂ->ce->ce_ਬਹੁਲਕ = fixed_portion->ce_ਬਹੁਲਕ;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(pc_ਵਸਤੂ->ce->name, variant, pc_ਵਸਤੂ->ce->ce_namelen);
	oidcpy(&pc_ਵਸਤੂ->ce->oid, &fixed_portion->oid);

	pc_ਵਸਤੂ->id = fixed_portion->id;
	pc_ਵਸਤੂ->ca.crlf_action = fixed_portion->crlf_action;
	pc_ਵਸਤੂ->ca.ident = fixed_portion->ident;
	pc_ਵਸਤੂ->ca.working_ਰੁੱਖ_encoding = encoding;
}

ਸਥਿਰ ਖਾਲੀ report_ਨਤੀਜਾ(ਬਣਤਰ parallel_checkout_ਵਸਤੂ *pc_ਵਸਤੂ)
{
	ਬਣਤਰ pc_ਵਸਤੂ_ਨਤੀਜਾ res = { 0 };
	ਮਾਪ_t ਮਾਪ;

	res.id = pc_ਵਸਤੂ->id;
	res.ਸਥਿਤੀ = pc_ਵਸਤੂ->ਸਥਿਤੀ;

	ਜੇ (pc_ਵਸਤੂ->ਸਥਿਤੀ == PC_ਵਸਤੂ_WRITTEN) {
		res.st = pc_ਵਸਤੂ->st;
		ਮਾਪ = ਆਕਾਰ(res);
	} ਨਹੀਂਤਾਂ {
		ਮਾਪ = PC_ਵਸਤੂ_ਨਤੀਜਾ_ਅਧਾਰ_ਮਾਪ;
	}

	packet_write(1, (ਅਚਲ ਅੱਖਰ *)&res, ਮਾਪ);
}

/* Free the worker-side malloced data, but not pc_item itself. */
ਸਥਿਰ ਖਾਲੀ release_pc_ਵਸਤੂ_ਅੰਕੜੇ(ਬਣਤਰ parallel_checkout_ਵਸਤੂ *pc_ਵਸਤੂ)
{
	ਮੁਕਤ((ਅੱਖਰ *)pc_ਵਸਤੂ->ca.working_ਰੁੱਖ_encoding);
	discard_cache_ਇੰਦਰਾਜ(pc_ਵਸਤੂ->ce);
}

ਸਥਿਰ ਖਾਲੀ ਕਾਮਾ_loop(ਬਣਤਰ checkout *state)
{
	ਬਣਤਰ parallel_checkout_ਵਸਤੂ *ਚੀਜ਼ਾਂ = ਨਿਰਾ;
	ਮਾਪ_t i, nr = 0, alloc = 0;

	ਜਦੋਂ (1) {
		ਪੂਰਨ len = packet_read(0, packet_ਬਫਰ, ਆਕਾਰ(packet_ਬਫਰ),
				      0);

		ਜੇ (len < 0)
			BUG("packet_read() returned negative value");
		ਨਹੀਂਤਾਂ ਜੇ (!len)
			ਰੋਕੋ;

		ALLOC_GROW(ਚੀਜ਼ਾਂ, nr + 1, alloc);
		packet_ਮੰਜ਼ਿਲ_pc_ਵਸਤੂ(packet_ਬਫਰ, len, &ਚੀਜ਼ਾਂ[nr++]);
	}

	ਲਈ (i = 0; i < nr; i++) {
		ਬਣਤਰ parallel_checkout_ਵਸਤੂ *pc_ਵਸਤੂ = &ਚੀਜ਼ਾਂ[i];
		write_pc_ਵਸਤੂ(pc_ਵਸਤੂ, state);
		report_ਨਤੀਜਾ(pc_ਵਸਤੂ);
		release_pc_ਵਸਤੂ_ਅੰਕੜੇ(pc_ਵਸਤੂ);
	}

	packet_flush(1);

	ਮੁਕਤ(ਚੀਜ਼ਾਂ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ checkout_ਕਾਮਾ_usage[] = {
	N_("git checkout--worker [<options>]"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_checkout_ਕਾਮਾ(ਪੂਰਨ argc,
			 ਅਚਲ ਅੱਖਰ **argv,
			 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ checkout state = CHECKOUT_INIT;
	ਬਣਤਰ option checkout_ਕਾਮਾ_options[] = {
		OPT_STRING(0, "prefix", &state.ਅਧਾਰ_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 checkout_ਕਾਮਾ_usage,
					 checkout_ਕਾਮਾ_options);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, checkout_ਕਾਮਾ_options,
			     checkout_ਕਾਮਾ_usage, 0);
	ਜੇ (argc > 0)
		usage_with_options(checkout_ਕਾਮਾ_usage, checkout_ਕਾਮਾ_options);

	ਜੇ (state.ਅਧਾਰ_dir)
		state.ਅਧਾਰ_dir_len = ਲੜੀ_ਲੰਬਾਈ(state.ਅਧਾਰ_dir);

	/*
	 * Setting this on a worker won't actually update the index. We just
	 * need to tell the checkout machinery to lstat() the written entries,
	 * so that we can send this data back to the main process.
	 */
	state.refresh_cache = 1;

	ਕਾਮਾ_loop(&state);
	ਵਾਪਸ 0;
}
