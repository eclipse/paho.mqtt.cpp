#*******************************************************************************
#  Copyright (c) 2016
# 
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v1.0
#  and Eclipse Distribution License v1.0 which accompany this distribution. 
# 
#  The Eclipse Public License is available at 
#     http://www.eclipse.org/legal/epl-v10.html
#  and the Eclipse Distribution License is available at 
#    http://www.eclipse.org/org/documents/edl-v10.php.
# 
#  Contributors:
#     Guilherme Maciel Ferreira - initial contribuition
#*******************************************************************************/

###############################################################################
# Autotools setup
###############################################################################

AC_PREREQ([2.65])
AC_INIT([paho.mqtt.cpp], [1.0.0])
AC_CONFIG_AUX_DIR([autotools_build])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG_CPLUSPLUS

AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign subdir-objects])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AM_SILENT_RULES([yes])

LT_INIT([shared static pic-only])

AC_PROG_CC
AC_PROG_CXX([clang++ cxx cc++ c++ g++])
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL

###############################################################################
# Input dependencies
###############################################################################

AC_CHECK_LIB([pthread], [pthread_create], , AC_MSG_ERROR([missing pthread library]))
AC_CHECK_HEADER([pthread.h], , AC_MSG_ERROR([missing pthread.h header]))

AC_SEARCH_LIBS([main], [stdc++], , AC_MSG_ERROR([missing libstdc++]))
AC_CHECK_HEADER([iostream], , AC_MSG_ERROR([missing STL iostream header]))
AC_CHECK_HEADER([list], , AC_MSG_ERROR([missing STL list header]))
AC_CHECK_HEADER([memory], , AC_MSG_ERROR([missing STL memory header]))
AC_CHECK_HEADER([string], , AC_MSG_ERROR([missing STL string header]))
AC_CHECK_HEADER([vector], , AC_MSG_ERROR([missing STL vector header]))
#AC_CHECK_HEADER([chrono], , AC_MSG_ERROR([missing STL chrono header]))
#AC_CHECK_HEADER([condition_variable], , AC_MSG_ERROR([missing STL condition_variable header]))
#AC_CHECK_HEADER([exception], , AC_MSG_ERROR([missing STL exception header]))
#AC_CHECK_HEADER([thread], , AC_MSG_ERROR([missing STL thread header]))
AC_HEADER_STDBOOL

AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

AC_C_INLINE

AC_FUNC_MALLOC

AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

###############################################################################
# Input options
###############################################################################

# Build samples
AC_ARG_ENABLE(
	[samples],
	AS_HELP_STRING(
		[--enable-samples=@<:@yes/no@:>@],
		[build sample programs @<:@default=no@:>@]
	),
	,
	enable_samples=no
)

AM_CONDITIONAL([PAHO_BUILD_SAMPLES], [test "$enable_samples" = yes])


# Build documentation
AC_ARG_ENABLE(
	[doc],
	AS_HELP_STRING(
		[--enable-doc=@<:@yes/no@:>@],
		[build documentation @<:@default=no@:>@]
	),
	,
	enable_doc=no
)

AS_IF([test "x$enable_doc" = "xyes"], [
	AC_CHECK_PROGS([DOXYGEN], [doxygen])

	AS_IF([test -z "$DOXYGEN"], [
		AC_MSG_ERROR([Doxygen not found - disabling documentation build])
	], [
		AC_CONFIG_FILES([doc/Doxyfile])
	])

])

AM_CONDITIONAL([PAHO_BUILD_DOC], [test "$enable_doc" = yes && test -n "$DOXYGEN"])


# Peak warnings
AC_ARG_ENABLE(
	[peak_warnings],
	AS_HELP_STRING(
		[--enable-peak-warnings=@<:@yes/no@:>@],
		[enable peak warnings level @<:@default=no@:>@]
	),
	,
	enable_peak_warnings=no
)

