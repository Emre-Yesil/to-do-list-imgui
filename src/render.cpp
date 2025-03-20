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

    static char log[25];

    ImGui::SetNextWindowPos(ImVec2(10.0F, 10.0F));
    ImGui::SetNextWindowSize(popUpSize);

    if(ImGui::BeginPopupModal("###add_popup", nullptr, popupFlags)){

        ImGui::Text("Task name :");
        ImGui::SameLine();
        ImGui::InputText("###add_Task_name", log, sizeof(log));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
        if(ImGui::Button("Save")){
            show_modal_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();
    
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        if(ImGui::Button("Cancel")){
            show_modal_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();

        ImGui::EndPopup();
    }  
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
