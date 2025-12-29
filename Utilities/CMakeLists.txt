project("utilities")

message("Compiling Maxutilities's utilities!")



file(GLOB ENG_SRC 
	"src/*.cpp"
	"src/*.h"
)

foreach(s ${ENG_SRC})
	message(STATUS "Got source file: ${s}!")
endforeach()

# Create your game executable target as usual
add_executable(utilities WIN32 main.cpp ${ENG_SRC})
#add_library(utilities SHARED main.cpp  ${ENG_SRC})

# Link to the actual SDL3 library.
#target_include_directories(utilities PRIVATE "${CMAKE_SOURCE_DIR}/utilities/src" )
target_link_libraries(utilities PRIVATE engine)
target_link_libraries(utilities PRIVATE SDL3::SDL3)
target_link_libraries(utilities PRIVATE SDL3_mixer::SDL3_mixer)







#target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#target_link_libraries(MaximusGameutilities PRIVATE SDL3_mixer::SDL3_mixer)