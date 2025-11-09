set(CPACK_PACKAGE_DESCRIPTION "lumos")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Code statistics tool, convenient and fast!")

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
foreach(_component MAJOR MINOR PATCH TWEAK)
    if(DEFINED PROJECT_VERSION_${_component})
        set(CPACK_PACKAGE_VERSION_${_component} "${PROJECT_VERSION_${_component}}")
    endif()
endforeach()

set(_lumos_runtime_dest "bin")

if(WIN32)
    set(_lumos_runtime_dest ".")
    set(CPACK_GENERATOR "ZIP;WIX")
    set(_lumos_win_arch "")
    if(DEFINED VCPKG_TARGET_TRIPLET)
        string(REGEX MATCH "^[^-]+" _lumos_vcpkg_arch "${VCPKG_TARGET_TRIPLET}")
        if(_lumos_vcpkg_arch)
            set(_lumos_win_arch "${_lumos_vcpkg_arch}")
        endif()
    endif()

    if(NOT _lumos_win_arch AND CMAKE_SYSTEM_PROCESSOR)
        string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _lumos_proc_lower)
        if(_lumos_proc_lower STREQUAL "amd64")
            set(_lumos_win_arch "x64")
        elseif(_lumos_proc_lower STREQUAL "arm64")
            set(_lumos_win_arch "arm64")
        elseif(_lumos_proc_lower STREQUAL "arm")
            set(_lumos_win_arch "arm")
        elseif(_lumos_proc_lower STREQUAL "x86")
            set(_lumos_win_arch "x86")
        elseif(_lumos_proc_lower)
            set(_lumos_win_arch "${_lumos_proc_lower}")
        endif()
    endif()

    if(_lumos_win_arch)
        set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${_lumos_win_arch}")
    else()
        set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
    endif()
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "lumos")

    set(CPACK_WIX_UPGRADE_GUID "E85CEA13-DC2D-4A1B-9B54-8A9535B2E4D8")
    set(_lumos_wix_fragment "${CMAKE_CURRENT_LIST_DIR}/wix/PathFragment.wxs")
    set(_lumos_wix_patch "${CMAKE_CURRENT_LIST_DIR}/wix/PathPatch.xml")
    set(CPACK_WIX_EXTRA_SOURCES "${_lumos_wix_fragment}")
    set(CPACK_WIX_PATCH_FILE "${_lumos_wix_patch}")

    set(LIBMAGIC_MAGICFILE "" CACHE FILEPATH "Path to the libmagic database (magic.mgc)")
    if(NOT LIBMAGIC_MAGICFILE)
        find_package(PkgConfig QUIET)
        if(PKG_CONFIG_FOUND)
            pkg_get_variable(LIBMAGIC_MAGICFILE libmagic magicfile)
        endif()
    endif()

    if(NOT LIBMAGIC_MAGICFILE OR NOT EXISTS "${LIBMAGIC_MAGICFILE}")
        set(_magic_search_roots)

        if(DEFINED VCPKG_TARGET_TRIPLET)
            list(APPEND _magic_search_roots
                "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}"
                "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}"
            )
        endif()

        list(APPEND _magic_search_roots
            "${CMAKE_BINARY_DIR}/vcpkg_installed"
            "${CMAKE_SOURCE_DIR}/vcpkg_installed"
        )

        find_file(LIBMAGIC_MAGICFILE
            NAMES magic.mgc
            PATHS ${_magic_search_roots}
            PATH_SUFFIXES share/libmagic/misc libmagic/misc misc
        )

        if(NOT LIBMAGIC_MAGICFILE OR NOT EXISTS "${LIBMAGIC_MAGICFILE}")
            file(GLOB _magic_matches
                "${CMAKE_SOURCE_DIR}/vcpkg_installed/*/share/libmagic/misc/magic.mgc"
                "${CMAKE_BINARY_DIR}/vcpkg_installed/*/share/libmagic/misc/magic.mgc"
                "${CMAKE_SOURCE_DIR}/vcpkg_installed/share/libmagic/misc/magic.mgc"
                "${CMAKE_BINARY_DIR}/vcpkg_installed/share/libmagic/misc/magic.mgc"
            )
            if(_magic_matches)
                list(GET _magic_matches 0 LIBMAGIC_MAGICFILE)
            endif()
        endif()
    endif()

    if(LIBMAGIC_MAGICFILE AND EXISTS "${LIBMAGIC_MAGICFILE}")
        install(FILES "${LIBMAGIC_MAGICFILE}" DESTINATION ${_lumos_runtime_dest})
    else()
        message(WARNING "magic.mgc was not found; the Windows package will miss the libmagic database.")
    endif()

    set(_lumos_runtime_patterns
        "boost_program_options*.dll"
        "magic-*.dll"
        "tre*.dll"
    )

    set(_runtime_search_dirs)

    if(DEFINED VCPKG_TARGET_TRIPLET)
        list(APPEND _runtime_search_dirs
            "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/bin"
            "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/bin"
        )
    endif()

    foreach(_candidate_triplet IN ITEMS x64-windows x86-windows arm64-windows)
        list(APPEND _runtime_search_dirs
            "${CMAKE_BINARY_DIR}/vcpkg_installed/${_candidate_triplet}/bin"
            "${CMAKE_SOURCE_DIR}/vcpkg_installed/${_candidate_triplet}/bin"
        )
    endforeach()

    list(APPEND _runtime_search_dirs
        "${CMAKE_BINARY_DIR}/bin"
        "${CMAKE_BINARY_DIR}/Release"
        "${CMAKE_BINARY_DIR}/Debug"
        "${CMAKE_BINARY_DIR}/lib"
    )

    list(REMOVE_DUPLICATES _runtime_search_dirs)

    set(_runtime_dlls)
    set(_missing_patterns)
    foreach(_dll_pattern IN LISTS _lumos_runtime_patterns)
        set(_pattern_matches)
        foreach(_dir IN LISTS _runtime_search_dirs)
            if(IS_DIRECTORY "${_dir}")
                file(GLOB _current_matches "${_dir}/${_dll_pattern}")
                if(_current_matches)
                    list(APPEND _pattern_matches ${_current_matches})
                endif()
            endif()
        endforeach()
        if(_pattern_matches)
            list(APPEND _runtime_dlls ${_pattern_matches})
        else()
            list(APPEND _missing_patterns "${_dll_pattern}")
        endif()
    endforeach()

    if(_runtime_dlls)
        list(REMOVE_DUPLICATES _runtime_dlls)
        install(FILES ${_runtime_dlls} DESTINATION ${_lumos_runtime_dest})
    endif()

    if(_missing_patterns)
        string(JOIN ", " _missing_patterns_text ${_missing_patterns})
        message(WARNING "No runtime DLLs found for patterns: ${_missing_patterns_text}; the Windows package may miss dependencies.")
    endif()

    set(VC_RUNTIME_DLLS
        "VCRUNTIME140.dll"
        "VCRUNTIME140_1.dll"
        "MSVCP140.dll"
        "MSVCP140_1.dll"
        "MSVCP140_2.dll"
        "vccorlib140.dll"
        "concrt140.dll"
    )

    set(VC_RUNTIME_PATHS
        "$ENV{VCINSTALLDIR}Redist/MSVC/*/x64/Microsoft.VC*.CRT"
        "$ENV{VCToolsRedistDir}/x64/Microsoft.VC*.CRT"
        "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Redist/MSVC/*/x64/Microsoft.VC*.CRT"
        "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Redist/MSVC/*/x64/Microsoft.VC*.CRT"
        "C:/Windows/System32"
    )

    set(_vc_runtime_search_dirs)
    foreach(_vc_path IN LISTS VC_RUNTIME_PATHS)
        if(NOT _vc_path)
            continue()
        endif()

        if(_vc_path MATCHES "\\*")
            file(GLOB _vc_expanded "${_vc_path}")
            if(_vc_expanded)
                list(APPEND _vc_runtime_search_dirs ${_vc_expanded})
            endif()
        elseif(IS_DIRECTORY "${_vc_path}")
            list(APPEND _vc_runtime_search_dirs "${_vc_path}")
        endif()
    endforeach()

    list(REMOVE_DUPLICATES _vc_runtime_search_dirs)

    foreach(_vc_dll IN LISTS VC_RUNTIME_DLLS)
        unset(_vc_dll_path CACHE)
        if(_vc_runtime_search_dirs)
            find_file(_vc_dll_path
                NAMES ${_vc_dll}
                PATHS ${_vc_runtime_search_dirs}
                NO_DEFAULT_PATH
            )
        else()
            find_file(_vc_dll_path
                NAMES ${_vc_dll}
            )
        endif()

        if(_vc_dll_path AND EXISTS "${_vc_dll_path}")
            message(STATUS "Found ${_vc_dll}: ${_vc_dll_path}")
            install(FILES "${_vc_dll_path}" DESTINATION ${_lumos_runtime_dest})
        else()
            message(WARNING "Could not find ${_vc_dll}; the package may miss MSVC runtime dependencies.")
        endif()
    endforeach()
endif()

install(TARGETS lumos
    RUNTIME DESTINATION ${_lumos_runtime_dest}
)

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Taipa Xu <taipaxu@gmail.com>")
    set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/taipaxu/lumos")
    set(CPACK_PACKAGE_LICENSE "GPL-3.0")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

    if(NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
        set(_lumos_deb_arch "${CMAKE_SYSTEM_PROCESSOR}")
        if(_lumos_deb_arch STREQUAL "x86_64")
            set(_lumos_deb_arch "amd64")
        elseif(_lumos_deb_arch STREQUAL "aarch64")
            set(_lumos_deb_arch "arm64")
        endif()
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${_lumos_deb_arch}")
    endif()

    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    install(FILES ${CMAKE_SOURCE_DIR}/debian/copyright DESTINATION share/doc/lumos)
endif()

include(CPack)
