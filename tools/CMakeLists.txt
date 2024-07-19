include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(tools tools.cpp)

target_link_libraries(tools PUBLIC cpr::cpr nlohmann_json::nlohmann_json)

