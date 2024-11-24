# - Try to find MySQL Connector/C++
# Once done this will define
#  MYSQLCONNECTORCPP_FOUND - System has MySQL Connector/C++
#  MYSQLCONNECTORCPP_INCLUDE_DIRS - The MySQL Connector/C++ include directories
#  MYSQLCONNECTORCPP_LIBRARIES - The libraries needed to use MySQL Connector/C++

find_path(MYSQLCONNECTORCPP_INCLUDE_DIR mysql_connection.h
          PATHS /usr/include /usr/local/include
          PATH_SUFFIXES mysql-connector-cpp)

find_library(MYSQLCONNECTORCPP_LIBRARY NAMES mysqlcppconn
             PATHS /usr/lib /usr/local/lib)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set MYSQLCONNECTORCPP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(MySQLConnectorCpp DEFAULT_MSG
                                  MYSQLCONNECTORCPP_LIBRARY MYSQLCONNECTORCPP_INCLUDE_DIR)

mark_as_advanced(MYSQLCONNECTORCPP_INCLUDE_DIR MYSQLCONNECTORCPP_LIBRARY)

set(MYSQLCONNECTORCPP_INCLUDE_DIRS ${MYSQLCONNECTORCPP_INCLUDE_DIR})
set(MYSQLCONNECTORCPP_LIBRARIES ${MYSQLCONNECTORCPP_LIBRARY})