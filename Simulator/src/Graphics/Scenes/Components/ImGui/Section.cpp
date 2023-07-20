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

    // Button colors
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.906f, 0.608f, 0.8f));

    // Set custom button border properties
    m_style.FrameBorderSize = 1.0f; // Set border thickness
    m_style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Section::End()
{
    // Slider colors
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar();

    // Button colors
    ImGui::PopStyleColor(2);
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


void Section::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
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

bool Section::Button(const char *label, const ImVec2& size)
{
    incrementPosition(35.0f);

    ImGui::PushFont(m_fonts[4]);

    bool result = ImGui::Button(label, size);

    m_previousButtonWidth = static_cast<int>(ImGui::GetItemRectSize().x);

    ImGui::PopFont();

    return result;
}

bool Section::InlineButton(const char* label, const ImVec2& size)
{
    m_currentPosition.x += m_previousButtonWidth;
    ImGui::SetCursorPos(m_currentPosition);

    ImGui::PushFont(m_fonts[4]);

    bool result = ImGui::Button(label, size);

    m_currentPosition.x -= m_previousButtonWidth;
    ImGui::SetCursorPos(m_currentPosition);
    m_previousButtonWidth += ImGui::GetItemRectSize().x;

    ImGui::PopFont();

    return result;
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



