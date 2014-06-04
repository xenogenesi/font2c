# Prelude.
AC_INIT([font2c], [0.1], [xenogenesi@virgilio.it])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_AR

# Checks for libraries.

# Checks for header files.
# assume stdlib.h string.h already checked
AC_CHECK_HEADERS([error.h errno.h limits.h fcntl.h getopt.h],,
                          AC_MSG_ERROR([required headers missing.]))


AC_HEADER_ASSERT
AC_HEADER_DIRENT
AC_HEADER_STAT

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.



PKG_CHECK_MODULES([FREETYPE2], [freetype2],,
        AC_MSG_ERROR([lib freetype2 not found.]))

AC_SUBST([LIBCONFIG_CFLAGS])
AC_SUBST([LIBCONFIG_LDFLAGS])

# Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
        Makefile
        src/Makefile
])
AC_OUTPUT
