#ifndef _UPLT_UPLT_DIALOG_HPP_
#define _UPLT_UPLT_DIALOG_HPP_
 
#include <QDialog>

#include <uplt/port_table_model.hpp>

namespace uplt {

class dialog: public QDialog {
public:
	dialog(port* p, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()): QDialog(parent, f), m_port(p) {
	}

public slots:
	virtual void on_port_removal(port* p) {
		if (not p)
			return;
		if (p == m_port)
			close();
	}
	
protected:
	port* m_port = nullptr;
};

}

#endif // !_UPLT_UPLT_DIALOG_HPP_
