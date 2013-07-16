netcdfLoad_SOURCES = \
	src/main.cpp \
	src/localtime.h \
	src/localtime.cpp \
	src/NetcdfFile.h \
	src/NetcdfFile.cpp \
	src/NetcdfField.h \
	src/NetcdfField.cpp \
	src/WriteQuery.h \
	src/WriteQuery.cpp \
	src/NetcdfTranslator.h \
	src/NetcdfTranslator.cpp \
	src/configuration/CdmLoaderConfiguration.h \
	src/configuration/CdmLoaderConfiguration.cpp \
	src/configuration/LoadElement.h \
	src/configuration/LoadElement.cpp \
	src/configuration/LoadConfiguration.h \
	src/configuration/LoadConfiguration.cpp \
	src/configuration/parameter/DataSpecification.h \
	src/configuration/parameter/DataSpecification.cpp \
	src/configuration/parameter/TimeSpecification.h \
	src/configuration/parameter/TimeSpecification.cpp

OTHER = \
	src/CdmLoader.h \
	src/CdmLoader.cpp \
	src/NetcdfLoader.h \
	src/NetcdfLoader.cpp 
	


EXTRA_DIST += src/netcdfLoad.mk
