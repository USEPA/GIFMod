ARCH = plateformarch
win32 { ARCH = $$QMAKE_TARGET.arch }
unix { ARCH = $$QMAKE_HOST.arch }
BUILD_CONFIG = buildconf

CONFIG(debug, debug|release) { BUILD_CONFIG = debug }
CONFIG(release, debug|release) { BUILD_CONFIG = release }

BUILD_DIR = $$PWD/builds/$${BUILD_CONFIG}
DEPLOY_DIR = $$PWD/builds/$${BUILD_CONFIG}


