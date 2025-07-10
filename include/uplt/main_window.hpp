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

private slots:
	void process_input_samples(port* pt);
	void clear_timeline();
	void start_stop_button_pressed();
	void add_port();

private:
	QVBoxLayout* m_lay = new QVBoxLayout;
	QWidget* m_central_widget = new QWidget;

	exprtk::symbol_table<double> m_expression_symbols;
	exprtk::expression<double> m_expression;
	exprtk::parser<double> m_parser;
	double x;

	QPushButton* m_start_stop_button = new QPushButton;
	QPushButton* m_clear_button = new QPushButton;
	QPushButton* m_add_port_button = new QPushButton;
	QLineEdit* m_transform_line_edit = new QLineEdit("x");
	QCustomPlot* m_plot = new QCustomPlot;
	QCPGraph* m_graph = nullptr;

	bool m_is_plotting = false;
	uint64_t m_t = 0;

	uplt::port_table_model* m_ports = new uplt::port_table_model;
};

}

#endif // !_MAIN_WINDOW_HPP_

