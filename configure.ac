# Initialization
AC_PREREQ([2.69])
AC_INIT([phosphor-pci-inventory],
        [1.0],
        [https://github.com/YADRO-KNS/phosphor-pci-inventory/issues],
        [],
        [https://github.com/YADRO-KNS/phosphor-pci-inventory])
AC_LANG([C++])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign dist-xz])
AM_SILENT_RULES([yes])

# Check for programs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_HEADER_STDC
AM_PROG_AR

# Compiler flags
AX_CXX_COMPILE_STDCXX_17([noext])
AX_APPEND_COMPILE_FLAGS([-Wall -Wextra], [CXXFLAGS])

# Checks for external dependencies
AX_PTHREAD([], [AC_MSG_ERROR([pthread required but not found])])
PKG_CHECK_MODULES([SYSTEMD], [libsystemd >= 221])
PKG_CHECK_MODULES([LIBIPMID], [libipmid])
PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging])
PKG_CHECK_MODULES([SDBUSPLUS], [sdbusplus])
AC_PATH_PROG([SDBUSPLUSPLUS], [sdbus++])
AS_IF([test "x$SDBUSPLUSPLUS" = "x"],
      [AC_MSG_ERROR([sdbus++ required but not found])])

# Checks for library functions
LT_INIT([disable-static shared])

# Create configured output
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
