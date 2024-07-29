set(BINARY ${CMAKE_PROJECT_NAME})

list(APPEND MY_SOURCES
        mp6502.cpp
        mp6502.h
        Bus.cpp
        Bus.h
)

add_executable(${BINARY} ${MY_SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

add_library(${BINARY}_lib STATIC ${MY_SOURCES})
