/**
 * @m_file csvWriter.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief generates csv m_files for data
 * @version 0.1
 * @date 2023-06-28
 *
 *
 */

#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "FileNamer.h"

template <typename... Args>
class CSVWriter
{
public:
    explicit CSVWriter(const std::string &filename, const std::vector<std::string>& headers) : m_fileNamer(filename)
    {
        openFile();

        std::stringstream ss;
        for (size_t i = 0; i < headers.size(); ++i)
        {
            ss << headers[i];
            if (i != headers.size() - 1)
            {
                ss << ',';
            }
        }

        m_file << ss.str() << '\n';

    }

    ~CSVWriter()
    {
        closeFile();
    }

    void addRow(const Args &...values)
    {
        if (!m_file.is_open())
        {
            std::cerr << "File is not open!" << std::endl;
            return;
        }

        std::stringstream ss;
        appendCSV(ss, values...);
        m_file << ss.str() << '\n';
    }

private:
    FileNamer m_fileNamer;
    std::ofstream m_file;

    // Base case for the variadic template recursion
    void appendCSV(std::stringstream &ss) {}

    // Recursive function to append the values to the CSV string
    template <typename T, typename... Rest>
    void appendCSV(std::stringstream &ss, const T &value, const Rest &...rest)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            ss << std::fixed << std::setprecision(5) << value;
        }
        else
        {
            ss << value;
        }
        if (sizeof...(rest) > 0)
        {
            ss << ',';
        }
        appendCSV(ss, rest...);
    }

    void openFile()
    {
        std::string newFilename = m_fileNamer.getAvailableFilename();
        m_file.open(newFilename);
        if (!m_file.is_open())
        {
            std::cerr << "Failed to open file: " << newFilename << std::endl;
        }
    }

    void closeFile()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }
};

#endif