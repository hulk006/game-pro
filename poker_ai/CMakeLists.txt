cmake_minimum_required(VERSION 2.8)
project(poker_ai)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        tools.h
        ai.cpp
        ai.h
        card.cpp
        card.h
        daemon.h
        hand.cpp
        hand.h
        machine.cpp
        machine.h
        main.cpp
        mygame.cpp
        mygame.h
        config.h
        config.cpp tools.cpp)

add_executable(poker_ai ${SOURCE_FILES})
