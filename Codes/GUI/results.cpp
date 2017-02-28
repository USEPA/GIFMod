#pragma once

#include "results.h"
#include "MediumSet.h"


CBTCSet Results::realization(const QString &observation) {
	if (hasRealization(observation))
	{
		int index = model->lookup_observation(observation.toStdString());
		return realizations[0][index];
	}
	return CBTCSet();
}
CBTCSet Results::noiseRealization(const QString &observation) {
	if (hasNoiseRealization(observation))
		return noiseRealizations[0][this->model->lookup_observation(observation.toStdString())];
	return CBTCSet();
}
CBTCSet Results::realizationPercentile(const QString &observation) {
	if (hasRealizationPercentile(observation))
		return realizationsPercentiles[0][this->model->lookup_observation(observation.toStdString())];
	return CBTCSet();
}
CBTCSet Results::noiseRealizationPercentile(const QString &observation) {
	if (hasNoiseRealizationPercentile(observation))
		return noiseRealizationsPercentiles[0][this->model->lookup_observation(observation.toStdString())];
	return CBTCSet();
}
bool Results::hasRealization(const QString &observation) {
	if (!realizations.size()) return false;
	int index = model->lookup_observation(observation.toStdString());
	if (index == -1) return false;
	if (!realizations[0].size()) return false;
	if (!realizations[0][index].nvars) return false;
	if (!realizations[0][index].BTC[0].n) return false;
	return true;
}
bool Results::hasRealizationPercentile(const QString &observation) {
	if (!realizationsPercentiles.size()) return false;
	int index = model->lookup_observation(observation.toStdString());
	if (index == -1) return false;
	if (!realizationsPercentiles[0].size()) return false;
	if (!realizationsPercentiles[0][index].nvars) return false;
	if (!realizationsPercentiles[0][index].BTC[0].n) return false;
	return true;
}
bool Results::hasNoiseRealization(const QString &observation) {
	if (!noiseRealizations.size()) return false;
	int index = model->lookup_observation(observation.toStdString());
	if (index == -1) return false;
	if (!noiseRealizations[0].size()) return false;
	if (!noiseRealizations[0][index].nvars) return false;
	if (!noiseRealizations[0][index].BTC[0].n) return false;
	return true;
}
bool Results::hasNoiseRealizationPercentile(const QString &observation) {
	if (!noiseRealizationsPercentiles.size()) return false;
	int index = model->lookup_observation(observation.toStdString());
	if (index == -1) return false;
	if (!noiseRealizationsPercentiles[0].size()) return false;
	if (!noiseRealizationsPercentiles[0][index].nvars) return false;
	if (!noiseRealizationsPercentiles[0][index].BTC[0].n) return false;
	return true;
}

