#include "utils.hpp"

#include <stdlib.h> /* exit */

#include <spdlog/spdlog.h>

int main(int argc, char** argv)
{
    process_argv(argc, argv);

    spdlog::info("Undeleting files on {}", g_opts.device);

        

    exit(EXIT_SUCCESS);
}
