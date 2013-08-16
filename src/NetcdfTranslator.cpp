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

#include "NetcdfTranslator.h"
#include <NetcdfField.h>
#include <wdbLogHandler.h>
#include <fimex/CDMReader.h>
#include <fimex/Data.h>
#include <boost/foreach.hpp>


NetcdfTranslator::NetcdfTranslator(const CdmLoaderConfiguration & conf) :
	conf_(conf),
	loadConfiguration_(conf.loadConfiguration())
{
}

NetcdfTranslator::~NetcdfTranslator()
{
}


class DataRetriever
{
public:
	DataRetriever(const LoadElement & loadElement, const NetcdfField & field, unsigned timeIndex, unsigned realizationIndex,
			const DataSpecification & querySpec) :
		loadElement_(loadElement),
		field_(field),
		timeIndex_(timeIndex),
		realizationIndex_(realizationIndex),
		querySpec_(querySpec)
	{
	}

	WriteQuery::RawData operator() () const
	{
		WDB_LOG & log = WDB_LOG::getInstance( "wdb.netcdfload.get_data" );

		WriteQuery::RawData ret;

		boost::shared_ptr<MetNoFimex::CDMReader> reader = field_.reader();

		MetNoFimex::SliceBuilder slicer(reader->getCDM(), loadElement_.cfName());

		const std::string & timeDimension = field_.timeDimension();
		if ( not timeDimension.empty() )
			slicer.setStartAndSize(timeDimension, timeIndex_, 1);

		const std::string & realizationDimension = field_.realizationDimension();
		if ( not realizationDimension.empty() )
			slicer.setStartAndSize(realizationDimension, realizationIndex_, 1);

		BOOST_FOREACH(const LoadElement::IndexNameToValue::value_type & nameValue, loadElement_.indicesToLoad())
		{
			unsigned index = loadElement_.cdmIndex(* reader, nameValue.first, nameValue.second);
			slicer.setStartAndSize(nameValue.first, index, 1);
		}

		std::string unit = querySpec_.wdbUnits();
		MetNoFimex::DataPtr data;
		if ( not unit.empty() )
		{
			try
			{
				data = reader->getScaledDataSliceInUnit(loadElement_.cfName(), unit, slicer);
			}
			catch ( std::exception & e )
			{
				static std::set<std::string> parametersWarnedAbout;
				if ( parametersWarnedAbout.find(querySpec_.wdbParameter()) == parametersWarnedAbout.end() )
				{
					log.errorStream() << e.what();
					log.warnStream() << "Ignoring automatic conversion of units for parameter <" << querySpec_.wdbParameter() << '>';
					parametersWarnedAbout.insert(querySpec_.wdbParameter());
				}
				unit.clear();
			}
		}
		if ( unit.empty() )
			data = reader->getDataSlice(loadElement_.cfName(), slicer);

		ret.numberOfValues = data->size();
		ret.data = data->asFloat();

		if ( querySpec_.scale() != 1 )
			for ( int i = 0; i < ret.numberOfValues; ++ i )
				ret.data[i] *= querySpec_.scale();

		return ret;
	}
private:
	LoadElement loadElement_;
	const NetcdfField & field_;
	unsigned timeIndex_;
	unsigned realizationIndex_;
	DataSpecification querySpec_;
};

std::vector<WriteQuery> NetcdfTranslator::queries(const NetcdfField & field) const
{
	std::vector<WriteQuery> ret;

	BOOST_FOREACH( const LoadElement & loadElement, loadConfiguration_.getLoadElement(field) )
	{
		const DataSpecification & querySpec = loadElement.wdbDataSpecification();

		WriteQuery base;
		base.dataProvider(conf_.loading().dataProvider);
		base.referenceTime(field.referenceTime());
		setLocation_(base, field);

		std::vector<int> realizations = field.realizations();

		base.valueParameterName(querySpec.wdbParameter());

		base.levelParameterName(querySpec.level().name());
		base.levelFrom(querySpec.level().value());
		base.levelTo(querySpec.level().value());


		const std::vector<Time> & times = field.times();
		for ( unsigned timeIndex = 0; timeIndex < times.size(); ++ timeIndex )
		{
			base.validTimeFrom(querySpec.validTimeFrom().getTime(field.referenceTime(), times[timeIndex]));
			base.validTimeTo(querySpec.validTimeTo().getTime(field.referenceTime(), times[timeIndex]));

			BOOST_FOREACH(int realization, realizations)
			{
				base.dataVersion(realization);

				base.data(DataRetriever(loadElement, field, timeIndex, realization, querySpec));

				ret.push_back(base);
			}
		}
	}
	return ret;
}

void NetcdfTranslator::setLocation_(WriteQuery & out, const NetcdfField & field) const
{
	if ( not conf_.loading().placeName.empty() )
		out.placeName(conf_.loading().placeName);
	out.location(field.placeSpecification());
	out.loadPlaceDefinition(conf_.loading().loadPlaceDefinition);
}