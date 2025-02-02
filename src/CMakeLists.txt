file(GLOB SOURCES *.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CUSTOM_LIBS
        abstract_game
        commons
        ui_elements
        views
        scene
        csv
        logger
)

set(GAME_LIBS
        calc
        reaction
        color_match
        letter_salad
        matrix
        typeracer
        remembering
        rows_of_numbers
        memory
        analogy
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${CUSTOM_LIBS}
        ${GAME_LIBS}
        ImGui-SFML::ImGui-SFML
        sfml-graphics
        box2d
)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif ()

# Commented out because openal32.dll is only required for audio stuff
if (WIN32)
     add_custom_command(
             TARGET ${CMAKE_PROJECT_NAME}
             COMMENT "Copy OpenAL DLL"
             PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
             ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Athena>
             VERBATIM)
endif ()
