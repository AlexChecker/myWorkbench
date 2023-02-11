#include <iostream>
#include "SQL/SQLHelper.h"
#include <vector>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad.h>
#include <glfw3.h>

using namespace std;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800,600,"SQL Workbench Legacy by AlexChecker",NULL,NULL);
    if(window==NULL)
    {
        std::cout<<"Error while creating window!"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<<"Error while initializing GLAD"<<std::endl;
        glfwTerminate();
        return -1;
    }


    //ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGuiWindowFlags standart=0;
    standart |= ImGuiWindowFlags_NoMove;
    standart |= ImGuiWindowFlags_NoCollapse;
    standart |= ImGuiWindowFlags_NoResize;
    int width =0;
    int height =0;
    while(!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window,&width,&height);
        glClearColor(.5,.5,.5,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Database explorer",nullptr,standart);
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(width/4.5,height/3*2));
        ImGui::Text("Window width: %f",ImGui::GetWindowWidth());
        ImGui::Text("Screen size width: %i", width);
        ImGui::End();

        ImGui::Begin("Log",nullptr,standart);
        ImGui::SetWindowSize(ImVec2(width,height/3));
        ImGui::SetWindowPos(ImVec2(0,height/3*2));
        ImGui::Text("Window width: %f",ImGui::GetWindowWidth());
        ImGui::Text("Window height: %f",ImGui::GetWindowHeight());
        ImGui::Text("Screen size width: %i", width);
        ImGui::Text("Screen size height: %i", height);
        ImGui::Text("Window pos: %f | %f",ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
        ImGui::End();

        ImGui::Begin("Smth",nullptr,standart);
        ImGui::SetWindowSize(ImVec2(width/1.6,height/3*2));
        ImGui::SetWindowPos(ImVec2(width/4.5,0));
        ImGui::Text("Window width: %f",ImGui::GetWindowWidth());
        ImGui::Text("Window height: %f",ImGui::GetWindowHeight());
        ImGui::Text("Screen size width: %i", width);
        ImGui::Text("Screen size height: %i", height);
        ImGui::Text("Window pos: %f | %f",ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
        ImGui::End();

        ImGui::Begin("Not used now",nullptr,standart);
        ImGui::SetWindowSize(ImVec2(width - ((width/4.5)+(width/1.6)),height/3*2));
        ImGui::SetWindowPos(ImVec2((width/4.5)+(width/1.6),0));
        ImGui::Text("Window width: %f",ImGui::GetWindowWidth());
        ImGui::Text("Window height: %f",ImGui::GetWindowHeight());
        ImGui::Text("Screen size width: %i", width);
        ImGui::Text("Screen size height: %i", height);
        ImGui::Text("Window pos: %f | %f",ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
        ImGui::End();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
    return 0;
}
