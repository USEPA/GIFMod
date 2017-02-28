#pragma once
#include "colorscheme.h"
#include "qgraphicsitem.h"
#include "GWidget.h"
#include "Medium.h"
#include "qstatusbar.h"
#include "qmenubar.h"

/*vector<QColor> color(vector<CBTC> data, float t, QString theme, bool logType, int numberofGroups, vector<double> factors, vector<double> shifts)
{
		vector<QColor> colors(numberofGroups);
		bool factor = factors.size();
		bool shift = shifts.size();

		if (theme == "Green")
			for (int i = 0; i < numberofGroups; i++)
				colors[i] = QColor::fromRgb(0, i * 255 / numberofGroups, 0, 128);
		float min=-1e100, max=1e100;
		vector<QColor> r(data.size());
		vector <double> values(data.size());

		for (int i = 0; i < data.size(); i++)
		{
			values[i] = data[i].interpol(t);
			if (factor)
				values[i] *= factors[i];
			if (shift)
				values[i] += shifts[i];
		}
		//updating min, max
		for (int i = 0; i < data.size(); i++)
		{
			if (values[i] < min) min = values[i];
			if (values[i] > max) max = values[i];
		}
		for (int i = 0; i < data.size(); i++)
		{
			int g;
			if (values[i] == max)
				g = numberofGroups - 1;
			else 
				g = (min - values[i]) / (max - min) * (numberofGroups);
			r[i] = colors[g];
		}
		return r;
	}
	*/
colorlegend colorScheme::colorandLegend(colorlegend &colors, double t, QString theme, bool logType, int numberofGroups)
	{
		colors.time = t;
		colors.legendColors.resize(numberofGroups);
		colors.legendTexts.resize(numberofGroups);
		colors.colors.resize(colors.data.size());
		colors.middleText.resize(colors.data.size());
		bool factor = colors.factors.size();
		bool shift = colors.shifts.size();

		if (theme == "Green")
			for (int i = 0; i < numberofGroups; i++)
			{
				QColor c = QColor(0, i * 255 / numberofGroups, 0, 255);
				colors.legendColors[i] = c.toRgb();
			}
		if (theme == "Blue-Red")
			for (int i = 0; i < numberofGroups; i++)
			{
				QColor c = QColor(i * 255 / (numberofGroups - 1), 0, 255 - i * 255 / (numberofGroups - 1), 255);
				colors.legendColors[i] = c.toRgb();
			}

		double min = 1e200, max = -1e200;

		vector <double> values(colors.data.size());

		for (int i = 0; i < colors.data.size(); i++)
		{
			values[i] = colors.data[i].interpol(t);
			if (factor)
				values[i] *= colors.factors[i];
			if (shift)
				values[i] += colors.shifts[i];
		}
		//updating min, max
		for (int i = 0; i < colors.data.size(); i++)
		{
			if (values[i] < min) min = values[i];
			if (values[i] > max) max = values[i];
		}

		//updating legend
		for (int i = 0; i < numberofGroups; i++)
		{
			float groupRange = (max - min) / numberofGroups;
			colors.legendTexts[i] = QString ("%1-%2").arg(min + groupRange *i).arg(min + groupRange *(i+1));
		}
		for (int i = 0; i < colors.data.size(); i++)
		{
			int g;
			colors.middleText[i] = QString::number(values[i]);
			if (values[i] == max)
				g = numberofGroups - 1;
			else
				g = (values[i] - min) / (max - min) * (numberofGroups);
			if (g < 0)
				g = 0;
			if (g > numberofGroups - 1)
				g = numberofGroups - 1;
			colors.colors[i] = colors.legendColors[g];
		}
		return colors;
	}

