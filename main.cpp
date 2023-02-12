#include <iostream>
#include "SQL/SQLHelper.h"
#include <vector>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

vector<string> updateDatabaseList(SQLHelper *sql);

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

struct Table
{
public:
    string name;
    vector<string> columns;
};

enum TableTypes
{
    TABLE_INT=5,
    TABLE_DATE_TIME=17,
    TABLE_STRING=13,

};

struct View_Table
{
public:
    vector<string> columns;
    vector<vector<string>> rows;
};

View_Table setupViewTable(string tableName,SQLHelper *sql);
vector<Table> updateTableList(SQLHelper *sql);

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
    ImFont* font = io.Fonts->AddFontDefault();
    ImFontConfig cfg;
    cfg.MergeMode=true;

    io.Fonts->AddFontFromFileTTF("./res/cyrillic.ttf", 8,&cfg,io.Fonts->GetGlyphRangesCyrillic());
    io.Fonts->Build();


    ImGuiWindowFlags standart=0;
    standart |= ImGuiWindowFlags_NoMove;
    standart |= ImGuiWindowFlags_NoCollapse;
    standart |= ImGuiWindowFlags_NoResize;
    int width =0;
    int height =0;
    SQLHelper sql= SQLHelper("tcp://192.168.1.10","alex","12589632547Qq.");
    string selected_db="";
    string old_selected_db="";
    vector<string> databases = updateDatabaseList(&sql);
    vector<Table> tables;
    GLuint reload_texture;
    int reload_texture_w;
    int reload_texture_h;
    LoadTextureFromFile("./img/reload.png",&reload_texture,&reload_texture_w,&reload_texture_h);
    bool viewTable=false;
    string viewTableName="";
    string oldViewTableName="";
    ImGuiTableFlags tableView=0;
    tableView |= ImGuiTableFlags_Borders;
    tableView |= ImGuiTableFlags_RowBg;
    tableView |= ImGuiTableFlags_ScrollX;
    tableView |= ImGuiTableFlags_ScrollY;

    vector<string> cols;
    View_Table view;
    while(!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window,&width,&height);
        glClearColor(.5,.5,.5,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Database explorer",nullptr,standart);

        if(ImGui::ImageButton((void*)(std::intptr_t)reload_texture,ImVec2(reload_texture_w/3,reload_texture_h/3)))
        {
            updateDatabaseList(&sql);
        }
        ImGui::SetWindowPos(ImVec2(0,0));
        ImGui::SetWindowSize(ImVec2(width/4.5,height/3*2));
        ImGui::BeginTabBar("tab_id",0);
        if(ImGui::BeginTabItem("Databases"))
        {
            //ImGui::Text("Here goes databases");
            for(string db : databases)
            {
                if(ImGui::Button(db.c_str()))
                {
                    selected_db=db;
                    sql.selectDB(db);
                    //setupViewTable("appeal",&sql);
                }
            }
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Tables"))
        {
            ImGui::Text("Selected database %s", selected_db.c_str() );
            if(selected_db!="")
            {
                if(selected_db!=old_selected_db)
                {
                    tables= updateTableList(&sql);
                    old_selected_db=selected_db;
                }
                //if(viewTable)
                //ImGui::TextColored(ImVec4(.0,1.0,.0,1.0),"true");
                //else ImGui::TextColored(ImVec4(1.0,.0,.0,1.0),"false");
                for(auto t : tables)
                {
                    if(ImGui::TreeNode(t.name.c_str()))
                    {
                        for(auto c : t.columns)
                        {
                            ImGui::Text(c.c_str());
                        }
                        ImGui::TreePop();
                    }
                    ImGui::SameLine();
                    if(ImGui::SmallButton(("View "+t.name).c_str()))
                    {
                        viewTable=true;
                        viewTableName=t.name;
                        cols.clear();
                    }
                }
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();

        ImGui::End();

        if(viewTable) {

            ImGui::Begin("View Table",&viewTable);
            ImGui::SetWindowSize(ImVec2(500,300));
            if(viewTableName!=oldViewTableName)
            {
                view = setupViewTable(viewTableName,&sql);
                oldViewTableName=viewTableName;
            }
            if(ImGui::BeginTable("table_1",view.columns.size(),tableView))
            {
                for(int i=0;i<view.columns.size();i++)
                {
                    ImGui::TableSetupColumn(view.columns.at(i).c_str(),ImGuiTableColumnFlags_None);
                }
                ImGui::TableHeadersRow();
                //cout<<view.rows.at(0).size()<<" | "<<view.columns.size()<<endl;
                for(int y=0;y<view.rows.size();y++)
                {
                    ImGui::TableNextRow();
                    for(int x=0;x<view.columns.size();x++)
                    {
                        ImGui::TableSetColumnIndex(x);
                        ImGui::Text(view.rows.at(y).at(x).c_str());
                    }
                }


                    ImGui::EndTable();
            }
            ImGui::End();
        }

        ImGui::Begin("Log",nullptr,standart);
        ImGui::SetWindowSize(ImVec2(width,height/3));
        ImGui::SetWindowPos(ImVec2(0,height/3*2));
        for(auto item : sql._log)
        {
            ImGui::Text(item.c_str());
        }
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
        //ImGui::ShowDemoWindow();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
    return 0;
}

vector<string> updateDatabaseList(SQLHelper *sql)
{
    vector<string> result ;
    if(sql->statement!=nullptr)
    {
        auto res = sql->fetchDatabases();
        while(res->next())
        {
            result.push_back(res->getString(1));
        }
    }
    return result;
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == nullptr)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}


vector<Table> updateTableList(SQLHelper *sql)
{
    vector<Table> result;
    auto tables = sql->fetchTables();
    while(tables->next())
    {
        Table tab;
        tab.name=tables->getString(1);
        auto columns = sql->fetchColumns(tab.name);
        while(columns->next())
        {
            tab.columns.push_back(columns->getString(1));
        }
        result.push_back(tab);
    }

    return result;
}


View_Table setupViewTable(string tableName,SQLHelper *sql)
{
    View_Table result;

    auto cols = sql->fetchColumns(tableName);
    while(cols->next())
    {
        result.columns.push_back(cols->getString(1).c_str());
    }
    auto contents=sql->Query("select * from "+tableName);
    DataType types();
    vector<int> tps;

    for(int i=0;i<result.columns.size();i++)
    {
        //cout<<"column type"<<contents->getMetaData()->getColumnType(i+1)<<endl;
        tps.push_back(contents->getMetaData()->getColumnType(i+1));
    }
    while(contents->next())
    {
        vector<string> row;
        for(int i=0;i<result.columns.size();i++)
        {
            switch (tps.at(i)) {
                case DataType::INTEGER:
                    row.push_back(to_string(contents->getInt(i+1)));
                    break;
                case DataType::VARCHAR:
                    row.push_back(contents->getString(i+1));
                    break;
                case DataType::TIMESTAMP:
                    row.push_back(to_string(contents->getInt(i+1)));
                    break;
                case DataType::UNKNOWN:
                    row.push_back(to_string(contents->getInt64(i+1)));
                    break;
                case DataType::DATE:
                    row.push_back(to_string(contents->getInt(i+1)));
                    break;
            }
        }

        result.rows.push_back(row);
    }

    return result;
}