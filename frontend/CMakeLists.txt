file(GLOB_RECURSE FrontendSources src/*.cpp include/*.h)

add_library(frontend ${FrontendSources})

target_include_directories(frontend PUBLIC include)

target_link_libraries(frontend Qt6::Widgets)

target_link_libraries(frontend backend)