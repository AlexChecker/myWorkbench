cmake_minimum_required(VERSION 3.23)
project(myWorkbench)


set(MYSQL_H /usr/local/mysql-connector-c++-8.0.18/include)
set(OPENSSL_H /usr/local/Cellar/openssl@1.1/1.1.1t/include)
set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.8/include/GLFW)
set(GLM_H /usr/local/Cellar/glm/0.9.9.8/include/glm)
set(GLAD_H /usr/local/include/glad)
set(KHR_H /usr/local/include/KHR)
set(IMGUI_H /usr/local/include/imgui)
include_directories(${MYSQL_H} ${OPENSSL_H} ${GLEW_H} ${GLFW_H} ${GLM_H} ${GLAD_H} ${KHR_H} ${IMGUI_H})

#set(LIBCRYPTO_LINK /usr/local/Cellar/openssl@1.1/1.1.1t/lib/libcrypto.1.1.dylib)
set(OPENSSL_LINK /usr/local/Cellar/openssl@1.1/1.1.1t/lib/libssl.dylib)
set(MYSQL_LINK /usr/local/mysql-connector-c++-8.0.18/lib64/libmysqlcppconn.7.8.0.18.dylib )
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.8/lib/libglfw.3.dylib)
link_libraries(${OPENSSL_LINK}  ${MYSQL_LINK} ${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

set(CMAKE_CXX_STANDARD 11)

add_executable(myWorkbench main.cpp glad.c SQL/SQLHelper.cpp SQL/SQLHelper.h
        ${IMGUI_H}/imgui.cpp ${IMGUI_H}/imgui_demo.cpp ${IMGUI_H}/imgui_draw.cpp ${IMGUI_H}/imgui_tables.cpp ${IMGUI_H}/imgui_widgets.cpp ${IMGUI_H}/backends/imgui_impl_glfw.cpp ${IMGUI_H}/backends/imgui_impl_opengl3.cpp stb_image.h)

if (APPLE)
    target_link_libraries(myWorkbench "-framework OpenGL")
    target_link_libraries(myWorkbench "-framework GLUT")
endif()