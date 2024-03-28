#pragma once

#include <iostream>
#include <array>
#include "CommonDefs.h"

/**
 * OrderBookPerSide class maintains an array of price levels per side.
 */
class OrderBookPerSide
{
public:
    static constexpr size_t MAX_LEVELS = 5;
    
    using PriceLevels = std::array<PriceLevel, MAX_LEVELS>;

    // Insert price level at the given index and shift remaining price levels.
    void insert_level(Index idx, Price price, Quantity qty)
    {
        LOG_DEBUG("insert_level: "
           << "idx=" << idx
           << ", price=" << price
           << ", qty=" << qty);
        // Shift the levels.
        // 4 - 1
        // 3 -> 4
        // 2 -> 3
        // 1 -> 2
        for (Index i = MAX_LEVELS - 1; i > idx; --i)
        {
            m_levels[i] = m_levels[i-1];
        }
        m_levels[idx].m_price = price;
        m_levels[idx].m_qty = qty;
    }
    
    // Update price level at the given index.
    void update_level(Index idx, Price price, Quantity qty)
    {
        LOG_DEBUG("update_level: "
           << "idx=" << idx
           << ", price=" << price
           << ", qty=" << qty);
        m_levels[idx].m_price = price;
        m_levels[idx].m_qty = qty;
    }

    // Delete price level at the given index and shift remaining price levels.
    void delete_level(Index idx)
    {
        LOG_DEBUG("delete_level: "
           << "idx=" << idx);

        // Shift the levels.
        Index i = idx;
        // 1 - 4
        // 2 -> 1
        // 3 -> 2
        // 4 -> 3
        for (i = idx; i < MAX_LEVELS-1; ++i)
        {
            m_levels[i] = m_levels[i+1];
        }
        // Erase the last level.
        m_levels[i] = PriceLevel();
    }

    template <typename OStreamT>
    friend
    OStreamT& operator << (OStreamT& os, const OrderBookPerSide& book)
    {
        for (Index i = 0; i < MAX_LEVELS; ++i)
        {
            if (book.m_levels[i].m_price == 0)
            {
                os << "_,_";
            }
            else
            {
                os << book.m_levels[i].m_price << "," << book.m_levels[i].m_qty;
            }
            if (i != MAX_LEVELS - 1)
            {
                os << ",";
            }
        }
        return os;
    }

    const PriceLevels& get_levels() const
    { 
        return m_levels;
    }

     bool is_recovered()
    {
        if (!m_recovered && m_levels[MAX_LEVELS-1].m_price != 0)
        {
            m_recovered = true;
        }
        return m_recovered;
    }

private:
    PriceLevels m_levels;
    bool m_recovered{false};
};

/**
 * OrderBook class maintains OrderBookPerSide for bid and ask.
 */
class OrderBook
{
public:
    template <eSide side>
    void insert_level(Index idx, Price price, Quantity qty)
    {
        if constexpr (side == eSide::BID)
        {
            m_bid_book.insert_level(idx, price, qty);
        }
        else
        {
            m_ask_book.insert_level(idx, price, qty);
        }
    }
  
    template <eSide side>
    void update_level(Index idx, Price price, Quantity qty)
    {
        if constexpr (side == eSide::BID)
        {
            m_bid_book.update_level(idx, price, qty);
        }
        else
        {
            m_ask_book.update_level(idx, price, qty);
        }
    }
  
    template <eSide side>
    void delete_level(Index idx, Price price, Quantity qty)
    {
        if constexpr (side == eSide::BID)
        {
            m_bid_book.delete_level(idx);
        }
        else
        {
            m_ask_book.delete_level(idx);
        }
    }

    bool is_crossed() const
    {
        const auto& best_bid = m_bid_book.get_levels()[0];
        const auto& best_ask = m_ask_book.get_levels()[0];

        return (best_bid.m_price != 0 && best_ask.m_price != 0 &&
                best_bid.m_price >= best_ask.m_price);
    }


    bool is_recovered()
    {
        return (m_bid_book.is_recovered() && m_ask_book.is_recovered());
    }
    
    template <typename OStreamT>
    friend
    OStreamT& operator << (OStreamT& os, const OrderBook& book)
    {
        os << book.m_bid_book;
        os << ",";
        os << book.m_ask_book;
        return os;
    }

private:
    OrderBookPerSide m_bid_book;
    OrderBookPerSide m_ask_book;
};
