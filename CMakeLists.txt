cmake_minimum_required (VERSION 3.29)

project (tep70)

set(RRS_VERSION 1.9.0)

# No lib prefix in all shared libraries
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory (tep70)
add_subdirectory (tep70-autopilot)
#add_subdirectory (tep70bs)
#add_subdirectory (klub-display)
#add_subdirectory (msut-display)
