#include "../include/not_implemented_exception.h"

NotImplementedException::NotImplementedException() noexcept {
    string = "NotInmplementedException: method or function does not provide implementation";
}

NotImplementedException::NotImplementedException(const_string data) noexcept {
    string = data;
}

NotImplementedException::const_string NotImplementedException::what() const noexcept {
    return string;
}
        