#include <uplt/port_table_view.hpp>

#include <QStyledItemDelegate>
#include <QPainter>
#include <QHeaderView>
#include <QMenu>
#include <QFileDialog>

#include <functional>

#include <uplt/port_table_model.hpp>
#include <uplt/port_spec_dialog.hpp>
#include <uplt/graph_spec_dialog.hpp>

namespace uplt { 

class PixmapDelegate : public QStyledItemDelegate {
public:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
		painter->save();
		QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
		if (!pixmap.isNull()) {
			QRect rect = option.rect;
			QSize pixmapSize = pixmap.size();
			QPoint center = rect.center() - QPoint(pixmapSize.width()/2, pixmapSize.height()/2);
			painter->drawPixmap(center, pixmap);
		} else {
			QStyledItemDelegate::paint(painter, option, index);
		}
		painter->restore();
	}
};

port_table_view::port_table_view(QWidget* parent): QTableView(parent) { 
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);

	resizeColumnsToContents();
	resizeRowsToContents();
	horizontalHeader()->setStretchLastSection(true);
	setItemDelegateForColumn(0, new PixmapDelegate());

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(
		this, &QWidget::customContextMenuRequested,
		this, &port_table_view::show_context_menu
	);
}

void port_table_view::show_context_menu(const QPoint& pos) {
	QModelIndex index = indexAt(pos);
	if (!index.isValid())
		return;

	auto menu = new QMenu;
	port* p = static_cast<port_table_model*>(model())->ports()[index.row()].get();
	const std::size_t row = index.row();
	menu->setAttribute(Qt::WA_DeleteOnClose);
	using namespace std::placeholders;
	connect( 
		menu->addAction("Edit"), &QAction::triggered, 
		this, std::bind(&port_table_view::edit_action, this, p, row)
	);
	connect( 
		menu->addAction("Edit visuals"), &QAction::triggered, 
		this, std::bind(&port_table_view::edit_visuals_action, this, p, row)
	);
	connect( 
		menu->addAction("Delete"), &QAction::triggered, 
		this, std::bind(&port_table_view::delete_action, this, p, row)
	);

	menu->addSeparator();

	connect( 
		menu->addAction("Export data"), &QAction::triggered, 
		this, std::bind(&port_table_view::export_data_action, this, p, row)
	);

	menu->popup(viewport()->mapToGlobal(pos));
}

void port_table_view::edit_action(port* p, std::size_t row) {
	auto dialog = new port_spec_dialog(p, this);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->set_spec(*static_cast<port_spec*>(p));
	dialog->setWindowTitle(QString("Editing port %1").arg(p->alias.size() != 0 ? p->alias : p->name));
	dialog->set_parser(m_parser);
	dialog->set_symbol_table(m_symbol_table);
	connect(
		static_cast<port_table_model*>(model()), &port_table_model::port_removed,
		dialog, &dialog::on_port_removal
	);
	connect(
		dialog, &QDialog::accepted,
		[=]() { static_cast<port_table_model*>(model())->update_port(p, dialog->spec()); }
	);
	dialog->show();
}

void port_table_view::edit_visuals_action(port* p, std::size_t row) {
	auto dialog = new graph_spec_dialog(p, this);
	connect(
		static_cast<port_table_model*>(model()), &port_table_model::port_removed,
		dialog, &dialog::on_port_removal
	);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->setWindowTitle(QString("Editing visuals on %1").arg(p->alias.size() != 0 ? p->alias : p->name));
	dialog->show();
}

void port_table_view::delete_action(port* p, std::size_t row) {
	model()->removeRow(row);
}

void port_table_view::export_data_action(port* p, std::size_t row) {
	auto dialog = new QFileDialog(
		this, QString("Exporting data from %1").arg(p->alias.size() == 0 ? p->name : p->alias),
		"", "Text files (*.txt);;CSV files (*.csv)"
	);
	dialog->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
	dialog->selectFile("data");
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	connect( 
		dialog, &QFileDialog::fileSelected,
		[=](const QString& file) {
			QFile out(file);
			out.open(QFile::WriteOnly);
			QTextStream stream(&out);
			for (const auto& d: *(p->graph->data().get()))
				stream << QString::number(d.mainKey(), 'g', 10) << ' ' << QString::number(d.mainValue(), 'g', 5) << '\n';
		}
	);
	dialog->show();
}

}
