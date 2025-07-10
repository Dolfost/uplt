#ifndef _PART_TABLE_MODEDL_HPP_
#define _PART_TABLE_MODEDL_HPP_

#include <QAbstractTableModel>
#include <QSerialPort>
#include <qcustomplot.h>

#include <memory>
#include <vector>

#include <uplt/port_spec.hpp>

namespace uplt { 

struct port: public port_spec {
	port(QSerialPort* p, const port_spec& ps): port_spec(ps), serial(p) {};
	port() = delete;
	std::unique_ptr<QSerialPort> serial;
};

class port_table_model: public QAbstractTableModel { 
	Q_OBJECT
public:
	using port_list = std::vector<port>;

public:
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	enum column { 
		plot_icon,
		name, 
		data_bits,
		stop_bits,
		baud,
		columns
	};

	void add_port(const port_spec& port);
	port_list& ports();

protected:
	port_list m_ports;
};

}

#endif // !_PART_TABLE_MODEDL_HPP_
