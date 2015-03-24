cmake_minimum_required(VERSION 3.1)

project(cta)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Werror -std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(tests src/test.cpp)
target_link_libraries(tests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
enable_testing()
add_test(tests tests)

install(FILES 
	include/cta.hpp
	include/differentiator.hpp
	include/evaluator.hpp
	include/functions.hpp
	include/is_constant.hpp
	include/macros.hpp
	include/operators.hpp
	include/output.hpp
	include/simplifier.hpp
	include/terms.hpp
	include/types.hpp
	DESTINATION include/cta
)
