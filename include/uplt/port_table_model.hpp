#ifndef _PART_TABLE_MODEDL_HPP_
#define _PART_TABLE_MODEDL_HPP_

#include <QAbstractTableModel>
#include <QSerialPort>

#include <qcustomplot.h>
#include <exprtk.hpp>

#include <memory>
#include <vector>
#include <cstdint>

#include <uplt/port_spec.hpp>
#include <uplt/graph.hpp>

namespace uplt { 

struct port: public port_spec {
	port(QSerialPort* p, const port_spec& ps): port_spec(ps), serial(p) {};
	port() = delete;
	uint64_t sample_no = 0;
	std::unique_ptr<QSerialPort> serial;
	graph* graph;
	void apply_settings_to_serial_port() {
		serial->setPortName(QString::fromStdString(name));
		serial->setBaudRate(baud);
		switch (data_bits) { 
			case 5: { serial->setDataBits(QSerialPort::DataBits::Data5); break; }
			case 6: { serial->setDataBits(QSerialPort::DataBits::Data6); break; }
			case 7: { serial->setDataBits(QSerialPort::DataBits::Data7); break; }
			case 8: { serial->setDataBits(QSerialPort::DataBits::Data8); break; }
		}
		switch (stop_bits) { 
			case 1: { serial->setStopBits(QSerialPort::StopBits::OneStop); break; }
			case 2: { serial->setStopBits(QSerialPort::StopBits::TwoStop); break; }
		}
	}
};

class port_table_model: public QAbstractTableModel { 
	Q_OBJECT
public:
	using port_list = std::vector<std::unique_ptr<port>>;

public:
	void add_port(const port_spec& spec, graph* graph);
	void update_port(port* p, const port_spec& spec);
	port_list& ports();

	enum column { 
		plot_icon,
		name, 
		data_bits,
		stop_bits,
		baud,
		transform_expression,
		columns
	};

signals:
	void port_added(port*);
	void port_removed(port*);

public:
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

protected:
	port_list m_ports;
};

}

#endif // !_PART_TABLE_MODEDL_HPP_
