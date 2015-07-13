###############################################################################
# Find Cartan
#
# This sets the following variables:
# CARTAN_FOUND - True if Cartan was found.
# CARTAN_INCLUDE_DIRS - Directories containing the Cartan include files.
# CARTAN_LIBRARY_DIRS - Directories containing the Cartan library.
# CARTAN_LIBRARIES - Cartan library files.

if (WIN32)
find_path(CARTAN_INCLUDE_DIR cartan
    HINTS "/usr/include" "/usr/local/include" "/usr/x86_64-w64-mingw32/include" "$ENV{PROGRAMFILES}")
else()
find_path(CARTAN_INCLUDE_DIR cartan
    HINTS "/usr/include" "/usr/local/include" "$ENV{PROGRAMFILES}")
endif()

if (WIN32)
    find_library(CARTAN_LIBRARY_PATH cartan HINTS "/usr/x86_64-w64-mingw32/bin")
else()
    find_library(CARTAN_LIBRARY_PATH cartan HINTS "/usr/lib" "/usr/local/lib")
endif()

if(EXISTS ${CARTAN_LIBRARY_PATH})
get_filename_component(CARTAN_LIBRARY ${CARTAN_LIBRARY_PATH} NAME)
if(WIN32)
find_path(CARTAN_LIBRARY_DIR ${CARTAN_LIBRARY} HINTS "/usr/lib" "/usr/local/lib" "/usr/x86_64-w64-mingw32/lib/")
else()
find_path(CARTAN_LIBRARY_DIR ${CARTAN_LIBRARY} HINTS "/usr/lib" "/usr/local/lib")
endif()
endif()

set(CARTAN_INCLUDE_DIRS ${CARTAN_INCLUDE_DIR})
set(CARTAN_LIBRARY_DIRS ${CARTAN_LIBRARY_DIR})
set(CARTAN_LIBRARIES ${CARTAN_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cartan DEFAULT_MSG CARTAN_INCLUDE_DIR CARTAN_LIBRARY CARTAN_LIBRARY_DIR)

mark_as_advanced(CARTAN_INCLUDE_DIR)
mark_as_advanced(CARTAN_LIBRARY_DIR)
mark_as_advanced(CARTAN_LIBRARY)
mark_as_advanced(CARTAN_LIBRARY_PATH)
