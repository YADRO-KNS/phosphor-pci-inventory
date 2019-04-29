# Library to build
libpciinventory_LTLIBRARIES = libpciinventory.la

# Install directory
libpciinventorydir = ${libdir}/ipmid-providers

# Source files
libpciinventory_la_SOURCES = \
	src/inventory.cpp \
	src/inventory.hpp \
	src/ipmi.cpp \
	src/ipmi.hpp \
	src/pcidevice.cpp \
	src/pcidevice.hpp \
	src/workqueue.cpp \
	src/workqueue.hpp

# General build flags
libpciinventory_la_CXXFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(PHOSPHOR_LOGGING_CFLAGS) \
	$(SDBUSPLUS_CFLAGS) \
	$(LIBIPMID_CFLAGS)
libpciinventory_la_LIBADD = \
	$(PTHREAD_LIBS) \
	$(PHOSPHOR_LOGGING_LIBS) \
	$(SDBUSPLUS_LIBS) \
	$(LIBIPMID_LIBS)

# Library version information
libpciinventory_la_LDFLAGS = -version-info 1:0:0

# Fucken boost!
libpciinventory_la_CXXFLAGS += \
	-DBOOST_COROUTINES_NO_DEPRECATION_WARNING \
	-DBOOST_ASIO_DISABLE_THREADS

# Additional target to format source code
format:
	clang-format -style=file --verbose -i src/*.cpp src/*.hpp
