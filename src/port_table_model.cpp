#include <uplt/port_table_model.hpp>

namespace uplt { 

bool port_table_model::insertRows(int row, int count, const QModelIndex &parent) {
	beginInsertRows(parent, row, row + count - 1);

	for (int i = 0; i < count; ++i)
		m_ports.insert(m_ports.begin() + row + i, std::make_unique<QSerialPort>());

	endInsertRows();
	return true;
}

bool port_table_model::removeRows(int row, int count, const QModelIndex &parent) {
	beginRemoveRows(parent, row, row + count - 1);

	m_ports.erase(m_ports.begin() + row, m_ports.begin() + row + count);

	endRemoveRows();
	return true;
}

int port_table_model::columnCount(const QModelIndex &parent) const { 
	return column::columns;
};

int port_table_model::rowCount(const QModelIndex &parent) const { 
	return m_ports.size();
};

QVariant port_table_model::data(const QModelIndex &index, int role) const { 
	auto row = index.row();
	if (role == Qt::DisplayRole) {
		switch (index.column()) { 
			case column::plot_icon: {
				return QPixmap(100, 100); break; 
			} case column::name: {
				return m_ports[row]->portName(); break; 
			} case column::data_bits: { 
				return m_ports[row]->dataBits(); break; 
			} case column::stop_bits: { 
				return m_ports[row]->stopBits();  break; 
			} case column::baud: { 
				return m_ports[row]->baudRate();  break; 
			}
		}
	}
	return QVariant();
}
QVariant port_table_model::headerData(int section, Qt::Orientation orientation, int role) const { 
	if (role == Qt::DisplayRole) { 
		if (orientation == Qt::Vertical) 
			return section + 1;
		switch (section) { 
			case column::plot_icon: { 
				return "line"; break;
			} case column::name: { 
				return "name"; break;
			} case column::data_bits: { 
				return "db"; break;
			} case column::stop_bits: { 
				return "sb"; break;
			} case column::baud: { 
				return "baud"; break;
			} 
		}
	}
	return QVariant();
}

void port_table_model::add_port(const port_spec& port) {
	beginInsertRows(QModelIndex(), m_ports.size(), m_ports.size());

	auto sp = new QSerialPort;

	sp->setPortName(port.name);
	switch (port.data_bits) { 
		case 5: {sp->setDataBits(QSerialPort::DataBits::Data5); break; }
		case 6: {sp->setDataBits(QSerialPort::DataBits::Data6); break; }
		case 7: {sp->setDataBits(QSerialPort::DataBits::Data7); break; }
		case 8: {sp->setDataBits(QSerialPort::DataBits::Data8); break; }
	}
	switch (port.stop_bits) { 
		case 1: {sp->setStopBits(QSerialPort::StopBits::OneStop); break; }
		case 2: {sp->setStopBits(QSerialPort::StopBits::TwoStop); break; }
	}
	sp->setBaudRate(port.baud);
	sp->open(QSerialPort::ReadOnly);


	m_ports.push_back(std::unique_ptr<QSerialPort>(sp));

	endInsertRows();
}

port_table_model::port_list& port_table_model::ports() { return m_ports; };

}
