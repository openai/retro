#include <QApplication>
#include <QSurfaceFormat>

#include "MainWindow.h"
#include "EmulatorController.h"
#include "Screen.h"

#ifdef QT_STATIC
#include <QtPlugin>
#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif
#endif

int main(int argc, char* argv[]) {
	QCoreApplication::setOrganizationName("OpenAI");
	QCoreApplication::setOrganizationDomain("openai.com");
	QCoreApplication::setApplicationName("gym-retro-integration");
#ifdef Q_OS_WIN
	QCoreApplication::setAttribute(Qt::AA_NativeWindows);
#endif

	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(0);
	format.setSwapInterval(0);
	format.setSwapBehavior(QSurfaceFormat::SingleBuffer);
	QSurfaceFormat::setDefaultFormat(format);

	EmulatorController controller;

	MainWindow window;
	window.show();

	window.screen()->installEventFilter(&controller);
	window.setEmulator(&controller);

	if (app.arguments().length() == 2) {
		window.openAny(app.arguments()[1]);
	}

	app.exec();

	return 0;
}
