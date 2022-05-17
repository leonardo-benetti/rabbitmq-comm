cmake_minimum_required (VERSION 3.12)

project (rabbitMQ-sender LANGUAGES C)

set(rabbitmq-c_DIR "/usr/local/lib/x86_64-linux-gnu/cmake/rabbitmq-c/")
find_package(rabbitmq-c REQUIRED)

add_library(examples-common OBJECT)
target_sources(examples-common PRIVATE utils.h utils.c)
target_sources(examples-common PRIVATE unix/platform_utils.c)
target_link_libraries(examples-common PRIVATE rabbitmq::rabbitmq)

add_executable(amqp_producer amqp_producer.c)
target_link_libraries(amqp_producer PRIVATE examples-common rabbitmq::rabbitmq)

add_executable(image_sender image_sender.c)
target_link_libraries(image_sender PRIVATE examples-common rabbitmq::rabbitmq)
