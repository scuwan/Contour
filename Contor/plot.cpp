#include "plot.h"
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


namespace{
	class LinearColorMapRGB : public QwtLinearColorMap
	{
	public:
		LinearColorMapRGB() :
			QwtLinearColorMap(Qt::darkCyan, Qt::red, QwtColorMap::RGB)
		{
			addColorStop(0.1, Qt::cyan);
			addColorStop(0.6, Qt::green);
			addColorStop(0.95, Qt::yellow);
		}
	};
}

plot::plot(CIsodoseCurves& m_data, int index, bool contor, bool image) :spec(nullptr)
{
	QwtPlot *m_plot = this;
	//CIsodoseCurves m_data;
	QwtScaleWidget *rightAxis = m_plot->axisWidget(QwtPlot::yRight);
	rightAxis->setTitle("cGry");
	rightAxis->setColorBarEnabled(true);
	rightAxis->setColorMap(QwtInterval(0, 400), new LinearColorMapRGB());
	m_plot->setAxisScale(QwtPlot::yLeft, 0, 64);
	m_plot->setAxisScale(QwtPlot::xBottom, 0, 64);
	m_plot->setAxisScale(QwtPlot::yRight, 0, 400);
	m_plot->enableAxis(QwtPlot::yRight);
	m_plot->plotLayout()->setAlignCanvasToScales(true);


	spec = new QwtPlotSpectrogramText();
	QwtMatrixRasterData *mdata = new QwtMatrixRasterData();
	int XAxis = m_data.m_x_;
	int YAxis = m_data.m_y_;
	mdata->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
	mdata->setInterval(Qt::XAxis, QwtInterval(0, XAxis));
	mdata->setInterval(Qt::YAxis, QwtInterval(0, YAxis));
	mdata->setInterval(Qt::ZAxis, QwtInterval(0, 400));
	QList<double> contourLevels;
	for (int i = 10; i <= 400; i = i + 40)
	{
		contourLevels.append(i);
	}
	spec->setContourLevels(contourLevels);
	QVector<double> matrixValue;
	int mSize = (XAxis / m_data.m_spacing_)*(XAxis / m_data.m_spacing_);
	int aIndex = index / m_data.m_spacing_;
	for (int i = 0; i < mSize; ++i)
	{
		matrixValue.append(m_data.m_doseGrid_[i + mSize*aIndex]);
	}
	mdata->setValueMatrix(matrixValue, m_data.m_y_ / m_data.m_spacing_);
	spec->setData(mdata);
	spec->setColorMap(new LinearColorMapRGB());
	spec->attach(m_plot);
	if (contor)
	{
		spec->setDisplayMode(QwtPlotSpectrogram::ContourMode, true);
	}
	else
	{
		spec->setDisplayMode(QwtPlotSpectrogram::ContourMode, false);
	}
	if (image)
	{
		spec->setDisplayMode(QwtPlotSpectrogram::ImageMode, true);
		spec->setDefaultContourPen(Qt::black, 0);
		
	}
	else
	{
		spec->setDisplayMode(QwtPlotSpectrogram::ImageMode, false);
		spec->setDefaultContourPen(QPen(Qt::NoPen));
	}
	
	m_plot->setAxisScale(QwtPlot::yLeft, 0, XAxis);
	m_plot->setAxisScale(QwtPlot::xBottom, 0, YAxis);
	QString str = "Layer (Z) : ";
	str += QString::number(index);
	m_plot->setTitle(str);
	m_plot->resize(600, 400);
	m_plot->show();
}


plot::~plot()
{
	if (spec != nullptr)
		delete spec;
}
