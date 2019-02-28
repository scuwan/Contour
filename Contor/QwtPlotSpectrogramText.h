#pragma once
#include "Qwt\qwt_plot_spectrogram.h"
#include<qstring.h>
class QwtPlotSpectrogramText :
	public QwtPlotSpectrogram
{
public:
	explicit QwtPlotSpectrogramText(const QString & title = QString::null);
	~QwtPlotSpectrogramText();
protected:
	virtual void drawContourLines(QPainter *p,
		const QwtScaleMap &xMap, const QwtScaleMap &yMap,
		const QwtRasterData::ContourLines& lines) const;
};

