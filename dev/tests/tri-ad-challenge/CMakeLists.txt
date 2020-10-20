get_filename_component(JMF_MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${JMF_APP_DIR}/tri-ad-challenge)

file(GLOB_RECURSE SRCS "**.cpp")

set(JMF_TEST_MODULE_NAME ${JMF_UNIT_TEST_PREFIX}${JMF_MODULE_NAME})
add_executable(${JMF_TEST_MODULE_NAME} ${SRCS})

set(JMF_LIB_MODULE_NAME_STATIC ${JMF_MODULE_NAME}-static)
target_link_libraries(${JMF_TEST_MODULE_NAME} 
                      PUBLIC ${JMF_LIB_MODULE_NAME_STATIC} gtest gmock)

# make sure that gtest/gmock will be compiled firstly
add_dependencies(${JMF_TEST_MODULE_NAME} gtest gmock)

install(TARGETS ${JMF_TEST_MODULE_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)
