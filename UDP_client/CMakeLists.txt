cmake_minimum_required (VERSION 3.8)

project(cmake VERSION 0.0.1)

set(BOOST_ROOT C:/Librarys/boost_1_79_0)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(UDP_client main.cpp)
add_library(udp_lib udp.cpp udp.h)
add_library(protocol_lib Main_Protocol.cpp Main_Protocol.h)
add_executable(TEST_banch Test.cpp)

add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

set_target_properties(UDP_client TEST_banch PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(UDP_client
    udp_lib
    protocol_lib
    Threads::Threads
    ${Boost_LIBRARIES}
)

target_link_libraries(udp_lib
    ${Boost_LIBRARIES}
)

target_link_libraries(protocol_lib
    udp_lib
    ${Boost_LIBRARIES}
)

target_link_libraries(TEST_banch
    ${Boost_LIBRARIES}
    protocol_lib
    udp_lib
)

install(TARGETS UDP_client RUNTIME DESTINATION bin)

enable_testing()

add_test(boost_test TEST_banch)