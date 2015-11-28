find_package (LLVM REQUIRED)
find_package (Clang REQUIRED)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(reflect ${SRC_FILES})

set_target_properties(reflect PROPERTIES CXX_STANDARD 11)

target_compile_definitions(reflect PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(reflect PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/optionparser/src)
target_include_directories(reflect PRIVATE ${LLVM_INCLUDE_DIRS})

target_link_libraries(reflect PRIVATE LLVMSupport)
target_link_libraries(reflect PRIVATE clangFrontend)

