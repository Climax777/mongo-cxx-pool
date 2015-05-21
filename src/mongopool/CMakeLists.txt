find_package(Boost COMPONENTS chrono regex system thread date_time REQUIRED)
find_package(MongoDB REQUIRED)

configure_file(
    "${SRC_PATH}/mongopool/config.h.in"
    "${PROJECT_BINARY_DIR}/src/mongopool/MongoConfig.h"
)

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
	
if(MSVC)
	add_definitions(-DLIBMONGOCLIENT_NO_AUTOLIB -DBOOST_ALL_DYN_LINK)
endif(MSVC)

include_directories(
	${PROJECT_BINARY_DIR}/src	
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
	${MongoDB_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR} ${SRC_PATH}
	)
	
add_library(mongo-cxx-pool SHARED ${LIB_FILES})
add_library(mongo-cxx-pool-static STATIC ${LIB_FILES})
generate_export_header(mongo-cxx-pool BASE_NAME libmongo_cxx_pool)
set_target_properties(mongo-cxx-pool-static PROPERTIES COMPILE_FLAGS -DLIBMONGO_CXX_POOL_STATIC_DEFINE)
target_link_libraries(mongo-cxx-pool PUBLIC ${MongoDB_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(mongo-cxx-pool-static ${MongoDB_LIBRARIES} ${Boost_LIBRARIES})

set_target_properties(mongo-cxx-pool-static PROPERTIES OUTPUT_NAME mongo-cxx-pool)
set_target_properties(mongo-cxx-pool-static PROPERTIES PREFIX "lib")
set_target_properties(mongo-cxx-pool PROPERTIES SOVERSION "${MONGO_CXX_POOL_MAJOR}.${MONGO_CXX_POOL_MINOR}.${MONGO_CXX_POOL_PATCH}.${MONGO_CXX_POOL_BUILD}")

install(FILES ${LIB_HEADERS}  ${PROJECT_BINARY_DIR}/src/mongopool/libmongo_cxx_pool_export.h DESTINATION include/mongopool)
install(FILES "${PROJECT_BINARY_DIR}/src/mongopool/MongoConfig.h" DESTINATION include/mongopool)

install(TARGETS mongo-cxx-pool mongo-cxx-pool-static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
