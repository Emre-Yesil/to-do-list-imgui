#include <iostream>

//#include <fmt/format.h>
#include <imgui.h>
//#include <implot.h>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <typeinfo>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawContent();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height);
    

    ImGui::End();
}


void WindowClass::DrawContent(){
    
    if(!task_name.empty())
    {
        //resize it
        if (task_is_done.size() != task_name.size()) {
           task_is_done.resize(task_name.size(), "0\n");
        }
        if (task_comment.size() != task_name.size()) {
            task_comment.resize(task_name.size(), "");
        }

        for(size_t i = 0; i < task_name.size(); i++)
        {
            std::string label = task_name[i] + std::to_string(i);  
            
            bool checked;
            auto c = task_is_done[i].c_str();
            if(*c == '0'){
                checked = false;
                std::cout<<"false is called"<< c << typeid(c).name() << "\n";
            }
            else{
                checked = true;
                std::cout<<"true is called "<< c << typeid(c).name() <<"\n";
            }
                

            if (ImGui::Checkbox(label.data(), &checked)) {
                std::cout<<"draw content is called"<<std::endl;
            }
            
        }
    }
    
    if(ImGui::Button("add")){
        addTask();
    }
    ImGui::SameLine();

    if(ImGui::Button("edit")){
        editTask();
    }

    ImGui::SameLine();
    
    if(ImGui::Button("delete")){
        deleteTask();
    }
}
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

//save contents


//buttons
void WindowClass::addTask(){

}
void WindowClass::editTask(){

}
void WindowClass::deleteTask(){

}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
