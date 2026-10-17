cmake_minimum_required(VERSION 3.20)

#----------------------------------------------------------
# Project version
#----------------------------------------------------------

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" NUMATHAP_VERSION_FILE)

string(
	REGEX MATCH
	"VERSION[ \t]*=[ \t]*\"([0-9]+\.[0-9]+\.[0-9]+)\""
	_
	"${NUMATHAP_VERSION_FILE}"
)

if(NOT CMAKE_MATCH_1)
	message(FATAL_ERROR
		"Could not determine numathap version from VERSION file."
	)
endif()

set(NUMATHAP_VERSION "${CMAKE_MATCH_1}")

project(numathap
	VERSION ${NUMATHAP_VERSION}
	LANGUAGES CXX
)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS
	"Build shared libraries"
	OFF
)

option(NUMATHAP_INSTALL_CPP
	"Install C++ SDK"
	ON
)

#----------------------------------------------------------
# numathap library
#----------------------------------------------------------

# Definição modular
file(GLOB SOURCES_BACKEND CONFIGURE_DEPENDS "src/backend/*.cpp")
file(GLOB SOURCES_INTEGRATE CONFIGURE_DEPENDS "src/backend/integrate/*.cpp")
file(GLOB SOURCES_DIFFERENTIATE CONFIGURE_DEPENDS "src/backend/differentiate/*.cpp")
file(GLOB SOURCES_SERIES CONFIGURE_DEPENDS "src/backend/series/*.cpp")
file(GLOB SOURCES_CONFIG CONFIGURE_DEPENDS "src/config/*.cpp")
file(GLOB SOURCES_CORE CONFIGURE_DEPENDS "src/core/*.cpp")
file(GLOB SOURCES_DISPATCH CONFIGURE_DEPENDS "src/dispatch/*.cpp")
file(GLOB SOURCES_MATH CONFIGURE_DEPENDS "src/math/*.cpp")
file(GLOB SOURCES_NUMERIC CONFIGURE_DEPENDS "src/numeric/*.cpp")
file(GLOB SOURCES_ORCH CONFIGURE_DEPENDS "src/orchestration/*.cpp")
file(GLOB SOURCES_PARSER CONFIGURE_DEPENDS "src/parser/*.cpp")
file(GLOB SOURCES_SYMBOLIC CONFIGURE_DEPENDS "src/symbolic/*.cpp")

add_library(numathap

	# backend
	${SOURCES_BACKEND}

	# config
	${SOURCES_CONFIG}

	# core
	${SOURCES_CORE}

	# dispatch
	${SOURCES_DISPATCH}

	# math
	${SOURCES_MATH}

	# numeric
	${SOURCES_NUMERIC}

	# orchestration
	${SOURCES_ORCH}

	# parser
	${SOURCES_PARSER}

	# symbolic
	${SOURCES_SYMBOLIC}

	# integrate
	${SOURCES_INTEGRATE}

	# differentiate
	${SOURCES_DIFFERENTIATE}

	# series
	${SOURCES_SERIES}
)

# Public modern name
add_library(numathap::numathap ALIAS numathap)

set_target_properties(numathap
	PROPERTIES POSITION_INDEPENDENT_CODE ON
)

target_include_directories(numathap
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

if(MSVC)
	target_compile_options(numathap PRIVATE /W4)
else()
	target_compile_options(numathap PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(NUMATHAP_BUILD_PYTHON "Build Python bindings" OFF)

if(NUMATHAP_BUILD_PYTHON)
	add_subdirectory(python)
endif()

option(
    NUMATHAP_BUILD_NEW_TESTS
    "Build new GoogleTest-based tests"
    OFF
)

if(NUMATHAP_BUILD_NEW_TESTS)
    add_subdirectory(newtest)
endif()

#----------------------------------------------------------
# Installation (SDK)
#----------------------------------------------------------
if(NUMATHAP_INSTALL_CPP)
	include(GNUInstallDirs)
	include(CMakePackageConfigHelpers)

	install(
		TARGETS numathap
		EXPORT numathapTargets
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)

	install(
		DIRECTORY include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	install(
		FILES ${CMAKE_CURRENT_SOURCE_DIR}/docs/developer/cpp_user.md
		DESTINATION .
		RENAME README.md
	)

	install(EXPORT numathapTargets
		FILE numathapTargets.cmake
		NAMESPACE numathap::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/numathap
	)

	configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/numathapConfig.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/numathapConfig.cmake
		INSTALL_DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/numathap
	)

	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/numathapConfig.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/numathap
	)

endif()

