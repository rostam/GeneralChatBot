cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)

project(GermanChatBot)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

file(GLOB project_SRCS src/*.cpp)
file(GLOB fasttext_SRCS src/fasttext/*.cc)

add_executable(GermanChatBot ${project_SRCS} ${fasttext_SRCS})
target_link_libraries(GermanChatBot ${wxWidgets_LIBRARIES})
#target_link_libraries(GermanChatBot ${CMAKE_SOURCE_DIR}/libfasttext.a)
target_include_directories(GermanChatBot PRIVATE ${wxWidgets_INCLUDE_DIRS})
