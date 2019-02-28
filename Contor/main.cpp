#include "contor.h"
#include <QtWidgets/QApplication>
#include "plot.h"
//class LinearColorMapRGB : public QwtLinearColorMap
//{
//public:
//	LinearColorMapRGB() :
//		QwtLinearColorMap(Qt::darkCyan, Qt::red, QwtColorMap::RGB)
//	{
//		addColorStop(0.1, Qt::cyan);
//		addColorStop(0.6, Qt::green);
//		addColorStop(0.95, Qt::yellow);
//	}
//};
//void CreateView();
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Contor w;
	w.show();
	return a.exec();
}