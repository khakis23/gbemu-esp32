if(ESP_PLATFORM)
  ### CHANGE ROM NAME HERE FOR ESP (for embedding) ###
  set(SELECTED_ROM "tetris.gb")

  configure_file(
    "${COMPONENT_DIR}/../roms/${SELECTED_ROM}"
    "${CMAKE_CURRENT_BINARY_DIR}/rom.gb"
    COPYONLY
  )

  idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES gbemu
    EMBED_FILES "${CMAKE_CURRENT_BINARY_DIR}/rom.gb"
  )
else()
  add_executable(gbemu main.c)

  target_link_libraries(gbemu PRIVATE emu)
  target_include_directories(gbemu PRIVATE "${CMAKE_SOURCE_DIR}/gbemu/include")

  if(NOT BUILD_FOR_ESP32)
    if(NOT WIN32)
      target_link_libraries(gbemu PRIVATE ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    endif()

    add_custom_command(
      TARGET gbemu POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/NotoSansMono-Medium.ttf"
        "$<TARGET_FILE_DIR:gbemu>"
      VERBATIM
    )
  endif()

  install(TARGETS gbemu
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
endif()
