#include <iostream>

//#include <fmt/format.h>
#include <imgui.h>
//#include <implot.h>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>
//#include <sstream>
//#include <typeinfo>

#include <windows.h>
#include <shlobj.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

    constexpr static auto window_size = ImVec2(360.0F, 480.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);
    
    DrawContent();
    
    ImGui::End();
}


void WindowClass::DrawContent(){
    
    if(!task_name.empty())
    {   
        //--------------------------------
        //add task button
        ImGui::Columns(2, "col", false);
        ImGui::SetColumnOffset(1, 38.0F);
        for(size_t i = 0; i < task_name.size()+1; i++){
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Text("| ");
            ImGui::PopStyleColor();
        }
        if(ImGui::Button("+", ImVec2(20.0F, 25.0F))){
        }
        //--------------------------------------------
        //resize if not sizo of vectors same
        if (task_is_done.size() != task_name.size()) {
           task_is_done.resize(task_name.size(), "0\n");
        }

        if (task_comment.size() != task_name.size()) {
            task_comment.resize(task_name.size(), "");
        }
        //-------------------------------------------------
        //draw task
        ImGui::NextColumn();
        for(size_t i = 0; i < task_name.size(); i++)
        {
            std::string label = "###" + std::to_string(i+1);  
            
            bool checked;
            auto c = task_is_done[i].c_str(); //auto  gives const char pointer array

            if(*c == '0')
                checked = false;
            else
                checked = true;
            
            if (ImGui::Checkbox(label.data(), &checked)) {
                //save the data to text file
                if(checked)
                    task_is_done[i] = '1';
                else{
                    task_is_done[i] = '0';
                }
                SaveContent(&task_is_done, program_check_data);
            }
            ImGui::SameLine();

            const auto check = (selectedTask == i);

            std::string selectableLabel = task_name[i];
            
            if(ImGui::Selectable(selectableLabel.data(), check, 0, ImVec2(100.0F, 0.0F))){
                selectedTask = i;
                editTask();
                std::cout<<selectedTask<<'\n';
            }
        }
    }else{
        //if task is empty
        if(ImGui::Button(" +")){
            addTask();
        }
    }
    
}
//-------------------------------------
//load contents
void WindowClass::loadContent(std::vector<std::string> *content, const std::string path)
{

    auto in = std::ifstream(path.data());

    if(in.is_open())
    {
        std::string line;
        
        while(std::getline(in, line))
        {
            content->push_back(line);
            std::cout << line << std::endl;
        } 
        in.close();
    }
}
//----------------------------------------
//save contents
void WindowClass::SaveContent(std::vector<std::string> *content, std::string path){
    
    std::cout<<"saveContent called\n";

    std::string textbuffer;

    std::string line;
    for(size_t i = 0;  i < content->size(); i++){
        line = (*content)[i];
        textbuffer = textbuffer + line + '\n';
    }
    std::cout<<textbuffer<<std::endl;
    std::ofstream out{path.data()};
    out << textbuffer;
}
//------------------------------------
//buttons
void WindowClass::addTask(){

}
void WindowClass::editTask(){

}
void WindowClass::deleteTask(){

}
void WindowClass::loadFont(){
    ImGuiIO& io = ImGui::GetIO();

    // Get Windows font directory
    char fontPath[MAX_PATH];
    SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, fontPath);
    strcat_s(fontPath, "\\arial.ttf");  // Use Arial as the default font

    // Load the font
    io.Fonts->AddFontFromFileTTF(fontPath, 18.0f);
} 

void render(WindowClass &window_obj)
{
    window_obj.Draw("###Label");
}
