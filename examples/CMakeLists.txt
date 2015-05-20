find_package(Boost COMPONENTS chrono regex system thread date_time REQUIRED)
find_package(MongoDB REQUIRED)
if(MSVC)
	add_definitions(-DLIBMONGOCLIENT_NO_AUTOLIB -DBOOST_ALL_DYN_LINK)
endif(MSVC)
include_directories("${PROJECT_BINARY_DIR}/src/")
include_directories(${MongoDB_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR} ${SRC_PATH})
add_executable(ex01 ex01.cpp)
#add_executable(exampleAuth example_auth.cpp)

target_link_libraries(ex01 mongo-cxx-pool )
#target_link_libraries(exampleAuth shared)
