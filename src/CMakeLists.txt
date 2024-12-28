find_package(Boost REQUIRED COMPONENTS program_options)
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
    analysis/iAnalyzer.hpp
    analysis/iAnalyzer.cpp
    analysis/analyzer.hpp
    analysis/analyzer.cpp
    analysis/multiLanguageAnalyzer.hpp
    analysis/multiLanguageAnalyzer.cpp
    analysis/analyzers.hpp

    threadSafeQueue.hpp
    magicWrapper.hpp
    magicWrapper.cpp

    printer/color.hpp
    printer/color.cpp
    printer/table.hpp
    printer/table.cpp

    utils/utils.hpp
    utils/utils.cpp
)

if(ENABLE_TESTS)
    add_library(lumosCore ${SOURCES})
endif()

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE Boost::program_options ${LIBMAGIC_LIBRARIES})
