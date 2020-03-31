
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-std=c++1z )

set(libname edupals-base)

set(stable log.cpp cmd.cpp system.cpp variant.cpp process.cpp filesystem.cpp dfa.cpp token.cpp lexer.cpp json.cpp bson.cpp base64.cpp user.cpp group.cpp)
set(unstable network.cpp )
set(source ${stable} ${unstable})

add_library(${libname} SHARED ${source})
target_link_libraries(${libname} stdc++fs pthread)
set_target_properties(${libname} PROPERTIES SOVERSION 2 VERSION "2.0.0")

install(TARGETS ${libname}
    LIBRARY DESTINATION "lib"
)

install(FILES "${PROJECT_SOURCE_DIR}/EdupalsBaseConfig.cmake"
    DESTINATION "lib/cmake/EdupalsBase"
)

#testing application
add_executable(testing testing.cpp)
target_link_libraries(testing ${libname})





