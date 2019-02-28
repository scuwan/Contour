#include "contor.h"
#include<qtoolbar.h>
#include<qtoolbutton.h>
#include<qlabel.h>
#include<qcombobox.h>
#include<qfiledialog.h>
#include<qmessagebox.h>
#include"plot.h"
#include"ConformityIndex.h"

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
Contor::Contor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionOpen_grid_file, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionGradientIndex, SIGNAL(triggered()), this, SLOT(gradientIndex()));
	connect(ui.actionConformityIndex, SIGNAL(triggered()), this, SLOT(conformityIndex()));
	connect(ui.btnDraw, SIGNAL(clicked()), this, SLOT(drawContor()));
	ui.checkBoxContor->setEnabled(true);
	ui.checkBoxContor->setChecked(true);
	ui.comboBox->setEnabled(false);
	Qt::WindowFlags flags = 0;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowCloseButtonHint;

	setWindowFlags(flags); // 设置禁止最大化
	setFixedSize(200, 250); // 禁止改变窗口大小。

}

Contor::~Contor()
{
	//delete m_plot;
	//delete spec;
}

void Contor::indexChanged(const QString& text)
{
	int index = text.toInt();
	if (index < 0 || index >= m_data.m_z_)
		return;
	if (m_data.m_girdOk_ == false)
		return;
	if (spec == nullptr)
		spec = new QwtPlotSpectrogram();
	//spec->setRenderThreadCount(0);
	QwtMatrixRasterData mdata;
	int XAxis = m_data.m_x_;
	int YAxis = m_data.m_y_;
	mdata.setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
	mdata.setInterval(Qt::XAxis, QwtInterval(0, XAxis));
	mdata.setInterval(Qt::YAxis, QwtInterval(0, YAxis));
	mdata.setInterval(Qt::ZAxis, QwtInterval(0, 400));
	QList<double> contourLevels;
	for (int i = 10; i <= 400; i = i + 40)
	{
		contourLevels.append(i);
	}
	spec->setContourLevels(contourLevels);
	QVector<double> matrixValue;
	int mSize = (XAxis / m_data.m_spacing_)*(XAxis / m_data.m_spacing_);
	for (int i = 0; i < mSize; ++i)
	{
		matrixValue.append(m_data.m_doseGrid_[i + mSize*index]);
	}
	mdata.setValueMatrix(matrixValue, m_data.m_y_/m_data.m_spacing_);
	spec->setData(&mdata);
	spec->attach(m_plot);
	spec->setDisplayMode(QwtPlotSpectrogram::ContourMode, true);
	spec->setDisplayMode(QwtPlotSpectrogram::ImageMode, true);
	spec->setDefaultContourPen(Qt::black, 0);
	m_plot->setAxisScale(QwtPlot::yLeft, 0, XAxis);
	m_plot->setAxisScale(QwtPlot::xBottom, 0, YAxis);
	m_plot->replot();
}
void Contor::open()
{
	QString dir = QFileDialog::getOpenFileName(this, tr("Open grid file"), QDir::currentPath(), "File (*.txt)");
	if (!dir.isEmpty())
	{
		std::string str = dir.toStdString();
		m_data.loadGrid(str.c_str());
		int z = m_data.m_z_;
		ui.comboBox->clear();
		for (int i = 0; i < z; i++)
		{
			ui.comboBox->addItem(QString::number(i));
		}
		ui.comboBox->setEnabled(true);
	}
	else
	{
		ui.comboBox->setEnabled(false);
		QMessageBox::about(this, tr("Warning"), (tr("fail open file")));
	}
} 

void Contor::drawContor()
{
	if (!m_data.m_girdOk_)
	{
		QMessageBox::about(this, tr("Warning"), (tr("please open correct grid file first.")));
		return ;
	}
	int index = ui.comboBox->currentText().toInt();
	//index = index / m_data.m_spacing_;
	bool b_contor = ui.checkBoxContor->isChecked();
	bool b_image = ui.checkBoxImage->isChecked();
	plot *mplot = new plot(m_data,index,b_contor,b_image);
}
void Contor::gradientIndex()
{
	double index=CConformityIndex::getInstance().GetGradientIndex();
	QMessageBox::about(this, tr("Gradient Index :"), QString::number(index));
}
void Contor::conformityIndex()
{
	double index = CConformityIndex::getInstance().GetConformityIndex();
	QMessageBox::about(this, tr("Conformity Index :"), QString::number(index));
}