cmake_minimum_required (VERSION 3.19)
set(BINDINGS_SCOPE "CLIENT")
# include(../shared/cmake/GenerateBindings.cmake)
# include(../shared/cmake/GenerateEnums.cmake)

project(altv-client-jsv2)

set(ALTV_JSV2_DEPS_DIR ${PROJECT_SOURCE_DIR}/deps)

# cpp-sdk
if(NOT ALTV_JSV2_CPP_SDK)
  set(ALTV_JSV2_CPP_SDK ../shared/deps)
else()
  set(ALTV_JSV2_DEINIT_CPPSDK 1)
endif()
# Fetch deps

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")
file(GLOB_RECURSE PROJECT_SHARED_FILES "../shared/*.h" "../shared/*.cpp")

macro(GroupSources curdir groupindex)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)

  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      GroupSources(${curdir}/${child} ${groupindex}/${child})
    else()

      string(REPLACE "/" "\\" groupname ${groupindex})

      source_group(${groupname} FILES ${curdir}/${child})
    endif()
  endforeach()
endmacro()

GroupSources(${PROJECT_SOURCE_DIR}/src "Source Files")
GroupSources("../shared" "Shared Files")

# TODO
# make_includable("src/bootstrap.js" "src/bootstrap.js.gen")

include_directories(
  ${ALTV_JSV2_CPP_SDK}
  ${ALTV_JSV2_DEPS_DIR}/v8/include
  ../shared/src
  ../deps
  ${PROJECT_SOURCE_DIR}/src
)

set(ALTV_JS_LINKS
  # Platform binaries
  Winmm.lib
  DbgHelp.lib
  shlwapi.lib

  # V8
  ${ALTV_JSV2_DEPS_DIR}/v8/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/v8_monolith.lib
)

set(ALTV_JSV2_DEFS
  # Compliation
  -DCXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"

  # Platform
  -DUNICODE
  -D_UNICODE

  # alt:V
  -DALT_CLIENT
  -DALT_CLIENT_API

  # v8
  -DV8_COMPRESS_POINTERS
  -DV8_31BIT_SMIS_ON_64BIT_ARCH
  -DV8_IMMINENT_DEPRECATION_WARNINGS

  # jsv2
  -DMODULE_VERSION="1.0.0" # TODO
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi /bigobj")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /bigobj")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")

if(WIN32 AND (MSVC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(/MP)
endif()

set(SDK_PROJECT_NAME alt-sdk-jsv2)
if(NOT TARGET ${SDK_PROJECT_NAME})
    include(${ALTV_JSV2_CPP_SDK}/cpp-sdk/CMakeLists.txt)
endif()

add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/../build/BindingsMap.cpp
    COMMAND node ${CMAKE_CURRENT_SOURCE_DIR}/../tools/generate-bindings.js ${CMAKE_CURRENT_SOURCE_DIR}/..
)

if(DYNAMIC_BUILD EQUAL 1)
  ## SHARED
  add_library(
    ${PROJECT_NAME} SHARED
    ${PROJECT_SOURCE_FILES}
    ${PROJECT_SHARED_FILES}
    ../build/BindingsMap.cpp
  )

  set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
  )
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${ALTV_JSV2_DEFS}
    -DALTV_JSV2_SHARED
    -DDYNAMIC_BUILD
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ${ALTV_JSV2_LINKS}
  )
else()
  ## STATIC
  add_library(
    ${PROJECT_NAME}-static STATIC
    ${PROJECT_SOURCE_FILES}
    ${PROJECT_SHARED_FILES}
    ../build/BindingsMap.cpp
  )
  set_target_properties(${PROJECT_NAME}-static PROPERTIES
    CXX_STANDARD 20
  )
  target_compile_definitions(${PROJECT_NAME}-static PRIVATE
    ${ALTV_JSV2_DEFS}
  )
  target_link_libraries(${PROJECT_NAME}-static PRIVATE
    ${ALTV_JSV2_LINKS}
  )
endif()

if(ALTV_JSV2_DEINIT_CPPSDK)
  if(CMAKE_HOST_WIN32)
      add_custom_command(TARGET ${PROJECT_NAME}-static
        PRE_BUILD
        COMMAND cmd /C "..\\tools\\deinit-cppsdk.bat"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      )
  else()
      if(EXISTS "../deps/cpp-sdk/SDK.h")
          add_custom_command(TARGET ${PROJECT_NAME}-static
            PRE_BUILD
            COMMAND "git submodule deinit -f ../deps/cpp-sdk"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
          )
      endif()
  endif()
endif()
