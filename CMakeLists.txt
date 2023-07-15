cmake_minimum_required(VERSION 3.15)

project(hsjson)
set(hsjson_VERSION 0.1)

include(GNUInstallDirs)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)



add_library(hsjson STATIC hsjson.cc)

# Will include correct folder
#  - When build as source, will use /
#  - When used as dependency, will use /include
target_include_directories(hsjson 
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install library as a target HsJson name and specifies where to install them
#  - Library: .so files
#  - Archive: .a files
#  - Runtime: .exe files
#  - Includes: .hh files
# After this call a libhsjson.a will copied to destination folder
install(TARGETS hsjson
    EXPORT HsJson
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install (copy) header file to destination
install(FILES hsjson.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the targets, user can include() HsJson.cmake and
# got all information about include, lib path, etc.
install(EXPORT HsJson
    FILE HsJson.cmake
    NAMESPACE HsJson::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HsJson)
# Also allow user to use build directory without installing
export(EXPORT HsJson
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/HsJson.cmake"
    NAMESPACE HsJson::
)


include(CMakePackageConfigHelpers)

set_property(TARGET hsjson PROPERTY VERSION ${hsjson_VERSION})
set_property(TARGET hsjson PROPERTY SOVERSION 0)
set_property(TARGET hsjson PROPERTY INTERFACE_hsjson_MAJOR_VERSION 0)
set_property(TARGET hsjson APPEND PROPERTY
    COMPATIBLE_INTERFACE_STRING hsjson_MAJOR_VERSION
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/HsJsonConfigVersion.cmake"
  VERSION "${hsjson_VERSION}"
  COMPATIBILITY AnyNewerVersion
)


configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/HsJsonConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HsJson
)


install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/HsJsonConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/HsJsonConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HsJson
)


