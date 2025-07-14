# Examples CMakeLists.txt

add_executable(simple_client simple_client.c)
add_executable(window_example window_example.c)

# Install examples
install(TARGETS simple_client window_example DESTINATION bin)
