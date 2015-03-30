cmake_minimum_required(VERSION 2.8.12)

########################################################################################################################

# Possible generators for Windows, choose one
set(GENERATOR "Visual Studio 12 2013 Win64")
#set(GENERATOR "Visual Studio 11 2012 Win64")

# Comment lines below to enable/disable download/build of 3rdParty modules
set(BUILD_LIB3DS ON)
set(BUILD_FTLLIB ON)
set(BUILD_BOOST ON)
set(BUILD_TINYXML ON)
set(BUILD_DEVIL ON)
set(BUILD_FREEGLUT ON)
set(BUILD_GLEW ON)

########################################################################################################################

set(BUILD_ARCH x64)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty" CACHE PATH "default install path" FORCE)
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/temp/sources")
set(DOWNLOAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/downloads")
set(PATCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/patches")
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/temp/build")

message("Creating 3rdparty library folder for ${GENERATOR}")
message("Install prefix: ${CMAKE_INSTALL_PREFIX}")

file(MAKE_DIRECTORY ${SOURCE_DIR})
file(MAKE_DIRECTORY ${BUILD_DIR})

macro(lib3ds)
    message("Building lib3ds")
    set(FILENAME "lib3ds-20080909.zip")
    if (NOT EXISTS "${DOWNLOAD_DIR}/${FILENAME}")
        file(DOWNLOAD "https://lib3ds.googlecode.com/files/${FILENAME}" "${DOWNLOAD_DIR}/${FILENAME}" STATUS downloaded)
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_DIR}/${FILENAME} WORKING_DIRECTORY "${SOURCE_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PATCH_DIR}/${FILENAME} WORKING_DIRECTORY "${SOURCE_DIR}")

    set(BUILD_DIRECTORY "${BUILD_DIR}/lib3ds")
    if (EXISTS "${BUILD_DIRECTORY}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_DIRECTORY}")
    endif()
    file(MAKE_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND ${CMAKE_COMMAND} "-G${GENERATOR}" "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/lib3ds" "${SOURCE_DIR}/lib3ds-20080909" WORKING_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND devenv.exe "${BUILD_DIRECTORY}/lib3ds.sln" /Build "Release|${BUILD_ARCH}" WORKING_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND devenv.exe "${BUILD_DIRECTORY}/lib3ds.sln" /Build "Release|${BUILD_ARCH}" /Project INSTALL WORKING_DIRECTORY "${BUILD_DIRECTORY}")
endmacro()

macro(fltlib)
    message("Building fltlib")
    set(FILENAME "fltlib-1.0.zip")
    if (NOT EXISTS "${DOWNLOAD_DIR}/${FILENAME}")
        file(DOWNLOAD "http://downloads.sourceforge.net/project/fltlib/fltlib/fltlib%201.0/${FILENAME}" "${DOWNLOAD_DIR}/${FILENAME}" STATUS downloaded)
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${DOWNLOAD_DIR}/${FILENAME}" WORKING_DIRECTORY "${SOURCE_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${PATCH_DIR}/${FILENAME}" WORKING_DIRECTORY "${SOURCE_DIR}")

    set(BUILD_DIRECTORY "${BUILD_DIR}/fltlib")
    if (EXISTS "${BUILD_DIRECTORY}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_DIRECTORY}")
    endif()
    file(MAKE_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND ${CMAKE_COMMAND} "-G${GENERATOR}" "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/fltlib" "${SOURCE_DIR}/fltlib" WORKING_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND devenv.exe "${BUILD_DIRECTORY}/fltlib.sln" /Build "Release|${BUILD_ARCH}" WORKING_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND devenv.exe "${BUILD_DIRECTORY}/fltlib.sln" /Build "Release|${BUILD_ARCH}" /Project INSTALL WORKING_DIRECTORY "${BUILD_DIRECTORY}")
endmacro()

macro(tinyxml)
    message("Building TinyXML")
    set(FILENAME "tinyxml_2_6_2.zip")
    if (NOT EXISTS "${DOWNLOAD_DIR}/${FILENAME}")
        file(DOWNLOAD "http://downloads.sourceforge.net/project/tinyxml/tinyxml/2.6.2/${FILENAME}" "${DOWNLOAD_DIR}/${FILENAME}" STATUS downloaded)
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${DOWNLOAD_DIR}/${FILENAME}" WORKING_DIRECTORY "${SOURCE_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${PATCH_DIR}/${FILENAME}" WORKING_DIRECTORY "${SOURCE_DIR}")

    set(BUILD_DIRECTORY "${BUILD_DIR}/tinyxml")
    if (EXISTS "${BUILD_DIRECTORY}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${BUILD_DIRECTORY}")
    endif()
    file(MAKE_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND ${CMAKE_COMMAND} "-G${GENERATOR}" "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/tinyxml" "${SOURCE_DIR}/tinyxml" WORKING_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND devenv.exe "${BUILD_DIRECTORY}/tinyxml.sln" /Build "Release|${BUILD_ARCH}" WORKING_DIRECTORY "${BUILD_DIRECTORY}")
    execute_process(COMMAND devenv.exe "${BUILD_DIRECTORY}/tinyxml.sln" /Build "Release|${BUILD_ARCH}" /Project INSTALL WORKING_DIRECTORY "${BUILD_DIRECTORY}")
endmacro()

macro(boost)
    message("Building Boost.")
    set(BOOST_VERSION "1.57.0")
    string(REPLACE "." "_" BOOST_VERSION_NAME ${BOOST_VERSION}) #replace "." with "_" in BOOST_VERSION variable
    if (NOT EXISTS "${DOWNLOAD_DIR}/boost_1_57_0.tar.bz2")
      message("Downloading Boost. This might take a while.")
      file(DOWNLOAD "http://downloads.sourceforge.net/project/boost/boost/1.57.0/boost_1_57_0.tar.bz2" "${DOWNLOAD_DIR}/boost_1_56_0.tar.bz2" STATUS downloaded)
    endif()
    message("Extracting Boost")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${DOWNLOAD_DIR}/boost_1_57_0.tar.bz2" WORKING_DIRECTORY "${SOURCE_DIR}")
    execute_process(COMMAND cmd.exe "/C" "bootstrap.bat" WORKING_DIRECTORY "${SOURCE_DIR}/boost_1_57_0")

    # select the boost toolset according to the compiler being used
    if ("${GENERATOR}" MATCHES "^(Visual Studio 12).*")
      set(BOOST_TOOLSET msvc-12.0)
    else()
      set(BOOST_TOOLSET msvc-11.0)
    endif()

    execute_process(COMMAND cmd.exe /C b2 -j "$ENV{NUMBER_OF_PROCESSORS}" --toolset=${BOOST_TOOLSET} address-model=64 install --prefix=${CMAKE_INSTALL_PREFIX}/boost/ WORKING_DIRECTORY "${SOURCE_DIR}/boost_1_57_0")
endmacro()

macro(devil)
    if (NOT EXISTS "${DOWNLOAD_DIR}/DevIL-SDK-x64-1.7.8.zip")
      message("Downloading DeVIL")
      file(DOWNLOAD "http://downloads.sourceforge.net/project/openil/DevIL%20Windows%20SDK/1.7.8/DevIL-SDK-x64-1.7.8.zip" "${DOWNLOAD_DIR}/DevIL-SDK-x64-1.7.8.zip" STATUS downloaded)
    endif()
    message("Installing DeVIL")
    file(MAKE_DIRECTORY "${CMAKE_INSTALL_PREFIX}/devil")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${DOWNLOAD_DIR}/DevIL-SDK-x64-1.7.8.zip" WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/devil")
endmacro()

macro(freeglut)
    message("Downloading freeglut")
    if (NOT EXISTS "${DOWNLOAD_DIR}/freeglut-MSVC.zip")
        file(DOWNLOAD "http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip" "${DOWNLOAD_DIR}/freeglut-MSVC.zip" STATUS downloaded)
    endif()
    message("Installing freeglut")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${DOWNLOAD_DIR}/freeglut-MSVC.zip" WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
endmacro()

macro(glew)
    message("Downloading GLEW")
    set(GLEW_VERSION "1.12.0")
    set(FILENAME "glew-${GLEW_VERSION}-win32.zip")
    if (NOT EXISTS "${DOWNLOAD_DIR}/${FILENAME}")
        file(DOWNLOAD "http://downloads.sourceforge.net/project/glew/glew/${GLEW_VERSION}/${FILENAME}" "${DOWNLOAD_DIR}/${FILENAME}" STATUS downloaded)
    endif()
    message("Installing GLEW")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${DOWNLOAD_DIR}/${FILENAME}" WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}")

    # remove old glew directory
    if (EXISTS "${CMAKE_INSTALL_PREFIX}/glew")
      execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_INSTALL_PREFIX}/glew")
    endif()

    file(RENAME "${CMAKE_INSTALL_PREFIX}/glew-${GLEW_VERSION}" "${CMAKE_INSTALL_PREFIX}/glew")
endmacro()

if(BUILD_LIB3DS)
  lib3ds()
endif(BUILD_LIB3DS)

if(BUILD_FTLLIB)
  fltlib()
endif(BUILD_FTLLIB)

if(BUILD_TINYXML)
  tinyxml()
endif(BUILD_TINYXML)

if(BUILD_BOOST)
  boost()
endif(BUILD_BOOST)

if(BUILD_DEVIL)
  devil()
endif(BUILD_DEVIL)

if(BUILD_FREEGLUT)
  freeglut()
endif(BUILD_FREEGLUT)

if(BUILD_GLEW)
  glew()
endif(BUILD_GLEW)

message("Cleaning up")
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_SOURCE_DIR}/temp")
