#ifndef _UPLT_GRAPH_SPEC_DIALOG_HPP_
#define _UPLT_GRAPH_SPEC_DIALOG_HPP_

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

#include <exprtk.hpp>

#include <uplt/graph_spec.hpp>
#include <uplt/port_table_model.hpp>

namespace uplt {

class graph_spec_dialog: public QDialog { 
	Q_OBJECT 
public:
	graph_spec_dialog(port* port, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	void set_spec(const graph_spec& spec);

protected:
	QSpinBox* m_pen_width = new QSpinBox;
	QComboBox* m_pen_style = new QComboBox;
	QPushButton* m_pen_color_button = new QPushButton("Color...");
	QComboBox* m_graph_line_style = new QComboBox;
	QComboBox* m_graph_scatter_style = new QComboBox;

	QComboBox* m_brush_style = new QComboBox;
	QPushButton* m_brush_color_button = new QPushButton("Color...");

	QLabel* m_preview_label = new QLabel;
	
	port* m_port = nullptr;

protected:
	void update_preview();
};



}

#endif // !_UPLT_GRAPH_SPEC_DIALOG_HPP_
