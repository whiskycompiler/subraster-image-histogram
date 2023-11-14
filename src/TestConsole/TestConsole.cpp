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

#include <iostream>
#include <iomanip>
#include <numeric>
#include <HistogramLib/HistogramLib.h>

using namespace std;

#define COL_LEFT setw(15) << left <<

double normalisedBhattacharyyaDistance(vector<int>* x, vector<int>* y)
{
	const double xSum = max(1, reduce(x->begin(), x->end()));
	const double ySum = max(1, reduce(y->begin(), y->end()));

	double b = 0;
	for (unsigned int i = 0; i < x->size(); i++)
	{
		b += sqrt(((*x)[i] / xSum) * ((*y)[i] / ySum));
	}

	if(b <= 0 || b >= 1)
	{
		return 0;
	}

	return sqrt(1.0 - min(b, 1.0));
}

void printHistogramVector(const vector<int>* vector, const int bucketCount)
{
	cout << vector->size() << endl << endl;
	int index = 0;
	for (const int value : *vector)
	{
		if (index++ % bucketCount == 0)
		{
			cout << "----" << endl;
		}

		cout << value << endl;
	}
}

void compareSetWithNormalBhattacharyya(
	vector<int>* p_hist1,
	vector<int>* p_hist2,
	vector<int>* p_hist3,
	vector<int>* p_hist4)
{
	const double diff1v2 = normalisedBhattacharyyaDistance(p_hist1, p_hist2);
	const double diff2v3 = normalisedBhattacharyyaDistance(p_hist2, p_hist3);
	const double diff1v3 = normalisedBhattacharyyaDistance(p_hist1, p_hist3);

	const double diff1v4 = normalisedBhattacharyyaDistance(p_hist1, p_hist4);
	const double diff2v4 = normalisedBhattacharyyaDistance(p_hist2, p_hist4);

	cout << diff1v2 << endl;
	cout << diff2v3 << endl;
	cout << diff1v3 << endl << endl;
	cout << diff1v4 << endl;
	cout << diff2v4 << endl;
}

double fragmentedBhattacharyyaDistance(
	const vector<int>* x,
	const vector<int>* y,
	const unsigned int fragmentLength)
{
	if (x->size() != y->size())
	{
		throw length_error{ "Vector sizes are not identical!" };
	}

	if (x->size() % fragmentLength != 0)
	{
		throw invalid_argument{ "Vector size is not dividable by given fragment size!" };
	}

	const unsigned long long fragmentCount = x->size() / fragmentLength;
	const auto foo = new vector<double>(fragmentCount);
	for (unsigned int fragmentIndex = 0; fragmentIndex < fragmentCount; fragmentIndex++)
	{
		auto iteratorX = x->begin() + static_cast<long long>(fragmentLength) * fragmentIndex;
		auto iteratorY = y->begin() + static_cast<long long>(fragmentLength) * fragmentIndex;
		auto fragmentX = new vector(iteratorX, iteratorX + fragmentLength);
		auto fragmentY = new vector(iteratorY, iteratorY + fragmentLength);

		(*foo)[fragmentIndex] = normalisedBhattacharyyaDistance(fragmentX, fragmentY);
	}

	for (double& val : *foo)
	{
		if (val >= 0.5)
		{
			val = sqrt(val);
		}
		else
		{
			val = val * val;
		}
	}

	return reduce(foo->begin(), foo->end()) / static_cast<int>(fragmentCount);
}

void compareSetWithFragmentedBhattacharyya(
	const vector<int>* p_hist1,
	const vector<int>* p_hist2,
	const vector<int>* p_hist3,
	const vector<int>* p_hist4,
	const int fragmentLength)
{
	const double diff1v2 = fragmentedBhattacharyyaDistance(p_hist1, p_hist2, fragmentLength);
	const double diff2v3 = fragmentedBhattacharyyaDistance(p_hist2, p_hist3, fragmentLength);
	const double diff1v3 = fragmentedBhattacharyyaDistance(p_hist1, p_hist3, fragmentLength);

	const double diff1v4 = fragmentedBhattacharyyaDistance(p_hist1, p_hist4, fragmentLength);
	const double diff2v4 = fragmentedBhattacharyyaDistance(p_hist2, p_hist4, fragmentLength);

	cout << COL_LEFT diff1v2 << " | ";
	cout << COL_LEFT diff2v3 << " | ";
	cout << COL_LEFT diff1v3 << " | ";
	cout << COL_LEFT diff1v4 << " | ";
	cout << COL_LEFT diff2v4 << " | ";
	cout << COL_LEFT p_hist1->size() << endl;
}

//void callback(int* data, int length)
//{
//	for (int i = 0; i < length; i++)
//	{
//		cout << data[i] << endl;
//	}
//}


int main()
{
	//const vector<tuple<int, int>> params = { { 32, 32},{ 24, 24},{ 18, 18},{ 16, 16}, {14,14}, {12,12}, {10, 10}, {8,8} };
	//const vector<tuple<int, int>> params = { { 32, 16},{ 24, 16},{ 18, 16},{ 16, 16}, {14,16}, {12,16}, {10, 16}, {8,16} };
	const vector<tuple<int, int>> params = { { 32, 14},{ 24, 14},{ 18, 14},{ 16, 14}, {14,14}, {12,14}, {10, 14}, {8,14} };
	//const vector<tuple<int, int>> params = { { 16, 32},{ 16, 24},{ 16, 18},{ 16, 16}, {16,14}, {16,12}, {16, 10}, {16,8} };

	cout << COL_LEFT "buckets" << " | ";
	cout << COL_LEFT "raster" << " || ";
	cout << COL_LEFT "similar1" << " | ";
	cout << COL_LEFT "similar2" << " | ";
	cout << COL_LEFT "very similar" << " | ";
	cout << COL_LEFT "different" << " | ";
	cout << COL_LEFT "different" << " | ";
	cout << COL_LEFT "size" << endl;

	for (const auto& param : params)
	{
		const int bucketCount = get<0>(param);
		const int rasterSideLength = get<1>(param);

		const vector<int>* p_hist1 = getFullSubrasterHistogramFromFile("C:\\workdir\\sim3a.jpg", bucketCount, rasterSideLength);
		const vector<int>* p_hist2 = getFullSubrasterHistogramFromFile("C:\\workdir\\sim3b.jpg", bucketCount, rasterSideLength);
		const vector<int>* p_hist3 = getFullSubrasterHistogramFromFile("C:\\workdir\\sim3c.jpg", bucketCount, rasterSideLength);
		const vector<int>* p_hist4 = getFullSubrasterHistogramFromFile("C:\\workdir\\test.jpg", bucketCount, rasterSideLength);

		//compareSetWithNormalBhattacharyya(p_hist1, p_hist2, p_hist3, p_hist4);
		cout << COL_LEFT bucketCount << " | ";
		cout << COL_LEFT rasterSideLength << " || ";
		compareSetWithFragmentedBhattacharyya(p_hist1, p_hist2, p_hist3, p_hist4, bucketCount);
	}
}
