cmake_minimum_required (VERSION 3.12)

project (rabbitMQ-sender)

set(rabbitmq-c_DIR "/usr/local/lib/x86_64-linux-gnu/cmake/rabbitmq-c/")
find_package(rabbitmq-c REQUIRED)

add_executable(sender sender.cpp)

target_link_libraries(sender ${rabbitmq-c_LIBS})
