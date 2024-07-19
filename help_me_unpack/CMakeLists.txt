
add_executable(help_me_unpack main.cpp)

target_link_libraries(help_me_unpack PRIVATE tools)
target_include_directories(help_me_unpack PRIVATE ../tools)
