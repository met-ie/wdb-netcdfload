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


#ifndef ABSTRACTDATARETIREVER_H_
#define ABSTRACTDATARETIREVER_H_

#include "RawData.h"
#include <boost/shared_ptr.hpp>

class AbstractDataRetriever
{
public:
	typedef boost::shared_ptr<AbstractDataRetriever> Ptr;

	virtual ~AbstractDataRetriever() {}

	virtual RawData operator() () const =0;
};


#endif /* ABSTRACTDATARETIREVER_H_ */
