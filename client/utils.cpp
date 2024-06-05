#include "utils.hpp"

#include <cstdarg>
#include <cstdio>  /* printf */
#include <cstdlib> /* exit */
#include <getopt.h>

#include <spdlog/spdlog.h>

constexpr char prog_name[] = "xfs_undelete";

xfs_opts g_opts = {};

[[noreturn]] void die(const char* errstr, ...)
{
    va_list ap;
    va_start(ap, errstr);
    vfprintf(stderr, errstr, ap);
    va_end(ap);
    exit(1);
}

[[noreturn]] static void usage()
{
    die("usage: %s -d device\n"
        "       %s --device device\n",
        prog_name,
        prog_name);
}

void process_argv(int argc, char** argv)
{
    int c = 0;

    while (true)
    {
        int                        option_index   = 0;
        const static struct option long_options[] = {
            {"device", required_argument, nullptr, 'd'},
            {"output", required_argument, nullptr, 'o'},
            {"verbose", no_argument, nullptr, 'v'},
            {nullptr, 0, nullptr, 0}};
        const static char optstring[] = "d:o:v";

        c = getopt_long(argc, argv, optstring, long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
        case 'd':
            g_opts.device = optarg;
            break;

        case 'o':
            g_opts.output = optarg;
            break;

        case 'v':
            spdlog::set_level(spdlog::level::trace);
            break;

        default:
            usage();
        }
    }

    if (optind < argc)
    {
        printf("unknown non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
        usage();
    }

    if (g_opts.device.empty())
    {
        spdlog::error("no device given!");
        usage();
    }
}
