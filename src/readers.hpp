//
// Created by antos07 on 10/25/23.
//

#ifndef SYSPROG_LAB4_READERS_HPP
#define SYSPROG_LAB4_READERS_HPP

#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include "grammar.hpp"

namespace readers {
    class ReaderException : public std::exception {
        std::string message_;

    public:
        explicit ReaderException(std::string message);

        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
    };

    grammar::Grammar ReadGrammar(std::istream &in);

    std::vector<char> ReadTokens(std::istream &in);
}

#endif //SYSPROG_LAB4_READERS_HPP
