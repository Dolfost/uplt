#include <uplt/port_table_view.hpp>

#include <QStyledItemDelegate>
#include <QPainter>

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


port_table_view::port_table_view() { 
	setSelectionBehavior(QAbstractItemView::SelectRows);
	resizeColumnsToContents();
	resizeRowsToContents();
	setItemDelegateForColumn(0, new PixmapDelegate());
}

}
