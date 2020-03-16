#
#   EVENTPP CMake script
#

# ┌──────────────────────────────────────────────────────────────────┐
# │                       CMAKE PROPERTIES                           │
# └──────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# ┌──────────────────────────────────────────────────────────────────┐
# │                       PROJECT SETTINGS                           │
# └──────────────────────────────────────────────────────────────────┘

set(EVENTPP_TARGET "eventpp" CACHE STRING "Target Name")

# ┌──────────────────────────────────────────────────────────────────┐
# │                       STATUS                                     │
# └──────────────────────────────────────────────────────────────────┘

project(${EVENTPP_TARGET} VERSION "0.1.1" LANGUAGES CXX)

# ┌──────────────────────────────────────────────────────────────────┐
# │                       LIBRARY                                    │
# └──────────────────────────────────────────────────────────────────┘

add_library(${EVENTPP_TARGET} INTERFACE)
set(EVENTPP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(EVENTPP_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/eventpp")

set(EVENTPP_SRCS
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/callbacklist.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/eventdispatcher.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/eventpolicies.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/eventqueue.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/hetercallbacklist.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/hetereventdispatcher.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/hetereventqueue.h"
    )

set(EVENTPP_UTILITIES_SRCS
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/utilities/conditionalremover.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/utilities/counterremover.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/utilities/eventmaker.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/utilities/eventutil.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/utilities/scopedremover.h"
    )

set(EVENTPP_MIXINS_SRCS
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/mixins/mixinfilter.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/mixins/mixinheterfilter.h"
    )

set(EVENTPP_INTERNAL_SRCS
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/internal/eventpolicies_i.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/internal/eventqueue_i.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/internal/hetercallbacklist_i.h"
    "${EVENTPP_PRIVATE_INCLUDE_DIR}/internal/typeutil_i.h"
    )

set(EVENTPP_ALL_SRCS ${EVENTPP_SRCS} ${EVENTPP_UTILITIES_SRCS} ${EVENTPP_MIXINS_SRCS} ${EVENTPP_INTERNAL_SRCS})

foreach(SRC_FILE ${EVENTPP_ALL_SRCS})
    target_sources(${EVENTPP_TARGET} INTERFACE ${SRC_FILE})
endforeach()

source_group("EventPP" FILES ${EVENTPP_SRCS})
source_group("EventPP\\Utilities" FILES ${EVENTPP_UTILITIES_SRCS})
source_group("EventPP\\Mixins" FILES ${EVENTPP_MIXINS_SRCS})
source_group("EventPP\\Internal" FILES ${EVENTPP_INTERNAL_SRCS})

target_include_directories(${EVENTPP_TARGET} INTERFACE ${EVENTPP_INCLUDE_DIR})