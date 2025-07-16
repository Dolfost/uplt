#ifndef _UPLT_PORT_SPEC_DIALOG_HPP_
#define _UPLT_PORT_SPEC_DIALOG_HPP_

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>

#include <exprtk.hpp>

#include <uplt/port_spec.hpp>

namespace uplt {

class port_spec_dialog: public QDialog { 
	Q_OBJECT 
public:
	port_spec_dialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	const port_spec& spec() const { return m_spec; }
	void set_spec(const port_spec& spec);

	exprtk::symbol_table<double>* symbol_table() { return m_symbol_table; }
	void set_symbol_table(exprtk::symbol_table<double>* st) { m_symbol_table = st; }
	exprtk::parser<double>* parser() { return m_parser; }
	void set_parser(exprtk::parser<double>* par) { m_parser = par; }


protected:
	QLineEdit* m_name_text = new QLineEdit;
	QLineEdit* m_alias_text = new QLineEdit;
	QLineEdit* m_fx_text = new QLineEdit;
	QSpinBox* m_data_bits = new QSpinBox;
	QSpinBox* m_stop_bits = new QSpinBox;
	QSpinBox* m_baud = new QSpinBox;

	exprtk::symbol_table<double>* m_symbol_table = nullptr;
	exprtk::parser<double>* m_parser = nullptr;

protected: 
	port_spec m_spec;
};

}

#endif // !_UPLT_PORT_SPEC_DIALOG_HPP_
