AM_CFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include $(GLOBAL_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(GLOBAL_CXXFLAGS)
AM_LDFLAGS =


lib_LTLIBRARIES = libgeneral.la

libgeneral_la_CFLAGS = $(AM_CFLAGS)
libgeneral_la_CXXFLAGS = $(AM_CXXFLAGS)
libgeneral_la_LDFLAGS = $(AM_LDFLAGS)
libgeneral_la_SOURCES = exception.cpp

if !BUILD_MINIMAL
libgeneral_la_SOURCES += Event.cpp \
						 GuardAccess.cpp \
						 Manager.cpp \
						 Mem.cpp \
						 Utils.cpp
endif
