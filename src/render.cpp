#include <iostream>

//#include <fmt/format.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <imgui_impl_opengl3.h>
//#include <implot.h>

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <algorithm>
//#include <sstream>
//#include <typeinfo>
#include <GLFW/glfw3.h>

#include <windows.h>
#include <shlobj.h>

#include "render.hpp"


void WindowClass::Draw(std::string_view label)
{
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
        for(size_t i = 0; i < task_name.size(); i++){
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::SetWindowFontScale(1.35f);
            ImGui::Text(" |");
            ImGui::PopStyleColor();
        }
        ImGui::SetWindowFontScale(1.0f);
        if(ImGui::Button("+", ImVec2(20.0F, 25.0F))){
            show_modal_popup = true;
            ImGui::OpenPopup("###add_popup");
        }
        if (show_modal_popup) {
            addTask();
        }
        //-------------------------------------------- move it another place

        //resize if not sizo of vectors same
        if (task_is_done.size() != task_name.size()) {
           task_is_done.resize(task_name.size(), "0");
        }

        if (task_comment.size() != task_name.size()) {
            task_comment.resize(task_name.size(), "\n");
        }
        //-------------------------------------------------
        //draw task
        ImGui::NextColumn();
        size_t i;
        for(i = 0; i < task_name.size(); i++)
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
                show_modal_popup = true;
                ImGui::OpenPopup("###edit_popup");
                std::cout<<selectedTask<<'\n';
            }
        }
        if(show_modal_popup)
            editTask();
        
    }else{
        if(ImGui::Button("+", ImVec2(20.0F, 25.0F))){
            show_modal_popup = true;
            ImGui::OpenPopup("###add_popup");
        }
        if (show_modal_popup) {
            addTask();
        }
    } 
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 80 ,ImGui::GetWindowHeight() - 80));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.412f, 0.384f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.713f, 0.70f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.412f, 0.384f, 1.0f));

    if(ImGui::Button("Delete", ImVec2(60.0F, 60.0F)) && task_name.size() != 0 && selectedTask != -1){

        task_name.erase(task_name.begin() + selectedTask);
        task_comment.erase(task_comment.begin() + selectedTask);
        task_is_done.erase(task_is_done.begin() + selectedTask);

        SaveContent(&task_name, program_name_data);
        SaveContent(&task_comment, program_comment_data);
        SaveContent(&task_is_done, program_check_data);

        for(size_t i = 0; i < task_name.size(); i++){
            std::cout<<task_name[i]<<std::endl;
        }
    }
    ImGui::PopStyleColor(3);
}
//-------------------------------------
//load contents
void WindowClass::loadContent(std::vector<std::string> *content, const std::string path)
{

    auto in = std::ifstream(path.data());

    if(in.is_open())
    {
        std::string line;

        while(std::getline(in, line)) // while(std::getline(std::cin, line, '$')) /*while(std::getline(in, line)) */
        {
            //line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
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
        textbuffer = textbuffer + line + "\n"; /*'\n'*/
    }
    std::cout<<textbuffer<<std::endl;
    std::ofstream out{path.data()};
    out << textbuffer;
}
//------------------------------------
//buttons
void WindowClass::addTask(){

    static char nameLog[25];
    static char commentLog[512];
    
    ImGui::SetNextWindowPos(ImVec2(10.0F, 10.0F));
    ImGui::SetNextWindowSize(popUpSize);

    if(ImGui::BeginPopupModal("###add_popup", nullptr, popupFlags)){

        ImGui::Text("Task name :");
        ImGui::SameLine();
        ImGui::InputText("###add_Task_name", nameLog, sizeof(nameLog));
         
        ImGui::Separator();

        ImGui::Text("Comment :");
        ImGui::SameLine();
        ImGui::InputTextMultiline("###add_comment", commentLog, sizeof(commentLog));

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height );
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.466f, 0.866f, 0.462f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.741f, 0.90f, 0.741f, 1.0f));

        if(ImGui::Button("Save"))
        {
            show_modal_popup = false;

            task_name.push_back(std::string(nameLog));
            std::string buffer(commentLog);

            std::replace(buffer.begin(), buffer.end(), '\n', '%');
            std::strcpy(commentLog, buffer.c_str());
            //it should be just a buffer , extra step
            task_comment.push_back(std::string(commentLog)); 
            task_is_done.push_back("0");

            SaveContent(&task_name, program_name_data);
            SaveContent(&task_comment, program_comment_data);

            memset(nameLog, 0, sizeof(nameLog));
            memset(commentLog, 0, sizeof(commentLog));

            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(2);
    
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.412f, 0.384f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.713f, 0.70f, 1.0f));

        if(ImGui::Button("Cancel"))
        {
            show_modal_popup = false;

            memset(nameLog, 0, sizeof(nameLog));
            memset(commentLog, 0, sizeof(commentLog));

            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(2);

        ImGui::EndPopup();
        
    }  
    
}
void WindowClass::editTask(){
    static char nameLog[25];
    static char commentLog[512];

    std::cout<<"1a"<<std::endl;
    std::strcpy(nameLog, (task_name[selectedTask].c_str())); //potantial error when selected is zero
    std::strcpy(commentLog, (task_comment.at(selectedTask).c_str()));

    std::cout<<nameLog<<std::endl;
    std::cout<<commentLog<<std::endl;

    ImGui::SetNextWindowPos(ImVec2(10.0F, 10.0F));
    ImGui::SetNextWindowSize(popUpSize);

    std::cout<<"2b"<<std::endl;
    if(ImGui::BeginPopupModal("###edit_popup", nullptr, popupFlags)){

        std::cout<<"3c"<<std::endl;
        ImGui::Text("Task name :");
        ImGui::SameLine();
        std::cout<<nameLog<<std::endl;

        if(ImGui::InputText("###edit_Task_name", nameLog, sizeof(nameLog))){
            std::cout<<nameLog<<"---++"<<std::endl;
            task_name[selectedTask] = std::string(nameLog);
        }
         
        ImGui::Separator();

        ImGui::Text("Comment :");
        ImGui::SameLine();
        // it is wrong becouse of commend log is empty
        std::string buffer(commentLog); 
        std::replace(buffer.begin(), buffer.end(), '%', '\n');
        std::strcpy(commentLog, buffer.c_str());

        if(ImGui::InputTextMultiline("###edit_comment", commentLog, sizeof(commentLog))){
            std::cout<<commentLog<<"---++"<<std::endl;
            std::replace(buffer.begin(), buffer.end(), '\n', '%');
            std::strcpy(commentLog, buffer.c_str()); //it is unnessesery
            std::cout<<commentLog<<" "<<buffer<<std::endl;
            task_comment[selectedTask] = std::string(commentLog);
        }

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height );
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.466f, 0.866f, 0.462f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.741f, 0.90f, 0.741f, 1.0f));
        std::cout<<"4d"<<std::endl;

        if(ImGui::Button("Save"))
        {
            show_modal_popup = false;

            //comments
            std::string buffer(commentLog);

            std::replace(buffer.begin(), buffer.end(), '\n', '%');
            std::strcpy(commentLog, buffer.c_str()); //it is unnessesery

            //I want to just replace it its wrong

            SaveContent(&task_name, program_name_data);
            SaveContent(&task_comment, program_comment_data);

            memset(nameLog, 0, sizeof(nameLog));
            memset(commentLog, 0, sizeof(commentLog));

            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(2);
    
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.412f, 0.384f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.713f, 0.70f, 1.0f));

        if(ImGui::Button("Cancel"))
        {
            show_modal_popup = false;

            //memset(nameLog, 0, sizeof(nameLog));
            //memset(commentLog, 0, sizeof(commentLog));

            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(2);

        ImGui::EndPopup();
    }  
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
