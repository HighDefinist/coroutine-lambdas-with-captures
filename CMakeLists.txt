cmake_minimum_required (VERSION 3.16)
project("coroutine_lambdas_with_captures")

if(MSVC)
   string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive- /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -stdlib=libc++ -fcoroutines-ts")
else()
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Wold-style-cast -Wcast-align -Woverloaded-virtual -Wconversion -Wlogical-op -Wuseless-cast -Wdouble-promotion -Wformat=2")
   if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.6)
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wduplicated-branches -Wnull-dereference")
   endif()
endif() 

# Setup as header-only library for parent cmake project
set (CMAKE_CXX_STANDARD 20)
if(WITH_CLANG_TIDY) 
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-llvmlibc*,-cppcoreguidelines-pro-type-vararg,-hicpp-vararg,-modernize-use-trailing-return-type")
endif()
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
include_directories(${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(src) 
     