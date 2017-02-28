#pragma once
#include <QList>
#include <QMap>
#include <qvariant.h>
#include "BTCSet.h"
#include "GWidget.h"
#include "enums.h"
//class CMediumSet;
#ifdef GIFMOD
class CMediumSet;
//#include "MediumSet.h"
//#include "medium.h"
#endif
#ifdef GWA
#include "gwa.h"
#endif

//#include "XString.h"

//#include "mProplist.h"
//#include "Proplist.h"
//#include "PropModel.h"
//class mPropList;

//enum EntityType {Precipitation, Inflow, Buildup, Environmental, ExternalFlux, Observation, Parameters, Constituents};

using namespace std;

class Results
{
public:
	Results(){
	}
//	Results(CMediumSet *model){
//		this->model = model;
//	}
	~Results() {
	
	}
#ifdef GIFMOD
	CMediumSet *model;
#endif
#ifdef GWA
	CGWA *model;
#endif
	vector<percentileData> percentiles;
	vector<vector<CBTCSet>> realizations, realizationsPercentiles, noiseRealizations, noiseRealizationsPercentiles;
	CMatrix localSensitivityMatrix, globalSensitivityMatrix, correlationMatrix;
	vector<CBTCSet> youngAgeDistributionRealizations, youngAgeDistributionRealizationPercentiles, cumulativeYoungAgeDistributionRealizations, cumulativeYoungAgeDistributionRealizationPercentiles;
	vector<CBTCSet> projectedTracerConcentrations, projectedTracerConcentrationPercentiles;
	CBTCSet ANS_obs, projected;
	CBTCSet posteriors;
	CBTCSet priors;
	QStringList observations;
	percentileData percentile(const QString &parameter) const{
		int index = indexOf(parameter);
		if (index != -1)
			return percentiles[index];
	}
	CBTC& posterior(const QString &parameter) {
		int index = indexOf(parameter);
		if (index != -1)
			return posteriors.BTC[index];
	}
	CBTC& prior(const QString &parameter) {
		int index = indexOf(parameter);
		if (index != -1)
			return priors.BTC[index];
	}
	CBTCSet realization(const QString &observation);
	CBTCSet noiseRealization(const QString &observation);
	CBTCSet realizationPercentile(const QString &observation);
	CBTCSet noiseRealizationPercentile(const QString &observation);int indexOf(const QString &parameter) const {
		for (int i = 0; i < percentiles.size(); i++)
			if (percentiles[i].parameter == parameter.toStdString())
				return i;
		return -1;
	}
	bool hasPercentile(const QString &parameter) {
		if (!percentiles.size()) return false;
		if (indexOf(parameter) == -1)return false;
		return true;
	}
	bool hasPrior(const QString &parameter) {
		if (!priors[parameter.toStdString()].n) return false;
		return true;
	}
	bool hasPosterior(const QString &parameter) {
		if (!posteriors[parameter.toStdString()].n) return false;
		return true;
	}
	bool hasRealization(const QString &observation);
	bool hasRealizationPercentile(const QString &observation);
	bool hasNoiseRealization(const QString &observation);
	bool hasNoiseRealizationPercentile(const QString &observation);
/*	void setObservations(const vector<string> &measurements){
		observations.clear();
		for (int i = 0; i < measurements.size(); i++)
			observations
	}*/

	QMap<QString, QVariant> Results::compact() const;
	static Results* Results::unCompact(QMap<QString, QVariant>);


protected:

private:
};