project(loxo)

set(CMAKE_CXX_STANDARD 17)


# file(GLOB_RECURSE my_c_list RELATIVE ${CMAKE_SOURCE_DIR} "src/*")
file(GLOB allFiles
  "include/*.h"
  "src/*.cpp"
  "test/*.cpp"
  )

# -------- Create the executable --------
add_executable(${PROJECT_NAME}.out ${allFiles})
target_include_directories(${PROJECT_NAME}.out PRIVATE "include")
target_link_libraries(${PROJECT_NAME}.out stdc++fs)

# ---------- Create the library ----------
#add_library(${PROJECT_NAME} ${allFiles})
#target_include_directories(${PROJECT_NAME} PUBLIC "include")
#target_link_libraries(${PROJECT_NAME} stdc++fs)
