#include "utils.hpp"

#include <spdlog/spdlog.h>

#include <spdlog/formatter.h>

#include <spdlog/fmt/fmt.h>

#include <spdlog/fmt/bin_to_hex.h>

#include <array>
#include <cstdlib>
#include <iostream>

#include "lib/linux_file.hpp"
#include "lib/xfs_inode_entry.hpp"
#include "lib/xfs_parser.hpp"

static bool save_file(uf::xfs::InodeEntry entry)
{
    uint64_t ino = 0;
    if (!entry.GetInodeNumber(&ino))
    {
        spdlog::error("GetInodeNumber failed");
        return false;
    }

    uint64_t bytes_left = 0;
    if (!entry.GetSize(&bytes_left))
    {
        spdlog::error("GetSize failed for ino {}", ino);
        return false;
    }

    uint64_t                  bytes_read    = 0;
    uint64_t                  size          = 0;
    uint64_t                  offset        = 0;
    uint64_t                  bytes_to_read = 0;
    std::array<uint8_t, 4096> buffer        = {};
    uint64_t                  buffer_size   = buffer.size();

    utils::LinuxFile target(g_opts.output + "/" + std::to_string(ino), true, false);

    while (bytes_left != 0 && entry.GetNextAvailableOffset(&offset, &size))
    {
        bytes_read    = 0;
        bytes_to_read = std::min(size, buffer_size);
        while (bytes_to_read != 0 && entry.GetFileContent(buffer.data(), offset, bytes_to_read, &bytes_read))
        {
            bytes_left -= bytes_read;
            size -= bytes_read;
            bytes_to_read = std::min(size, buffer_size);
            target.WriteAtOffset(buffer.data(), offset, bytes_read);
            spdlog::debug("wrote {} bytes at offset {}", bytes_read, offset);
        }
    }

    return true;
}

static bool XfsCallback(const uf::xfs::InodeEntry& entry)
{
    uint64_t ino = 0;
    if (entry.GetInodeNumber(&ino))
    {
        spdlog::info("ino: {}", ino);
    }
    else
    {
        spdlog::error("GetInodeNumber failed");
        return false;
    }

    uint64_t bytes_left = 0;
    if (entry.GetSize(&bytes_left))
    {
        spdlog::info("size: {}", bytes_left);
    }
    else
    {
        spdlog::error("GetSize failed for ino {}", ino);
        return false;
    }

    if (!save_file(entry))
    {
        spdlog::error("save_file failed for ino {}", ino);
    }
    else
    {
        spdlog::info("saved file {}", ino);
    }

    return true;
}

int main(int argc, char** argv)
{
    process_argv(argc, argv);

    spdlog::info("Undeleting files on {}", g_opts.device);

    uf::xfs::Parser xfs_parser(g_opts.device);

    xfs_parser.DumpInodes(XfsCallback);

    return EXIT_SUCCESS;
}
