#pragma once

#include "results.h"
#include "MediumSet.h"


CBTCSet Results::realization(const QString &observation) {
	if (hasRealization(observation))
	{
		int index = model->lookup_observation(observation.toStdString());
		return realizations[0][index];
	}
}
CBTCSet Results::noiseRealization(const QString &observation) {
	if (hasNoiseRealization(observation))
		return noiseRealizations[0][this->model->lookup_observation(observation.toStdString())];
}
CBTCSet Results::realizationPercentile(const QString &observation) {
	if (hasRealizationPercentile(observation))
		return realizationsPercentiles[0][this->model->lookup_observation(observation.toStdString())];
}
CBTCSet Results::noiseRealizationPercentile(const QString &observation) {
	if (hasNoiseRealizationPercentile(observation))
		return noiseRealizationsPercentiles[0][this->model->lookup_observation(observation.toStdString())];
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

