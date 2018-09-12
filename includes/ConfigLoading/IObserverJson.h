#pragma once

#include "GymEnv/StateObserver/IStateObserver.hpp"
#include "GymEnv/StateObserver/GridObserver.hpp"
#include <json.hpp>
#include <memory>

namespace GymEnv { namespace StateObserver
{
	void to_json(nlohmann::json& j, const IStateObserver* observer);
	void to_json(nlohmann::json& j, const GridObserver* observer);

	void from_json(const nlohmann::json& j, std::shared_ptr<IStateObserver>& observer);
	void from_json(const nlohmann::json& j, std::shared_ptr<GridObserver>& observer);
}}
