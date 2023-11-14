/* -----------------------------------------------------------------------
	Copyright (C) 2023 whiskycompiler

	This file is part of the subraster image histogram library.

	This program is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <https://www.gnu.org/licenses/>.
--------------------------------------------------------------------------*/

#pragma once

#ifdef HistogramLib_EXPORTS
#define HISTOGRAM_API __declspec(dllexport)
#else
#define HISTOGRAM_API __declspec(dllimport)
#endif
#include <vector>

/// <summary>
/// Gets a histogram from an image by binning all pixel channel values using the given raster size
/// from the top left to the bottom right of the image.
/// </summary>
HISTOGRAM_API std::vector<int>* getFullSubrasterHistogramFromFile(
	const char* filePath,
	unsigned int bucketCount,
	int rasterSideLength);

/// <summary>
/// Gets a histogram from an image by binning all pixel channel values using the given raster size
/// from the top left to the bottom right of the image.
/// </summary>
HISTOGRAM_API void getFullSubrasterHistogramFromFile(
	const char* filePath,
	unsigned int bucketCount,
	int rasterSideLength,
	void (callback)(int* data, int length));
