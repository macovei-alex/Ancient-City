#pragma once

#include <iostream>
#include <fstream>
#include <string_view>
#include <string>

class Logger
{
public:
	enum class Level : uint8_t
	{
		Info,
		Warning,
		Error
	};

public:
	Logger(std::ostream& os, Level minimumLevel = Level::Info);
	Logger(const std::string& filename, Level minimumLevel = Level::Info);

	void SetMinimumLogLevel(Level level);

	void Log(std::string_view message, Level level = Level::Info) const;
	void operator()(std::string_view message, Level level = Level::Info) const;

public:
	static Logger cout;

private:
	std::ostream& m_os;
	Level m_minimumLevel;
	std::ofstream temp_outputStream;
};
