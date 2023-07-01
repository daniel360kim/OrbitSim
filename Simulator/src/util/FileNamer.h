/**
 * @file FileNamer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief makes iterative file names
 * @version 0.1
 * @date 2023-06-28
 *
 *
 */



#ifndef FILE_NAMER_H
#define FILE_NAMER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

class FileNamer
{
public:
    explicit FileNamer(const std::string &filename) : m_filename(filename), m_counter(1) {}

    std::string getAvailableFilename()
    {
        std::string newFilename = m_filename;
        while (fileExists(newFilename))
        {
            newFilename = generateFilenameWithCounter();
            m_counter++;
        }
        return newFilename;
    }

private:
    std::string m_filename;
    int m_counter;

    std::string generateFilenameWithCounter() const
    {
        size_t dotPos = m_filename.find_last_of('.');
        if (dotPos != std::string::npos)
        {
            return m_filename.substr(0, dotPos) + std::to_string(m_counter) + m_filename.substr(dotPos);
        }
        return m_filename + std::to_string(m_counter);
    }

    bool fileExists(const std::string &filename) const
    {
        std::ifstream file(filename);
        return file.good();
    }
};

#endif // FILE_NAMER_H