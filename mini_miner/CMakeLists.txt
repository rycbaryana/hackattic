
find_package(OpenSSL REQUIRED)

add_executable(mini_miner main.cpp)

target_link_libraries(mini_miner PRIVATE OpenSSL::Crypto tools)
target_include_directories(mini_miner PRIVATE ../tools)
