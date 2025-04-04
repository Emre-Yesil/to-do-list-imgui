#include <iostream>
#include <cstdint>
#include <filesystem>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui.h>
#include <implot.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "render.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

constexpr auto WINDOW_WIDTH = std::uint32_t{360};
constexpr auto WINDOW_HEIGHT = std::uint32_t{480};

namespace fs = std::filesystem;

static void glfw_error_callback(int error, const char *description);

void start_cycle()
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}

void end_cycle(GLFWwindow *const window)
{
    const auto clear_color =
        ImVec4(30.0F / 255.0F, 30.0F / 255.0F, 30.0F / 255.0F, 1.00f);
    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

int main(int, char **)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return 1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics context
    auto *window = glfwCreateWindow(static_cast<std::int32_t>(WINDOW_WIDTH),
                                    static_cast<std::int32_t>(WINDOW_HEIGHT),
                                    "To Do List",
                                    nullptr,
                                    nullptr);
    if (window == nullptr)
    {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImPlot::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    //set theme I love this kind of stuff while building gui 
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 6.0f;
    style.WindowRounding = 8.0f;
    style.ScrollbarRounding = 5.0f;
    style.ItemSpacing = ImVec2(8, 6);
    style.WindowPadding = ImVec2(15, 15);
    
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Beyaz metin
    //colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.14f, 0.17f, 1.0f); // Koyu gri pencere
    //colors[ImGuiCol_Button] = ImVec4(1.0f, 0.1f, 0.1f, 1.0f); // Mavi buton
    //colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.4f, 1.0f, 1.0f);
    //colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.2f, 0.6f, 1.0f);
    //colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.16f, 0.18f, 1.0f); // Giriş kutuları arka planı
    //colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.25f, 1.0f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    

    WindowClass window_obj;
    window_obj.loadContent(&(window_obj.task_name) ,(window_obj.program_name_data));
    window_obj.loadContent(&(window_obj.task_is_done) ,(window_obj.program_check_data));
    window_obj.loadContent(&(window_obj.task_comment) ,(window_obj.program_comment_data));
    //window_obj.loadContent(&(window_obj.task_is_done) ,(window_obj.programe_check_data));
    window_obj.loadFont();

    while (!glfwWindowShouldClose(window))
    {
        start_cycle();
        ImGui::NewFrame();
        render(window_obj);
        ImGui::Render();

        end_cycle(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

