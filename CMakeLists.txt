cmake_minimum_required(VERSION 3.5.0)

project(CLua VERSION 1.0.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Append this to debug library builds
SET(CMAKE_DEBUG_POSTFIX d)
# define the absolute path to this directory for the rest of the tree to reference
get_filename_component(CLUA_HOME . ABSOLUTE)

# Install all libraries and public headers to top level locations
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CLUA_HOME}" CACHE PATH "..." FORCE)
endif()

# Set compiler definitions/options for each target
# These will apply to all included directories
if(WIN32)
add_definitions(
  -DPLATFORM_WINDOWS
  -D_CRT_SECURE_NO_WARNINGS
)
endif(WIN32)

if(UNIX)
add_definitions(
  -DPLATFORM_LINUX
)
endif(UNIX)

# Fetch external modules
include(FetchContent)

# lua ********************************
FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua.git
  GIT_TAG        master
  SOURCE_DIR     ${CMAKE_CURRENT_BINARY_DIR}/_deps/lua
)

FetchContent_GetProperties(lua)
if(NOT lua_POPULATED)
  message(NOTICE ":Fetching/Updating Lua sources. This may take a few minutes...")
  FetchContent_Populate(lua)
  
  # lua project does not provide a CMakeLists.txt, so put the defs here
  set(LUA_SRCS ${lua_SOURCE_DIR}/lapi.c ${lua_SOURCE_DIR}/lauxlib.c ${lua_SOURCE_DIR}/lbaselib.c
      ${lua_SOURCE_DIR}/lcode.c ${lua_SOURCE_DIR}/lcorolib.c ${lua_SOURCE_DIR}/lctype.c
      ${lua_SOURCE_DIR}/ldblib.c ${lua_SOURCE_DIR}/ldebug.c ${lua_SOURCE_DIR}/ldo.c
      ${lua_SOURCE_DIR}/ldump.c ${lua_SOURCE_DIR}/lfunc.c ${lua_SOURCE_DIR}/lgc.c
      ${lua_SOURCE_DIR}/linit.c ${lua_SOURCE_DIR}/liolib.c ${lua_SOURCE_DIR}/llex.c
      ${lua_SOURCE_DIR}/lmathlib.c ${lua_SOURCE_DIR}/lmem.c ${lua_SOURCE_DIR}/loadlib.c
      ${lua_SOURCE_DIR}/lobject.c ${lua_SOURCE_DIR}/lopcodes.c ${lua_SOURCE_DIR}/loslib.c
      ${lua_SOURCE_DIR}/lparser.c ${lua_SOURCE_DIR}/lstate.c ${lua_SOURCE_DIR}/lstring.c 
      ${lua_SOURCE_DIR}/lstrlib.c ${lua_SOURCE_DIR}/ltable.c ${lua_SOURCE_DIR}/ltablib.c
      ${lua_SOURCE_DIR}/ltm.c ${lua_SOURCE_DIR}/lundump.c ${lua_SOURCE_DIR}/lvm.c
      ${lua_SOURCE_DIR}/lzio.c ${lua_SOURCE_DIR}/lutf8lib.c
   )
endif()
# lua ********************************
# End of external modules

  # Add the CLua target
  add_library(CLua STATIC ${LUA_SRCS} cLua.cpp)
  target_include_directories(CLua PRIVATE ${lua_SOURCE_DIR})

install(TARGETS CLua 
            CONFIGURATIONS Debug
            DESTINATION ${CLUA_HOME}/lib
        )
install(TARGETS CLua 
            CONFIGURATIONS Release
            DESTINATION ${CLUA_HOME}/lib
        )
        
install(FILES cLua.h 
            DESTINATION ${CLUA_HOME}/include
        )
        


# build test app
add_executable(TestCLua test/test.cpp)
add_dependencies(TestCLua CLua)
target_include_directories(TestCLua PUBLIC ${CLUA_HOME})
target_link_libraries(TestCLua CLua)