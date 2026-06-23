cmake_minimum_required(VERSION 4.2)
project(F0)

set(CMAKE_CXX_STANDARD 14)
set(BOOST_ROOT "Users/vadimkhairullin/Desktop/boost_1_91_0")
find_package(Boost REQUIRED) # Ищем Boost
include_directories(${Boost_INCLUDE_DIRS}) # Добавляем пути для заголовочных файлов
include_directories(.)

add_executable(F0
        Equal.h
        Hash.h
        HashTable.h
        main.cpp
        Vector.h
        Slot.h
        Graph.h
        Queue.h
        Tests.cpp
        Social.h
        Social.cpp
)
target_link_libraries(F0 ${Boost_LIBRARIES})