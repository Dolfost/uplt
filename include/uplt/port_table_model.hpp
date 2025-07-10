#ifndef _PART_TABLE_MODEDL_HPP_
#define _PART_TABLE_MODEDL_HPP_

#include <QAbstractTableModel>
#include <QSerialPort>
#include <qcustomplot.h>

#include <memory>
#include <vector>

namespace uplt { 

class port_table_model: public QAbstractTableModel { 
	Q_OBJECT
public:
	using port_list = std::vector<std::unique_ptr<QSerialPort>>;

public:
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
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

	void add_port(QSerialPort* port);

protected:
	port_list m_ports;
};

}

#endif // !_PART_TABLE_MODEDL_HPP_
