#ifndef _UPLT_GRAPH_HPP_
#define _UPLT_GRAPH_HPP_

#include <qcustomplot.h>

namespace uplt {

class graph: public QCPGraph {
public:
	using QCPGraph::QCPGraph;

public:
	QPixmap legend_icon(const QSize& s);
};

}

#endif // !_UPLT_GRAPH_HPP_
