/**
 * @file global.h
 * Some global definitions that are useful in all source files
 * @date Jul 8, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "Failure.h"
#include <QDebug>
#include <QStringBuilder>
// fast string concatenation with + instead of %
#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS

/** The current program version */
static const QString VERSION = "0.1alpha";

#endif /* GLOBAL_H_ */
