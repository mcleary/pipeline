if (WIN32)
  set (BOOSTVERSION 1.54.0)
  file( TO_CMAKE_PATH "$ENV{DP_3RDPARTY_PATH}/Boost/${BOOSTVERSION}" BOOSTROOT)
  set(Boost_USE_STATIC_LIBS "ON")
  if ( EXISTS "${BOOSTROOT}" )
    if ( NOT "${BOOSTROOT}" STREQUAL "${BOOST_ROOT}" )

      # Configure boost to use the BUILD_TOOLS_DIR
      if(MSVC90)
        set (COMPILER "win32-msvc2008-${DP_ARCH}")
      elseif(MSVC10)
        set (COMPILER "win32-msvc2010-${DP_ARCH}")
      elseif(MSVC11)
        set (COMPILER "win32-msvc2012-${DP_ARCH}")
      elseif(CMAKE_COMPILER_IS_GNUCC)
        execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        string(STRIP "${GCC_VERSION}" GCC_VERSION)
        set (COMPILER "mingw-${GCC_VERSION}-${DP_ARCH}")
      else()
        message(FATAL_ERROR "Compiler version not supported")
      endif()
    endif(WIN32)
    
    if ( UNIX )
      set ( COMPILER "gcc-4.1-${DP_ARCH}" )
    endif( UNIX )

    set( BOOST_ROOT "${BOOSTROOT}")
    set( BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/${COMPILER}/lib" )
    
    message("Using boost: ${BOOST_ROOT}")
  endif()
endif()

if( UNIX )
  set(Boost_USE_STATIC_LIBS       OFF)
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_RUNTIME    OFF)

endif()
