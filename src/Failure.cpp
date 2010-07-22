/**
 * @file Failure.cpp
 * Generic Failure class with user-defined message
 * @date Jul 7, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "Failure.h"

namespace geojackal {

Failure::Failure(QString message) :
  message_(message) {
}

/** @return Error message given by the thrower */
const char* Failure::what() const throw() {
  return message_.toUtf8();
}

}
