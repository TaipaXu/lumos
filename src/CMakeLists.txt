find_package(Boost REQUIRED COMPONENTS algorithm program_options)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMAGIC REQUIRED libmagic)
include_directories(${LIBMAGIC_INCLUDE_DIRS})
link_directories(${LIBMAGIC_LIBRARY_DIRS})
add_definitions(${LIBMAGIC_CFLAGS_OTHER})

configure_file(./config.hpp.in ./config.hpp)

set(TARGET lumos)
set(SOURCES
    main.cpp
    core.hpp
    core.cpp

    models/result.hpp
    models/result.cpp

    analysis/analyseManager.hpp
    analysis/analyseManager.cpp
    analysis/analyzer.hpp
    analysis/analyzer.cpp
    analysis/analyzers.hpp

    threadSafeQueue.hpp
    magicWrapper.hpp
    magicWrapper.cpp
)
add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE Boost::algorithm Boost::program_options ${LIBMAGIC_LIBRARIES})
