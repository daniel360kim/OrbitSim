/**
 * @file ProgressBar.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief progress bar
 * @version 0.1
 * @date 2023-06-28
 *
 *
 */

#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <iostream>
#include <string>

class ProgressBar
{
public:
    ProgressBar(int width) : width(width) {}

    void update(float progress)
    {
        int completedWidth = static_cast<int>(progress * width);
        int remainingWidth = width - completedWidth;

        std::string progressBar;
        progressBar += '[';
        progressBar += std::string(completedWidth, '=');
        progressBar += std::string(remainingWidth, ' ');
        progressBar += ']';

        std::cout << "\r" << progressBar << " " << static_cast<int>(progress * 100) << "%" << std::flush;
    }

private:
    int width;
};

#endif