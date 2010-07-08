/*
 * @file GCSpider.cpp
 * @date 22.02.2010
 * @author Roland Hieber
 */

#include "GCSpider.h"
#include "Failure.h"
#include "GCSpiderCachePage.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>

using namespace geojackal;

const QByteArray GCSpider::USER_AGENT =
  "Mozilla/5.0 (X11; U; Linux x86; en-US; "
    "rv:1.9.2.6) Gecko/20100628 Ubuntu/10.04 (lucid) Firefox/3.6.6";

/**
 * Constructor.
 * @param username geocaching.com user name to use for log in
 * @param password geocaching.com password to use for log in
 * @throws Failure if anything goes wrong
 */
GCSpider::GCSpider(const QString username, const QString password) :
  pnam_(0), username_(username), password_(password), loggedIn_(false) {
  pnam_ = new QNetworkAccessManager(this);
  if(!pnam_) {
    throw Failure("Could not create QNetworkAccessManager instance!");
  }
  login();
  if(!loggedIn_)
    throw Failure("Not logged in!");
}

GCSpider::~GCSpider() {
  if(pnam_)
    delete pnam_;
}

/**
 * Log in to geocaching.com.
 * Logs the user in and saves the HTTP authentication cookie for later use.
 * This function blocks until the login cookie has ben retrieved. The cookies
 * are saved to the loginCookie_ member variable, and the loggedIn_ member
 * variable is set according to whether the login was successful.
 * @throws Failure if anything goes wrong
 */
void GCSpider::login() {
  if(username_.trimmed().isEmpty()) {
    throw Failure("User name is empty!");
  }
  if(password_.trimmed().isEmpty()) {
    throw Failure("Password is empty!");
  }
  if(!pnam_) {
    throw Failure("No QNetworkAccessManager instance!");
  }

  // login magic for post data
  static QByteArray LOGIN_MAGIC =
    "__VIEWSTATE=/wEPDwULLTE4MjM1Mjc4MzIPFgIeDkxv"
    "Z2luLlJlZGlyZWN0ZRYCZg9kFgQCAQ9kFhQCAg8WAh4EVGV4dGRkAgQPFgIfAQUWWW91IGFyZS"
    "Bub3QgbG9nZ2VkIGluLmQCBQ8PFgQeC05hdmlnYXRlVXJsBXFodHRwOi8vd3d3Lmdlb2NhY2hp"
    "bmcuY29tL2xvZ2luL2RlZmF1bHQuYXNweD9SRVNFVD1ZJnJlZGlyPWh0dHAlM2ElMmYlMmZ3d3"
    "cuZ2VvY2FjaGluZy5jb20lMmZsb2dpbiUyZkRlZmF1bHQuYXNweB8BBQZMb2cgaW5kZAIHD2QW"
    "AgIBDw8WAh8BZGRkAg8PFgIeB1Zpc2libGVnZAITDw8WAh8CBQwvcmV2aWV3cy9ncHNkZAIVD2"
    "QWAgIBDw8WAh8BBZcCPGlmcmFtZSBzcmM9Imh0dHA6Ly9iYW5tYW41Lmdyb3VuZHNwZWFrLmNv"
    "bS9iYW5tYW41L2FkLmFzcHg/Wm9uZUlEPTkmVGFzaz1HZXQmU2l0ZUlEPTEmWD0nMDJmNjdhNT"
    "IyZmQ0NDkzNDlhNmQ0NWMwOWNlOTViZjAnIiB3aWR0aD0iMTIwIiBoZWlnaHQ9IjI0MCIgTWFy"
    "Z2lud2lkdGg9IjAiIE1hcmdpbmhlaWdodD0iMCIgSHNwYWNlPSIwIiBWc3BhY2U9IjAiIEZyYW"
    "1lYm9yZGVyPSIwIiBTY3JvbGxpbmc9Im5vIiBzdHlsZT0id2lkdGg6MTIwcHg7SGVpZ2h0OjI0"
    "MHB4OyI%2BPC9pZnJhbWU%2BZGQCFg9kFgJmDxYCHgtfIUl0ZW1Db3VudAIGFgwCAQ9kFgICAQ"
    "8PFggeD0NvbW1hbmRBcmd1bWVudAUFZW4tVVMeC0NvbW1hbmROYW1lBQ1TZXRUZW1wTG9jYWxl"
    "HwEFB0VuZ2xpc2geEENhdXNlc1ZhbGlkYXRpb25oZGQCAg9kFgICAQ8PFggfBQUFZGUtREUfBg"
    "UNU2V0VGVtcExvY2FsZR8BBQdEZXV0c2NoHwdoZGQCAw9kFgICAQ8PFggfBQUFZnItRlIfBgUN"
    "U2V0VGVtcExvY2FsZR8BBQlGcmFuw6dhaXMfB2hkZAIED2QWAgIBDw8WCB8FBQVwdC1QVB8GBQ"
    "1TZXRUZW1wTG9jYWxlHwEFClBvcnR1Z3XDqnMfB2hkZAIFD2QWAgIBDw8WCB8FBQVjcy1DWh8G"
    "BQ1TZXRUZW1wTG9jYWxlHwEFCcSMZcWhdGluYR8HaGRkAgYPZBYCAgEPDxYIHwUFBXN2LVNFHw"
    "YFDVNldFRlbXBMb2NhbGUfAQUHU3ZlbnNrYR8HaGRkAhgPDxYCHwIFDy9yZXZpZXdzL2hvdGVs"
    "c2RkAhkPDxYCHwIFDC9yZXZpZXdzL2dwc2RkAgMPFgIfAQUvU2VydmVyOiBXRUIwOTsgQnVpbG"
    "Q6IFRGUy5Qcm9kdWN0aW9uXzIwMTAwNjMwLjFkGAEFHl9fQ29udHJvbHNSZXF1aXJlUG9zdEJh"
    "Y2tLZXlfXxYBBRhjdGwwMCRDb250ZW50Qm9keSRjb29raWVxOtnyvDYWzN6BAcyo9eo9tADd2A"
    "%3D%3D&ctl00%24ContentBody%24cookie=on&ctl00%24ContentBody%24Button1="
    "Login&ctl00%24ContentBody%24myUsername=";
  QByteArray postData = LOGIN_MAGIC.append(username_);
  postData.append("&ctl00%24ContentBody%24myPassword=");
  postData.append(password_);

  // Request login page
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), this,
    SLOT(loginFinished(QNetworkReply*)));
  QNetworkRequest req(QUrl("http://www.geocaching.com/login/Default.aspx"));
  req.setRawHeader("User-Agent", USER_AGENT);
  req.setRawHeader("Host", "www.geocaching.com");
  pnam_->post(req, postData);

  // execute an event loop to process the request (nearly-synchronous)
  QEventLoop eventLoop;
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
  eventLoop.exec();
  disconnect(pnam_, SIGNAL(finished(QNetworkReply *)), this,
    SLOT(loginFinished(QNetworkReply*))); // so we don't get called again
}

