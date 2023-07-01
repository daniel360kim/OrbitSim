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
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "FileNamer.h"

template <typename... Args>
class CSVWriter
{
public:
    explicit CSVWriter(const std::string& filename, const std::vector<std::string>& headers)
        : m_fileNamer(filename)
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

    void addRow(const Args&... values)
    {
        std::ostringstream row;

        appendCSV(row, values...);
        row << '\n';

        m_file << row.str();
    }

private:
    FileNamer m_fileNamer;
    std::ofstream m_file;

    // Base case for the variadic template recursion
    void appendCSV(std::ostringstream&) {}

    // Recursive function to append the values to the CSV row
    template <typename T, typename... Rest>
    void appendCSV(std::ostringstream& row, const T& value, const Rest&... rest)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            row << std::fixed << std::setprecision(5) << value;
        }
        else
        {
            row << value;
        }
        if constexpr (sizeof...(rest) > 0)
        {
            row << ',';
        }
        appendCSV(row, rest...);
    }

    void openFile()
    {
        std::string newFilename = m_fileNamer.getAvailableFilename();
        m_file.open(newFilename, std::ios_base::out | std::ios_base::binary);
        if (!m_file.is_open())
        {
            std::cerr << "Failed to open file: " << newFilename << std::endl;
        }
        m_file.rdbuf()->pubsetbuf(nullptr, 0);  // Enable buffering
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

