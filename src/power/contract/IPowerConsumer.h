//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_IPOWERCONSUMER_H
#define __INET_IPOWERCONSUMER_H

#include "PowerDefs.h"

namespace inet {

/**
 * This purely virtual interface provides an abstraction for different power consumers.
 *
 * @author Levente Meszaros
 */
class INET_API IPowerConsumer
{
  public:
    virtual ~IPowerConsumer() {}

//    virtual const char *getName() = 0;

    virtual W getPowerConsumption() = 0;
};

} // namespace inet

#endif // ifndef __INET_IPOWERCONSUMER_H

