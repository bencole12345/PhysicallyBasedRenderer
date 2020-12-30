add_executable(main
        main.cpp)

target_compile_features(main PRIVATE cxx_std_17)

if (APPLE)
    target_link_libraries(main
        PRIVATE
            pbr
            glfw
            GLEW::GLEW
            glm
            "-framework OpenGL")
else()
    target_link_libraries(main
        PRIVATE
            pbr
            glfw
            GLEW::GLEW
            glm)
endif()