cmake_minimum_required(VERSION 3.5.1)
project(fribidi LANGUAGES)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_modules")
include("koreader_thirdparty_common")
include("koreader_thirdparty_git")

assert_var_defined(CC)
assert_var_defined(CFLAGS)
assert_var_defined(CHOST)
assert_var_defined(LDFLAGS)

set(CFG_ENV_VAR "CC=${CC}" "CFLAGS=${CFLAGS}" "LDFLAGS=${LDFLAGS}")
set(CFG_OPTS --prefix=${INSTALL_DIR} --disable-static --enable-shared --host=${CHOST})
list(APPEND CFG_CMD COMMAND env ${CFG_ENV_VAR} ${SOURCE_DIR}/autogen.sh ${CFG_OPTS})

# Disable building docs, as it requires c2man
list(APPEND CFG_CMD COMMAND ${ISED} "s|SUBDIRS = gen.tab lib bin doc test|SUBDIRS = gen.tab lib bin test|g" Makefile)

if(DEFINED ENV{ANDROID})
    list(APPEND CFG_CMD COMMAND ${ANDROID_LIBTOOL_FIX_CMD})
endif()

list(APPEND BUILD_CMD COMMAND ${KO_MAKE_RECURSIVE})

list(APPEND INSTALL_CMD COMMAND ${KO_MAKE_RECURSIVE} install)

ko_write_gitclone_script(
    GIT_CLONE_SCRIPT_FILENAME
    https://github.com/fribidi/fribidi.git
    tags/v1.0.14
    ${SOURCE_DIR}
)

external_project(
    DOWNLOAD_COMMAND ${CMAKE_COMMAND} -P ${GIT_CLONE_SCRIPT_FILENAME}
    CONFIGURE_COMMAND ${CFG_CMD}
    BUILD_COMMAND ${BUILD_CMD}
    INSTALL_COMMAND ${INSTALL_CMD}
)
