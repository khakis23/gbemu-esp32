if(ESP_PLATFORM)
  file(GLOB_RECURSE SOURCES "lib/*.c")
  idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS "include"
  )
else()
  file(GLOB SOURCES CONFIGURE_DEPENDS "lib/*.c")
  file(GLOB HEADERS CONFIGURE_DEPENDS "include/*.h")

  add_library(emu STATIC ${SOURCES} ${HEADERS})

  target_include_directories(emu PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )

  if(NOT BUILD_FOR_ESP32)
    if(WIN32)
      target_include_directories(emu PUBLIC "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/include")
      target_include_directories(emu PUBLIC "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/include")
    else()
      target_include_directories(emu PUBLIC ${SDL2_INCLUDE_DIR})
      target_link_libraries(emu PUBLIC ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
    endif()
  endif()
endif()
