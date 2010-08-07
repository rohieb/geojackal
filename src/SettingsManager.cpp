/**
 * @file SettingsManager.cpp
 * @date Jul 20, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "SettingsManager.h"
#include <QDebug>
#include <QDesktopServices>

using namespace geojackal;

SettingsManager * geojackal::g_settings;

SettingsManager * SettingsManager::instance_ = 0;

SettingsManager::SettingsManager() : s(0) {
}

SettingsManager::~SettingsManager() {
  if(s) {
    delete s;
  }
}

/**
 * Get single instance
 */
SettingsManager * SettingsManager::instance() {
  static Guard guard; // delete the instance at end of run time

  if(!instance_) {
    instance_ = new SettingsManager;
  }

  instance_->s = new QSettings(storageLocation().absoluteFilePath("prefs.ini"),
    QSettings::IniFormat);
  qDebug() << "Storing data in" << instance_->s->fileName();

  return instance_;
}

/**
 * Get the path where the profile data is stored
 */
QDir SettingsManager::storageLocation() {
  return QDir(QDesktopServices::
    storageLocation(QDesktopServices::DataLocation));
}

/**
 * @{
 * The username used to login to geocaching.com
 */
QString SettingsManager::gcUsername() {
  return s->value("gc/username").toString();
}
void SettingsManager::setGcUsername(const QString username) {
  s->setValue("gc/username", username);
}
/** @} */

/**
 * @{
 * The password associated with the geocaching.com username
 */
QString SettingsManager::gcPassword() {
  return s->value("gc/password").toString();
}
void SettingsManager::setGcPassword(const QString password) {
  s->setValue("gc/password", password);
}
/** @} */

/**
 * @{
 * The maximum distance between the center and the geocaches to be imported
 */
qreal SettingsManager::maxImportDist() {
  bool ok;
  return s->value("gc/maxImportDist", 2.0).toDouble(&ok);
}
void SettingsManager::setMaxImportDist(qreal dist) {
  s->setValue("gc/maxImportDist", dist);
}
/** @} */

/**
 * @{
 * The center coordinate
 */
Coordinate SettingsManager::center() {
  bool ok;
  // default: IZ coordinates
  Angle lat = s->value("gc/centerLat", 52.273).toDouble(&ok);
  Angle lon = s->value("gc/centerLon", 10.52524).toDouble(&ok);
  return Coordinate(lat, lon);
}
void SettingsManager::setCenter(const Coordinate& center) {
  s->setValue("gc/centerLat", (double)center.lat);
  s->setValue("gc/centerLon", (double)center.lon);
}
/** @} */
