#include <uplt/graph.hpp>

namespace uplt {

QPixmap graph::legend_icon(const QSize& size) {
	QPixmap pixmap(size);
	pixmap.fill(Qt::transparent);

	QCPPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);

	drawLegendIcon(&painter, QRect(0, 0, size.width(), size.height()));
	return pixmap;
}

}
