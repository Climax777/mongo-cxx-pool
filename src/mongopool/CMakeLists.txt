find_package(Boost COMPONENTS system thread date_time REQUIRED)
find_package(MongoDB REQUIRED)

configure_file(
    "${SRC_PATH}/mongopool/config.h.in"
    "${PROJECT_BINARY_DIR}/src/mongopool/config.h"
)

include_directories("${PROJECT_BINARY_DIR}/src/")

set(LIB_SOURCES
    MongoConnection.cpp
    MongoConnectionPool.cpp
    MongoHostConnectionPool.cpp
    )
set(LIB_HEADERS
    MongoConnection.h
    MongoConnectionPool.h
    MongoHostConnectionPool.h
    )
set(LIB_FILES
    ${LIB_SOURCES}
    ${LIB_HEADERS}
    )
include_directories(${MongoDB_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR} ${SRC_PATH})
add_library(mongo-cxx-pool SHARED ${LIB_FILES})
add_library(mongo-cxx-pool-static STATIC ${LIB_FILES})
target_link_libraries(mongo-cxx-pool ${MongoDB_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(mongo-cxx-pool-static ${MongoDB_LIBRARIES} ${Boost_LIBRARIES})
if(NOT MSVC)
    set_target_properties(mongo-cxx-pool-static PROPERTIES OUTPUT_NAME mongo-cxx-pool)
endif(NOT MSVC)

set_target_properties(mongo-cxx-pool PROPERTIES SOVERSION "${MONGO_CXX_POOL_MAJOR}.${MONGO_CXX_POOL_MINOR}.${MONGO_CXX_POOL_PATCH}.${MONGO_CXX_POOL_BUILD}")

install(FILES "${PROJECT_BINARY_DIR}/src/mongopool/config.h" DESTINATION include/mongopool)
install(FILES ${LIB_HEADERS} DESTINATION include/mongopool)
install(TARGETS mongo-cxx-pool mongo-cxx-pool-static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
