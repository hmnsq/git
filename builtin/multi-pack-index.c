#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "abspath.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "midx.h"
#include "strbuf.h"
#include "trace2.h"
#include "odb.h"
#include "odb/source.h"
#include "odb/source-files.h"
#include "replace-object.h"
#include "repository.h"

#define BUILTIN_MIDX_WRITE_USAGE \
	N_("git multi-pack-index [<options>] write [--preferred-pack=<pack>]\n" \
	   "  [--[no-]bitmap] [--[no-]incremental] [--[no-]stdin-packs]\n" \
	   "  [--refs-snapshot=<path>] [--[no-]write-chain-file]\n" \
	   "  [--base=<checksum>]")

#define BUILTIN_MIDX_COMPACT_USAGE \
	N_("git multi-pack-index [<options>] compact [--[no-]incremental]\n" \
	   "  [--[no-]bitmap] [--base=<checksum>] [--[no-]write-chain-file]\n" \
	   "  <from> <to>")

#define BUILTIN_MIDX_VERIFY_USAGE \
	N_("git multi-pack-index [<options>] verify")

#define BUILTIN_MIDX_EXPIRE_USAGE \
	N_("git multi-pack-index [<options>] expire")

#define BUILTIN_MIDX_REPACK_USAGE \
	N_("git multi-pack-index [<options>] repack [--batch-size=<size>]")

static var_na const * const builtin_multi_pack_index_write_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	NULL
};
static var_na const * const builtin_multi_pack_index_compact_usage[] = {
	BUILTIN_MIDX_COMPACT_USAGE,
	NULL
};
static var_na const * const builtin_multi_pack_index_verify_usage[] = {
	BUILTIN_MIDX_VERIFY_USAGE,
	NULL
};
static var_na const * const builtin_multi_pack_index_expire_usage[] = {
	BUILTIN_MIDX_EXPIRE_USAGE,
	NULL
};
static var_na const * const builtin_multi_pack_index_repack_usage[] = {
	BUILTIN_MIDX_REPACK_USAGE,
	NULL
};
static var_na const * const builtin_multi_pack_index_usage[] = {
	BUILTIN_MIDX_WRITE_USAGE,
	BUILTIN_MIDX_COMPACT_USAGE,
	BUILTIN_MIDX_VERIFY_USAGE,
	BUILTIN_MIDX_EXPIRE_USAGE,
	BUILTIN_MIDX_REPACK_USAGE,
	NULL
};

static sa_nracanaa opts_multi_pack_index {
	var_na *object_dir;
	const var_na *preferred_pack;
	const var_na *incremental_base;
	var_na *refs_snapshot;
	unsigned long batch_size;
	unsigned flags;
	ကိန်းပြည့် stdin_packs;
} opts;


static ကိန်းပြည့် parse_object_dir(const sa_nracanaa option *opt, const var_na *arg,
			    ကိန်းပြည့် unset)
{
	var_na **value = opt->value;
	free(*value);
	အကယ်၍ (unset)
		*value = xstrdup(the_repository->objects->source->path);
	မဟုတ်ပါက
		*value = real_pathdup(arg, 1);
	ပြန်ပေး 0;
}

static sa_nracanaa odb_source_packed *handle_object_dir_option(sa_nracanaa repository *repo)
{
	sa_nracanaa odb_files_dir *dir = odb_source_files_find_dir(repo->objects, opts.object_dir);
	အကယ်၍ (!dir)
		die(_("object directory is not an alternate of the current repository: '%s'"),
		    opts.object_dir);
	ပြန်ပေး dir->packed;
}

static sa_nracanaa option common_opts[] = {
	OPT_CALLBACK(0, "object-dir", &opts.object_dir,
	  N_("directory"),
	  N_("object directory containing set of packfile and pack-index pairs"),
	  parse_object_dir),
	OPT_BIT(0, "progress", &opts.flags, N_("force progress reporting"),
		MIDX_PROGRESS),
	OPT_END(),
};

static sa_nracanaa option *add_common_options(sa_nracanaa option *prev)
{
	ပြန်ပေး parse_options_concat(common_opts, prev);
}

static ကိန်းပြည့် git_multi_pack_index_write_config(const var_na *var, const var_na *value,
					     const sa_nracanaa config_context *ctx UNUSED,
					     void *cb UNUSED)
{
	အကယ်၍ (!strcmp(var, "pack.writebitmaphashcache")) {
		အကယ်၍ (git_config_bool(var, value))
			opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;
		မဟုတ်ပါက
			opts.flags &= ~MIDX_WRITE_BITMAP_HASH_CACHE;
	}

	အကယ်၍ (!strcmp(var, "pack.writebitmaplookuptable")) {
		အကယ်၍ (git_config_bool(var, value))
			opts.flags |= MIDX_WRITE_BITMAP_LOOKUP_TABLE;
		မဟုတ်ပါက
			opts.flags &= ~MIDX_WRITE_BITMAP_LOOKUP_TABLE;
	}

	/*
	 * We should never make a fall-back call to 'git_default_config', since
	 * this was already called in 'cmd_multi_pack_index()'.
	 */
	ပြန်ပေး 0;
}

