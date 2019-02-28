#pragma once
#include "Qwt\qwt_plot.h"
#include<Qwt\qwt_plot_spectrogram.h>
#include"QwtPlotSpectrogramText.h"
#include"isodosecurves.h"
class plot :
	public QwtPlot
{
public:
	plot(CIsodoseCurves&, int index,bool contor,bool image);
	~plot();
private:
	QwtPlotSpectrogram *spec;
	//QwtPlotSpectrogramText *spec;
};

