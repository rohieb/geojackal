/**
 * @file Failure.h
 * Generic Failure class with user-defined message
 * @date Jul 7, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 *
 * Copyright (C) 2010 Roland Hieber
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License, version 3, as published 
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FAILURE_H_
#define FAILURE_H_

#include <exception>
#include <QString>

namespace geojackal {

/**
 * Generic Failure class with user-defined message
 */
class Failure : public std::exception {
public:
  Failure(QString message);
  virtual ~Failure() throw() {}
  virtual const char* what() const throw();

protected:
  /** Internal message */
  QByteArray message_;
};

}

#endif /* FAILURE_H_ */
