#pragma once

struct OutputData
{
	bool output_enabled;
	bool debug;
	OutputData(const bool output_enabled, const bool debug) : output_enabled(output_enabled), debug(debug) {}
};