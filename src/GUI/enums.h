#pragma once
#ifdef QT_version
#include "qcolor.h"
#endif // QT_version
#define PI 3.14159265359
class GraphWidget;
enum Operation_Modes{ Draw_Connector, Node1_selected, Pan, NormalMode, resizeNode };
enum Object_Types { Void, Block, Connector, RayLine };
enum mListReadStatus{fileNotValid, readSuccessfully, readBefore, errorInContents};
enum corners{ none, topleft, topright, bottomleft, bottomright };
enum edgesides{ noside, topside, leftside, bottomside, rightside };

enum Role {
	TypeRole =							2700,
	InputMethodRole =					2702,
	DefaultValuesListRole =				2704,
	VariableTypeRole =					2706,
	VariableNameRole =					2707,
	VariableNameToolTipRole =			2708,
	warningConditionRole =				2709,
	warningErrorRole =					2710,
	warningErrorDescRole =				2711,
	DescriptionCodeRole =				2712,
	experimentDependentRole =			2713,
	differentValuesRole =				2714,
	differentValuesMultiObjectRole =	2716,
	UnitRole =							2802,
	defaultUnitRole =					2803,
	UnitsListRole =						2804,
	allUnitsRole =						2805,
	TreeItemType =						2901,
	TreeParentItemType =				2902,
	XStringEditRole =					3001,
	setParamRole =						3002,
	fullFileNameRole =					30704,
	saveIndex =							3101,
	loadIndex =							3102,
	loadIndexandInputMethodRole =		3103,
	loadIndexandDefaultValuesListRole = 3104,
	loadIndexandVariableTypeRole =		3105,
	allowableWordsRole =				3201,

};
enum XStringMetaTypes { XStringType = 1025, XStringListType = 1026 };
/*enum PCodes{
	ks = 0,
	z1 = 0,
	z2 = 1,
	theta_s = 1,
	theta_r = 2,
	diameter = 2,
	pipe_c = 3,
	vg_alpha = 3,
	vg_n = 4,
	vg_m = 5,
	width = 5,
	nmanning = 6,
	lambda = 6,
	flow_exponent = 8,

};*/

#ifdef QT_version
struct objectColor
{
	QColor color1, color2, defaultColor;
};
struct condition
{
	QStringList Condition, error, errorDesc;
};
#endif // QT_version
struct percentileData{
	std::string parameter;
	double p25, p50, p975, mean, std;
};



//template <class T> const T& min(const T& a, const T& b);
double min(double x, double y);
float min(float x, float y);
int min(int x, int y);

double max(double x, double y);
float max(float x, float y);
int max(int x, int y);

#ifdef QT_version
QStringList percentile2List(percentileData p);
percentileData percentileFromList(QStringList r);
QString OnlyFilename(const QString &PathandFilename);
QString OnlyFilenames(const QString &PathandFilenames);
QString OnlyPath(const QString& PathandFilename);
QString relativePathFilename(const QString& fileAndPath, const QString&dir);
QString updateRelativePath(const QString&fileWithRelativePath, const QString& oldPath, const QString& newPath);
QString fullFilename(QString fileWithRelativePath, QString dir);
QStringList uniques(const QStringList &);
QString operator&(const QString &, const QString &);
//QStringList operator&(const QStringList &, const QStringList &);
//QString and(const QString &, const QString &);
//QStringList QSplit(const QString &QS, QChar del = ',');
QString QMerge(const QStringList &QSL, QChar del = ';');
std::string convertstringtoStringOP(const QString& s, GraphWidget *gWidget);
#endif
