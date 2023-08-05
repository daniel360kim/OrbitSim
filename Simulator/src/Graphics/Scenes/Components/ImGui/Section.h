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
    ~Section();

    void End();

    void Title(const char* title, ...);
    void Heading(const char* heading, ...);
    void Text(const char* text, ...);
    void SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "0.3f", ImGuiSliderFlags flags = 0);
    void SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool Button(const char* label, const ImVec2& size = ImVec2(0, 0), ImVec4 color = ImVec4(0, 0, 0, 0), ImVec4 hoverColor = ImVec4(1.0f, 0.906f, 0.608f, 0.8f));
    bool InlineButton(const char* label, const ImVec2& size = ImVec2(0, 0), ImVec4 color = ImVec4(0, 0, 0, 0), ImVec4 hoverColor = ImVec4(1.0f, 0.906f, 0.608f, 0.8f));
    std::vector<bool> DropDown(const char* label, const char* previewText, std::vector<std::string> selectionLabels, ImGuiComboFlags flags = 0);
    void Table(int numCols, std::vector<std::string> headings, std::vector<std::vector<std::string>> rows, const char* buttonLabel, const char* deselectButtonLabel, std::vector<bool>& selectedRows, int maxRows = 30);
    void SingleSelectTable(int numCols, std::vector<std::string> headings, std::vector<std::vector<std::string>> rows, const char* buttonLabel, const char* deselectButtonLabel, std::vector<bool>& selectedRows, int maxRows = 30);
    void TextInput(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0);
    

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

    size_t m_previousNumRows;

    bool m_initialTitleAdded = false;
    bool m_justAddedSeparator;
    void incrementPosition(float increment);

    static int m_numSections;

};
