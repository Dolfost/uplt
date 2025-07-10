#include <uplt/port_spec_dialog.hpp>

#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>

namespace uplt {

port_spec_dialog::port_spec_dialog(QWidget *parent, Qt::WindowFlags f) {
	setWindowTitle("Adding new serial connection");
	auto main_lay = new QVBoxLayout;
	auto name_lay = new QHBoxLayout;
	auto alias_lay = new QHBoxLayout;
	auto data_stop_baud_lay = new QHBoxLayout;

	name_lay->addWidget(new QLabel("Name"));
	auto name_text = new QLineEdit;
	name_text->setPlaceholderText("COM1 or /dev/cu.usbserial1120");
	connect( 
		name_text, &QLineEdit::textEdited,
		[&, name_text]() { m_spec.name = name_text->text().toStdString(); }
	);
	name_lay->addWidget(name_text);

	alias_lay->addWidget(new QLabel("Alias"));
	auto alias_text = new QLineEdit;
	alias_text->setPlaceholderText("eq. thermometer");
	connect( 
		alias_text, &QLineEdit::textEdited,
		[&, alias_text]() { m_spec.alias = alias_text->text().toStdString(); }
	);
	alias_lay->addWidget(alias_text);


	auto data_bits = new QSpinBox;
	data_bits->setRange(5, 8);
	data_bits->setValue(m_spec.data_bits);
	connect( 
		data_bits, &QSpinBox::valueChanged,
		[&, data_bits]() { m_spec.data_bits = data_bits->value(); } 
	);
	data_bits->setPrefix("Data bits ");
	data_stop_baud_lay->addWidget(data_bits);

	data_stop_baud_lay->addSpacing(20);

	auto stop_bits = new QSpinBox;
	stop_bits->setRange(1, 2);
	stop_bits->setValue(m_spec.stop_bits);
	connect( 
		stop_bits, &QSpinBox::valueChanged,
		[&, stop_bits]() { m_spec.stop_bits = stop_bits->value(); } 
	);
	stop_bits->setPrefix("Stop bits ");
	data_stop_baud_lay->addWidget(stop_bits);

	auto baud = new QSpinBox;
	baud->setRange(1, 999999999);
	baud->setValue(m_spec.baud);
	connect( 
		baud, &QSpinBox::valueChanged,
		[&, baud]() { m_spec.baud = baud->value(); } 
	);
	baud->setPrefix("BAUD ");
	data_stop_baud_lay->addWidget(baud);


	auto buttons_lay = new QHBoxLayout;
	auto cancel = new QPushButton("Cancel");
	auto confirm = new QPushButton("Confirm");
	buttons_lay->addWidget(cancel);
	buttons_lay->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Expanding));
	buttons_lay->addWidget(confirm);

	confirm->setDefault(true);
	connect( 
		confirm, &QPushButton::pressed,
		this, &port_spec_dialog::accept
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

}
