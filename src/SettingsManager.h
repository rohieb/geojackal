/**
 * @file SettingsManager.h
 * @date Jul 20, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "Coordinate.h"
#include <QSettings>
#include <QDir>

namespace geojackal {

struct Coordinate;

/**
 * Simple wrapper for QSettings. It basically translates the QSettings
 * setValue/value interface to a semantically interface and provides default
 * values.
 */
class SettingsManager {
public:

  static SettingsManager * instance();

  static QDir storageLocation();

  QString gcUsername();
  void setGcUsername(const QString username);

  QString gcPassword();
  void setGcPassword(const QString password);

  qreal maxImportDist();
  void setMaxImportDist(qreal dist);

  Coordinate center();
  void setCenter(const Coordinate& center);

private:
  SettingsManager();
  SettingsManager(const SettingsManager&);
  virtual ~SettingsManager();

  /** Private guard class to get rid of the instance at end of run time */
  class Guard {
  public:
    ~Guard() {
      if(SettingsManager::instance_ != 0) {
        delete SettingsManager::instance_;
      }
    }
  };
  friend class Guard;

  /** Singleton instance */
  static SettingsManager * instance_;

  /** Wrapped QSettings object */
  QSettings * s;
};

}

#endif /* SETTINGSMANAGER_H_ */
