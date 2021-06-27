#ifndef _XFS_PARSER_HPP_
#define _XFS_PARSER_HPP_

#include "linux_file.hpp"                                   // utils::LinuxFile
//#include "undelete_xfs/undelete_xfs_callback_interface.hpp" // uf::xfs::IInodeCallback

namespace uf::xfs {
    class IInodeCallback;
}

#include "xfs_extent.hpp"                                   // uf::xfs::Extent
#include "xfs_inode.hpp"                                    // uf::xfs::Inode
#include "xfs_superblock.h"                                 // xfs_superblock_t

#include <cstdint> // uint16_t uint32_t
#include <fstream> // std::ifstream
#include <string>  // std::string

namespace uf {
    namespace xfs {
        class Parser;
    } // namespace xfs
} // namespace uf

class uf::xfs::Parser
{
public:
    explicit Parser(const std::string& DevicePath);

    bool DumpInodes(uf::xfs::IInodeCallback* InodeCallback);

private:
    void ReadSuperblock();
    void CheckSuperblockFlags() const;
    bool InodeBTreeCallback(
        xfs_agnumber_t           AGIndex,
        xfs_inobt_rec_t          InobtRecord,
        uint32_t                 AGFRoot,
        uf::xfs::IInodeCallback* InodeCallback);

    // read an ag-relative inode and construct the object
    Inode ReadInode(xfs_agnumber_t AGIndex, xfs_agino_t AGInode, uint32_t AGFRoot);

    // returns only the extents within the Allocation Group Free space tree
    std::vector<Extent> OnlyWithinAGF(Extent Extent, xfs_agnumber_t AGIndex, uint32_t AGFRoot);

    // helper function for OnlyWithinAGF
    void TreeCheck(
        const Extent&                 Extent,
        uint64_t                      AGOffset,
        uint64_t                      BlockOffset,
        uint64_t*                     ExtentBegin,
        uint64_t*                     ExtentEnd,
        std::vector<uf::xfs::Extent>* Result);

    bool inline HasVersionFeature(uint16_t Flag) const noexcept;
    bool inline HasVersion2Feature(uint32_t Flag) const noexcept;
    bool inline HasROCompatFeature(uint32_t Flag) const noexcept;
    bool inline HasIncompatFeature(uint32_t Flag) const noexcept;

    utils::LinuxFile m_device;

    xfs_superblock_t m_superblock;
};

#endif // !_XFS_PARSER_HPP_
