project(fastdds)
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})


if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

# if(NOT foonathan_memory_FOUND)
#     find_package(foonathan_memory REQUIRED)
# endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

set(TYPE_SUPPORT
    dds_type_support_adapter.cpp
)

add_executable(hello_pub ${TYPE_SUPPORT} ${ROOT}/hello_pub.cpp)
target_link_libraries(hello_pub fastrtps foonathan_memory pthread roscpp_serialization)

add_executable(hello_sub ${TYPE_SUPPORT} ${ROOT}/hello_sub.cpp)
target_link_libraries(hello_sub fastrtps foonathan_memory pthread roscpp_serialization)

# data sharing

add_executable(hello_data_sharing_pub ${TYPE_SUPPORT} ${ROOT}/hello_data_sharing_pub.cpp)
target_link_libraries(hello_data_sharing_pub fastrtps foonathan_memory pthread roscpp_serialization)

add_executable(hello_data_sharing_sub ${TYPE_SUPPORT} ${ROOT}/hello_data_sharing_sub.cpp)
target_link_libraries(hello_data_sharing_sub fastrtps foonathan_memory pthread roscpp_serialization)


# shared memory

add_executable(hello_shm_pub ${TYPE_SUPPORT} ${ROOT}/hello_shm_pub.cpp)
target_link_libraries(hello_shm_pub fastrtps foonathan_memory pthread roscpp_serialization)

add_executable(hello_shm_sub ${TYPE_SUPPORT} ${ROOT}/hello_shm_sub.cpp)
target_link_libraries(hello_shm_sub fastrtps foonathan_memory pthread roscpp_serialization)

# zero copy

add_executable(hello_zero_copy_pub ${TYPE_SUPPORT} ${ROOT}/hello_zero_copy_pub.cpp)
target_link_libraries(hello_zero_copy_pub fastrtps foonathan_memory pthread roscpp_serialization)

add_executable(hello_zero_copy_sub ${TYPE_SUPPORT} ${ROOT}/hello_zero_copy_sub.cpp)
target_link_libraries(hello_zero_copy_sub fastrtps foonathan_memory pthread roscpp_serialization)
