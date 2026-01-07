#include "zlib.h"
#include "zlib.hpp"

#include <format>

namespace utils
{

    auto zlib::compress(std::vector<uint8_t> const& data) -> std::vector<uint8_t>
    {
        auto length = compressBound(static_cast<uLong>(data.size()));

        auto output = std::vector<uint8_t>{};
        output.resize(length);

        auto result = compress2(reinterpret_cast<Bytef*>(output.data()), &length, reinterpret_cast<const Bytef*>(data.data()), static_cast<uLong>(data.size()), Z_BEST_COMPRESSION);

        if (result == Z_OK)
        {
            output.resize(length);
            return output;
        }

        throw error(std::format("zlib compress error {}", result));
    }

    auto zlib::decompress(std::vector<uint8_t> const& data, uint32_t length) -> std::vector<uint8_t>
    {
        auto output = std::vector<uint8_t>{};
        output.resize(length);

        auto result = uncompress(reinterpret_cast<Bytef*>(output.data()), reinterpret_cast<uLongf*>(&length), reinterpret_cast<const Bytef*>(data.data()), static_cast<uLong>(data.size()));

        if (result == Z_OK)
            return output;

        throw error(std::format("zlib decompress error {}", result));
    }
}