QMap<QString, QVariant> Results::compact() const
{
/*	QMap<QString, QVariant> r;
	r["GUI"] = "Results";
	r["nPercentiles"] = percentiles.size();
	QStringList pList;
	for (int i = 0; i < percentiles.size(); i++)
	{
		QString code = QString("Percentile %1").arg(i);
		r[code] = percentile2List(percentiles[i]);
	}
	
	r["nRealizations"] = realizations.size();
	QStringList rList;
	for (int i = 0; i < realizations.size(); i++)
	{
		QString code = QString("Realization %1").arg(i);
		r[code] = percentile2List(percentiles[i]);
	}

	QString vvCName;
	vector<vector<CBTCSet>> vvC;
	int i, j;
	QStringList sizes;

	vvC = realizations;
	vvCName = "realizations";
	i = vvC.size();
	j = (i) ? vvC[0].size() : 0;
	sizes.clear();
	sizes.append(QString::number(i));
	sizes.append(QString::number(j));
	QString code = QString("nn%1").arg(vvCName);
	r[code] = sizes;
	for (int ii = 0; ii < i; ii++)
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			r[code] = vvC[ii][jj].compact();
		}

	vvC = realizationsPercentiles;
	vvCName = "realizationsPercentiles";
	i = vvC.size();
	j = (i) ? vvC[0].size() : 0;
	sizes.clear();
	sizes.append(QString::number(i));
	sizes.append(QString::number(j));
	code = QString("nn%1").arg(vvCName);
	r[code] = sizes;
	for (int ii = 0; ii < i; ii++)
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			r[code] = vvC[ii][jj].compact();
		}

	vvC = noiseRealizations;
	vvCName = "noiseRealizations";
	i = vvC.size();
	j = (i) ? vvC[0].size() : 0;
	sizes.clear();
	sizes.append(QString::number(i));
	sizes.append(QString::number(j));
	code = QString("nn%1").arg(vvCName);
	r[code] = sizes;
	for (int ii = 0; ii < i; ii++)
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			r[code] = vvC[ii][jj].compact();
		}

	vvC = noiseRealizationsPercentiles;
	vvCName = "noiseRealizationsPercentiles";
	i = vvC.size();
	j = (i) ? vvC[0].size() : 0;
	sizes.clear();
	sizes.append(QString::number(i));
	sizes.append(QString::number(j));
	code = QString("nn%1").arg(vvCName);
	r[code] = sizes;
	for (int ii = 0; ii < i; ii++)
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			r[code] = vvC[ii][jj].compact();
		}

	r["localSensitivityMatrix"] = localSensitivityMatrix.compact();
	r["globalSensitivityMatrix"] = globalSensitivityMatrix.compact();
	r["correlationMatrix"] = correlationMatrix.compact();

	r["nyoungAgeDistributionRealizations"] = youngAgeDistributionRealizations.size();
	for (int i = 0; i < youngAgeDistributionRealizations.size(); i++)
	{
		QString code = QString("youngAgeDistributionRealization %1").arg(i);
		r[code] = youngAgeDistributionRealizations[i].compact();
	}

	r["nyoungAgeDistributionRealizationPercentiles"] = youngAgeDistributionRealizationPercentiles.size();
	for (int i = 0; i < youngAgeDistributionRealizationPercentiles.size(); i++)
	{
		QString code = QString("youngAgeDistributionRealizationPercentile %1").arg(i);
		r[code] = youngAgeDistributionRealizationPercentiles[i].compact();
	}

	r["ncumulativeYoungAgeDistributionRealizations"] = cumulativeYoungAgeDistributionRealizations.size();
	for (int i = 0; i <cumulativeYoungAgeDistributionRealizations.size(); i++)
	{
		QString code = QString("cumulativeYoungAgeDistributionRealization %1").arg(i);
		r[code] = cumulativeYoungAgeDistributionRealizations[i].compact();
	}

	r["ncumulativeYoungAgeDistributionRealizationPercentiles"] = cumulativeYoungAgeDistributionRealizationPercentiles.size();
	for (int i = 0; i <cumulativeYoungAgeDistributionRealizationPercentiles.size(); i++)
	{
		QString code = QString("cumulativeYoungAgeDistributionRealizationPercentile %1").arg(i);
		r[code] = cumulativeYoungAgeDistributionRealizationPercentiles[i].compact();
	}

	r["nprojectedTracerConcentrations"] = projectedTracerConcentrations.size();
	for (int i = 0; i <projectedTracerConcentrations.size(); i++)
	{
		QString code = QString("projectedTracerConcentration %1").arg(i);
		r[code] = projectedTracerConcentrations[i].compact();
	}

	r["nprojectedTracerConcentrationPercentiles"] = projectedTracerConcentrationPercentiles.size();
	for (int i = 0; i <projectedTracerConcentrationPercentiles.size(); i++)
	{
		QString code = QString("projectedTracerConcentrationPercentile %1").arg(i);
		r[code] = projectedTracerConcentrationPercentiles[i].compact();
	}
	
	r["ANS_obs"] = ANS_obs.compact();
	r["projected"] = projected.compact();
	r["posteriors"] = posteriors.compact();
	r["priors"] = priors.compact();
	r["observations"] = observations;

	return r;
	*/
return QMap<QString, QVariant>();
}

