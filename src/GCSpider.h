/*
 * @file GCSpider.h
 * @date 22.02.2010
 * @author Roland Hieber
 */

#ifndef GCSPIDER_H_
#define GCSPIDER_H_

#include "global.h"
#include "Cache.h"
#include "Coordinate.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkCookie>

using namespace std;

namespace geojackal {

/**
 * Class that encapsulate functionalty to retrieve caches from the
 * geocaching.com database.
 *
 * This class is intented to use as a singleton, the instance can be acquired
 * by calling @a login().
 */
class GCSpider : public QObject {
  Q_OBJECT

public:

  static GCSpider * login(const QString username, const QString password);
  void logout();

  bool nearest(const Coordinate center, const float maxDist,
    QList<Cache *>& buf);
  bool loadCache(const QString waypoint, Cache& buf);

  /** HTTP User Agent to use */
  static const QByteArray USER_AGENT;

  /**
   * @return @c true if the user has beed successfully logged in
   */
  inline bool loggedIn() {
    return loggedIn_;
  }
public slots:
  void loadPageFinished(QNetworkReply * reply);

protected:
  QNetworkReply * loadPage(const QUrl& url, const QByteArray * formData = 0);

private:
  GCSpider(const QString username, const QString password);
  GCSpider(const GCSpider&);
  virtual ~GCSpider();

  /** QNetworkAccessManager instance for HTTP communication */
  QNetworkAccessManager * pnam_;
  /** QNetworkReply of the currently loaded page, used by @a loadPage() */
  QNetworkReply * loadPageNetReply_;
  /** geocaching.com user name to use for log in */
  QString username_;
  /** geocaching.com password to use for log in */
  QString password_;
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
