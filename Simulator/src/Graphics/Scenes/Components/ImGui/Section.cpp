/**
 * @file Section.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-19
 *
 *
 */

#include "Section.h"

#include <cmath>
#include <stdexcept>
#include <iostream>

int Section::m_numSections = 0;

Section::Section(ImVec2 initialPosition, std::vector<ImFont *> fonts)
    : m_initialPosition(initialPosition), m_currentPosition(initialPosition), m_fonts(fonts), m_justAddedSeparator(false)
{
    ImGui::SetCursorPos(m_initialPosition);

    m_titleColor = ImVec4(1.0f, 0.906f, 0.608f, 1.0f);
    m_headingColor = ImVec4(0.867f, 0.345f, 0.839f, 1.0f);
    m_textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Slider colors
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.867f, 0.863f, 0.859f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(1.0f, 0.906f, 0.608f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(1.0f, 0.906f, 0.608f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.333f, 0.333f, 0.333f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.333f, 0.333f, 0.333f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f));

    // Set custom button border properties
    m_style.FrameBorderSize = 1.0f; // Set border thickness
    m_style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    m_numSections++;
}

Section::~Section()
{
}

void Section::End()
{
    // Slider colors
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar();
}

void Section::Title(const char *title, ...)
{
    if (m_initialTitleAdded)
    {
        incrementPosition(60.0f);
    }
    else
    {
        m_initialTitleAdded = true;
    }

    m_style.Colors[ImGuiCol_Text] = m_titleColor;
    ImGui::PushFont(m_fonts[2]);

    va_list args;
    va_start(args, title);
    ImGui::TextV(title, args);
    va_end(args);

    ImGui::PopFont();
    m_style.Colors[ImGuiCol_Text] = m_textColor;
}

void Section::Heading(const char *heading, ...)
{
    incrementPosition(50.0f);

    m_style.Colors[ImGuiCol_Text] = m_headingColor;
    ImGui::PushFont(m_fonts[3]);

    va_list args;
    va_start(args, heading);
    ImGui::TextV(heading, args);
    va_end(args);

    ImGui::PopFont();
    m_style.Colors[ImGuiCol_Text] = m_textColor;

    m_currentPosition.y += 10.0f;
}

void Section::Text(const char *text, ...)
{
    incrementPosition(30.0f);

    ImGui::PushFont(m_fonts[4]);

    va_list args;
    va_start(args, text);
    ImGui::TextV(text, args);
    va_end(args);

    ImGui::PopFont();
}

void Section::SliderFloat(const char *label, float *v, float v_min, float v_max, const char *format, ImGuiSliderFlags flags)
{
    incrementPosition(35.0f);
    ImGui::PushFont(m_fonts[4]);

    ImGui::Text(label);
    m_currentPosition.x += 110.0f;
    ImGui::SetCursorPos(m_currentPosition);
    m_style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImGui::SliderFloat(label, v, v_min, v_max, format, flags);
    m_style.Colors[ImGuiCol_Text] = m_textColor;
    m_currentPosition.x -= 110.0f;

    ImGui::PopFont();
    m_style.Colors[ImGuiCol_Text] = m_textColor;
}

void Section::SliderInt(const char *label, int *v, int v_min, int v_max, const char *format, ImGuiSliderFlags flags)
{
    incrementPosition(35.0f);
    ImGui::PushFont(m_fonts[4]);

    ImGui::Text(label);
    m_currentPosition.x += 110.0f;
    ImGui::SetCursorPos(m_currentPosition);
    m_style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImGui::SliderInt(label, v, v_min, v_max, format, flags);
    m_style.Colors[ImGuiCol_Text] = m_textColor;
    m_currentPosition.x -= 110.0f;

    ImGui::PopFont();
    m_style.Colors[ImGuiCol_Text] = m_textColor;
}

bool Section::Button(const char *label, const ImVec2 &size, ImVec4 color, ImVec4 hoverColor)
{
    incrementPosition(35.0f);

    ImGui::PushFont(m_fonts[4]);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);

    bool result = ImGui::Button(label, size);

    m_previousButtonWidth = static_cast<int>(ImGui::GetItemRectSize().x);

    ImGui::PopFont();
    ImGui::PopStyleColor(2);

    return result;
}

bool Section::InlineButton(const char *label, const ImVec2 &size, ImVec4 color, ImVec4 hoverColor)
{
    m_currentPosition.x += m_previousButtonWidth;
    ImGui::SetCursorPos(m_currentPosition);

    ImGui::PushFont(m_fonts[4]);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);

    bool result = ImGui::Button(label, size);

    m_currentPosition.x -= m_previousButtonWidth;
    ImGui::SetCursorPos(m_currentPosition);
    m_previousButtonWidth += ImGui::GetItemRectSize().x;

    ImGui::PopFont();
    ImGui::PopStyleColor(2);

    return result;
}

