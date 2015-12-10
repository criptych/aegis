INCLUDE(CheckCXXSourceCompiles)

FUNCTION(CHECK_CXX11_SUPPORTED VAR FLAGS_VAR)
    IF(DEFINED "${VAR}")
        RETURN()
    ENDIF()

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
            SET(${VAR} TRUE)
            SET(${FLAGS_VAR} "${FLAGS}")
            BREAK()
        ENDIF()
    ENDFOREACH()

    UNSET(TEST_CXX11_SUPPORT CACHE)

    SET(${VAR} "${${VAR}}" CACHE BOOL "Compiler supports C++11?")
    SET(${FLAGS_VAR} "${${FLAGS_VAR}}" CACHE STRING "Compiler flags to enable C++11")
ENDFUNCTION()
