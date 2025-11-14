#pragma once
#include <stdint.h>
#include <string>
#include <array>
#include <map>


#define NODISCARD_MSG "This value is not being used!"
#define NODISCARD [[nodiscard(NODISCARD_MSG)]]
#define PI 3.14159265

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using String = std::string;

template<typename K, typename V>
using Map = std::map<K,V>;

template<class T, size_t N>
using Array = std::array<T, N>;