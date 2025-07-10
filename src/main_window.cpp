#include <uplt/main_window.hpp>

#include <QLabel>
#include <QTableView>
#include <QSplitter>

#include <uplt/port_table_view.hpp>

namespace uplt {

MainWindow::MainWindow(
	QWidget* parent, 
	Qt::WindowFlags flags
): QMainWindow(parent, flags) {
	setWindowTitle("USART Plotter");

	m_lay->setContentsMargins(0,0,0,0);
	m_central_widget->setLayout(m_lay);
	setCentralWidget(m_central_widget);

	m_clear_button->setText("Clear");
	m_start_stop_button->setText("Start");
	connect(
		m_start_stop_button, &QPushButton::pressed, 
		this, &MainWindow::start_stop_button_pressed 
	);

	m_expression_symbols.add_pi();
	m_expression_symbols.add_variable("x", x);
	m_expression.register_symbol_table(m_expression_symbols);

	m_plot->setAutoAddPlottableToLegend(true);
	m_plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
	m_graph = new QCPGraph(m_plot->xAxis, m_plot->yAxis);

	auto splitter = new QSplitter(Qt::Horizontal);
	splitter->addWidget(m_plot);

	auto table = new uplt::port_table_view;
	table->setModel(m_ports);
	splitter->addWidget(table);
	m_lay->addWidget(splitter);

	connect( 
		m_ports, &QAbstractItemModel::rowsInserted, 
		table, &port_table_view::resizeColumnsToContents
	);
	connect( 
		m_ports, &QAbstractItemModel::rowsInserted, 
		table, &port_table_view::resizeRowsToContents
	);

	auto hlay = new QHBoxLayout;
	hlay->setContentsMargins(10,0,10,10);
	m_lay->addLayout(hlay);
	hlay->addWidget(new QLabel("y(x) = "));
	hlay->addWidget(m_transform_line_edit);
	hlay->addWidget(m_clear_button);
	hlay->addWidget(m_start_stop_button);

	m_port->setBaudRate(9600);
	m_port->setDataBits(QSerialPort::Data8);
	m_port->setStopBits(QSerialPort::OneStop);
	m_port->setPortName("/dev/cu.usbserial-1120");
	m_port->open(QSerialPort::ReadOnly);

	m_ports->add_port(m_port);
	
	connect(
		m_port, &QSerialPort::readyRead,
		this, &MainWindow::append_sample 
	);
}

void MainWindow::append_sample() { 
	if (not m_is_plotting)
		return;

	if (not m_parser.compile(m_transform_line_edit->text().toStdString(), m_expression)) { 
		return;
	}

	QCPGraphData data;

	while (not m_port->atEnd()) { 
		uint8_t sample;
		m_port->read(reinterpret_cast<char*>(&sample), 1);
		x = sample;
		m_graph->addData(m_t++, m_expression.value());
		qDebug() << QString("%1, %2").arg(m_t - 1).arg(sample);
	}

	m_plot->replot();
}

void MainWindow::clear_timeline() { 
	m_graph->data().clear();
	m_plot->replot();
}

void MainWindow::start_stop_button_pressed() { 
	if (m_is_plotting) {
		m_start_stop_button->setText("Start");
		m_is_plotting = false;
	} else {
		m_port->flush();
		m_start_stop_button->setText("Stop");
		m_is_plotting = true;
	}
}

}
