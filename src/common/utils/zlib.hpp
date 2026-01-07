#pragma once
#include <stdexcept>
#include <vector>

namespace utils
{
    struct zlib
    {
        using error = std::runtime_error;

        static auto compress(std::vector<uint8_t> const& data) -> std::vector<uint8_t>;
        static auto decompress(std::vector<uint8_t> const& data, uint32_t length) -> std::vector<uint8_t>;
    };
}