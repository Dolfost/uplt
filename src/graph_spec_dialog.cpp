#include <uplt/graph_spec_dialog.hpp>

#include <QVBoxLayout>
#include <QGroupBox>

namespace uplt {

graph_spec_dialog::graph_spec_dialog(port* p, QWidget *parent, Qt::WindowFlags f): QDialog(parent, f), m_port(p) {
	m_preview_label->setAlignment(Qt::AlignCenter);
	auto main_layout = new QVBoxLayout;
	this->setLayout(main_layout);
	main_layout->addWidget(m_preview_label);

	auto pen_group = new QGroupBox("Pen");
	main_layout->addWidget(pen_group);
	pen_group->setFlat(true);
	auto grid = new QGridLayout;
	pen_group->setLayout(grid);
	grid->addWidget(new QLabel("Width"), 2, 0);
	grid->addWidget(m_pen_width, 2, 1);
	grid->addWidget(new QLabel("Style"), 3, 0);
	grid->addWidget(m_pen_style, 3, 1);
	grid->addWidget(new QLabel("Graph line style"), 4, 0);
	grid->addWidget(m_graph_line_style, 4, 1);
	grid->addWidget(new QLabel("Graph scatter style"), 5, 0);
	grid->addWidget(m_graph_scatter_style, 5, 1);
	grid->addWidget(m_pen_color_button, 6, 0, 6, 1);

	auto brush_group = new QGroupBox("Brush");
	main_layout->addWidget(brush_group);
	grid = new QGridLayout;
	brush_group->setLayout(grid);
	grid->addWidget(new QLabel("Style "), 0, 0);
	grid->addWidget(m_brush_style, 0, 1);
	grid->addWidget(m_brush_color_button, 1, 0, 1, 1);

	m_pen_width->setRange(1, 20);
	m_pen_style->addItems({
		"No pen", "Solid line", "Dash line", "Dot line", "Dash dot line", "Dash dot dot line"
	});
	m_graph_line_style->addItems({
		"None", "Line", "Left step", "Right step", "Center step", "Impulse"
	});
	m_graph_scatter_style->addItems({
		"None", "Dot", "Cross", "Plus", "Circle", "Disk", "Square", "Diamond",
		"Star", "Triangle", "Inverted triangle", "Crossed square", "Plus square",
		"Crossed circle", "Plus circle", "Peace"
	});
	m_brush_style->addItems({
		"None", "Solid", "Dense 1", "Dense 2", "Dense 3", "Dense 4", "Dense 5",
		"Dense 6", "Dense 7", "Horizontal lines", "Vertical lines", "Crossing",
		"Backward diagonal", "Forward diagonal", "Crossing diagonal", 
		"Linear gradient", "Conical gradient", "Radial gradient"
	});

	connect(
		m_pen_width, &QSpinBox::valueChanged,
		[this](int val) {
			QPen p = m_port->graph->pen();
			p.setWidth(val);
			update_preview();
		}
	);
	connect(
		m_pen_style, &QComboBox::currentIndexChanged,
		[this](int val) {
			QPen p = m_port->graph->pen();
			p.setStyle(Qt::PenStyle(val));
			update_preview();
		}
	);
	connect(
		m_graph_line_style, &QComboBox::currentIndexChanged,
		[this](int val) {
			m_port->graph->setLineStyle(QCPGraph::LineStyle(val));
			update_preview();
		}
	);
	connect(
		m_graph_scatter_style, &QComboBox::currentIndexChanged,
		[this](int val) {
			auto ss = m_port->graph->scatterStyle();
			ss.setShape(QCPScatterStyle::ScatterShape(val));
			m_port->graph->setScatterStyle(ss);
			update_preview();
		}
	);
	connect(
		m_brush_style, &QComboBox::currentIndexChanged,
		[this](int val) {
			auto b = m_port->graph->brush();
			b.setStyle(Qt::BrushStyle(val));
			m_port->graph->setBrush(b);
			update_preview();
		}
	);
}

void graph_spec_dialog::update_preview() {
	m_preview_label->setPixmap(m_port->graph->legend_icon({40, 40}));
}

}
