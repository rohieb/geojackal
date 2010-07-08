/**
 * @file Failure.h
 * Generic Failure class with user-defined message
 * @date Jul 7, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
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
  QString message_;
};

}

#endif /* FAILURE_H_ */
