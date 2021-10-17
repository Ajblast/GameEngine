#pragma once

#include "GRAVEngine.h"
#include "snakeAgent.h"

class directionSensor : public GRAVEngine::AI::Sensors::ISensor
{
public:
	directionSensor(GRAVEngine::ref<snakeAgent> agent);
	virtual ~directionSensor() = default;

	virtual size_t write(GRAVEngine::AI::Sensors::observationWriter& writer) override;
	virtual void update() override;
	virtual void reset() override;
	virtual GRAVEngine::AI::Sensors::observationParams getObservationParams() override;

	// Used for sorting for determinism
	virtual std::string getName() override;
private:
	GRAVEngine::ref<snakeAgent> agent;
	float direction[1] = { 0 };
};

class headSensor : public GRAVEngine::AI::Sensors::ISensor
{
public:
	headSensor(GRAVEngine::ref<snakeAgent> agent);
	virtual ~headSensor() = default;

	virtual size_t write(GRAVEngine::AI::Sensors::observationWriter& writer) override;
	virtual void update() override;
	virtual void reset() override;
	virtual GRAVEngine::AI::Sensors::observationParams getObservationParams() override;

	// Used for sorting for determinism
	virtual std::string getName() override;
private:
	GRAVEngine::ref<snakeAgent> agent;
	float position[2] = { 0, 0 };
};

class fruitSensor : public GRAVEngine::AI::Sensors::ISensor
{
public:
	fruitSensor(GRAVEngine::ref<snakeAgent> agent);
	virtual ~fruitSensor() = default;

	virtual size_t write(GRAVEngine::AI::Sensors::observationWriter& writer) override;
	virtual void update() override;
	virtual void reset() override;
	virtual GRAVEngine::AI::Sensors::observationParams getObservationParams() override;

	// Used for sorting for determinism
	virtual std::string getName() override;
private:
	GRAVEngine::ref<snakeAgent> agent;
	float relativePosition[4] = { 0, 0, 0, 0 };
};

class wallSensor : public GRAVEngine::AI::Sensors::ISensor
{
public:
	wallSensor(GRAVEngine::ref<snakeAgent> agent, int wallWidth, int wallHeight, bool accountBody);
	virtual ~wallSensor() = default;

	virtual size_t write(GRAVEngine::AI::Sensors::observationWriter& writer) override;
	virtual void update() override;
	virtual void reset() override;
	virtual GRAVEngine::AI::Sensors::observationParams getObservationParams() override;

	// Used for sorting for determinism
	virtual std::string getName() override;
private:
	GRAVEngine::ref<snakeAgent> agent;
	float relativePositions[4] = { 0, 0, 0, 0 };
	int m_WallWidth, m_WallHeight;
	bool m_AccountBody;
};

class bodySizeSensor : public GRAVEngine::AI::Sensors::ISensor
{
public:
	bodySizeSensor(GRAVEngine::ref<snakeAgent> agent);
	virtual ~bodySizeSensor() = default;

	virtual size_t write(GRAVEngine::AI::Sensors::observationWriter& writer) override;
	virtual void update() override;
	virtual void reset() override;
	virtual GRAVEngine::AI::Sensors::observationParams getObservationParams() override;

	// Used for sorting for determinism
	virtual std::string getName() override;
private:
	GRAVEngine::ref<snakeAgent> agent;
	float bodySize[1] = { 0 };
};