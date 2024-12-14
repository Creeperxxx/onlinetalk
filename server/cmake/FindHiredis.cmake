# - Try to find hiredis
# Once done this will define
#  HIREDIS_FOUND - System has hiredis
#  HIREDIS_INCLUDE_DIRS - The hiredis include directories
#  HIREDIS_LIBRARIES - The libraries needed to use hiredis

find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h
          PATHS /usr/include /usr/local/include
          PATH_SUFFIXES hiredis)

find_library(HIREDIS_LIBRARY NAMES hiredis
             PATHS /usr/lib /usr/local/lib)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set HIREDIS_FOUND to TRUE if all listed variables are TRUE
find_package_handle_standard_args(Hiredis DEFAULT_MSG
                                  HIREDIS_LIBRARY HIREDIS_INCLUDE_DIR)

mark_as_advanced(HIREDIS_INCLUDE_DIR HIREDIS_LIBRARY)

set(HIREDIS_INCLUDE_DIRS ${HIREDIS_INCLUDE_DIR})
set(HIREDIS_LIBRARIES ${HIREDIS_LIBRARY})

if(HIREDIS_FOUND AND NOT TARGET Hiredis::hiredis)
    add_library(Hiredis::hiredis UNKNOWN IMPORTED)
    set_target_properties(Hiredis::hiredis PROPERTIES
                          IMPORTED_LOCATION "${HIREDIS_LIBRARY}"
                          INTERFACE_INCLUDE_DIRECTORIES "${HIREDIS_INCLUDE_DIR}")
endif()