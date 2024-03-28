#pragma once 

#include <byteswap.h>
#include <iostream>
#include <string>

#define DEBUG 0
#if DEBUG
#define LOG_DEBUG(x) std::cout << x << "\n";
#else
#define LOG_DEBUG(x)
#endif

#define LOG_ERROR(x) std::cerr << x << "\n";
#define LOG_INFO(x) std::cout << x << "\n";

enum class eSide
{
    BID,
    ASK
};

using Price = double;
using Quantity = uint64_t;
using Index = size_t;

struct PriceLevel
{
    Price m_price{0};
    Quantity m_qty{0};
};

