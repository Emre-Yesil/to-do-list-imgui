#pragma once

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

    const std::string program_name_data = "program_name_data.txt";
    const std::string program_comment_data = "program_comment_data.txt";
    const std::string program_check_data = "program_check_data.txt";

    std::vector<std::string> task_name;
    std::vector<std::string> task_comment;
    std::vector<std::string> task_is_done;

    void Draw(std::string_view label);

    void loadContent(std::vector<std::string> *content, std::string path);
    void SaveContent(std::vector<std::string> *content, std::string path);

private:   
    size_t selectedTask;

    void DrawContent();

    void editTask();
    void addTask();
    void deleteTask();
    
};

void render(WindowClass &window_obj);
