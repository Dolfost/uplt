#ifndef _UPLT_PORT_SPEC_DIALOG_HPP_
#define _UPLT_PORT_SPEC_DIALOG_HPP_

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>

#include <uplt/port_spec.hpp>

namespace uplt {

class port_spec_dialog: public QDialog { 
	Q_OBJECT 
public:
	port_spec_dialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	const port_spec& spec() const { return m_spec; }
	void set_spec(const port_spec& spec);

protected:
	QLineEdit* m_name_text = new QLineEdit;
	QLineEdit* m_alias_text = new QLineEdit;
	QSpinBox* m_data_bits = new QSpinBox;
	QSpinBox* m_stop_bits = new QSpinBox;
	QSpinBox* m_baud = new QSpinBox;

protected: 
	port_spec m_spec;
};

}

#endif // !_UPLT_PORT_SPEC_DIALOG_HPP_
