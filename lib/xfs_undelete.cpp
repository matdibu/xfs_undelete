// #include "undelete_xfs/undelete_xfs.hpp" // uf::xfs::ISoInterface, uf::xfs::SO_INTERFACE_VERSION
// #include "pandora_log.hpp"               // PAN_LOG_*
#include "spdlog/spdlog.h"
#include "singleton.hpp"                 // utils::SingletonHeap
#include "xfs_parser.hpp"                // uf::xfs::Parser

using uf::xfs::IInodeCallback;
using uf::xfs::ISoInterface;
using uf::xfs::Parser;
using uf::xfs::SO_INTERFACE_VERSION;
using utils::SingletonHeap;

struct UfXfsSo
    : public ISoInterface
    , public SingletonHeap<UfXfsSo>
{
public:
    bool GetInodes(const std::string& DevicePath, IInodeCallback* Callback) noexcept final
    {
        if (nullptr == Callback)
        {
            PAN_LOG_ERROR("GetInodes nullptr Callback")
            return false;
        }

        try
        {
            return Parser(DevicePath).DumpInodes(Callback);
        }
        catch (const std::exception& exc)
        {
            PAN_LOG_ERROR("Get Inodes: %s", exc.what())
            return false;
        }
    }
};

bool GetSoInterface(const uint32_t InterfaceVersion, ISoInterface** SoInterface)
{
    if (SO_INTERFACE_VERSION != InterfaceVersion)
        return false;

    try
    {
        *SoInterface = UfXfsSo::GetInstance();
        return true;
    }
    catch (const std::exception& exc)
    {
        PAN_LOG_ERROR("GetSoInterface uf::xfs: %s", exc.what())
        return false;
    }
}

bool ReleaseSoInterface(ISoInterface* /*Unused*/)
{
    try
    {
        UfXfsSo::ReleaseInstance();
        return true;
    }
    catch (const std::exception& exc)
    {
        PAN_LOG_ERROR("ReleaseSoInterface uf::xfs: %s", exc.what())
        return false;
    }
}
