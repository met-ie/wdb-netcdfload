/*
 pgen_wdbSave

 Copyright (C) 2011 met.no

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

#ifndef CDMLOADERCONFIGURATION_H_
#define CDMLOADERCONFIGURATION_H_

#include "configuration/parameter/NetcdfParameterSpecification.h"
#include <wdb/LoaderConfiguration.h>
#include <boost/lexical_cast.hpp>


class CdmLoaderConfiguration : public wdb::load::LoaderConfiguration
{
public:
	CdmLoaderConfiguration();
	virtual ~CdmLoaderConfiguration();

	const std::string fileType() const { return fileType_; }
	const std::string & fileTypeConfiguration() const { return fileTypeConfiguration_; }
	const std::string & loadConfiguration() const { return loadConfiguration_; }

	class Point
	{
	public:
		Point(const std::string & spec);
		double longitude() const;
		double latitude() const;
		std::string getPlaceName() const;
	private:
		friend class CdmLoaderConfiguration;

		// We use string types here to avoid any inconsistencies with rounding
		std::string longitude_;
		std::string latitude_;
	};
	//const Point * point() const;
	const std::vector<Point> * points() const { return pointsFile_.empty() ? 0 : & points_; }

	const std::vector<NetcdfParameterSpecification> & elementsToLoad() const { return elementsToLoad_; }

	virtual void parse( int argc, char ** argv );

private:
	std::string fileType_;
	std::string fileTypeConfiguration_;
	std::string loadConfiguration_;

	std::vector<NetcdfParameterSpecification> elementsToLoad_;

	//Point point_;
	std::string pointsFile_;
	std::vector<Point> points_;
};

#endif /* CDMLOADERCONFIGURATION_H_ */
