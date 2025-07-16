#include <uplt/port_table_model.hpp>

namespace uplt { 

bool port_table_model::removeRows(int row, int count, const QModelIndex &parent) {
	beginRemoveRows(parent, row, row + count - 1);

	for (auto beg = m_ports.begin() + row; beg < m_ports.begin() + row + count; beg++)
		emit port_removed(beg->get());
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
	if (!index.isValid() || index.row() >= m_ports.size())
    return QVariant();

	const port& p = *m_ports[index.row()];
	switch (role) {
		case Qt::DisplayRole: {
			switch (index.column()) { 
				case column::name:
					if (p.serial->isOpen()) {
						if (not p.alias.empty())
							return QString::fromStdString(p.alias);
						return p.serial->portName();
					}
					return QString("<unavailable>");
				case column::data_bits:
					return p.serial->dataBits();
				case column::stop_bits:
					return p.serial->stopBits();
				case column::baud:
					return p.serial->baudRate();
				case column::transform_expression:
					return QString::fromStdString(p.expression_str);
			}
		} case Qt::ToolTipRole: {
			switch (index.column()) {
				case column::name:
					if (not p.serial->isOpen())
						return QString::fromStdString(p.name);
					if (not p.alias.empty()) 
						return QString::fromStdString(p.alias);
				case column::data_bits: return "data bits";
				case column::stop_bits: return "stop bits";
				case column::baud: return "baud rate";
				case column::transform_expression: return "signal transformation expression";
			}
		} case Qt::DecorationRole: {
			switch (index.column())
				case column::plot_icon: return p.graph->legend_icon({30, 15});
		} case Qt::TextAlignmentRole: {
			switch (index.column())
				case column::plot_icon: return Qt::AlignCenter;
		}
	}

	return QVariant();
}

QVariant port_table_model::headerData(int section, Qt::Orientation orientation, int role) const { 
	if (role == Qt::DisplayRole) { 
		if (orientation == Qt::Vertical) 
			return section + 1;
		switch (section) { 
			case column::plot_icon: return "line";
			case column::name: return "name";
			case column::data_bits: return "db";
			case column::stop_bits: return "sb";
			case column::baud: return "baud";
			case column::transform_expression: return "sig(x)";
		}
	}
	return QVariant();
}

void port_table_model::add_port(const port_spec& spec, graph* graph) {
	beginInsertRows(QModelIndex(), m_ports.size(), m_ports.size());

	auto p = std::make_unique<port>(port(new QSerialPort, spec));
	p->graph = graph;
	p->apply_settings_to_serial_port();
	p->serial->open(QSerialPort::ReadOnly);
	m_ports.push_back(std::move(p));

	endInsertRows();
	emit port_added(m_ports.back().get());
}

port_table_model::port_list& port_table_model::ports() { return m_ports; };

void port_table_model::update_port(port* p, const port_spec& spec) {
	auto pt = std::find_if(m_ports.begin(), m_ports.end(), [=](auto& l) { return l.get() == p; });
	if (pt == m_ports.end()) 
		return;
	*static_cast<port_spec*>(pt->get()) = spec;
	(*pt)->serial->close();
	(*pt)->apply_settings_to_serial_port();
	(*pt)->serial->open(QSerialPort::ReadOnly);
	const auto row = std::distance(m_ports.begin(), pt);
	emit dataChanged(
		index(row, 0), 
		index(row, column::columns+10)
	);
}

}
