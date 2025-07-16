#include <uplt/port_spec_dialog.hpp>

#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QMessageBox>

namespace uplt {

port_spec_dialog::port_spec_dialog(QWidget *parent, Qt::WindowFlags f) {
	setWindowTitle("Adding new serial connection");
	auto main_lay = new QVBoxLayout;
	auto name_lay = new QHBoxLayout;
	auto alias_lay = new QHBoxLayout;
	auto data_stop_baud_lay = new QHBoxLayout;

	name_lay->addWidget(new QLabel("Name"));
	m_name_text->setPlaceholderText("COM1 or /dev/cu.usbserial1120");
	connect( 
		m_name_text, &QLineEdit::textEdited,
		[this]() { m_spec.name = m_name_text->text().toStdString(); }
	);
	name_lay->addWidget(m_name_text);

	alias_lay->addWidget(new QLabel("Alias"));
	m_alias_text->setPlaceholderText("eq. thermometer");
	connect( 
		m_alias_text, &QLineEdit::textEdited,
		[this]() { m_spec.alias = m_alias_text->text().toStdString(); }
	);
	alias_lay->addWidget(m_alias_text);


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
			accept();
		}
	);
	connect( 
		cancel, &QPushButton::pressed,
		this, &port_spec_dialog::reject
	);

	main_lay->addLayout(name_lay);
	main_lay->addLayout(alias_lay);
	main_lay->addLayout(data_stop_baud_lay);
	main_lay->addLayout(buttons_lay);
	setLayout(main_lay);
}

void port_spec_dialog::set_spec(const port_spec& spec) {
	m_spec = spec;
	m_name_text->setText(QString::fromStdString(m_spec.name));
	m_alias_text->setText(QString::fromStdString(m_spec.alias));
	m_data_bits->setValue(m_spec.data_bits);
	m_stop_bits->setValue(m_spec.stop_bits);
	m_baud->setValue(m_spec.baud);
}

}
