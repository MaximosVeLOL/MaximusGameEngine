project("engine")

message("Compiling MaxEngine's engine!")



file(GLOB ENG_SRC 
	"src/*.cpp"
	"src/*.h"
)

foreach(s ${ENG_SRC})
	message(STATUS "Got source file: ${s}!")
endforeach()

# Create your game executable target as usual
add_executable(engine WIN32 main.cpp ${ENG_SRC})
#add_library(engine SHARED main.cpp  ${ENG_SRC})

# Link to the actual SDL3 library.
#target_include_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/engine/src" )
target_link_libraries(engine PRIVATE SDL3::SDL3)
target_link_libraries(engine PRIVATE SDL3_mixer::SDL3_mixer)







#target_include_directories(engine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
#target_link_libraries(MaximusGameEngine PRIVATE SDL3_mixer::SDL3_mixer)