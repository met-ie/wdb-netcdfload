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

#ifndef NETCDFTRANSLATOR_H_
#define NETCDFTRANSLATOR_H_

#include "WriteQuery.h"
#include "configuration/CdmLoaderConfiguration.h"
#include "configuration/LoadConfiguration.h"
#include <vector>

class AbstractNetcdfField;

/**
 * Creates WriteQuery objects from AbstractNetcdfField objects, based on program configuration.
 */
class NetcdfTranslator
{
public:
	NetcdfTranslator(const CdmLoaderConfiguration & conf);
	~NetcdfTranslator();

	std::vector<WriteQuery> queries(const AbstractNetcdfField & field) const;
	std::vector<WriteQuery> queries(const AbstractNetcdfField & field, const std::vector<CdmLoaderConfiguration::Point> & points) const;

	const LoadConfiguration & loadConfiguration() const { return loadConfiguration_; }

private:
	void setLocation_(WriteQuery & out, const AbstractNetcdfField & field) const;
	WriteQuery adaptQuery_(WriteQuery query, const CdmLoaderConfiguration::Point & point, float value) const;

	/**
	 * Remove any elements that conf_ does not say should be there.
	 */
	std::vector<LoadElement> filter_(const std::vector<LoadElement> & loadElements) const;

	CdmLoaderConfiguration conf_;
	LoadConfiguration loadConfiguration_;
};

#endif /* NETCDFTRANSLATOR_H_ */
