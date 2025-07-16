#ifndef _PORT_TABLE_VIEW_HPP_
#define _PORT_TABLE_VIEW_HPP_

#include <QTableView>

#include <exprtk.hpp>

#include <uplt/port_table_model.hpp>

namespace uplt { 

class port_table_view: public QTableView { 
Q_OBJECT
public: 
	port_table_view();

public: 
	exprtk::symbol_table<double>* symbol_table() { return m_symbol_table; }
	void set_symbol_table(exprtk::symbol_table<double>* st) { m_symbol_table = st; }
	exprtk::parser<double>* parser() { return m_parser; }
	void set_parser(exprtk::parser<double>* par) { m_parser = par; }

protected:
	exprtk::symbol_table<double>* m_symbol_table = nullptr;
	exprtk::parser<double>* m_parser = nullptr;

protected slots:
	void show_context_menu(const QPoint& pos);

protected slots:
	void edit_action(port* p, std::size_t row);
	void delete_action(port* p, std::size_t row);
	void export_data_action(port* p, std::size_t row);

};

}

#endif // !_PORT_TABLE_VIEW_HPP_
