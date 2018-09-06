#pragma once

#include "QTable.h"
#include <string>

namespace AI { namespace QLearning { namespace Utils
{
	void SaveQTable(const QTable& qtab, const char* filePath);
	QTable LoadQTable(const char* filePath);
}}}
