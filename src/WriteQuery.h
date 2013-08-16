/*
 netcdfload

 Copyright (C) 2013 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA
 */

#ifndef WRITEQUERY_H_
#define WRITEQUERY_H_

#include "localtime.h"
#include <GridGeometry.h>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>
#include <iostream>

namespace wdb
{
namespace load
{
class LoaderDatabaseConnection;
}
}

class WriteQuery
{
public:
	WriteQuery();
	~WriteQuery();

	std::ostream & list(std::ostream & out = std::cout) const;

	void write(wdb::load::LoaderDatabaseConnection & wdbConnection) const;

	struct RawData
	{
		unsigned numberOfValues;
		boost::shared_array<float> data;
	};

	typedef boost::function<RawData ()> DataRetrievalFunction;

	void data(DataRetrievalFunction function) { function_ = function; }
	void dataProvider(const std::string & val) { dataProvider_ = val; }
	void location(const boost::shared_ptr<GridGeometry> & val) { location_ = val; }
	void placeName(const std::string & placeName) { placeName_ = placeName; }
	void referenceTime(const Time & val) { referenceTime_ = val; }
	void validTimeFrom(const Time & val) { validTimeFrom_ = val; }
	void validTimeTo(const Time & val) { validTimeTo_ = val; }
	void valueParameterName(const std::string & val) { valueParameterName_ = val; }
	void levelParameterName(const std::string & val) { levelParameterName_ = val; }
	void levelFrom(float val) { levelFrom_ = val; }
	void levelTo(float val) { levelTo_ = val; }
	void dataVersion(unsigned val) { dataVersion_ = val; }

	void loadPlaceDefinition(bool doLoad) { loadPlaceDefinition_ = doLoad; }

private:
	DataRetrievalFunction function_;
	std::string dataProvider_;
	boost::shared_ptr<GridGeometry> location_;
	std::string placeName_;
	bool loadPlaceDefinition_;
	Time referenceTime_;
	Time validTimeFrom_;
	Time validTimeTo_;
	std::string valueParameterName_;
	std::string levelParameterName_;
	float levelFrom_;
	float levelTo_;
	unsigned dataVersion_;
};

#endif /* WRITEQUERY_H_ */