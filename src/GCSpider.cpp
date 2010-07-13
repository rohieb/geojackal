/*
 * @file GCSpider.cpp
 * @date 22.02.2010
 * @author Roland Hieber
 */

#include "GCSpider.h"
#include "GCSpiderCachePage.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRegExp>

#include <QStringList>

using namespace geojackal;

const QByteArray GCSpider::USER_AGENT = "Mozilla/5.0 (X11; U; Linux x86; "
  "en-US; rv:1.9.2.6) Gecko/20100628 Ubuntu/10.04 (lucid) Firefox/3.6.6";

/**
 * Constructor.
 * @param username geocaching.com user name to use for log in
 * @param password geocaching.com password to use for log in
 * @throws Failure if anything goes wrong
 */
GCSpider::GCSpider(const QString username, const QString password) :
  pnam_(0), loadPageNetReply_(0), username_(username), password_(password),
  loggedIn_(false) {
  pnam_ = new QNetworkAccessManager(this);
  if(!pnam_) {
    throw Failure("Could not create QNetworkAccessManager instance!");
  }
  login();
  if(!loggedIn_) {
    throw Failure("Not logged in!");
  }
}

GCSpider::~GCSpider() {
  if(pnam_)
    delete pnam_;
}

/**
 * Load a web page over HTTP. If the member variable @a loginCookies_ contains
 * cookies, these are also included in the HTTP request.
 * @param url URL to load
 * @param formData If this parameter is set, an HTTP POST request is sent, along
 *  with this form data. Otherwise, an HTTP GET request is sent.
 * @return The QNetworkReply received by the unterlying framework. You can use
 * this request to get the HTTP headers and the contents of the page. Do not
 * delete the request by yourself, instead, you can use
 * QNetworkReply::deleteLater().
 * @throws Failure if anything goes wrong
 */
QNetworkReply * GCSpider::loadPage(const QUrl& url, const QByteArray *
  formData) {
  if(!pnam_) {
    throw Failure("No QNetworkAccessManager instance!");
  }
  if(!url.toString().startsWith("http://")) {
    throw Failure("Only URLs beginning with http:// are supported");
  }
  qDebug() << "fetching" << url.toString();

  // prepare network request
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), this,
    SLOT(loadPageFinished(QNetworkReply*)));
  QNetworkRequest req = QNetworkRequest(url);
  req.setRawHeader("User-Agent", USER_AGENT);
  req.setRawHeader("Host", "www.geocaching.com");
  if(formData != 0) {
    // HTTP POST with form data
    pnam_->post(req, *formData);
  } else {
    // HTTP GET
    pnam_->get(req);
  }
  // execute an event loop to process the request (nearly-synchronous)
  QEventLoop eventLoop;
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
  eventLoop.exec();

  // loadPageFinished() stuffs the QNetworkReply to loadPageNetReply_

  if(loadPageNetReply_->error() != QNetworkReply::NoError) {
    throw Failure(loadPageNetReply_->errorString());
  }
  return loadPageNetReply_;
}

/**
 * @internal
 * Called from @a loadPage() when the QNetworkAccessManager finishes to load a
 * cache page. This function just saves the network reply for later use to the
 * member @a loadPageNetReply_.
 * @param reply Network reply
 */
