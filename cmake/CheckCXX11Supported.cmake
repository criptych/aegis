INCLUDE(CheckCXXSourceCompiles)

FUNCTION(CHECK_CXX11_SUPPORTED OUT FLAGS_OUT)
    UNSET(${OUT} CACHE)
    UNSET(${OUT} PARENT_SCOPE)

    IF(NOT ARGN)
        SET(ARGN
            ""              # supported by default?
            "-std=c++11"    # GCC/Clang
            "-std=c++0x"    # GCC/Clang
            # add flags for other compilers here
        )
    ENDIF()

    FOREACH(FLAGS IN LISTS ARGN)
        IF(FLAGS)
            MESSAGE(STATUS "Checking C++11 support with flags: ${FLAGS}")
        ELSE()
            MESSAGE(STATUS "Checking C++11 support without flags")
        ENDIF()

        UNSET(TEST_CXX11_SUPPORT CACHE)
        UNSET(TEST_CXX11_SUPPORT)

        SET(CMAKE_REQUIRED_FLAGS "${FLAGS}")
        CHECK_CXX_SOURCE_COMPILES(
            "#include <list>\nint main(){for(int i:std::list<int>()){static_cast<void>(i);}return 0;}"
            TEST_CXX11_SUPPORT
        )

        IF(TEST_CXX11_SUPPORT)
            IF(${FLAGS})
                MESSAGE(STATUS "C++11 is supported with flags: ${FLAGS}")
            ELSE()
                MESSAGE(STATUS "C++11 is supported without flags.")
            ENDIF()
            SET(${OUT} TRUE CACHE BOOL "Compiler supports C++11?")
            SET(${FLAGS_OUT} "${FLAGS}" PARENT_SCOPE)
            BREAK()
        ENDIF()
    ENDFOREACH()

    UNSET(TEST_CXX11_SUPPORT CACHE)
ENDFUNCTION()
