# Tests CMakeLists.txt

# Simple test framework (no external dependencies)
add_executable(test_runner
    test_main.cpp
    test_types.cpp
    test_atomspace.cpp
    test_attention.cpp
    test_pattern.cpp
    test_pln.cpp
)

target_link_libraries(test_runner opencog_core)

# Add test
add_test(NAME opencog_tests COMMAND test_runner)
