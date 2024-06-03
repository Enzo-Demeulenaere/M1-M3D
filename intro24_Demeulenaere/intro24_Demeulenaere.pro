# relative paths to lib p3d and media
P3D_PATH = p3d
MEDIA_PATH = ../media

# common QT project configuration
QT += widgets opengl
CONFIG += debug_and_release
CONFIG += c++17
CONFIG(debug,debug|release) {
  TARGET=$$join(TARGET,,,"_d")
}
TEMPLATE = app

DESTDIR = $$_PRO_FILE_PWD_/bin

DEFINES+=GLEW_NO_GLU
DEFINES += MEDIA_PATH=\\\"$$MEDIA_PATH\\\"
DEFINES+= RENDER_SHADER_PATH=\\\"$$MEDIA_PATH/p3dShader\\\"



# mac os config : ** not tested **
macx {
    INCLUDEPATH += /opt/local/include/
    LIBS += -framework OpenGL
}



# win32 config : tested on win10 64bits (qt mingw)
win32 {
  CONFIG += console
  DEFINES+=GLEW_STATIC _STDCALL_SUPPORTED
  LIBS+= -lopengl32
}


INCLUDEPATH += src/p3d src/p3d/glad/ src/application/ src/p3d/gui/ 
DEPENDPATH += src/p3d/glad/ src/application/ src/p3d/gui/ 

SOURCES += \
	src/p3d/glad/gl.c \
	src/application/GLApplication.cpp \
	src/p3d/gui/Tools.cpp \
	src/p3d/gui/GLWidget.cpp \
	src/p3d/gui/GLText.cpp \
	src/p3d/gui/main.cpp \
	src/p3d/gui/MainWindow.cpp
HEADERS += \
	src/p3d/glad/gl.h \
	src/application/GLApplication.h \
	src/p3d/gui/Tools.h \
	src/p3d/glad/glsupport.h \
	src/p3d/gui/GLText.h \
	src/p3d/gui/GLWidget.h \
	src/p3d/gui/MainWindow.h
OTHER_FILES +=\
	media/lagoon.jpg\
	media/simple.frag\
	media/simple.vert
