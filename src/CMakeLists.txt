find_package(Boost COMPONENTS system thread date_time REQUIRED)
find_package(MongoDB REQUIRED)

configure_file(
    "${SRC_PATH}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

include_directories("${PROJECT_BINARY_DIR}")

set(LIB_SOURCES
    MongoConnection.cpp
    MongoConnection.h
    MongoConnectionPool.cpp
    MongoConnectionPool.h
    MongoHostConnectionPool.cpp
    MongoHostConnectionPool.h
    )

include_directories(${MongoDB_INCLUDE_DIR}
     ${Boost_INCLUDE_DIR})
 add_library(mongo-cxx-pool SHARED ${LIB_SOURCES})
target_link_libraries(mongo-cxx-pool ${MongoDB_LIBRARIES} ${Boost_LIBRARIES})



