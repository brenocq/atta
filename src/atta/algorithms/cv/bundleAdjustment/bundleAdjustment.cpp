//--------------------------------------------------
// Atta Algorithms - Computer Vision
// bundleAdjustment.cpp
// Date: 2021-04-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/cv/bundleAdjustment/bundleAdjustment.h>

namespace atta::cv
{
	BundleAdjustment::BundleAdjustment()
	{

	}

	void BundleAdjustment::run()
	{

	}

	void BundleAdjustment::clear()
	{
		_measurements.clear();
		_cameraPoses.clear();
		_points3D.clear();
	}

	void BundleAdjustment::addImage(vec cameraPose, std::vector<std::pair<size_t, vec2>> measurements)
	{
		_cameraPoses.push_back(cameraPose);
		for(const auto& measurement : measurements)
		{
			Measurement m;
			m.imageIndex = _cameraPoses.size()-1;
			m.point3DIndex = measurement.first;
			m.point = measurement.second;
			_measurements.push_back(m);
		}
	}
}
