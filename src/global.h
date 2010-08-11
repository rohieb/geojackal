/**
 * @file global.h
 * Some global definitions that are useful in all source files
 * @date Jul 8, 2010
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

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "Failure.h"
#include "SettingsManager.h"
#include <QDebug>
//#include <QStringBuilder>
// fast string concatenation with + instead of %
#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS

namespace geojackal {

class SettingsManager;

/** The program name */
static const QString APPNAME = "GeoJackal";
/** The current program version */
static const QString VERSION = "0.1alpha";

/** Global object for user defined settings */
extern SettingsManager * g_settings;

}

#endif /* GLOBAL_H_ */
