#include <uplt/main_window.hpp>

#include <QLabel>
#include <QTableView>
#include <QSplitter>

#include <uplt/port_table_view.hpp>
#include <uplt/port_spec_dialog.hpp>

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
	m_add_port_button->setText("Add port...");
	connect(
		m_start_stop_button, &QPushButton::pressed, 
		this, &MainWindow::start_stop_button_pressed 
	);
	connect(
		m_add_port_button, &QPushButton::pressed, 
		this, &MainWindow::add_port 
	);
	connect(
		m_clear_button, &QPushButton::pressed, 
		this, &MainWindow::clear_timeline 
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
	hlay->addWidget(m_add_port_button);
	hlay->addWidget(m_clear_button);
	hlay->addWidget(m_start_stop_button);

	connect(
		m_ports, &port_table_model::port_added,
		[&](port* port) {
			if (not port->serial->isOpen())
				return;
			connect( 
				port->serial.get(), &QSerialPort::readyRead,
				[this, port]() { process_input_samples(port); }
			);
		}
	);
}

void MainWindow::process_input_samples(port* pt) { 
	if (not m_is_plotting)
		return;

	if (not m_parser.compile(m_transform_line_edit->text().toStdString(), m_expression))
		return;

	while (not pt->serial->atEnd()) { 
		uint8_t sample;
		pt->serial->read(reinterpret_cast<char*>(&sample), 1);
		x = sample;
		m_graph->addData(m_t++, m_expression.value());
		qDebug() << QString("%1: %2, %3").arg(pt->name).arg(m_t - 1).arg(sample);
	}

	m_plot->replot();
}

void MainWindow::clear_timeline() { 
	m_graph->data().clear();
	m_t = 0;
	m_plot->replot();
}

void MainWindow::start_stop_button_pressed() { 
	if (m_is_plotting) {
		m_start_stop_button->setText("Start");
		m_is_plotting = false;
	} else {
		m_ports->ports()[0]->serial->readAll();
		m_start_stop_button->setText("Stop");
		m_is_plotting = true;
	}
}

void MainWindow::add_port() {
	auto dialog = new uplt::port_spec_dialog(this);
	dialog->exec();
	if (dialog->result() == QDialog::Accepted)
		m_ports->add_port(dialog->spec());
	delete dialog;
}

}
