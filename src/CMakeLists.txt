set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-rtti")

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(reflect ${SRC_FILES} ${HDR_FILES})

set_target_properties(reflect PROPERTIES CXX_STANDARD 11)

target_compile_definitions(reflect PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(reflect PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/optionparser/src)
target_include_directories(reflect PRIVATE ${LLVM_INCLUDE_DIRS})

target_link_libraries(reflect PRIVATE LLVMSupport)
target_link_libraries(reflect PRIVATE clangFrontend)
target_link_libraries(reflect PRIVATE clangAST)
target_link_libraries(reflect PRIVATE clangParse)
target_link_libraries(reflect PRIVATE clangARCMigrate)
target_link_libraries(reflect PRIVATE clangASTMatchers)
target_link_libraries(reflect PRIVATE clangCodeGen)
target_link_libraries(reflect PRIVATE clangRewriteFrontend)