static void read_packs_from_stdin(sa_nracanaa string_list *to)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	နေစဉ် (strbuf_getline(&buf, stdin) != EOF)
		string_list_append(to, buf.buf);
	string_list_sort(to);

	strbuf_release(&buf);
}

static ကိန်းပြည့် cmd_multi_pack_index_write(ကိန်းပြည့် argc, const var_na **argv,
				      const var_na *prefix,
				      sa_nracanaa repository *repo)
{
	sa_nracanaa option *options;
	static sa_nracanaa option builtin_multi_pack_index_write_options[] = {
		OPT_STRING(0, "preferred-pack", &opts.preferred_pack,
			   N_("preferred-pack"),
			   N_("pack for reuse when computing a multi-pack bitmap")),
		OPT_BIT(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_INDEX),
		OPT_STRING(0, "base", &opts.incremental_base, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_BIT(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_BOOL(0, "stdin-packs", &opts.stdin_packs,
			 N_("write multi-pack index containing only given indexes")),
		OPT_FILENAME(0, "refs-snapshot", &opts.refs_snapshot,
			     N_("refs snapshot for selecting bitmap commits")),
		OPT_END(),
	};
	sa_nracanaa odb_source_packed *packed_source;
	ကိန်းပြည့် ret;

	opts.flags |= MIDX_WRITE_BITMAP_HASH_CACHE;

	repo_config(the_repository, git_multi_pack_index_write_config, NULL);

	options = add_common_options(builtin_multi_pack_index_write_options);

	trace2_cmd_mode(argv[0]);

	အကယ်၍ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_write_usage,
			     0);
	အကယ်၍ (argc)
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);

	အကယ်၍ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	အကယ်၍ (opts.incremental_base &&
	    !(opts.flags & MIDX_WRITE_NO_CHAIN)) {
		error(_("cannot use --base without --no-write-chain-file"));
		usage_with_options(builtin_multi_pack_index_write_usage,
				   options);
	}

	packed_source = handle_object_dir_option(repo);

	FREE_AND_NULL(options);

	အကယ်၍ (opts.stdin_packs) {
		sa_nracanaa string_list packs = STRING_LIST_INIT_DUP;

		read_packs_from_stdin(&packs);

		ret = write_midx_file_only(packed_source, &packs,
					   opts.preferred_pack,
					   opts.refs_snapshot,
					   opts.incremental_base, opts.flags);

		string_list_clear(&packs, 0);
		free(opts.refs_snapshot);

		ပြန်ပေး ret;

	}

	ret = write_midx_file(packed_source, opts.preferred_pack,
			      opts.refs_snapshot, opts.flags);

	free(opts.refs_snapshot);
	ပြန်ပေး ret;
}

static ကိန်းပြည့် cmd_multi_pack_index_compact(ကိန်းပြည့် argc, const var_na **argv,
					const var_na *prefix,
					sa_nracanaa repository *repo)
{
	sa_nracanaa multi_pack_index *m, *cur;
	sa_nracanaa multi_pack_index *from_midx = NULL;
	sa_nracanaa multi_pack_index *to_midx = NULL;
	sa_nracanaa odb_source_packed *packed_source;
	ကိန်းပြည့် ret;

	sa_nracanaa option *options;
	static sa_nracanaa option builtin_multi_pack_index_compact_options[] = {
		OPT_STRING(0, "base", &opts.incremental_base, N_("checksum"),
			   N_("base MIDX for incremental writes")),
		OPT_BIT(0, "bitmap", &opts.flags, N_("write multi-pack bitmap"),
			MIDX_WRITE_BITMAP | MIDX_WRITE_REV_INDEX),
		OPT_BIT(0, "incremental", &opts.flags,
			N_("write a new incremental MIDX"), MIDX_WRITE_INCREMENTAL),
		OPT_NEGBIT(0, "write-chain-file", &opts.flags,
			N_("write the multi-pack-index chain file"),
			MIDX_WRITE_NO_CHAIN),
		OPT_END(),
	};

	repo_config(repo, git_multi_pack_index_write_config, NULL);

	options = add_common_options(builtin_multi_pack_index_compact_options);

	trace2_cmd_mode(argv[0]);

	အကယ်၍ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_compact_usage,
			     0);

	အကယ်၍ (argc != 2)
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);

	အကယ်၍ (opts.flags & MIDX_WRITE_NO_CHAIN &&
	    !(opts.flags & MIDX_WRITE_INCREMENTAL)) {
		error(_("cannot use %s without %s"),
		      "--no-write-chain-file", "--incremental");
		usage_with_options(builtin_multi_pack_index_compact_usage,
				   options);
	}

	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	m = get_multi_pack_index(packed_source);

	အတွက် (cur = m; cur && !(from_midx && to_midx); cur = cur->base_midx) {
		const var_na *midx_csum = midx_get_checksum_hex(cur);

		အကယ်၍ (!from_midx && !strcmp(midx_csum, argv[0]))
			from_midx = cur;
		အကယ်၍ (!to_midx && !strcmp(midx_csum, argv[1]))
			to_midx = cur;
	}

	အကယ်၍ (!from_midx)
		die(_("could not find MIDX: %s"), argv[0]);
	အကယ်၍ (!to_midx)
		die(_("could not find MIDX: %s"), argv[1]);
	အကယ်၍ (from_midx == to_midx)
		die(_("MIDX compaction endpoints must be unique"));

	အတွက် (m = from_midx; m; m = m->base_midx) {
		အကယ်၍ (m == to_midx)
			die(_("MIDX %s must be an ancestor of %s"), argv[0], argv[1]);
	}

	ret = write_midx_file_compact(packed_source, from_midx, to_midx,
				      opts.incremental_base, opts.flags);

	ပြန်ပေး ret;
}

