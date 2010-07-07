/*
 * @file GCSpider.h
 * @date 22.02.2010
 * @author Roland Hieber
 */

#ifndef GCSPIDER_H_
#define GCSPIDER_H_

#include "Cache.h"
#include "Coordinate.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkCookie>

using namespace std;

namespace geojackal {

/**
 * Class that encapsulate functionalty to retrieve caches from the
 * geocaching.com database.
 * @todo how to use?
 */
class GCSpider : public QObject {
Q_OBJECT

public:
  /** Phases of the spider */
  enum SpiderPhase {
    /** Login phase */
    PHASE_LOGIN,
    /** Searching for caches */
    PHASE_SEARCH,
    /** Loading a cache */
    PHASE_LOAD
  };

  GCSpider(const QString username, const QString password);
  virtual ~GCSpider();

  bool nearest(const Coordinate center, const float maxDist,
    QVector<Cache *>& buf);
  bool loadCache(QString waypoint);

  /** HTTP User Agent to use */
  static const QByteArray USER_AGENT;

public slots:
  void replyFinished(SpiderPhase phase, QNetworkReply * reply);

protected:
  void login();

private:
  /** QNetworkAccessManager instance for HTTP communication */
  QNetworkAccessManager * pnam_;
  /** geocaching.com user name to use for log in */
  QString username_;
  /** geocaching.com password to use for log in */
  QString password_;
  /** Is the user already logged in? */
  bool loggedIn;
  /** Cookies sent by the server on login */
  QList<QNetworkCookie> loginCookies;
};

}

#endif /* GCSPIDER_H_ */
