find_package(Boost COMPONENTS system thread date_time regex REQUIRED)
find_package(MongoDB REQUIRED)

include_directories("${PROJECT_BINARY_DIR}/src/")
include_directories(${MongoDB_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR} ${SRC_PATH})
add_executable(ex01 ex01.cpp)
#add_executable(exampleAuth example_auth.cpp)

target_link_libraries(ex01 mongo-cxx-pool )
target_link_libraries(ex01 ${Boost_LIBRARIES} )
