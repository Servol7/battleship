file(GLOB_RECURSE BackendSources src/*.cpp include/*.h)

add_library(backend ${BackendSources})

target_include_directories(backend PUBLIC include)
