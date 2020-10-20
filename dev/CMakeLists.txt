set(JMF_APP_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/apps)
set(JMF_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(JMF_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(JMF_UNIT_TEST_PREFIX "utest-")

add_subdirectory(apps)
#add_subdirectory(libs)
add_subdirectory(tests)