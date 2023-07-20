/**
 * @file Section.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief ImGui components that match the style of the rest of the program
 * @version 0.1
 * @date 2023-07-19
 * 
 * 
 */

#pragma once

#include <string>
#include <vector>

#include <imgui.h>

class Section
{
public:
    Section(ImVec2 initialPosition, std::vector<ImFont*> fonts);

    void End();

    void Title(const char* title, ...);
    void Heading(const char* heading, ...);
    void Text(const char* text, ...);
    void SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "0.3f", ImGuiSliderFlags flags = 0);
    void SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
    bool InlineButton(const char* label, const ImVec2& size = ImVec2(0, 0));
    void Separator();
    void Spacer(float height = 10.0f);
private:
    ImVec2 m_initialPosition;
    ImVec2 m_currentPosition;
    std::vector<ImFont*> m_fonts;
    ImGuiStyle &m_style = ImGui::GetStyle();

    ImVec4 m_titleColor;
    ImVec4 m_headingColor;
    ImVec4 m_textColor;

    int m_previousButtonWidth = 0;

    bool m_initialTitleAdded = false;
    bool m_justAddedSeparator;
    void incrementPosition(float increment);

};
