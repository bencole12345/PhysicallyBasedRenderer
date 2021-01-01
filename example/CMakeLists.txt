add_executable(CubesWithSkybox CubesWithSkybox.cpp)
target_link_libraries(CubesWithSkybox PRIVATE pbr)

add_executable(ObjectLoading ObjectLoading.cpp)
target_link_libraries(ObjectLoading PRIVATE pbr)
