#pragma once
#include <imgui.h>
#include <cstdint>
#include <string_view>
#include <vector>
#include <fstream>
#include <string>
#include <string_view>


class WindowClass
{
public:
    constexpr static auto lower_section_height = 50.0F;

    static constexpr auto popupFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        |  ImGuiWindowFlags_NoCollapse |  ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popUpSize = ImVec2(340.0F, 460.0F);

    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

    constexpr static auto input_flags =
     ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_CallbackEdit;

     constexpr static auto selectable_flags = ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap;

    constexpr static auto window_size = ImVec2(360.0F, 480.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);


    const std::string program_name_data = "program_name_data.txt";
    const std::string program_comment_data = "program_comment_data.txt";
    const std::string program_check_data = "program_check_data.txt";

    std::vector<std::string> task_name;
    std::vector<std::string> task_comment;
    std::vector<std::string> task_is_done;

    void Draw(std::string_view label);

    void loadContent(std::vector<std::string> *content, std::string path);
    void SaveContent(std::vector<std::string> *content, std::string path);//it maybe go to the private
    void loadFont();


private:   
    int selectedTask = -1;

    void DrawContent();

    void editTask();
    void addTask();
    void deleteTask();

    bool show_modal_popup = false; 
    
};

void render(WindowClass &window_obj);