void GCSpider::loadPageFinished(QNetworkReply * reply) {
  loadPageNetReply_ = reply;
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

  // login magic for post data
  const static QByteArray LOGIN_MAGIC =
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
  QByteArray postData = LOGIN_MAGIC;
  postData.append(QUrl::toPercentEncoding(username_));
  postData.append("&ctl00%24ContentBody%24myPassword=");
  postData.append(QUrl::toPercentEncoding(password_));

  // Request login page
  QNetworkReply * reply = loadPage(QUrl("http://www.geocaching.com/login/"
    "Default.aspx"), &postData);
  // extract and validate cookies
  QList<QNetworkCookie> loginCookies;
  QVariant var = reply->header(QNetworkRequest::SetCookieHeader);
  if(var.isValid()) {
    bool aspNetCookie = false, userIdCookie = false;
    loginCookies = qvariant_cast<QList<QNetworkCookie> > (var);
    QList<QNetworkCookie>::iterator it = loginCookies.begin();
    for(it = loginCookies.begin(); it != loginCookies.end(); ++it) {
      if((*it).name() == "ASP.NET_SessionId") {
        aspNetCookie = true;
      } else if((*it).name() == "userid") {
        userIdCookie = true;
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
    loggedIn_ = false;
    throw Failure("No valid cookies found!");
  }
  reply->deleteLater();
}

/**
 * Log out of geocaching.com. Deletes the cookies with the identity information.
 */
void GCSpider::logout() {
  // simpy delete the whole cookie jar
  pnam_->setCookieJar(new QNetworkCookieJar(pnam_));
}

/**
 * Get nearest caches around a coordinate up to a specified distance.
 * @param center Center coordinates
 * @param maxDist Maximum distance in km from cache to center point
 * @param buf Buffer that receives the caches to be retrieved. The calling
 *  context is responsible for freeing the members of this vector.
 * @return @c true if all caches could be retrieved correctly, @c false
 *  otherwise
 * @throws Failure if anything goes wrong
 */
bool GCSpider::nearest(const Coordinate center, const float maxDist, QVector<
  Cache *>& buf) {
  // @todo

  QNetworkReply * listReply = loadPage(QUrl(QString("http://www.geocaching.com/seek"
    "/nearest.aspx?lat=%1&lng=%2").arg(center.lat, 0, 'f').
    arg(center.lon, 0, 'f')));
  QString text = listReply->readAll();

//  // cap order: bearing, distance, cache guid
//  QRegExp rx("<tr .*class=\"Data BorderTop\">\\s*<td><img .* />(N|S|E|W|NW"
//    "|NE|SE|SW)<br />([0-9]+(?:\\.[0-9])?+)km\\s*</td>\\s*<td>\\s*</td>\\s*<td>"
//    "<a .*>\\s*<img .* /></a>.*</td>\\s*<td>.*</td><td>.*</td>\\s*<td><a .*"
//    "href=\"/seek/cache_details.aspx?guid=([-0-9a-f]{36})\".*>.*</a>.*</td>");
//  QRegExp rx("<tr bgcolor='[^\']+'\\s*class=\"Data BorderTop\">\\s*<td>.*<br />\\s*"
//    "([0-9]+(?:\\.[0-9]+)?)\\s*km\\s*</td>.*"
//    "<a href=\"/seek/cache_details.aspx?guid=(.{36})\">");

  QRegExp rx("<tr bgcolor='[^']+'\\s*class=\"Data BorderTop\">\\s*<td>.*"
    "([0-9]+(?:\\.[0-9]+)?)\\s*km\\s*</td>.*<a href=\"/seek/cache_details"
    "\\.aspx\\?guid=([-0-9a-zA-Z]+)\">.*</a>");
  rx.setMinimal(true);

  int curPos = 0;
  double cacheDist = 0; // distance to current cache
  while((curPos = rx.indexIn(text, curPos)) >= 0 && cacheDist <= maxDist) {
    curPos += rx.cap(0).size(); // move to end of current chunk
    if(rx.cap(1).isEmpty() || rx.cap(2).isEmpty()) {
      return false;
    }

    // check for distance
    bool ok = false;
    cacheDist = rx.cap(1).toDouble(&ok);
    if(!ok) {
      return false;
    }
    if(cacheDist > maxDist) {
      break;
    }

    // load cache page
    QNetworkReply * cacheReply = loadPage(QUrl("http://www.geocaching.com/"
        "seek/cache_details.aspx?guid=" + rx.cap(2)));
    GCSpiderCachePage gcscp(QString(cacheReply->readAll()));
    Cache * pCache = new Cache;
    gcscp.all(*pCache);
    buf.append(pCache);
  }

  listReply->deleteLater();
  return true;
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
  // try to login
  if(!loggedIn_) {
    throw Failure("Not logged in!");
  }
  if(!waypoint.startsWith("GC", Qt::CaseInsensitive)) {
    throw Failure("Waypoint must begin with \"GC\"");
  }

  QNetworkReply * reply = loadPage(QUrl("http://www.geocaching.com/seek/cache_"
    "details.aspx?wp=" + waypoint));

  QString content = reply->readAll();
  GCSpiderCachePage gcscp(content);
  return gcscp.all(buf);
}
