#ifndef _PORT_TABLE_VIEW_HPP_
#define _PORT_TABLE_VIEW_HPP_

#include <QTableView>

#include <uplt/port_table_model.hpp>

namespace uplt { 

class port_table_view: public QTableView { 
Q_OBJECT
	public: 
	port_table_view();

protected slots:
	void show_context_menu(const QPoint& pos);

protected slots:
	void edit_action(port* p, std::size_t row);
	void delete_action(port* p, std::size_t row);

};

}

#endif // !_PORT_TABLE_VIEW_HPP_
