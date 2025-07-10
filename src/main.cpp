#include <QApplication>
#include <QDebug>

#include <uplt/main_window.hpp>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	uplt::MainWindow mainWindow = uplt::MainWindow();
	mainWindow.show();

	return app.exec();
}
