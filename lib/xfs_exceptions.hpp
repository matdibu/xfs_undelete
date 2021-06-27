#ifndef _XFS_EXCEPTIONS_HPP_
#define _XFS_EXCEPTIONS_HPP_

#include <exception> // std::exception
#include <string>    // std::string

namespace uf {
    namespace xfs {
        // Validation inconsistencies
        class ValidationException;
    } // namespace xfs
} // namespace uf

class uf::xfs::ValidationException : public std::exception
{
public:
    explicit ValidationException(const char* Message);
    explicit ValidationException(std::string Message) noexcept;
    const char* what() const noexcept override;

private:
    const std::string m_message;
};

#endif // !_XFS_EXCEPTIONS_HPP_
