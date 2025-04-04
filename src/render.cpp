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

            std::string selectableLabel = task_name[i] +  "##" +  std::to_string(i);
            
            if(ImGui::Selectable(selectableLabel.data(), check, selectable_flags, ImVec2(300.0F, 0.0F))){
                selectedTask = i;
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                    show_modal_popup = true;
                    ImGui::OpenPopup("###edit_popup");
                }
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

        selectedTask = -1;

        SaveContent(&task_name, program_name_data);
        SaveContent(&task_comment, program_comment_data);
        SaveContent(&task_is_done, program_check_data);

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
        } 
        in.close();
    }
}
//----------------------------------------
//save contents
void WindowClass::SaveContent(std::vector<std::string> *content, std::string path){
    
    std::string textbuffer;

    std::string line;
    for(size_t i = 0;  i < content->size(); i++){
        line = (*content)[i];
        textbuffer = textbuffer + line + "\n"; /*'\n'*/
    }
    std::ofstream out{path.data()};
    out << textbuffer;
}
//------------------------------------
//buttons
void WindowClass::addTask(){

    static char nameLog[30];
    static char commentLog[512];
    
    ImGui::SetNextWindowPos(ImVec2(10.0F, 10.0F));
    ImGui::SetNextWindowSize(popUpSize);

    if(ImGui::BeginPopupModal("###add_popup", nullptr, popupFlags)){

        ImGui::Text("Task name :");
        ImGui::SameLine();
        ImGui::InputText("###add_Task_name", nameLog, sizeof(nameLog));
         
        ImGui::Separator();

        ImGui::Text("Comment :");

        ImGui::InputTextMultiline("###add_comment", commentLog, sizeof(commentLog), ImVec2(340.0F, 300.0F) ,input_flags);

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height );
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.466f, 0.866f, 0.462f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.741f, 0.90f, 0.741f, 1.0f));

        if(ImGui::Button("Save"))
        {
            show_modal_popup = false;

            if(nameLog[0] != '\0'){
                task_name.push_back(std::string(nameLog));
                SaveContent(&task_name, program_name_data);
            }else{
                task_name.push_back(" ");
                SaveContent(&task_name, program_name_data);
            }
            
            if(commentLog[0] != '\0'){
                std::string buffer(commentLog);
                std::replace(buffer.begin(), buffer.end(), '\n', '%');
                std::strcpy(commentLog, buffer.c_str());
                task_comment.push_back(std::string(commentLog)); 
                SaveContent(&task_comment, program_comment_data);
            }else{
                task_comment.push_back(" "); 
                SaveContent(&task_comment, program_comment_data);
            }
    
            task_is_done.push_back("0");

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
        ImGui::Separator();
        
        ImGui::PopStyleColor(2);

        ImGui::EndPopup();
        
    }  
}
void WindowClass::editTask(){

    static char nameLog[30];
    static char commentLog[512];

    static std::string bufferNameLog;
    static std::string bufferCommentLog;

    static bool initialize = false;

    ImGui::SetNextWindowPos(ImVec2(10.0F, 10.0F));
    ImGui::SetNextWindowSize(popUpSize);

    if(ImGui::BeginPopupModal("###edit_popup", nullptr, popupFlags))
    {
        if(!initialize){
            bufferNameLog = task_name[selectedTask];
            strcpy(nameLog, bufferNameLog.c_str());

            bufferCommentLog = task_comment[selectedTask];
            std::replace(bufferCommentLog.begin(), bufferCommentLog.end(), '%', '\n');
            strcpy(commentLog, bufferCommentLog.c_str());

            initialize = true;
        }
        
        ImGui::Text("Task name :");
        ImGui::SameLine();
        if(ImGui::InputText("##namelog", nameLog, sizeof(nameLog))){
            bufferNameLog = std::string(nameLog);
        }

        ImGui::Separator();

        ImGui::Text("Task comment :");
        if(ImGui::InputTextMultiline("##commentlog", commentLog, sizeof(commentLog), ImVec2(340.0F, 300.0F), input_flags)){ 
            bufferCommentLog = std::string(commentLog);
        }

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - lower_section_height );
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.466f, 0.866f, 0.462f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.741f, 0.90f, 0.741f, 1.0f));

        if(ImGui::Button("Save")){
            show_modal_popup = false;

            if(!bufferNameLog.empty()){
                task_name[selectedTask] = bufferNameLog;
            }else{
                task_name[selectedTask] = ' ';
            }
            SaveContent(&task_name, program_name_data);
            
            if(!bufferCommentLog.empty()){
                std::replace(bufferCommentLog.begin(), bufferCommentLog.end(), '\n', '%');
                task_comment[selectedTask] = bufferCommentLog;
            }else{
                task_comment[selectedTask] = ' ';
            }
            SaveContent(&task_comment, program_comment_data);
        
            memset(nameLog, 0, sizeof(nameLog));
            memset(commentLog, 0, sizeof(commentLog));

          
            bufferNameLog.clear();
            bufferCommentLog.clear();

            initialize = false;

            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(2);
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.412f, 0.384f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.713f, 0.70f, 1.0f));

        if(ImGui::Button("Cancel")){

            show_modal_popup = false;

            memset(nameLog, 0, sizeof(nameLog));
            memset(commentLog, 0, sizeof(commentLog));

            bufferNameLog.clear();
            bufferCommentLog.clear();

            initialize = false;

            ImGui::CloseCurrentPopup();

        }

        ImGui::PopStyleColor(2);

        ImGui::EndPopup();
    }
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
