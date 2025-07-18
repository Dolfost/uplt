#ifndef _UPLT_GRAPH_SPEC_
#define _UPLT_GRAPH_SPEC_

#include <qcustomplot.h>

namespace uplt {

struct graph_spec {
	QCPGraph::LineStyle line_style;
	QCPScatterStyle scatter_style;
	QPen pen;
	QBrush brush;

public:
	void apply_graph_settings(QCPGraph* g) {
		g->setLineStyle(line_style);
		g->setScatterStyle(scatter_style);
		g->setPen(pen);
		g->setBrush(brush);
	}
};

}

#endif // !_UPLT_GRAPH_SPEC_
