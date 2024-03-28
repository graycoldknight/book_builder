#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include "CommonDefs.h"

class Reader{
private:
    std::string m_fileName;
    std::ifstream m_file;
    bool m_validFile = 0;

public:

    Reader(const std::string &fileName_): m_fileName(fileName_)
    {
        m_file.open(m_fileName);
        if (!m_file.is_open())
        { 
            LOG_ERROR("The input file: " << m_fileName << " cannot be open! ");
        }
        else
        {
            LOG_DEBUG("Opened " << m_fileName << " to read  messages.");
            m_validFile = 1;
        }
    }

    bool isValid(void) const
    {
        return m_validFile;
    }

    std::string readLine(void)
    {
        std::string line;
        std::getline(m_file, line);
        return line;
    }

    virtual ~Reader()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

    bool eof(void)
    {
        return m_file.eof();
    }

    std::string getFileName(void) const {return m_fileName;}
};