QGraphicsView* colorScheme::showColorandLegend(colorlegend &legend, QString title, GraphWidget* gwidget)
{
	if (!legend.window)
	{
		legend.window = new QMainWindow;
		QObject::connect(legend.window, SLOT(closeEvent()), gwidget, SLOT(colorSchemeLegend_closed()));

		if (legend.window->objectName().isEmpty())
			legend.window->setObjectName(QStringLiteral("Legend Window"));
		legend.window->resize(300, 350);
		QWidget*centralwidget = new QWidget(legend.window);
		centralwidget->setObjectName(QStringLiteral("centralwidget"));
		QVBoxLayout*verticalLayout_2 = new QVBoxLayout(centralwidget);
		verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
		QVBoxLayout*verticalLayout = new QVBoxLayout();
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		if (!legend.view)
		{
			legend.view = new QGraphicsView(centralwidget);
			legend.view->setObjectName(QStringLiteral("graphicsView"));
			verticalLayout->addWidget(legend.view);
		}

		if (!gwidget->legendSliderTime)
		{
			gwidget->legendSliderTime = new QSlider(centralwidget);
			gwidget->legendSliderTime->setObjectName(QStringLiteral("horizontalSlider"));
			gwidget->legendSliderTime->setOrientation(Qt::Horizontal);
			int value = (legend.time - gwidget->model->Timemin) / gwidget->model->dt() + 0.5;
			gwidget->legendSliderTime->setValue(value);
			verticalLayout->addWidget(gwidget->legendSliderTime);
			if (legend.nodeNames.size())
				QObject::connect(gwidget->legendSliderTime, SIGNAL(valueChanged(int)), gwidget, SLOT(legendSliderChanged_Nodes(int)));
			else if (legend.edgeNames.size())
				QObject::connect(gwidget->legendSliderTime, SIGNAL(valueChanged(int)), gwidget, SLOT(legendSliderChanged_Edges(int)));
		}
		verticalLayout_2->addLayout(verticalLayout);

			legend.window->setCentralWidget(centralwidget);
/*			QMenuBar*menubar = new QMenuBar(legend.window);
			menubar->setObjectName(QStringLiteral("menubar"));
			menubar->setGeometry(QRect(0, 0, 380, 21));
			legend.window->setMenuBar(menubar);
			QStatusBar*statusbar = new QStatusBar(legend.window);
			statusbar->setObjectName(QStringLiteral("statusbar"));
			legend.window->setStatusBar(statusbar);
			*/
		if (!legend.scene)
			legend.scene = new QGraphicsScene(legend.view);
		legend.view->setScene(legend.scene);
	}
	for (int i = 0; i < legend.legendTexts.size(); i++)
	{
		if (legend.texts.size() < i + 1)
			legend.texts.push_back(legend.scene->addText(legend.legendTexts[i]));
		else
			legend.texts[i]->setPlainText(legend.legendTexts[i]);
		legend.texts[i]->setX(legend.x);
		legend.texts[i]->setY(legend.y + i*legend.dy);

		QBrush brush = QBrush(legend.legendColors[i]);
		if (legend.boxes.size() < i + 1)
		{
			QRect rect;
			rect.setLeft(legend.x + legend.columnOffset);
			rect.setTop(legend.y + i*legend.dy);
			rect.setHeight(legend.boxHeight);
			rect.setWidth(legend.boxWidth);
			QPen pen;
			legend.boxes.push_back(legend.scene->addRect(rect, pen, brush));

		}
		else
			legend.boxes[i]->setBrush(brush);
	}

	if (!title.isEmpty())
		legend.window->setWindowTitle(title);


	//legend.view->show();
//	QObject::connect(legend.view, SIGNAL(destroyed()), gwidget, SLOT(colorSchemeLegend_closed()));
	gwidget->legendSliderTime->setMinimum(0);
	gwidget->legendSliderTime->setMaximum((gwidget->model->Timemax - gwidget->model->Timemin) / gwidget->model->dt());
	//legend.view->setParent(legend.window);
	//gwidget->legendSliderTime->setParent(legend.window);
	//gwidget->legendSliderTime->show();
	legend.window->show();
	return legend.view;
}
