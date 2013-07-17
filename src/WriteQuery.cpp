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

#include "WriteQuery.h"
#include <wdb/LoaderDatabaseConnection.h>

WriteQuery::WriteQuery() :
	loadPlaceDefinition_(false),
	levelFrom_(0),
	levelTo_(0),
	referenceTime_(INVALID_TIME),
	validTimeFrom_(INVALID_TIME),
	validTimeTo_(INVALID_TIME),
	dataVersion_(0)
{
}

WriteQuery::~WriteQuery()
{
}

std::ostream & WriteQuery::list(std::ostream & out) const
{
	out << dataProvider_ << '\t';
	if ( not placeName_.empty() )
		out << placeName_;
	else if ( location_ )
		out << location_->wktRepresentation();
	else
		out << "<unknown place>";
	out << '\t';
	out << time_to_postgresql_string(referenceTime_) << '\t';
	out << time_to_postgresql_string(validTimeFrom_) << '\t';
	out << time_to_postgresql_string(validTimeTo_) << '\t';
	out << valueParameterName_ << '\t';
	out << levelParameterName_ << '\t';
	out << levelFrom_ << '\t';
	out << levelTo_ << '\t';
	out << dataVersion_ << '\n';

	return out;
}

void WriteQuery::write(wdb::load::LoaderDatabaseConnection & wdbConnection) const
{
	RawData rawData = function_();

	std::string placeName = placeName_;
	if ( placeName_.empty() )
		placeName = wdbConnection.getPlaceName(* location_);
	if ( loadPlaceDefinition_ )
	{
		if ( placeName.empty() )
			placeName = "netcdfLoad auto";
		wdbConnection.addPlaceDefinition(placeName, * location_);
	}

	std::clog << "SELECT wci.write(<data>, "
			<< dataProvider_ << ", "
			<< placeName << ", "
			<< time_to_postgresql_string(referenceTime_) << ", "
			<< time_to_postgresql_string(validTimeFrom_) << ", "
			<< time_to_postgresql_string(validTimeTo_) << ", "
			<< valueParameterName_ << ", "
			<< levelParameterName_ << ", "
			<< levelFrom_ << ", "
			<< levelTo_ << ", "
			<< dataVersion_ << ", "
			<< "0);\n";

	wdbConnection.write(
			rawData.data.get(),
			rawData.numberOfValues,
			dataProvider_,
			placeName,
			time_to_postgresql_string(referenceTime_),
			time_to_postgresql_string(validTimeFrom_),
			time_to_postgresql_string(validTimeTo_),
			valueParameterName_,
			levelParameterName_,
			levelFrom_,
			levelTo_,
			dataVersion_,
			0
	);
}
