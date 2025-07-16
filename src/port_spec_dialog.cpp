#include <uplt/port_spec_dialog.hpp>

#include <QPushButton>
#include <QLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>

namespace uplt {

port_spec_dialog::port_spec_dialog(QWidget *parent, Qt::WindowFlags f) {
	setWindowTitle("Adding new serial connection");
	auto main_lay = new QVBoxLayout;
	auto grid_lay = new QGridLayout;
	auto data_stop_baud_lay = new QHBoxLayout;

	grid_lay->addWidget(new QLabel("Name"), 0, 0);
	m_name_text->setPlaceholderText("COM1 or /dev/cu.usbserial1120");
	connect( 
		m_name_text, &QLineEdit::textEdited,
		[this]() { m_spec.name = m_name_text->text().toStdString(); }
	);
	grid_lay->addWidget(m_name_text, 0, 1);

	grid_lay->addWidget(new QLabel("Alias"), 1, 0);
	m_alias_text->setPlaceholderText("eq. thermometer");
	connect( 
		m_alias_text, &QLineEdit::textEdited,
		[this]() { m_spec.alias = m_alias_text->text().toStdString(); }
	);
	grid_lay->addWidget(m_alias_text, 1, 1);

	grid_lay->addWidget(new QLabel("y(x) = "), 2, 0);
	m_fx_text->setPlaceholderText("sqrt(x) + log(e) + pi");
	connect( 
		m_fx_text, &QLineEdit::textEdited,
		[this]() { m_spec.fx = m_fx_text->text().toStdString(); }
	);
	grid_lay->addWidget(m_fx_text, 2, 1);


	m_data_bits->setRange(5, 8);
	connect( 
		m_data_bits, &QSpinBox::valueChanged,
		[this]() { m_spec.data_bits = m_data_bits->value(); } 
	);
	m_data_bits->setPrefix("Data bits ");
	data_stop_baud_lay->addWidget(m_data_bits);

	data_stop_baud_lay->addSpacing(20);

	m_stop_bits->setRange(1, 2);
	m_stop_bits->setValue(m_spec.stop_bits);
	connect( 
		m_stop_bits, &QSpinBox::valueChanged,
		[this]() { m_spec.stop_bits = m_stop_bits->value(); } 
	);
	m_stop_bits->setPrefix("Stop bits ");
	data_stop_baud_lay->addWidget(m_stop_bits);

	m_baud->setRange(1, 999999999);
	m_baud->setValue(m_spec.baud);
	connect( 
		m_baud, &QSpinBox::valueChanged,
		[this]() { m_spec.baud = m_baud->value(); } 
	);
	m_baud->setPrefix("Baud ");
	data_stop_baud_lay->addWidget(m_baud);


	auto buttons_lay = new QHBoxLayout;
	auto cancel = new QPushButton("Cancel");
	auto confirm = new QPushButton("Confirm");
	buttons_lay->addWidget(cancel);
	buttons_lay->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Expanding));
	buttons_lay->addWidget(confirm);

	confirm->setDefault(true);
	connect( 
		confirm, &QPushButton::pressed,
		[this]() {
			if (m_name_text->text().isEmpty()) {
				QMessageBox::critical(this, "No port name", "Port name can not be empty");
				return;
			}
			if (m_parser and m_symbol_table) {
				if (not (m_spec.ex = m_parser->compile(m_fx_text->text().toStdString(), *m_symbol_table))) {
					QMessageBox::critical(this, "Invalid transform expression", QString("y(x) compilation error: %1").arg(m_parser->error().c_str()));
					return;
				}
			}
			accept();
		}
	);
	connect( 
		cancel, &QPushButton::pressed,
		this, &port_spec_dialog::reject
	);

	main_lay->addLayout(grid_lay);
	main_lay->addLayout(data_stop_baud_lay);
	main_lay->addLayout(buttons_lay);
	setLayout(main_lay);

	set_spec(m_spec);
}

void port_spec_dialog::set_spec(const port_spec& spec) {
	m_spec = spec;
	m_name_text->setText(QString::fromStdString(m_spec.name));
	m_alias_text->setText(QString::fromStdString(m_spec.alias));
	m_fx_text->setText(QString::fromStdString(m_spec.fx));
	m_data_bits->setValue(m_spec.data_bits);
	m_stop_bits->setValue(m_spec.stop_bits);
	m_baud->setValue(m_spec.baud);
}

}
