cmake_minimum_required(VERSION 3.26)
project(SysProg_Lab4)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        SysProg_Lab4
        src/main.cpp
        src/grammar.cpp
        src/grammar.hpp
        src/readers.cpp
        src/readers.hpp
        src/firstfollow.cpp
        src/firstfollow.hpp
        src/readers.hpp
        src/parsing.cpp
        src/parsing.hpp
        src/ast/tree.cpp
        src/ast/tree.hpp
        src/ast/tree_visualization.hpp
        src/ast/tree_visualization.cpp
)

# find and link graphviz library
# you might need to change the paths or use different linking approach
set(GRAPHVIZ_INCLUDE_DIR "/opt/homebrew/Cellar/graphviz/9.0.0/include")
set(GRAPHVIZ_LIBRARY_DIR "/opt/homebrew/Cellar/graphviz/9.0.0/lib")
target_include_directories(SysProg_Lab4 PRIVATE ${GRAPHVIZ_INCLUDE_DIR})
target_link_libraries(SysProg_Lab4 PRIVATE ${GRAPHVIZ_LIBRARY_DIR}/libgvc.dylib ${GRAPHVIZ_LIBRARY_DIR}/libcgraph.dylib ${GRAPHVIZ_LIBRARY_DIR}/libcdt.dylib ${GRAPHVIZ_LIBRARY_DIR}/libcgraph.dylib ${GRAPHVIZ_LIBRARY_DIR}/libpathplan.dylib)



