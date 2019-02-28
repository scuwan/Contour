#ifndef CONTOR_H
#define CONTOR_H

#include <QtWidgets/QMainWindow>
#include "ui_contor.h"
#include <QtWidgets/QApplication>
#include <Qwt\qwt_plot.h>
#include <Qwt\qwt_plot_curve.h>
#include<Qwt\qwt_matrix_raster_data.h>
#include<Qwt\qwt_plot_spectrogram.h>
#include<Qwt\qwt_plot_spectrocurve.h>
#include<Qwt\qwt_scale_widget.h>
#include<Qwt\qwt_plot_layout.h>
#include<Qwt\qwt_color_map.h>
#include"isodosecurves.h"

class Contor : public QMainWindow
{
	Q_OBJECT

public:
	Contor(QWidget *parent = 0);
	~Contor();
	CIsodoseCurves m_data;
private:
	Ui::ContorClass ui;
	QwtPlot *m_plot;
	QwtPlotSpectrogram *spec;
private slots:
	void indexChanged(const QString& text );
	void open();
	void drawContor();
	void gradientIndex();
	void conformityIndex();
};

#endif // CONTOR_H
