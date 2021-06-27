#include <exception>

#include <string>

class pandora_exception : std::exception {};
class ErrnoException : pandora_exception {

    public:
        ErrnoException(std::string, int){}
        ErrnoException(const char*, int){}
};
