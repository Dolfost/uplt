#ifndef _UPLT_GRAPH_SPEC_DIALOG_HPP_
#define _UPLT_GRAPH_SPEC_DIALOG_HPP_

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QLabel>

#include <exprtk.hpp>

#include <uplt/graph_spec.hpp>
#include <uplt/port_table_model.hpp>
#include <uplt/dialog.hpp>

namespace uplt {

class graph_spec_dialog: public dialog { 
	Q_OBJECT 
public:
	graph_spec_dialog(port* port, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	void set_spec(const graph_spec& spec);

protected:
	QDoubleSpinBox* m_pen_width = new QDoubleSpinBox;
	QDoubleSpinBox* m_graph_scatter_size = new QDoubleSpinBox;
	QPushButton* m_pen_color_button = new QPushButton("Pen color");
	QComboBox* m_graph_line_style = new QComboBox;
	QComboBox* m_graph_scatter_shape = new QComboBox;

	QComboBox* m_brush_style = new QComboBox;
	QPushButton* m_brush_color_button = new QPushButton("Fill color");

	QLabel* m_preview_label = new QLabel;
	
	bool m_color_selection = false;

protected:
	void update_preview();
};



}

#endif // !_UPLT_GRAPH_SPEC_DIALOG_HPP_
