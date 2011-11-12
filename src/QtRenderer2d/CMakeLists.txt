set(QtRenderer2d_SOURCES main_qt.cpp QtRenderer2d.cpp JCCQWidget.cpp QOpenGLWidget.cpp)
set(QtRenderer2d_HEADERS JCCQWidget.h QOpenGLWidget.h)

QT4_WRAP_CPP(QtRenderer2d_HEADERS_MOC $(QtRenderer2d_HEADERS))

set(QT_USE_QTOPENGL TRUE)

add_library(QtRenderer2d $(QtRenderer2d_SOURCES) $(QtRenderer2d_HEADERS_MOC))
set(LIBS $(LIBS) QtRenderer2d)
