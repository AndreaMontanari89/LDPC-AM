#include "stdafx.h"
#include "Utils.h"

std::vector<int> IntVectorFromString(wxString strWork)
{
	std::vector<int>	retVal;
	wxString			strAccumulator = wxEmptyString;

	for (uint ch = 0; ch < strWork.length(); ch++)
	{
		if (strWork[ch] == ' ' || strWork[ch] == '\t')
		{
			retVal.push_back(wxAtoi(strAccumulator));
			strAccumulator.Clear();
		}
		else
			strAccumulator += strWork[ch];
	}
	if (strAccumulator != wxEmptyString)
		retVal.push_back(wxAtoi(strAccumulator));

	strAccumulator.Clear();

	return retVal;
}

std::vector<double> DoubleVectorFromString(wxString strWork)
{
	std::vector<double>	retVal;
	wxString			strAccumulator = wxEmptyString;

	strWork.Trim(false);

	for (uint ch = 0; ch < strWork.length(); ch++)
	{
		if (strWork[ch] == ' ' || strWork[ch] == '\t')
		{
			retVal.push_back(wxAtof(strAccumulator));
			strAccumulator.Clear();
		}
		else
			strAccumulator += strWork[ch];
	}
	if (strAccumulator != wxEmptyString)
		retVal.push_back(wxAtoi(strAccumulator));

	strAccumulator.Clear();

	return retVal;
}

