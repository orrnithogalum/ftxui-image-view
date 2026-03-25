add_executable(ftxui-imageview-example main.cpp)
target_link_libraries(ftxui-imageview-example PRIVATE ftxui-image-view)

target_compile_options(ftxui-imageview-example PRIVATE
    $<$<CONFIG:Release>:-O3>
)
