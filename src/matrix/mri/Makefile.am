SUBDIRS = tests

AM_CPPFLAGS = -DTIXML_USE_STL $(OPENMP_CXXFLAGS) -I$(top_srcdir)/src -I$(top_srcdir)/src/matrix -I$(top_srcdir)/src/matrix/simd -I$(top_srcdir)/src/matrix/linalg -I$(top_srcdir)/src/matrix/io -I$(top_srcdir)/src/matrix/arithmetic -I$(top_srcdir)/src/misc -I$(top_srcdir)/src/tinyxml

lib_LTLIBRARIES = libcodeare-mri.la
libcodeare_mri_la_SOURCES = MRI.hpp MRI.cpp
libcodeare_mri_la_LDFLAGS = -module -avoid-version @PLUGIN_LDFLAGS@

include_HEADERS = codeare-mri.h
