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

#ifndef NETCDFFILE_H_
#define NETCDFFILE_H_

#include "NetcdfField.h"
#include "localtime.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <iosfwd>


namespace MetNoFimex
{
class CDMReader;
}

/**
 * Handler class for a netcdf file
 */
class NetcdfFile
{
public:
	NetcdfFile(const std::string & fileName, const std::string & configurationFile, const std::string & fileType = "netcdf");
	~NetcdfFile();

	std::vector<NetcdfField::Ptr> getFields() const;

	Time referenceTime() const { return referenceTime_; }

private:
	boost::shared_ptr<MetNoFimex::CDMReader> reader_;
	Time referenceTime_;
};



#endif /* NETCDFFILE_H_ */
