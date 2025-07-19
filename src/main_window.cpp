#include <uplt/main_window.hpp>

#include <QLabel>
#include <QTableView>
#include <QSplitter>

#include <uplt/port_table_view.hpp>
#include <uplt/port_spec_dialog.hpp>

//  TODO: add "reopen" context menu option in port table view
//  TODO: add ability to recieve and process multibyte data

namespace uplt {

MainWindow::MainWindow(
	QWidget* parent, 
	Qt::WindowFlags flags
): QMainWindow(parent, flags) {
	setWindowTitle("USART Plotter");
	setup_menubar();

	m_lay->setContentsMargins(0,0,0,0);
	m_central_widget->setLayout(m_lay);
	setCentralWidget(m_central_widget);

	m_transformation_symbol_table.add_pi();
	m_transformation_symbol_table.add_variable("x", x);

	m_plot->setAutoAddPlottableToLegend(true);
	m_plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

	m_graph_table_splitter->addWidget(m_plot);

	auto table_message_widget = new QWidget;
	table_message_widget->setLayout(new QVBoxLayout);
	table_message_widget->layout()->setContentsMargins(0,0,0,0);
	table_message_widget->layout()->addWidget(m_table_message_splitter);
	auto table = new uplt::port_table_view(this);
	table->setModel(m_ports);
	table->set_symbol_table(&m_transformation_symbol_table);
	table->set_parser(&m_parser);
	m_table_message_splitter->addWidget(table);
	m_table_message_splitter->addWidget(m_messages);
	m_table_message_splitter->setSizes({1, 0});
	m_graph_table_splitter->addWidget(table_message_widget);
	m_lay->addWidget(m_graph_table_splitter);
	m_graph_table_splitter->setStretchFactor(0, 1);
	m_graph_table_splitter->setStretchFactor(1, 0);

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

	connect(
		m_ports, &port_table_model::port_added,
		this, &MainWindow::register_port
	);
	connect(
		m_ports, &port_table_model::port_removed,
		this, &MainWindow::unregister_port
	);

	m_plot->xAxis->setRange(0, 60);
	m_plot->yAxis->setRange(-5, 260);
	m_plot->xAxis->setLabel("Seconds");
	m_plot->yAxis->setLabel("Value");
	m_plot->yAxis->setNumberPrecision(5);
	resize(1000, 500);

#ifdef UPLT_DEBUG 
	m_ports->add_port({UPLT_VIRTUAL_SERIAL_PORT_DIRECTORY "SINE", "sine" }, new graph(m_plot->xAxis, m_plot->yAxis));
	m_ports->add_port({UPLT_VIRTUAL_SERIAL_PORT_DIRECTORY "SQUARE", "square" }, new graph(m_plot->xAxis, m_plot->yAxis));
	m_ports->add_port({UPLT_VIRTUAL_SERIAL_PORT_DIRECTORY "RAMP", "ramp" }, new graph(m_plot->xAxis, m_plot->yAxis));
	m_parser.compile("x", m_ports->ports()[0]->ex);
	m_parser.compile("x", m_ports->ports()[1]->ex);
	m_parser.compile("x", m_ports->ports()[2]->ex);
	start_stop_plotting_action();
#endif
}

void MainWindow::process_input_samples(port* pt) { 
	if (not m_is_plotting)
		return;

	uint8_t sample;
	double when;
	while (not pt->serial->atEnd()) { 
		pt->serial->read(reinterpret_cast<char*>(&sample), 1);
		x = sample;
		when = std::chrono::duration<double>(clock::now() - m_start_time).count();
		pt->graph->addData(when, pt->ex.value());
		qDebug() << QString("%1: %2, %3").arg(pt->name).arg(when).arg(pt->ex.value());
	}

	m_plot->replot();

	if (m_follow_graph) { 
    double width = m_plot->xAxis->range().size();
    m_plot->xAxis->setRange(when - width, when);
	}
}

void MainWindow::clear_timeline_action() { 
	for (auto& p: m_ports->ports()) {
		p->graph->data()->clear();
		m_start_time = clock::now();
	}
	m_plot->replot();
}

void MainWindow::start_stop_plotting_action() { 
	if (m_is_plotting) {
		m_start_stop_action->setText("Start plotting");
		m_is_plotting = false;
	} else {
		m_ports->ports()[0]->serial->readAll();
		m_start_stop_action->setText("Stop plotting");
		m_is_plotting = true;
	}
}

void MainWindow::request_port_registration_action() {
	auto dialog = new uplt::port_spec_dialog(nullptr, this);
	dialog->set_parser(&m_parser);
	dialog->set_symbol_table(&m_transformation_symbol_table);
	dialog->exec();
	if (dialog->result() == QDialog::Accepted) {
		auto g = new graph(m_plot->xAxis, m_plot->yAxis);
		g->setAntialiased(m_antialiasing);
		if (m_ports->rowCount() == 0)
			m_start_time = clock::now();
		m_ports->add_port(dialog->spec(), g);
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
	p->ex.register_symbol_table(m_transformation_symbol_table);
}

void MainWindow::unregister_port(port* p) {
	m_plot->removePlottable(p->graph);
}

void MainWindow::show_message(const QString& msg) {
	if (not m_table_message_splitter->sizes()[1]) {
		m_table_message_splitter->setSizes({3, 1});
		m_messages->verticalScrollBar()->setValue(m_messages->verticalScrollBar()->maximum());
		if (not m_graph_table_splitter->sizes()[1])
			m_graph_table_splitter->setSizes({3, 1});
	}
	m_messages->append(QString("%1: %2").arg(QDateTime::currentDateTime().time().toString()).arg(msg));
}

void MainWindow::message_serial_port_error_string(QSerialPort* sp) {
	if (sp->error() == QSerialPort::NoError)
		return;
	show_message(QString("%1: %2").arg(sp->portName()).arg(sp->errorString()));
}

void MainWindow::set_graph_following_action(bool state) {
	m_follow_graph = state;
	if (m_follow_graph) {
		m_plot->axisRect()->setRangeDrag(Qt::Vertical);
	} else  {
		m_plot->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
	}
}

void MainWindow::antialiasing_action(bool state) {
	m_antialiasing = state;
	auto n = m_plot->graphCount();
	for (decltype(n) i = 0; i < n; i++)
		m_plot->graph(i)->setAntialiased(m_antialiasing);
}

void MainWindow::export_image_action() {
	auto dialog = new QFileDialog(this);
	dialog->setAcceptMode(QFileDialog::AcceptSave);
	dialog->setNameFilter("PNG Image (*.png)");
	dialog->setDefaultSuffix("png");
	dialog->selectFile("image.png");
	dialog->setWindowTitle("Save Image As...");
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	connect( 
		dialog, &QFileDialog::fileSelected,
		[=](const QString& file) {
			m_plot->savePng(file, 0, 0, 1, -1, 100);
		}
	);
	dialog->show();
}

void MainWindow::setup_menubar() {
	auto m = menuBar();
	QAction* a;

	auto file = m->addMenu("Ports");
	connect( 
		file->addAction("Add port"), &QAction::triggered,
		this, &MainWindow::request_port_registration_action
	);

	auto view = m->addMenu("View");
	connect( 
		(m_start_stop_action = view->addAction("Plotting")), &QAction::triggered,
		this, &MainWindow::start_stop_plotting_action
	);
	connect( 
		view->addAction("Clear timeline"), &QAction::triggered,
		this, &MainWindow::clear_timeline_action
	);
	a = view->addAction("Follow graph");
	a->setCheckable(true);
	connect( 
		a, &QAction::toggled,
		this, &MainWindow::set_graph_following_action
	);
	a = view->addAction("Antialiasing");
	a->setCheckable(true);
	connect( 
		a, &QAction::toggled,
		this, &MainWindow::antialiasing_action
	);
	connect( 
		view->addAction("Export image"), &QAction::triggered,
		this, &MainWindow::export_image_action
	);
}

}