static ကိန်းပြည့် cmd_multi_pack_index_verify(ကိန်းပြည့် argc, const var_na **argv,
				       const var_na *prefix,
				       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option *options;
	static sa_nracanaa option builtin_multi_pack_index_verify_options[] = {
		OPT_END(),
	};
	sa_nracanaa odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_verify_options);

	trace2_cmd_mode(argv[0]);

	အကယ်၍ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_verify_usage,
			     0);
	အကယ်၍ (argc)
		usage_with_options(builtin_multi_pack_index_verify_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ပြန်ပေး verify_midx_file(packed_source, opts.flags);
}

static ကိန်းပြည့် cmd_multi_pack_index_expire(ကိန်းပြည့် argc, const var_na **argv,
				       const var_na *prefix,
				       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option *options;
	static sa_nracanaa option builtin_multi_pack_index_expire_options[] = {
		OPT_END(),
	};
	sa_nracanaa odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_expire_options);

	trace2_cmd_mode(argv[0]);

	အကယ်၍ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options, builtin_multi_pack_index_expire_usage,
			     0);
	အကယ်၍ (argc)
		usage_with_options(builtin_multi_pack_index_expire_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ပြန်ပေး expire_midx_packs(packed_source, opts.flags);
}

static ကိန်းပြည့် cmd_multi_pack_index_repack(ကိန်းပြည့် argc, const var_na **argv,
				       const var_na *prefix,
				       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option *options;
	static sa_nracanaa option builtin_multi_pack_index_repack_options[] = {
		OPT_UNSIGNED(0, "batch-size", &opts.batch_size,
		  N_("during repack, collect pack-files of smaller size into a batch that is larger than this size")),
		OPT_END(),
	};
	sa_nracanaa odb_source_packed *packed_source;

	options = add_common_options(builtin_multi_pack_index_repack_options);

	trace2_cmd_mode(argv[0]);

	အကယ်၍ (isatty(2))
		opts.flags |= MIDX_PROGRESS;
	argc = parse_options(argc, argv, prefix,
			     options,
			     builtin_multi_pack_index_repack_usage,
			     0);
	အကယ်၍ (argc)
		usage_with_options(builtin_multi_pack_index_repack_usage,
				   options);
	packed_source = handle_object_dir_option(the_repository);

	FREE_AND_NULL(options);

	ပြန်ပေး midx_repack(packed_source, (size_t)opts.batch_size, opts.flags);
}

ကိန်းပြည့် cmd_multi_pack_index(ကိန်းပြည့် argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo)
{
	ကိန်းပြည့် res;
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option builtin_multi_pack_index_options[] = {
		OPT_SUBCOMMAND("repack", &fn, cmd_multi_pack_index_repack),
		OPT_SUBCOMMAND("write", &fn, cmd_multi_pack_index_write),
		OPT_SUBCOMMAND("compact", &fn, cmd_multi_pack_index_compact),
		OPT_SUBCOMMAND("verify", &fn, cmd_multi_pack_index_verify),
		OPT_SUBCOMMAND("expire", &fn, cmd_multi_pack_index_expire),
		OPT_END(),
	};
	sa_nracanaa option *options = parse_options_concat(builtin_multi_pack_index_options, common_opts);

	disable_replace_refs();

	repo_config(the_repository, git_default_config, NULL);

	အကယ်၍ (the_repository &&
	    the_repository->objects &&
	    the_repository->objects->source)
		opts.object_dir = xstrdup(the_repository->objects->source->path);

	argc = parse_options(argc, argv, prefix, options,
			     builtin_multi_pack_index_usage, 0);
	FREE_AND_NULL(options);

	res = fn(argc, argv, prefix, repo);

	free(opts.object_dir);
	ပြန်ပေး res;
}
