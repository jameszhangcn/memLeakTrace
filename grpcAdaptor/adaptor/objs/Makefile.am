# Prefixes : specify 'install' directory
# bin_ lib_ include_ pkginclude_ noinst_ ...

# Primary : specify target type
# PROGRAMS LIBRARIES LTLIBRARIES HEADERS ...

# Variable : specify expansions
# SOURCES CFLAGS CPPFLAGS CXXFLAGS LIBADD LDADD LDFLAGS ...

APP_PATH = $(top_srcdir)/../grpcAdaptor
VENDOR_PATH = $(top_srcdir)/../../vendor

noinst_LIBRARIES = libadaptor.a

libadaptor_a_CPPFLAGS = -std=c++11 -fpermissive -Wwrite-strings -fPIC \
						-I.. -I$(APP_PATH)/adaptor/include \
						-I$(APP_PATH)/common/Service \
						-I$(APP_PATH)/services/cucp/source \
						-I$(APP_PATH)/services/grpc/include \
						-I$(VENDOR_PATH)/grpc/dist/include \
						-I$(VENDOR_PATH)/grpc/third_party/protobuf/src

libadaptor_a_SOURCES = ../source/GrpcAdp.cpp \
						../source/GrpcAdpCntrl.cpp \
						../source/GrpcAdpThreads.cpp \
						../source/GrpcAdpRpcServerThread.cpp \
						../source/GrpcAdpRpcClientThread.cpp

#######################################################
LDFLAGS_W_ARCHIVE = -Wl,--whole-archive
LDFLAGS_N_ARCHIVE = --no-whole-archive

lib_LTLIBRARIES = libGrpcAdaptor.la

libGrpcAdaptor_la_CPPFLAGS = -fPIC

libGrpcAdaptor_la_SOURCES =

libGrpcAdaptor_la_LDFLAGS = \
	-avoid-version -lreadline \
	$(LDFLAGS_W_ARCHIVE),$(APP_PATH)/common/objs/Service/libService.a,$(LDFLAGS_N_ARCHIVE) \
	$(LDFLAGS_W_ARCHIVE),$(APP_PATH)/services/cucp/objs/libCucpService.a,$(LDFLAGS_N_ARCHIVE) \
	$(LDFLAGS_W_ARCHIVE),$(APP_PATH)/services/grpc/objs/libgrpcproto.a,$(LDFLAGS_N_ARCHIVE) \
	$(LDFLAGS_W_ARCHIVE),libadaptor.a,$(LDFLAGS_N_ARCHIVE)