std::vector<bool> Section::DropDown(const char *label, const char *previewText, std::vector<std::string> selectionLabels, ImGuiComboFlags flags)
{
    incrementPosition(35.0f);

    ImGui::PushFont(m_fonts[4]);

    m_style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    std::vector<bool> result;
    if (ImGui::BeginCombo(label, previewText, flags))
    {
        m_style.Colors[ImGuiCol_Text] = m_textColor;
        for (size_t i = 0; i < selectionLabels.size(); i++)
        {
            bool isSelected = false;
            if (ImGui::Selectable(selectionLabels[i].c_str(), &isSelected))
            {
                result.push_back(isSelected);
            }
            else if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
            else
            {
                result.push_back(false);
            }
        }
        ImGui::EndCombo();
    }

    m_style.Colors[ImGuiCol_Text] = m_textColor;
    ImGui::PopStyleColor();

    ImGui::PopFont();

    m_currentPosition.y += 10.0f;

    return result;
}

void Section::Table(int numCols, std::vector<std::string> headings, std::vector<std::vector<std::string>> rows, const char *buttonLabel, const char *deselectButtonLabel, std::vector<bool> &selectedRows, int maxRows)
{
    incrementPosition(30.0f);

    ImGui::PushFont(m_fonts[5]);

    // Calculate column widths based on the longest string in each column of the first row
    std::vector<float> columnWidths;
    if (rows.size() == 0)
    {
        for (int i = 0; i < numCols; i++)
        {
            columnWidths.push_back(70.0f);
        }
    }
    else
    {
        std::vector<std::string> firstRow = rows[0];
        for (auto &column : firstRow)
        {
            float width = ImGui::CalcTextSize(column.c_str()).x;
            for (auto &row : rows)
            {
                float rowWidth = ImGui::CalcTextSize(row[std::distance(firstRow.begin(), std::find(firstRow.begin(), firstRow.end(), column))].c_str()).x + 20.0f;
                if (rowWidth > width)
                {
                    width = rowWidth;
                }
            }
            columnWidths.push_back(width);
        }
    }

    ImGui::SetCursorPos(m_currentPosition);
    ImGui::Separator();

    // Print table headings
    m_currentPosition.y += 10.0f;
    ImVec2 originalPosition = m_currentPosition;
    for (auto &heading : headings)
    {
        ImGui::SetCursorPos(m_currentPosition);
        ImGui::Text(heading.c_str());
        // increment position by the width of the heading
        m_currentPosition.x += columnWidths[std::distance(headings.begin(), std::find(headings.begin(), headings.end(), heading))] + 10.0f;
    }

    m_currentPosition = originalPosition;
    m_currentPosition.y += 30.0f;

    ImGui::SetCursorPos(m_currentPosition);
    ImGui::Separator();

    // Print table rows
    ImGui::PopFont();

    if (rows.size() == 0)
    {
        ImGui::PushFont(m_fonts[4]);
        m_currentPosition.y += 10.0f;
        ImGui::SetCursorPos(m_currentPosition);
        ImGui::Text("No results found.");
        ImGui::PopFont();
        m_currentPosition.y += 30.0f;
        return;
    }

    ImGui::PushFont(m_fonts[4]);
    m_currentPosition.y += 10.0f;
    originalPosition = m_currentPosition;

    if (selectedRows.size() != rows.size())
    {
        selectedRows.resize(rows.size(), false);
    }

    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i >= maxRows)
        {
            break;
        }

        for (size_t j = 0; j < rows[i].size(); j++)
        {
            ImGui::SetCursorPos(m_currentPosition);
            ImGui::Text(rows[i][j].c_str());
            m_currentPosition.x += columnWidths[j] + 10.0f;
        }

        if (!selectedRows[i])
        {
            m_currentPosition.x += 10.0f;
            ImGui::SetCursorPos(m_currentPosition);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3490, 0.8431, 0.6039, 1.0f));

            std::string label = buttonLabel;
            label += "##" + std::to_string(i) + std::to_string(m_numSections);
            if (ImGui::Button(label.c_str()))
            {
                selectedRows[i] = true;
            }

            ImGui::PopStyleColor();
        }
        else
        {
            m_currentPosition.x += 10.0f;
            ImGui::SetCursorPos(m_currentPosition);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8235, 0.1216f, 0.2353f, 1.0f));

            std::string label = deselectButtonLabel;
            label += "##" + std::to_string(i) + std::to_string(m_numSections);
            if (ImGui::Button(label.c_str()))
            {
                selectedRows[i] = false;
            }

            ImGui::PopStyleColor();
        }

        m_currentPosition.x = originalPosition.x;
        m_currentPosition.y += 30.0f;
    }

    ImGui::PopFont();

    m_currentPosition.y += 10.0f;
}

