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
		this, &MainWindow::request_port_registration 
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

	auto table_message_widget = new QWidget;
	table_message_widget->setLayout(new QVBoxLayout);
	table_message_widget->layout()->setContentsMargins(0,0,0,0);
	auto table_message_splitter = new QSplitter(Qt::Vertical);
	table_message_widget->layout()->addWidget(table_message_splitter);
	auto table = new uplt::port_table_view;
	table->setModel(m_ports);
	table_message_splitter->addWidget(table);
	table_message_splitter->addWidget(m_messages);
	splitter->addWidget(table_message_widget);
	m_lay->addWidget(splitter);

	m_messages->setPlaceholderText("Messages will appear here");
	m_messages->setReadOnly(true);

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
		this, &MainWindow::register_port
	);
	connect(
		m_ports, &port_table_model::port_removed,
		this, &MainWindow::unregister_port
	);
}

void MainWindow::process_input_samples(port* pt) { 
	if (not m_is_plotting)
		return;

	if (not m_parser.compile(m_transform_line_edit->text().toStdString(), m_expression))
		return;

	uint8_t sample;
	while (not pt->serial->atEnd()) { 
		pt->serial->read(reinterpret_cast<char*>(&sample), 1);
		x = sample;
		pt->graph->addData(m_t++, m_expression.value());
		qDebug() << QString("%1: %2, %3").arg(pt->name).arg(m_t - 1).arg(sample);
	}

	m_plot->replot();
}

void MainWindow::clear_timeline() { 
	for (auto& p: m_ports->ports()) {
		p->graph->data()->clear();
	}
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

void MainWindow::request_port_registration() {
	auto dialog = new uplt::port_spec_dialog(this);
	dialog->exec();
	if (dialog->result() == QDialog::Accepted) {
		m_ports->add_port(dialog->spec(), new graph(m_plot->xAxis, m_plot->yAxis));
		if (not m_ports->ports().back()->serial->isOpen())
			message_serial_port_error_string(m_ports->ports().back()->serial.get());
	}
	delete dialog;
}

void MainWindow::register_port(port* p) {
	connect( 
		p->serial.get(), &QSerialPort::readyRead,
		[this, p]() { process_input_samples(p); }
	);
	connect( 
		p->serial.get(), &QSerialPort::errorOccurred,
		[this, p]() { message_serial_port_error_string(p->serial.get()); }
	);
}

void MainWindow::unregister_port(port* p) {
}

void MainWindow::show_message(const QString& msg) {
	m_messages->append(QString("%1: %2").arg(QDateTime::currentDateTime().time().toString()).arg(msg));
}

void MainWindow::message_serial_port_error_string(const QSerialPort* sp) {
	show_message(QString("%1: %2").arg(sp->portName()).arg(sp->errorString()));
}

}
