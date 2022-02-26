set(test_target unit_cyclic_iterator)

add_executable(${test_target} main.cpp)

target_compile_options(${test_target} PRIVATE 
    -std=c++11 -O2
    -Wall -Wextra -Wunreachable-code -pedantic
)

target_link_libraries(${test_target} PRIVATE
    cyclic_iterator
)

add_test(NAME ${test_target} COMMAND ${test_target})
