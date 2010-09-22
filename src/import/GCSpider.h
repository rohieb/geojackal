/*
 * @file GCSpider.h
 * @date 22.02.2010
 * @author Roland Hieber
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

#ifndef GCSPIDER_H_
#define GCSPIDER_H_

#include "global.h"
#include "logic/Geocache.h"
#include "logic/Coordinate.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkCookie>

using namespace std;

namespace geojackal {

/**
 * Class that encapsulate functionalty to retrieve geocaches from the
 * geocaching.com database.
 *
 * This class is intented to use as a singleton, the instance can be acquired
 * by calling @a login().
 */
class GCSpider : public QObject {
  Q_OBJECT

public:

  static GCSpider * login(const QString username, const QString password);
  static void logout();

  bool nearest(const Coordinate center, const float maxDist,
    QList<Geocache *>& buf);
  bool single(const QString waypoint, Geocache& buf);

  QMap<QString,QString> getAspFormFields(const QString& htmlText);

  /** HTTP User Agent to use */
  static const QByteArray USER_AGENT;

  /**
   * @return @c true if the user has been successfully logged in
   */
  inline bool loggedIn() {
    return loggedIn_;
  }
public slots:
  void loadPageFinished(QNetworkReply * reply);

protected:
  QNetworkReply * loadPage(const QUrl& url, const QByteArray * formData = 0);

private:
  GCSpider();
  GCSpider(const GCSpider&);
  virtual ~GCSpider();

  /** QNetworkAccessManager instance for HTTP communication */
  QNetworkAccessManager * pnam_;
  /** QNetworkReply of the currently loaded page, used by @a loadPage() */
  QNetworkReply * loadPageNetReply_;
  /** Is the user already logged in? */
  bool loggedIn_;

  /** The singleton instance */
  static GCSpider * instance_;

  /** Private guard class to get rid of the instance at end of run time */
  class Guard {
  public:
    ~Guard() {
      if(GCSpider::instance_ != 0) {
        delete GCSpider::instance_;
      }
    }
  };
  friend class Guard;
};

}

#endif /* GCSPIDER_H_ */
