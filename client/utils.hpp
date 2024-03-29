#pragma once

#include <string>

struct xfs_opts {
    std::string device;
    std::string output;
};

[[noreturn]] void die(const char* errstr, ...);

void process_argv(int argc, char** argv);

extern xfs_opts g_opts;
