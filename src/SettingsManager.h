/**
 * @file SettingsManager.h
 * @date Jul 20, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

//#include <QtGlobal>
#include <QSettings>
#include "Coordinate.h"

namespace geojackal {

class Coordinate;

/**
 * Simple wrapper for QSettings. It basically translates the QSettings
 * setValue/value interface to a semantically interface and provides default
 * values.
 */
class SettingsManager {
public:
  SettingsManager();
  virtual ~SettingsManager() {}

  QString gcUsername();
  void setGcUsername(const QString username);

  QString gcPassword();
  void setGcPassword(const QString password);

  qreal maxImportDist();
  void setMaxImportDist(qreal dist);

  Coordinate center();
  void setCenter(const Coordinate& center);

private:
  QSettings s;
};

}

#endif /* SETTINGSMANAGER_H_ */
