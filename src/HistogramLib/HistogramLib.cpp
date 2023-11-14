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

#include "HistogramLib.h"

#include <opencv2/opencv.hpp>

constexpr auto UINT8_VAL_RANGE = 256;

using namespace cv;
using namespace std;

extern HISTOGRAM_API vector<int>* getFullSubrasterHistogramFromFile(
	const char* filePath,
	const unsigned int bucketCount,
	const int rasterSideLength)
{
	const Mat image = imread(filePath);

	if (image.empty())
	{
		throw std::exception{ "Could not read image!" };
	}

	const int height = image.size().height;
	const int width = image.size().width;
	const int channels = image.channels();

	const long long lBucketCount = bucketCount;

	const auto buckets = new vector<int>(lBucketCount * channels * rasterSideLength * rasterSideLength);

	const uint8_t* pixelPtr = image.data;
	for (int y = 0; y < image.rows; y++)
	{
		const int segmentModY = y * rasterSideLength / height;
		for (int x = 0; x < image.cols; x++)
		{
			const unsigned int segment = x * rasterSideLength / width + segmentModY * rasterSideLength;
			const unsigned int segmentStartIndex = segment * bucketCount * channels;

			const uint8_t b = pixelPtr[y * image.cols * channels + x * channels + 0];
			const uint8_t g = pixelPtr[y * image.cols * channels + x * channels + 1];
			const uint8_t r = pixelPtr[y * image.cols * channels + x * channels + 2];

			(*buckets)[segmentStartIndex + b * lBucketCount / UINT8_VAL_RANGE]++;
			(*buckets)[segmentStartIndex + g * lBucketCount * 2 / UINT8_VAL_RANGE]++;
			(*buckets)[segmentStartIndex + r * lBucketCount * 3 / UINT8_VAL_RANGE]++;
		}
	}

	return buckets;
}

extern HISTOGRAM_API void getFullSubrasterHistogramFromFile(
	const char* filePath,
	const unsigned int bucketCount,
	const int rasterSideLength,
	void (callback)(int* data, int length))
{
	const auto result = getFullSubrasterHistogramFromFile(filePath, bucketCount, rasterSideLength);
	callback(result->data(), static_cast<int>(result->size()));
	delete result;
}
