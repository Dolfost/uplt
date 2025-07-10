#ifndef _UPLT_PORT_SPEC_DIALOG_HPP_
#define _UPLT_PORT_SPEC_DIALOG_HPP_

#include <QDialog>

#include <uplt/port_spec.hpp>

namespace uplt {

class port_spec_dialog: public QDialog { 
	Q_OBJECT 
public:
	port_spec_dialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	const port_spec& spec() const { return m_spec; }

protected: 
	port_spec m_spec;
};

}

#endif // !_UPLT_PORT_SPEC_DIALOG_HPP_
