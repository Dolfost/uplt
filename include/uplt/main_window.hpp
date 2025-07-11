#ifndef _MAIN_WINDOW_HPP_
#define _MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QSplitter>
#include <QSpinBox>
#include <QLineEdit>
#include <QSerialPort>

#include <qcustomplot.h>
#include <exprtk.hpp>

#include <uplt/port_table_model.hpp>

namespace uplt {

class MainWindow: public QMainWindow {
	Q_OBJECT
	public:
	MainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

protected slots:
	void clear_timeline_action();
	void start_stop_action();
	void set_graph_following_action(bool state);
	void request_port_registration_action();

protected slots:
	void process_input_samples(port* pt);

protected slots:
	void register_port(port*);
	void unregister_port(port*);

protected slots:
	void show_message(const QString& msg);
	void message_serial_port_error_string(QSerialPort*);

private:
	void setup_menubar();

private:
	QVBoxLayout* m_lay = new QVBoxLayout;
	QWidget* m_central_widget = new QWidget;

	exprtk::symbol_table<double> m_transformation_symbol_table;
	exprtk::expression<double> m_expression;
	exprtk::parser<double> m_parser;
	double x;

	QCustomPlot* m_plot = new QCustomPlot;
	QTextEdit* m_messages = new QTextEdit;
	QSplitter* m_table_message_splitter = new QSplitter(Qt::Vertical);
	QAction* m_start_stop_action = nullptr;

	bool m_is_plotting = false;
	uint64_t m_t = 0;

	uplt::port_table_model* m_ports = new uplt::port_table_model;
};

}

#endif // !_MAIN_WINDOW_HPP_