/**
 * Called when the QNetworkAccessManager finishes the login
 * @param reply Network reply
 * @throws Failure if anything goes wrong
 */
void GCSpider::loginFinished(QNetworkReply * reply) {
  if(reply->error() != QNetworkReply::NoError) {
    throw Failure(reply->errorString());
  }
  // extract cookies
  QVariant var = reply->header(QNetworkRequest::SetCookieHeader);
  if(var.isValid()) {
    bool aspNetCookie = false, userIdCookie = false;
    loginCookies_ = qvariant_cast<QList<QNetworkCookie> > (var);
    QList<QNetworkCookie>::iterator it = loginCookies_.begin();
    for(it = loginCookies_.begin(); it != loginCookies_.end(); ++it) {
      if((*it).name() == "ASP.NET_SessionId") {
        aspNetCookie = true;
      } else if((*it).name() == "userid") {
        userIdCookie = true;
      } else {
        // we don't need this
        loginCookies_.removeOne(*it);
      }
    }
    loggedIn_ = aspNetCookie && userIdCookie;
    if(loggedIn_) {
      qDebug() << "Login successful with username" << username_;
    } else {
      qDebug() << "Login failed with username" << username_;
    }
  } else {
    // who took the cookies from the cookie jar?
    loginCookies_ = QList<QNetworkCookie>();
    loggedIn_ = false;
    throw Failure("No valid cookies found!");
  }
  reply->deleteLater();
}

/**
 * Get nearest caches around a coordinate up to a specified distance.
 * @param center Center coordinates
 * @param maxDist Maximum distance of cache to center point
 * @param buf Buffer that receives the caches to be retrieved. The calling
 *  context is responsible for freeing the members of this vector.
 * @return @c true if all caches could be retrieved correctly, @c false
 *  otherwise
 */
bool GCSpider::nearest(const Coordinate center, const float maxDist, QVector<
  Cache *>& buf) {
  // @todo
  return false;
}

/**
 * Load a single cache.
 * @param waypoint Waypoint of the cache (e.g. <em>GC132V6</em>)
 * @param buf Buffer that receives the retrieved cache data. The calling
 *  context is responsible for freeing the members of this vector.
 * @return @c true if the cache could be retrieved correctly, @c false
 *  otherwise
 */
bool GCSpider::loadCache(QString waypoint, Cache& buf) {
  // @todo
  if(!loggedIn_) {
    throw Failure("Not logged in!");
  }
  if(!waypoint.startsWith("GC", Qt::CaseInsensitive)) {
    throw Failure("Waypoint must begin with \"GC\"");
  }

  QString url = "http://www.geocaching.com/seek/cache_details.aspx?wp=";
  url.append(waypoint);

  // prepare network request
  qDebug() << "fetching" << url;
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), this,
    SLOT(loadCacheFinished(QNetworkReply*)));
  QNetworkRequest req = QNetworkRequest(QUrl(url));
  req.setRawHeader("User-Agent", USER_AGENT);
  req.setRawHeader("Host", "www.geocaching.com");
  req.setHeader(QNetworkRequest::CookieHeader,
    qVariantFromValue(loginCookies_));
  pnam_->get(req);

  // execute an event loop to process the request (nearly-synchronous)
  QEventLoop eventLoop;
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
  eventLoop.exec();

  // loadCacheFinished() fills the cacheText_ variable

  GCSpiderCachePage gcscp(cacheText_);
  return gcscp.all(buf);
}

/**
 * Called when the QNetworkAccessManager finishes to load a cache page. The
 * content of the page is written to the cacheText_ member variable.
 * @param reply Network reply
 * @throws Failure if anything goes wrong
 */
void GCSpider::loadCacheFinished(QNetworkReply * reply) {
  //throw Failure("Not yet implemented!");
  if(reply->error() != QNetworkReply::NoError) {
    throw Failure(reply->errorString());
  }
  cacheText_ = reply->readAll();
//  qDebug() << cacheText_;
  reply->deleteLater();
}
