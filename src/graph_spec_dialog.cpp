#include <uplt/graph_spec_dialog.hpp>

#include <QVBoxLayout>
#include <QGroupBox>

namespace uplt {

graph_spec_dialog::graph_spec_dialog(port* p, QWidget *parent, Qt::WindowFlags f): dialog(p, parent, f) {
	m_preview_label->setAlignment(Qt::AlignCenter);
	auto main_layout = new QVBoxLayout;
	this->setLayout(main_layout);
	main_layout->addWidget(m_preview_label);

	auto grid = new QGridLayout;

	main_layout->addLayout(grid);
	grid->addWidget(new QLabel("Style"), 0, 0);
	grid->addWidget(m_graph_line_style, 0, 1);
	grid->addWidget(new QLabel("Line thickness"), 1, 0);
	grid->addWidget(m_pen_width, 1, 1);
	grid->addWidget(new QLabel("Scatter shape"), 2, 0);
	grid->addWidget(m_graph_scatter_shape, 2, 1);
	grid->addWidget(new QLabel("Scatter size"), 3, 0);
	grid->addWidget(m_graph_scatter_size, 3, 1);
	grid->addWidget(new QLabel("Fill pattern"), 4, 0);
	grid->addWidget(m_brush_style, 4, 1);

	auto buttons_lay = new QHBoxLayout;
	main_layout->addLayout(buttons_lay);
	buttons_lay->addWidget(m_pen_color_button);
	buttons_lay->addWidget(m_brush_color_button);

	m_pen_width->setRange(0.01, 20);
	m_pen_width->setSingleStep(0.1);
	m_graph_scatter_size->setRange(0.01, 20);
	m_graph_scatter_size->setSingleStep(0.1);
	m_graph_line_style->addItems({
		"None", "Line", "Left step", "Right step", "Center step", "Impulse"
	});
	m_graph_scatter_shape->addItems({
		"None", "Dot", "Cross", "Plus", "Circle", "Disk", "Square", "Diamond",
		"Star", "Triangle", "Inverted triangle", "Crossed square", "Plus square",
		"Crossed circle", "Plus circle", "Peace"
	});
	m_brush_style->addItems({
		"None", "Solid", "Dense 1", "Dense 2", "Dense 3", "Dense 4", "Dense 5",
		"Dense 6", "Dense 7", "Horizontal lines", "Vertical lines", "Crossing",
		"Backward diagonal", "Forward diagonal", "Crossing diagonal"
	});

	connect(
		m_pen_color_button, &QPushButton::pressed,
		[this]() {
			if (m_color_selection)
				return;
			m_color_selection = true;
			auto dialog = new QColorDialog(this);
			dialog->setAttribute(Qt::WA_DeleteOnClose);
			dialog->setCurrentColor(this->m_port->graph->pen().color());
			connect(
				dialog, &QColorDialog::finished,
				[this]() { m_color_selection = false; }
			);
			connect(
				dialog, &QColorDialog::colorSelected, 
				[this, dialog](const QColor& col) {
					auto p = this->m_port->graph->pen();
					p.setColor(col);
					this->m_port->graph->setPen(p);
					dialog->close();
				}
			);
			dialog->show();
		}
	);
	connect(
		m_brush_color_button, &QPushButton::pressed,
		[this]() {
			if (m_color_selection)
				return;
			m_color_selection = true;
			auto dialog = new QColorDialog(this);
			dialog->setAttribute(Qt::WA_DeleteOnClose);
			dialog->setCurrentColor(this->m_port->graph->brush().color());
			connect(
				dialog, &QColorDialog::finished,
				[this]() { m_color_selection = false; }
			);
			connect(
				dialog, &QColorDialog::colorSelected, 
				[this, dialog](const QColor& col) {
					auto p = this->m_port->graph->brush();
					p.setColor(col);
					this->m_port->graph->setBrush(p);
					dialog->close();
				}
			);
			dialog->show();
		}
	);

	connect(
		m_pen_width, &QDoubleSpinBox::valueChanged,
		[this](double val) {
			QPen p = m_port->graph->pen();
			p.setWidthF(val);
			m_port->graph->setPen(p);
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
		m_graph_scatter_shape, &QComboBox::currentIndexChanged,
		[this](int val) {
			auto ss = m_port->graph->scatterStyle();
			ss.setShape(QCPScatterStyle::ScatterShape(val));
			m_port->graph->setScatterStyle(ss);
			update_preview();
		}
	);
	connect(
		m_graph_scatter_size, &QDoubleSpinBox::valueChanged,
		[this](double val) {
			QCPScatterStyle ss = m_port->graph->scatterStyle();
			ss.setSize(val);
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

	const auto graph = m_port->graph;
	m_pen_width->setValue(graph->pen().widthF());
	m_graph_line_style->setCurrentIndex(graph->lineStyle());
	m_graph_scatter_shape->setCurrentIndex(graph->scatterStyle().shape());
	m_graph_scatter_size->setValue(graph->scatterStyle().size());
	m_brush_style->setCurrentIndex(graph->brush().style());

	update_preview();
}

void graph_spec_dialog::update_preview() {
	m_preview_label->setPixmap(m_port->graph->legend_icon({160, 40}));
}

}
