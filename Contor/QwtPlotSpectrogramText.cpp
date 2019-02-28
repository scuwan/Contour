#include "QwtPlotSpectrogramText.h"
#include "Qwt\qwt_scale_map.h"

QwtPlotSpectrogramText::QwtPlotSpectrogramText(const QString &title) :QwtPlotSpectrogram(title)
{
}


QwtPlotSpectrogramText::~QwtPlotSpectrogramText()
{
}


/*!
Paint the contour lines

\param painter Painter
\param xMap Maps x-values into pixel coordinates.
\param yMap Maps y-values into pixel coordinates.
\param contourLines Contour lines

\sa renderContourLines(), defaultContourPen(), contourPen()
*/
void QwtPlotSpectrogramText::drawContourLines(QPainter *painter,
	const QwtScaleMap &xMap, const QwtScaleMap &yMap,
	const QwtRasterData::ContourLines &contourLines) const
{
	QwtPlotSpectrogram::drawContourLines(painter, xMap, yMap, contourLines);
	QList<double> Levels = contourLevels();
	const int numLevels = Levels.size();
	for (int l = 0; l < numLevels; ++l)
	{
		const double level = Levels[l];
		QPen pen = defaultContourPen();
		if (pen.style() == Qt::NoPen)
			pen = contourPen(level);
		if (pen.style() == Qt::NoPen)
			continue;
		painter->setPen(pen);
		const QPolygonF &lines = contourLines[level];
		int pCount = lines.size();
		int tIndex = pCount / 2;
		if (tIndex == 0)
			continue;
		QPoint tPoint(xMap.transform(lines[tIndex].x()+1),
			yMap.transform(lines[tIndex].y()));
		painter->drawText(tPoint, QString::number(level));
	}

}