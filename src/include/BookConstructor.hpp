#pragma once

#include <sstream>
#include "Reader.hpp"
#include "OrderBookByIndex.hpp"

class Data
{
    public:
        uint64_t m_sequence_number;
        char m_action;
        char m_side;
        Index m_index;
        Price m_price;
        Quantity m_size;

        
        Data(const std::string& line_)
        {
            std::istringstream iss(line_);
            std::string token;
            std::getline(iss, token, ',');
            m_sequence_number = std::stoull(token);
            std::getline(iss, token, ',');
            m_action = token[0];
            std::getline(iss, token, ',');
            m_side = token[0];
            std::getline(iss, token, ',');
            m_index = std::stoull(token);
            std::getline(iss, token, ',');
            m_price = std::stod(token);
            std::getline(iss, token, ',');
            m_size = std::stoull(token);
        }

        void print()
        {
            std::cout << "sequence_number: " << m_sequence_number << std::endl;
            std::cout << "m_action: " << m_action << std::endl;
            std::cout << "m_side: " << m_side << std::endl;
            std::cout << "m_index: " << m_index << std::endl;
            std::cout << "m_price: " << m_price << std::endl;
            std::cout << "m_size: " << m_size << std::endl;
        }

};

class BookConstructor{
    public:
        Reader m_reader;
        int count = 0;
        OrderBook order_book;
        uint64_t last_sequence_number = 0;

        BookConstructor(const std::string& inputFileName) : m_reader(inputFileName)
         {
             
         } 

         void start()
         {
             while (!m_reader.eof() && m_reader.isValid())
             {
                 next();
             }
         }

         void next()
         {
                std::string data = m_reader.readLine();

                if (data == "")
                    return;

                Data data_(data);
                //data_.print();

                switch (data_.m_action)
                {
                    case 'I':
                    {
                        if (data_.m_side == 'B')
                        {
                            order_book.insert_level<eSide::BID>(data_.m_index, data_.m_price, data_.m_size);
                        }
                        else
                        {
                            order_book.insert_level<eSide::ASK>(data_.m_index, data_.m_price, data_.m_size);
                        }
                        break;
                    }
                    case 'U':
                    {
                        if (data_.m_side == 'B')
                        {
                            order_book.update_level<eSide::BID>(data_.m_index, data_.m_price, data_.m_size);
                        }
                        else
                        {
                            order_book.update_level<eSide::ASK>(data_.m_index, data_.m_price, data_.m_size);
                        }
                        break;
                    }
                    case 'D':
                    {
                        if (data_.m_side == 'B')
                        {
                            order_book.delete_level<eSide::BID>(data_.m_index, data_.m_price, data_.m_size);
                        }
                        else
                        {
                            order_book.delete_level<eSide::ASK>(data_.m_index, data_.m_price, data_.m_size);
                        }
                        break;
                    }

                }
                if (last_sequence_number != data_.m_sequence_number)
                {
                    if(order_book.is_recovered())
                        LOG_INFO(data_.m_sequence_number << "," << order_book);
                }
                last_sequence_number = data_.m_sequence_number;

                if (order_book.is_crossed())
                {
                    LOG_ERROR("CROSSED BOOK");
                }

                ++count;
                
         }
};
