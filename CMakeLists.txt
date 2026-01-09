cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(WebSocketServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/websocketpp)

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
    src/server_main.cpp
    src/ChatServer.cpp
    src/DataManager.cpp
)

set(HEADERS
    include/ChatServer.hpp
    include/config.hpp
    include/DataManager.hpp
)

# --- СЕРВЕР ---
add_executable(my_server ${SOURCES} ${HEADERS})

target_include_directories(my_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(my_server PRIVATE 
    nlohmann_json::nlohmann_json
    SQLite::SQLite3
    ${Boost_LIBRARIES} 
    pthread
)

# --- КЛИЕНТ ---
add_executable(my_client 
    src/ChatClient.cpp
    src/client_main.cpp
    include/ChatClient.hpp
    include/config.hpp
)

target_include_directories(my_client PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(my_client PRIVATE 
    nlohmann_json::nlohmann_json
    ${Boost_LIBRARIES} 
    pthread
)