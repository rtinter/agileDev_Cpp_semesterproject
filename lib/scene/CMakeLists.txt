set(LIBRARY_NAME scene)

file(GLOB SOURCES *.cpp)

add_library(${LIBRARY_NAME} ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IMGUI_DIR}
        ${SFML_INCLUDE_DIRS}
        ${ImGui-SFML_INCLUDE_DIRS}
)

set(CUSTOM_LIBS
        commons
        ui_elements
        abstract_game
        views
        calc
        reaction
        color_match
        sequence
        remembering
        letter_salad
        aim_trainer
        analogy
        typeracer
        matrix
        rows_of_numbers
        diagram
        rows_of_symbols
        memory
        csv
        logger
        absurd_questions
)

target_link_libraries(${LIBRARY_NAME} PUBLIC
        ${CUSTOM_LIBS}
        ImGui-SFML::ImGui-SFML
        sfml-graphics
        box2d
)
