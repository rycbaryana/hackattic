
find_package(OpenSSL REQUIRED)

add_executable(tales_of_ssl main.cpp)

target_link_libraries(tales_of_ssl PRIVATE OpenSSL::SSL tools)
target_include_directories(tales_of_ssl PRIVATE ../tools)