AS_IF([test "x$enable_peak_warnings" = "xyes"], [
	CXXFLAGS+=" -Wabi"
	CXXFLAGS+=" -Wall"
	CXXFLAGS+=" -Wcast-qual"
	CXXFLAGS+=" -Wconversion"
	CXXFLAGS+=" -Weffc++"
	CXXFLAGS+=" -Wextra"
	CXXFLAGS+=" -Wfloat-equal"
	CXXFLAGS+=" -Winline"
	CXXFLAGS+=" -Wmissing-declarations"
	CXXFLAGS+=" -Wmissing-format-attribute"
	CXXFLAGS+=" -Wno-deprecated-declarations"
	CXXFLAGS+=" -Wno-unused-result"
	CXXFLAGS+=" -Woverloaded-virtual"
	CXXFLAGS+=" -Wpacked"
	CXXFLAGS+=" -Wredundant-decls"
	CXXFLAGS+=" -Wshadow"
	CXXFLAGS+=" -Wsign-promo"
	CXXFLAGS+=" -Wstack-protector -fstack-protector"
	CXXFLAGS+=" -Wswitch-default"
	CXXFLAGS+=" -fno-common"
])


# Add Paho MQTT C path
AC_ARG_WITH(
	[paho-mqtt-c],
	AS_HELP_STRING(
		[--with-paho-mqtt-c=DIR],
		[add a path to paho.mqtt.c library and headers]
	),
	[
		LDFLAGS="$LDFLAGS -L$with_paho_mqtt_c/build/output -L$with_paho_mqtt_c/lib -L$with_paho_mqtt_c/lib64"
		CPPFLAGS="$CPPFLAGS -I$with_paho_mqtt_c/src -I$with_paho_mqtt_c/include"
	],
)

AC_CHECK_HEADER([MQTTAsync.h], , AC_MSG_ERROR([missing paho.mqtt.c headers]))


# Add OpenSSL support to build secure PAHO libraries
AC_ARG_WITH(
	[ssl],
	AS_HELP_STRING(
		[--with-ssl=@<:@yes/no@:>@],
		[with OpenSSL support @<:@default=no@:>@]
	),
	,
	with_ssl=yes
)

AS_IF([test "x$with_ssl" = "xyes"], [
	AC_CHECK_HEADER([openssl/crypto.h], , AC_MSG_ERROR([missing openssl/crypto.h header]))
	AC_CHECK_LIB([crypto], [CRYPTO_num_locks], , AC_MSG_ERROR([missing crypto library]))

	AC_CHECK_HEADER([openssl/ssl.h], , AC_MSG_ERROR([missing openssl/ssl.h header]))
	AC_CHECK_LIB([ssl], [SSL_connect], , AC_MSG_ERROR([missing ssl library]))

	AC_CHECK_LIB([paho-mqtt3as], [MQTTAsync_create], , AC_MSG_ERROR([missing paho.mqtt.c SSL library]))

	AC_DEFINE([OPENSSL], [], [OpenSSL support])
],
[
	AC_SEARCH_LIBS([MQTTAsync_create], [mqtt paho-mqtt mqtt3 paho-mqtt3 mqtt3a paho-mqtt3a], , AC_MSG_ERROR([missing paho.mqtt.c library]))
])

AM_CONDITIONAL([PAHO_WITH_SSL], [test "$with_ssl" = yes])


###############################################################################
# Output files
###############################################################################

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

###############################################################################
# Output message
###############################################################################

echo ""
echo "Paho MQTT C++ has been configured with the following options:"
echo "            Build as shared library : $enable_shared"
echo "            Build as static library : $enable_static"
echo "                      Build samples : $enable_samples"
echo "                Build documentation : $enable_doc"
echo "          Enable peak warning level : $enable_peak_warnings"
echo "                   With Paho MQTT C : $with_paho_mqtt_c"
echo "               With OpenSSL library : $with_ssl"
echo "                      With CPPFLAGS : $CPPFLAGS"
echo "                        With CFLAGS : $CFLAGS"
echo "                      With CXXFLAGS : $CXXFLAGS"
echo "                       With LDFLAGS : $LDFLAGS"
echo "                     With libraries : $LIBS"
