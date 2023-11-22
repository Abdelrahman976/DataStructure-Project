cmake_minimum_required(VERSION 3.26)
project(texteditor1)

set(CMAKE_CXX_STANDARD 23)

add_executable(texteditor1 main.cpp
        SuffixTree.cpp
        Suffix.cpp
        Node.cpp
        textEditor.cpp
)