Results* Results::unCompact(QMap<QString, QVariant> r)
{
	/*Results* results = new Results();
	int percentilesSize = r["nPercentiles"].toInt();
	for (int i = 0; i < percentilesSize; i++)
	{
		QString code = QString("Percentile %1").arg(i);
		results->percentiles.push_back(percentileFromList(r[code].toStringList()));
	}
	QString vvCName;
	vector<vector<CBTCSet>> vvC;
	int i, j;
	QStringList sizes;
	QString code;

	vvCName = "realizations";
	code = QString("nn%1").arg(vvCName);
	sizes = r[code].toStringList();
	i = sizes[0].toInt();
	j = sizes[1].toInt();
	if (i)
		results->realizations.resize(i);
	for (int ii = 0; ii < i; ii++)
	{
		results->realizations[ii].resize(j);
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			results->realizations[ii][jj] = CBTCSet::unCompact(r[code].toMap());
		}
	}

	vvCName = "realizationsPercentiles";
	code = QString("nn%1").arg(vvCName);
	sizes = r[code].toStringList();
	i = sizes[0].toInt();
	j = sizes[1].toInt();
	if (i)
		results->realizationsPercentiles.resize(i);
	for (int ii = 0; ii < i; ii++)
	{
		results->realizationsPercentiles[ii].resize(j);
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			results->realizationsPercentiles[ii][jj] = CBTCSet::unCompact(r[code].toMap());
		}
	}

	vvCName = "noiseRealizations";
	code = QString("nn%1").arg(vvCName);
	sizes = r[code].toStringList();
	i = sizes[0].toInt();
	j = sizes[1].toInt();
	if (i)
		results->noiseRealizations.resize(i);
	for (int ii = 0; ii < i; ii++)
	{
		results->noiseRealizations[ii].resize(j);
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			results->noiseRealizations[ii][jj] = CBTCSet::unCompact(r[code].toMap());
		}
	}

	vvCName = "noiseRealizationsPercentiles";
	code = QString("nn%1").arg(vvCName);
	sizes = r[code].toStringList();
	i = sizes[0].toInt();
	j = sizes[1].toInt();
	if (i)
		results->noiseRealizationsPercentiles.resize(i);
	for (int ii = 0; ii < i; ii++)
	{
		results->noiseRealizationsPercentiles[ii].resize(j);
		for (int jj = 0; jj < j; jj++)
		{
			QString code = QString("%1 %2, %3").arg(vvCName).arg(ii).arg(jj);
			results->noiseRealizationsPercentiles[ii][jj] = CBTCSet::unCompact(r[code].toMap());
		}
	}


	results->localSensitivityMatrix = CMatrix::unCompact(r["localSensitivityMatrix"].toMap());
	results->globalSensitivityMatrix = CMatrix::unCompact(r["globalSensitivityMatrix"].toMap());
	results->correlationMatrix = CMatrix::unCompact(r["correlationMatrix"].toMap());

	for (int i = 0; i < r["nyoungAgeDistributionRealizations"].toInt(); i++)
	{
		QString code = QString("youngAgeDistributionRealization %1").arg(i);
		results->youngAgeDistributionRealizations.push_back(CBTCSet::unCompact(r[code].toMap()));
	}
	for (int i = 0; i < r["nyoungAgeDistributionRealizationPercentiles"].toInt(); i++)
	{
		QString code = QString("youngAgeDistributionRealizationPercentile %1").arg(i);
		results->youngAgeDistributionRealizationPercentiles.push_back(CBTCSet::unCompact(r[code].toMap()));
	}
	for (int i = 0; i < r["ncumulativeYoungAgeDistributionRealizations"].toInt(); i++)
	{
		QString code = QString("cumulativeYoungAgeDistributionRealization %1").arg(i);
		results->cumulativeYoungAgeDistributionRealizations.push_back(CBTCSet::unCompact(r[code].toMap()));
	}
	for (int i = 0; i < r["ncumulativeYoungAgeDistributionRealizationPercentiles"].toInt(); i++)
	{
		QString code = QString("cumulativeYoungAgeDistributionRealizationPercentile %1").arg(i);
		results->cumulativeYoungAgeDistributionRealizationPercentiles.push_back(CBTCSet::unCompact(r[code].toMap()));
	}
	for (int i = 0; i < r["nprojectedTracerConcentrations"].toInt(); i++)
	{
		QString code = QString("projectedTracerConcentration %1").arg(i);
		results->projectedTracerConcentrations.push_back(CBTCSet::unCompact(r[code].toMap()));
	}
	for (int i = 0; i < r["nprojectedTracerConcentrationPercentiles"].toInt(); i++)
	{
		QString code = QString("projectedTracerConcentrationPercentile %1").arg(i);
		results->projectedTracerConcentrationPercentiles.push_back(CBTCSet::unCompact(r[code].toMap()));
	}
//	for (int i = 0; i < r[""].toInt(); i++)
//	{
//		QString code = QString(" %1").arg(i);
//		results->.push_back(CBTCSet::unCompact(r[code].toMap()));
//	}

	results->ANS_obs = CBTCSet::unCompact(r["ANS_obs"].toMap());
	results->projected = CBTCSet::unCompact(r["projected"].toMap());
	results->posteriors = CBTCSet::unCompact(r["posteriors"].toMap());
	results->priors = CBTCSet::unCompact(r["priors"].toMap());
	results->observations = r["observations"].toStringList();
	
	return results;
	*/
return 0;
}



QStringList percentile2List(percentileData p)
{
	QStringList r;
	r << QString::fromStdString(p.parameter) << QString::number(p.p25) << QString::number(p.p50) << QString::number(p.p975) << QString::number(p.mean) << QString::number(p.std);
	return r;
}
percentileData percentileFromList(QStringList r)
{
	percentileData p;
	p.parameter = r[0].toStdString();
	p.p25 = r[1].toDouble();
	p.p50 = r[2].toDouble();
	p.p975 = r[3].toDouble();
	p.mean = r[4].toDouble();
	p.std = r[5].toDouble();
	return p;
}

