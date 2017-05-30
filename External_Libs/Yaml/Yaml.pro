QT -= gui
QT -= core


TARGET   = yaml-cpp
TEMPLATE = lib

CONFIG += dll
CONFIG += c++14
CONFIG += warn_on

QMAKE_CXXFLAGS += -Werror=return-type
QMAKE_CXXFLAGS +=-fext-numeric-literals

DEFINES += YAML_CPP_API=Q_DECL_EXPORT
DEFINES += HAS_EXPERIMENTAL_STRING_VIEW
DEFINES -= QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

INCLUDEPATH += src \
               include

DEPENDPATH  += src \
              include

HEADERS +=  include/yaml-cpp/anchor.h                    \
            include/yaml-cpp/binary.h                    \
            include/yaml-cpp/contrib/anchordict.h        \
            include/yaml-cpp/contrib/graphbuilder.h      \
            include/yaml-cpp/dll.h                       \
            include/yaml-cpp/emitfromevents.h            \
            include/yaml-cpp/emitter.h                   \
            include/yaml-cpp/emitterdef.h                \
            include/yaml-cpp/emittermanip.h              \
            include/yaml-cpp/emitterstyle.h              \
            include/yaml-cpp/eventhandler.h              \
            include/yaml-cpp/exceptions.h                \
            include/yaml-cpp/mark.h                      \
            include/yaml-cpp/node/convert.h              \
            include/yaml-cpp/node/detail/bool_type.h     \
            include/yaml-cpp/node/detail/impl.h          \
            include/yaml-cpp/node/detail/iterator.h      \
            include/yaml-cpp/node/detail/iterator_fwd.h  \
            include/yaml-cpp/node/detail/memory.h        \
            include/yaml-cpp/node/detail/node.h          \
            include/yaml-cpp/node/detail/node_data.h     \
            include/yaml-cpp/node/detail/node_iterator.h \
            include/yaml-cpp/node/detail/node_ref.h      \
            include/yaml-cpp/node/emit.h                 \
            include/yaml-cpp/node/impl.h                 \
            include/yaml-cpp/node/iterator.h             \
            include/yaml-cpp/node/node.h                 \
            include/yaml-cpp/node/parse.h                \
            include/yaml-cpp/node/ptr.h                  \
            include/yaml-cpp/node/type.h                 \
            include/yaml-cpp/noncopyable.h               \
            include/yaml-cpp/null.h                      \
            include/yaml-cpp/ostream_wrapper.h           \
            include/yaml-cpp/parser.h                    \
            include/yaml-cpp/stlemitter.h                \
            include/yaml-cpp/traits.h                    \
            include/yaml-cpp/yaml.h                      \
            src/collectionstack.h                        \
            src/contrib/graphbuilderadapter.h            \
            src/directives.h                             \
            src/emitterstate.h                           \
            src/emitterutils.h                           \
            src/exp.h                                    \
            src/indentation.h                            \
            src/nodebuilder.h                            \
            src/nodeevents.h                             \
            src/ptr_vector.h                             \
            src/regex_yaml.h                             \
            src/regeximpl.h                              \
            src/scanner.h                                \
            src/scanscalar.h                             \
            src/scantag.h                                \
            src/setting.h                                \
            src/singledocparser.h                        \
            src/stream.h                                 \
            src/streamcharsource.h                       \
            src/stringsource.h                           \
            src/tag.h                                    \
            src/token.h

SOURCES +=                                      \
            src/binary.cpp                      \
            src/contrib/graphbuilder.cpp        \
            src/contrib/graphbuilderadapter.cpp \
            src/convert.cpp                     \
            src/directives.cpp                  \
            src/emit.cpp                        \
            src/emitfromevents.cpp              \
            src/emitter.cpp                     \
            src/emitterstate.cpp                \
            src/emitterutils.cpp                \
            src/exceptions.cpp                  \
            src/exp.cpp                         \
            src/memory.cpp                      \
            src/node.cpp                        \
            src/node_data.cpp                   \
            src/nodebuilder.cpp                 \
            src/nodeevents.cpp                  \
            src/null.cpp                        \
            src/ostream_wrapper.cpp             \
            src/parse.cpp                       \
            src/parser.cpp                      \
            src/regex_yaml.cpp                  \
            src/scanner.cpp                     \
            src/scanscalar.cpp                  \
            src/scantag.cpp                     \
            src/scantoken.cpp                   \
            src/simplekey.cpp                   \
            src/singledocparser.cpp             \
            src/stream.cpp                      \
            src/tag.cpp



# For Gcc compiler option, see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$OUT_PWD/debug)
  DESTDIR     = $$clean_path($$OUT_PWD/../../Bin/debug)
}
else: CONFIG(release, debug|release) {
  DEFINES += NDEBUG
  OBJECTS_DIR = $$clean_path($$OUT_PWD/release)
  DESTDIR     = $$clean_path($$OUT_PWD/../../Bin/release)
}


