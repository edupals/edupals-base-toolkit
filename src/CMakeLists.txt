
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-std=c++1z )


add_library(edupals-base SHARED system.cpp network.cpp cmd.cpp log.cpp process.cpp filesystem.cpp dfa.cpp token.cpp lexer.cpp variant.cpp json.cpp)
target_link_libraries(edupals-base stdc++fs pthread)
set_target_properties(edupals-base PROPERTIES SOVERSION 1)

install(TARGETS edupals-base LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

#testing application
add_executable(testing testing.cpp)
target_link_libraries(testing edupals-base)


