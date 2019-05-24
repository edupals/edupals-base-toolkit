
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-std=c++1z )

set(stable log.cpp cmd.cpp system.cpp)
set(unstable network.cpp process.cpp filesystem.cpp dfa.cpp token.cpp lexer.cpp variant.cpp json.cpp)
set(source ${stable} ${unstable})

add_library(edupals-base SHARED ${source})
target_link_libraries(edupals-base stdc++fs pthread)
set_target_properties(edupals-base PROPERTIES SOVERSION 1)

install(TARGETS edupals-base
    LIBRARY DESTINATION "lib"
)

install(FILES "${PROJECT_SOURCE_DIR}/EdupalsBaseConfig.cmake"
    DESTINATION "lib/cmake/EdupalsBase"
)

#testing application
add_executable(testing testing.cpp)
target_link_libraries(testing edupals-base)