void Section::SingleSelectTable(int numCols, std::vector<std::string> headings, std::vector<std::vector<std::string>> rows, const char *buttonLabel, const char *deselectButtonLabel, std::vector<bool> &selectedRows, int maxRows)
{
    incrementPosition(30.0f);

    ImGui::PushFont(m_fonts[5]);

    // Calculate column widths based on the longest string in each column of the first row
    std::vector<float> columnWidths;
    if (rows.size() == 0)
    {
        for (int i = 0; i < numCols; i++)
        {
            columnWidths.push_back(70.0f);
        }
    }
    else
    {
        std::vector<std::string> firstRow = rows[0];
        for (auto &column : firstRow)
        {
            float width = ImGui::CalcTextSize(column.c_str()).x;
            for (auto &row : rows)
            {
                float rowWidth = ImGui::CalcTextSize(row[std::distance(firstRow.begin(), std::find(firstRow.begin(), firstRow.end(), column))].c_str()).x + 20.0f;
                if (rowWidth > width)
                {
                    width = rowWidth;
                }
            }
            columnWidths.push_back(width);
        }
    }

    ImGui::SetCursorPos(m_currentPosition);
    ImGui::Separator();

    // Print table headings
    m_currentPosition.y += 10.0f;
    ImVec2 originalPosition = m_currentPosition;
    for (auto &heading : headings)
    {
        ImGui::SetCursorPos(m_currentPosition);
        ImGui::Text(heading.c_str());
        // increment position by the width of the heading
        m_currentPosition.x += columnWidths[std::distance(headings.begin(), std::find(headings.begin(), headings.end(), heading))] + 10.0f;
    }

    m_currentPosition = originalPosition;
    m_currentPosition.y += 30.0f;

    ImGui::SetCursorPos(m_currentPosition);
    ImGui::Separator();

    // Print table rows
    ImGui::PopFont();

    if (rows.size() == 0)
    {
        ImGui::PushFont(m_fonts[4]);
        m_currentPosition.y += 10.0f;
        ImGui::SetCursorPos(m_currentPosition);
        ImGui::Text("No results found.");
        ImGui::PopFont();
        m_currentPosition.y += 30.0f;
        return;
    }

    ImGui::PushFont(m_fonts[4]);
    m_currentPosition.y += 10.0f;
    originalPosition = m_currentPosition;

    if (selectedRows.size() != rows.size())
    {
        selectedRows.resize(rows.size(), false);
    }

    for (size_t i = 0; i < rows.size(); i++)
    {
        if (i >= maxRows)
        {
            break;
        }

        for (size_t j = 0; j < rows[i].size(); j++)
        {
            ImGui::SetCursorPos(m_currentPosition);
            ImGui::Text(rows[i][j].c_str());
            m_currentPosition.x += columnWidths[j] + 10.0f;
        }

        if (!selectedRows[i])
        {
            m_currentPosition.x += 10.0f;
            ImGui::SetCursorPos(m_currentPosition);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3490, 0.8431, 0.6039, 1.0f));

            std::string label = buttonLabel;
            label += "##" + std::to_string(i) + std::to_string(m_numSections);
            if (ImGui::Button(label.c_str()))
            {
                selectedRows[i] = true;
                // deselect all other rows
                for (size_t j = 0; j < selectedRows.size(); j++)
                {
                    if (j != i)
                    {
                        selectedRows[j] = false;
                    }
                }
            }

            ImGui::PopStyleColor();
        }
        else
        {
            m_currentPosition.x += 10.0f;
            ImGui::SetCursorPos(m_currentPosition);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8235, 0.1216f, 0.2353f, 1.0f));

            std::string label = deselectButtonLabel;
            label += "##" + std::to_string(i) + std::to_string(m_numSections);
            if (ImGui::Button(label.c_str()))
            {
                selectedRows[i] = false;
            }

            ImGui::PopStyleColor();
        }

        m_currentPosition.x = originalPosition.x;
        m_currentPosition.y += 30.0f;
    }

    ImGui::PopFont();

    m_currentPosition.y += 10.0f;
}

void Section::TextInput(const char *label, char *buf, size_t buf_size, ImGuiInputTextFlags flags)
{
    incrementPosition(45.0f);

    ImGui::PushFont(m_fonts[6]);

    ImGui::Text(label);
    m_currentPosition.x += 90.0f;
    ImGui::SetCursorPos(m_currentPosition);
    m_style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    ImGui::PopFont();

    ImGui::PushFont(m_fonts[4]);

    ImGui::InputText(label, buf, buf_size, flags);

    m_style.Colors[ImGuiCol_Text] = m_textColor;
    m_currentPosition.x -= 90.0f;

    ImGui::PopFont();
}

void Section::Separator()
{
    incrementPosition(50.0f);
    ImGui::Separator();

    m_justAddedSeparator = true;
}

void Section::Spacer(float height)
{
    incrementPosition(height);
}

void Section::incrementPosition(float increment)
{
    if (m_justAddedSeparator)
    {
        m_currentPosition.y += 10.0f;
        ImGui::SetCursorPos(m_currentPosition);
        m_justAddedSeparator = false;
    }
    else
    {
        m_currentPosition.y += increment;
        ImGui::SetCursorPos(m_currentPosition);
    }
}
