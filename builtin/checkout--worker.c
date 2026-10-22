#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "entry.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parallel-checkout.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "read-cache-ll.h"

ایستا تهی packet_to_pc_item(ثابت نویسه *buffer, صحیح len,
			      ساختار parallel_checkout_item *pc_item)
{
	ثابت ساختار pc_item_fixed_portion *fixed_portion;
	ثابت نویسه *variant;
	نویسه *encoding;

	اگر (len < اندازه(ساختار pc_item_fixed_portion))
		BUG("checkout worker received too short item (got %dB, exp %dB)",
		    len, (صحیح)اندازه(ساختار pc_item_fixed_portion));

	fixed_portion = (ساختار pc_item_fixed_portion *)buffer;

	اگر (len - اندازه(ساختار pc_item_fixed_portion) !=
		fixed_portion->name_len + fixed_portion->working_درخت_encoding_len)
		BUG("checkout worker received corrupted item");

	variant = buffer + اندازه(ساختار pc_item_fixed_portion);

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
	} وگرنه {
		encoding = پوچ;
	}

	memset(pc_item, 0, اندازه(*pc_item));
	pc_item->ce = make_empty_transient_cache_entry(fixed_portion->name_len, پوچ);
	pc_item->ce->ce_namelen = fixed_portion->name_len;
	pc_item->ce->ce_mode = fixed_portion->ce_mode;
	memcpy(pc_item->ce->name, variant, pc_item->ce->ce_namelen);
	oidcpy(&pc_item->ce->oid, &fixed_portion->oid);

	pc_item->id = fixed_portion->id;
	pc_item->ca.crlf_action = fixed_portion->crlf_action;
	pc_item->ca.ident = fixed_portion->ident;
	pc_item->ca.working_درخت_encoding = encoding;
}

ایستا تهی report_result(ساختار parallel_checkout_item *pc_item)
{
	ساختار pc_item_result res = { 0 };
	size_t size;

	res.id = pc_item->id;
	res.status = pc_item->status;

	اگر (pc_item->status == PC_ITEM_WRITTEN) {
		res.st = pc_item->st;
		size = اندازه(res);
	} وگرنه {
		size = PC_ITEM_RESULT_BASE_SIZE;
	}

	packet_write(1, (ثابت نویسه *)&res, size);
}

/* Free the worker-side malloced data, but not pc_item itself. */
ایستا تهی release_pc_item_data(ساختار parallel_checkout_item *pc_item)
{
	آزاد((نویسه *)pc_item->ca.working_درخت_encoding);
	discard_cache_entry(pc_item->ce);
}

ایستا تهی worker_loop(ساختار checkout *state)
{
	ساختار parallel_checkout_item *اقلام = پوچ;
	size_t i, nr = 0, alloc = 0;

	وقتی (1) {
		صحیح len = packet_read(0, packet_buffer, اندازه(packet_buffer),
				      0);

		اگر (len < 0)
			BUG("packet_read() returned negative value");
		وگرنه اگر (!len)
			بشکن;

		ALLOC_GROW(اقلام, nr + 1, alloc);
		packet_to_pc_item(packet_buffer, len, &اقلام[nr++]);
	}

	برای (i = 0; i < nr; i++) {
		ساختار parallel_checkout_item *pc_item = &اقلام[i];
		write_pc_item(pc_item, state);
		report_result(pc_item);
		release_pc_item_data(pc_item);
	}

	packet_flush(1);

	آزاد(اقلام);
}

ایستا ثابت نویسه * ثابت checkout_worker_usage[] = {
	N_("git checkout--worker [<options>]"),
	پوچ
};

صحیح cmd_checkout__worker(صحیح argc,
			 ثابت نویسه **argv,
			 ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ساختار checkout state = CHECKOUT_INIT;
	ساختار option checkout_worker_options[] = {
		OPT_STRING(0, "prefix", &state.base_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 checkout_worker_usage,
					 checkout_worker_options);

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, checkout_worker_options,
			     checkout_worker_usage, 0);
	اگر (argc > 0)
		usage_with_options(checkout_worker_usage, checkout_worker_options);

	اگر (state.base_dir)
		state.base_dir_len = strlen(state.base_dir);

	/*
	 * Setting this on a worker won't actually update the index. We just
	 * need to tell the checkout machinery to lstat() the written entries,
	 * so that we can send this data back to the main process.
	 */
	state.refresh_cache = 1;

	worker_loop(&state);
	بازگشت 0;
}
