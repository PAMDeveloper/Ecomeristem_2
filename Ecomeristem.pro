QT       += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 precompile_header
TEMPLATE = app
CONFIG += static
#CONFIG += console

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
##MSVC OPTIM FLAGS
QMAKE_CXXFLAGS_RELEASE += -Ox
##MINGW OPTIM FLAGS
#QMAKE_CXXFLAGS_RELEASE += -Ofast

## DEBUG OUTPUT
#QMAKE_CXXFLAGS += -save-temps #preprocessed headers
#QMAKE_CXXFLAGS += -P #preprocessed headers
#QMAKE_CFLAGS_RELEASE += -E

NAME = ecomeristem

SRC_ = src/
R_SRC_ = ../rEcomeristem/src
ARTIS_SRC_ = ../artis/src
3P_LIBS_ = ../../ext_libs
LIBS_ = ../../libs


ARCHI = x64
COMPILER = msvc14
#COMPILER = mingw-4.9.3
LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
LIBS += -L$$3P_LIBS_/$$COMPILER/$$ARCHI/static
LIBS += -L$$3P_LIBS_/$$COMPILER/$$ARCHI/static -llibpq
DEPENDPATH += -L$$3P_LIBS_/$$COMPILER/$$ARCHI/static

DEFINES += WITH_TRACE FORCE_TRACE_ENUM

CONFIG(debug, debug|release) {
    TARGET = $${NAME}d
    LIBS += -L$$LIBS_/$$COMPILER/$$ARCHI/static -lartisd
} else {
    TARGET = $${NAME}
    LIBS += -L$$LIBS_/$$COMPILER/$$ARCHI/static -lartis
}

LINK = static
equals(TEMPLATE,lib) {
    message(building LIB)
    DESTDIR = $$LIBS_/$$COMPILER/$$ARCHI/$$LINK
} else {
    message(building APP)
    DESTDIR = ../bin/$$COMPILER/$$ARCHI
}

INCLUDEPATH +=  $$3P_LIBS_/include \
                $$SRC_ \
                $$R_SRC_ \
                $$ARTIS_SRC_


message($$TARGET - $$TEMPLATE - $$ARCHI - $$LINK - $$COMPILER)
message(to: $$DESTDIR)

PRECOMPILED_HEADER  = $$R_SRC_/defines.hpp

HEADERS += \
    $$SRC_/qtapp/mainwindow.h \
    $$SRC_/qtapp/meteodatamodel.h \
    $$SRC_/qtapp/parametersdatamodel.h \
    $$SRC_/qtapp/view.h \
    $$SRC_/qtapp/callout.h \
    $$SRC_/qtapp/tracemodel.h \
    \
    $$R_SRC_/ModelParameters.hpp \
    $$R_SRC_/utils/ParametersReader.hpp \
    $$R_SRC_/utils/resultparser.h \
    $$R_SRC_/utils/juliancalculator.h\
    \
    $$R_SRC_/observer/PlantView.hpp \
    $$R_SRC_/plant/floralorgan/PanicleModel.hpp \
    $$R_SRC_/plant/floralorgan/PeduncleModel.hpp \
    $$R_SRC_/plant/phytomer/InternodeModel.hpp \
    $$R_SRC_/plant/phytomer/LeafModel.hpp \
    $$R_SRC_/plant/phytomer/PhytomerModel.hpp \
    $$R_SRC_/plant/processes/AssimilationModel.hpp \
    $$R_SRC_/plant/processes/CulmStockModel.hpp \
    $$R_SRC_/plant/processes/CulmStockModelNG.hpp \
    $$R_SRC_/plant/processes/PlantStockModel.hpp \
    $$R_SRC_/plant/processes/ThermalTimeModel.hpp \
    $$R_SRC_/plant/processes/ThermalTimeModelNG.hpp \
    $$R_SRC_/plant/processes/WaterBalanceModel.hpp \
    $$R_SRC_/plant/CulmModel.hpp \
    $$R_SRC_/plant/PlantModel.hpp \
    $$R_SRC_/plant/RootModel.hpp

SOURCES += \
    $$SRC_/qtapp/mainwindow.cpp \
    $$SRC_/qtapp/meteodatamodel.cpp \
    $$SRC_/qtapp/parametersdatamodel.cpp \
    $$SRC_/qtapp/view.cpp \
    $$SRC_/qtapp/callout.cpp \
    $$SRC_/qtapp/tracemodel.cpp \
    $$SRC_/main.cpp

FORMS += \
    $$SRC_/qtapp/mainwindow.ui
