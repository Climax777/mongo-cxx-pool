find_package(Boost COMPONENTS system thread unit_test_framework REQUIRED)
find_package(MongoDB REQUIRED)

configure_file(
    "${SRC_PATH}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

include_directories("${PROJECT_BINARY_DIR}")

set(SOURCES
    MongoConnection.cpp
    MongoConnection.h
    MongoConnectionPool.cpp
    MongoConnectionPool.h
    MongoHostConnectionPool.cpp
    MongoHostConnectionPool.h
    )

include_directories(${MongoDB_INCLUDE_DIR}
     ${Boost_INCLUDE_DIR})
add_library(mongo-cxx-pool SHARED ${SOURCES})
target_link_libraries(mongo-cxx-pool ${MongoDB_LIBRARIES} ${Boost_LIBRARIES})


#add_executable(example example.cpp)
#add_executable(exampleAuth example_auth.cpp)

#target_link_libraries(example shared)
#target_link_libraries(exampleAuth shared)
