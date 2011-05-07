/*
 * @file GCSpider.cpp
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

#include "import/GCSpider.h"
#include "import/GCSpiderCachePage.h"
#include <QtGui>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRegExp>

#include <QStringList>

using namespace geojackal;

const QByteArray GCSpider::USER_AGENT = "Mozilla/5.0 (X11; U; Linux x86; "
  "en-US; rv:1.9.2.6) Gecko/20100628 Ubuntu/10.04 (lucid) Firefox/3.6.6";

GCSpider * GCSpider::instance_ = 0;

/**
 * Constructor.
 */
GCSpider::GCSpider() :
  pnam_(0), loadPageNetReply_(0), loggedIn_(false) {
  pnam_ = new QNetworkAccessManager(this);
}

GCSpider::GCSpider(const GCSpider&) : QObject() {}

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
    throw Failure(tr("No QNetworkAccessManager instance!"));
  }
  if(!url.toString().startsWith("http://")) {
    throw Failure(tr("Only URLs beginning with http:// are supported"));
  }

  // prepare network request
  connect(pnam_, SIGNAL(finished(QNetworkReply *)), this,
    SLOT(loadPageFinished(QNetworkReply*)));
  QNetworkRequest req = QNetworkRequest(url);
  req.setRawHeader("User-Agent", USER_AGENT);
  req.setRawHeader("Host", "www.geocaching.com");
  if(formData != 0) {
    qDebug() << "fetching" << url.toString(); //<< "with POST data" << *formData;
    pnam_->post(req, *formData);
  } else {
    // HTTP GET
    qDebug() << "fetching" << url.toString();
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
 * geocache page. This function just saves the network reply for later use to
 * the member @a loadPageNetReply_.
 * @param reply Network reply
 */
void GCSpider::loadPageFinished(QNetworkReply * reply) {
  loadPageNetReply_ = reply;
}

/**
 * Extract hidden input form fields of an ASP.net formular, like the ones
 * geocaching.com uses.
 * @param text The full HTML text of the web site
 * @return A QMap which contains the field names as keys and contents as values
 */
QMap<QString,QString> GCSpider::getAspFormFields(const QString& htmlText) {
  QMap<QString,QString> map;
  QRegExp rx("<form .*name=\"aspnetForm\"[^>]*>(.*)</form>");
  rx.setMinimal(true);
  if(rx.indexIn(htmlText) > 0) {
    QString text = rx.cap(1); // childs of form element
    int curPos = 0;
    rx.setPattern("<input type=\"hidden\" name=\"([^\"]*)\" [^>]*value="
      "\"([^\"]*)\"");
    rx.setMinimal(true);
    while(rx.indexIn(text, curPos) > 0 && !rx.cap(1).isEmpty() &&
      !rx.cap(2).isNull()) {
      // stuff regexed field names (1) and values (2) into map
      curPos += rx.cap(0).length();
      map.insert(rx.cap(1), rx.cap(2));
    }
  }
  return map;
}

/**
 * Log in to geocaching.com.
 * Logs the user in and saves the HTTP authentication cookie for later use.
 * This function blocks until the login cookie has ben retrieved. The cookies
 * are saved to the loginCookie_ member variable, and the loggedIn_ member
 * variable is set according to whether the login was successful.
 *
 * User name and password can be empty if this function was previously called;
 * in this case, the previous user name and password are used.
 *
 * @param username geocaching.com user name to use for log in
 * @param password geocaching.com password to use for log in
 * @return An GCSpider instance that can be used for further operations.
 * @throws Failure if anything goes wrong
 */
GCSpider * GCSpider::login(const QString username = "",
  const QString password = "") {

  static Guard guard; // delete the instance at end of run time

  // instanciate ourself
  if(!instance_) {
    instance_ = new GCSpider;
  }

  // use previous login data, if possible and nothing else given
  if((username == "" || password == "") && instance_->loggedIn()) {
    return instance_;
  }

  // otherwise, do login with provided account data
  // but first log out to prevent interference with old session cookies
  logout();

  if(username.trimmed().isEmpty()) {
    throw Failure(tr("User name is empty!"));
  }
  if(password.trimmed().isEmpty()) {
    throw Failure(tr("Password is empty!"));
  }

  // login magic for post data
  // TODO read this from login page form?
  const static QByteArray LOGIN_MAGIC =
    "__VIEWSTATE=%2FwEPDwULLTE4MjM1Mjc4MzIPFgIeDkxvZ2luLlJlZGlyZWN0ZRYCZg9kFgQC"
    "AQ9kFhACBQ8WAh4EVGV4dGRkAgkPFgIfAQUWWW91IGFyZSBub3QgbG9nZ2VkIGluLmQCCw8PFg"
    "QeC05hdmlnYXRlVXJsBXFodHRwOi8vd3d3Lmdlb2NhY2hpbmcuY29tL2xvZ2luL2RlZmF1bHQu"
    "YXNweD9SRVNFVD1ZJnJlZGlyPWh0dHAlM2ElMmYlMmZ3d3cuZ2VvY2FjaGluZy5jb20lMmZsb2"
    "dpbiUyZkRlZmF1bHQuYXNweB8BBQZMb2cgaW5kZAIPD2QWAgIBDw8WAh8BZGRkAh8PFgIeB1Zp"
    "c2libGVnZAInDw8WAh8CBQwvcmV2aWV3cy9ncHNkZAIrD2QWAgIBDw8WAh8BBZcCPGlmcmFtZS"
    "BzcmM9Imh0dHA6Ly9iYW5tYW41Lmdyb3VuZHNwZWFrLmNvbS9iYW5tYW41L2FkLmFzcHg%2FWm"
    "9uZUlEPTkmVGFzaz1HZXQmU2l0ZUlEPTEmWD0nYmRiY2U0NThmZWJiNDQ0OGI5ZTRlMGZiYmRi"
    "MDk2ZTInIiB3aWR0aD0iMTIwIiBoZWlnaHQ9IjI0MCIgTWFyZ2lud2lkdGg9IjAiIE1hcmdpbm"
    "hlaWdodD0iMCIgSHNwYWNlPSIwIiBWc3BhY2U9IjAiIEZyYW1lYm9yZGVyPSIwIiBTY3JvbGxp"
    "bmc9Im5vIiBzdHlsZT0id2lkdGg6MTIwcHg7SGVpZ2h0OjI0MHB4OyI%2BPC9pZnJhbWU%2BZG"
    "QCLQ9kFgZmD2QWAmYPFgIeC18hSXRlbUNvdW50AgYWDAIBD2QWAgIBDw8WCB4PQ29tbWFuZEFy"
    "Z3VtZW50BQVlbi1VUx4LQ29tbWFuZE5hbWUFDVNldFRlbXBMb2NhbGUfAQUHRW5nbGlzaB4QQ2"
    "F1c2VzVmFsaWRhdGlvbmhkZAICD2QWAgIBDw8WCB8FBQVkZS1ERR8GBQ1TZXRUZW1wTG9jYWxl"
    "HwEFB0RldXRzY2gfB2hkZAIDD2QWAgIBDw8WCB8FBQVmci1GUh8GBQ1TZXRUZW1wTG9jYWxlHw"
    "EFCUZyYW7Dp2Fpcx8HaGRkAgQPZBYCAgEPDxYIHwUFBXB0LVBUHwYFDVNldFRlbXBMb2NhbGUf"
    "AQUKUG9ydHVndcOqcx8HaGRkAgUPZBYCAgEPDxYIHwUFBWNzLUNaHwYFDVNldFRlbXBMb2NhbG"
    "UfAQUJxIxlxaF0aW5hHwdoZGQCBg9kFgICAQ8PFggfBQUFc3YtU0UfBgUNU2V0VGVtcExvY2Fs"
    "ZR8BBQdTdmVuc2thHwdoZGQCAg8PFgIfAgUPL3Jldmlld3MvaG90ZWxzZGQCAw8PFgIfAgUML3"
    "Jldmlld3MvZ3BzZGQCAw8WAh8BBS1TZXJ2ZXI6IFdFQjEwOyBCdWlsZDogVEZTLlNwcmludDE1"
    "XzIwMTAwNzI5LjZkGAEFHl9fQ29udHJvbHNSZXF1aXJlUG9zdEJhY2tLZXlfXxYBBRhjdGwwMC"
    "RDb250ZW50Qm9keSRjb29raWV3vxJZvDX%2F7yG%2F4bYPocXurGizPQ%3D%3D"
    "&ctl00%24ContentBody%24cookie=on&ctl00%24ContentBody%24Button1=Login&"
    "ctl00%24ContentBody%24myUsername=";
  QByteArray postData = LOGIN_MAGIC;
  postData.append(QUrl::toPercentEncoding(username));
  postData.append("&ctl00%24ContentBody%24myPassword=");
  postData.append(QUrl::toPercentEncoding(password));

  // Request login page
  QNetworkReply * reply = instance_->loadPage(QUrl("http://www.geocaching.com/"
    "login/Default.aspx"), &postData);
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
    instance_->loggedIn_ = aspNetCookie && userIdCookie;
  } else {
    // who took the cookies from the cookie jar?
    instance_->loggedIn_ = false;
    throw Failure(tr("Login failed: No valid login cookies found!"));
  }

  if(!instance_->loggedIn_) {
    // delete all cookies, so the server does not remember us in any way
    instance_->pnam_->setCookieJar(new QNetworkCookieJar);
    throw Failure(tr("Login failed with username %1").arg(username));
  }
  reply->deleteLater();
  return instance_;
}

/**
 * Log out of geocaching.com. Deletes the cookies with the identity information.
 */
void GCSpider::logout() {
  if(instance_) {
    // delete cookies
    instance_->pnam_->setCookieJar(new QNetworkCookieJar(instance_->pnam_));
    instance_->loggedIn_ = false;
  }
}

/** Temporary type to save geocache GUIDs and waypoints */
struct WaypointsGuids {
  QString wp;
  QString guid;
};

/**
 * Get nearest geocaches around a coordinate up to a specified distance.
 * @param center Center coordinates
 * @param maxDist Maximum distance in km from loaded geocache to center point
 * @param buf Buffer that receives the geocaches to be retrieved. The calling
 *  context is responsible for freeing the members of this vector.
 * @return @c true if all geocaches could be retrieved correctly, @c false
 *  otherwise
 * @throws Failure if anything goes wrong
 */
bool GCSpider::nearest(const Coordinate center, const float maxDist, QList<
  Geocache *>& buf) {

//  // cap order: bearing, distance, geocache guid
//  QRegExp gcRx("<tr .*class=\"Data BorderTop\">\\s*<td><img .* />(N|S|E|W|NW"
//    "|NE|SE|SW)<br />([0-9]+(?:\\.[0-9])?+)km\\s*</td>\\s*<td>\\s*</td>\\s*<td>"
//    "<a .*>\\s*<img .* /></a>.*</td>\\s*<td>.*</td><td>.*</td>\\s*<td><a .*"
//    "href=\"/seek/cache_details.aspx?guid=([-0-9a-f]{36})\".*>.*</a>.*</td>");
//  QRegExp gcRx("<tr bgcolor='[^\']+'\\s*class=\"Data BorderTop\">\\s*<td>.*<br />\\s*"
//    "([0-9]+(?:\\.[0-9]+)?)\\s*km\\s*</td>.*"
//    "<a href=\"/seek/cache_details.aspx?guid=(.{36})\">");

  // Link to a geocache page
  QRegExp gcRx("<tr bgcolor='[^']+'\\s*class=\"Data BorderTop\">\\s*<td>.*"
    "([0-9]+(?:\\.[0-9]+)?)\\s*km\\s*</td>.*<a href=\"/seek/cache_details"
    "\\.aspx\\?guid=([-0-9a-zA-Z]+)\">.*\\((GC[A-Z0-9]{4,})\\).*</a>");
  gcRx.setMinimal(true);

  // "Next" link
  QRegExp nextRx("<a href=\"javascript:__doPostBack(.*)\"><b>Next &gt;</b>");
  nextRx.setMinimal(true);

  QProgressDialog progDialog(tr("Import geocaches"), tr("Abort"), 0, 1);
  progDialog.setWindowModality(Qt::WindowModal);
  progDialog.setMinimumDuration(0);
  progDialog.setValue(0);
  progDialog.setLabelText(tr("%1 geocaches found…").arg(0));

  QString viewState;
  QList<WaypointsGuids> geocacheList;
  double geocacheDist = 0; // distance to current geocache
  QByteArray postData;
  QString text;

  do { // while "Next" link exists

    QNetworkReply * listReply = loadPage(QUrl(QString("http://www.geocaching"
      ".com/seek/nearest.aspx?lat=%1&lng=%2").arg(center.lat, 0, 'f').
      arg(center.lon, 0, 'f')), &postData);
    int curPos = 0;  // pos of last parsed gc regex, so we don't get them twice
    text = listReply->readAll();

    // parse geocache links and stuff them into a list
    while((curPos = gcRx.indexIn(text, curPos)) >= 0 &&
      geocacheDist <= maxDist && !progDialog.wasCanceled()) {
      curPos += gcRx.cap(0).size(); // move to end of current chunk
      if(gcRx.cap(1).isEmpty() || gcRx.cap(2).isEmpty() ||
        gcRx.cap(3).isEmpty()) {
        return false; // something went wrong
      }

      // check for distance
      bool ok = false;
      geocacheDist = gcRx.cap(1).toDouble(&ok);
      if(!ok) {
        return false;
      }

      // append to list for next step
      WaypointsGuids c;
      c.guid = gcRx.cap(2);
      c.wp = gcRx.cap(3);
      qDebug() << "append {" << c.guid << "," << c.wp << "}";
      geocacheList.append(c);
    }

    progDialog.setLabelText(tr("%1 geocaches found…").arg(geocacheList.size()));

    // if no more geocache links: get next page
    if(geocacheDist > maxDist) {
      break;
    }

    listReply->deleteLater();

    // POST data for "Next" action
    QMap<QString,QString> fields = getAspFormFields(text);
    fields["__EVENTTARGET"] = "ctl00$ContentBody$pgrBottom$ctl08";
    postData = "";
    foreach(QString field, fields.keys()) {
      postData.append(QUrl::toPercentEncoding(field));
      postData.append("=");
      postData.append(QUrl::toPercentEncoding(fields.value(field)));
      postData.append("&");
    }

  } while(nextRx.indexIn(text) > 0);

  // second part: load geocache descriptions
  int progress = 0;
  progDialog.setMaximum(geocacheList.size());
  foreach(WaypointsGuids const& c, geocacheList) {
    if(progDialog.wasCanceled()) {
      break;
    }
    progress++;
    progDialog.setLabelText(QString(tr("Loading %1/%2: %3")).arg(progress).
      arg(geocacheList.size()).arg(c.wp));
    progDialog.setValue(progress);

    // load geocache page and extract data
    QNetworkReply * geocacheReply = loadPage(QUrl("http://www.geocaching.com/"
        "seek/cache_details.aspx?guid=" + c.guid));
    GCSpiderCachePage gcscp(QString(geocacheReply->readAll()));
    Geocache * pgc = new Geocache;
    gcscp.all(*pgc);
    buf.append(pgc);
  }

  progDialog.setValue(progDialog.maximum());

  qDebug() << "finished import from geocaching.com";

  return true;
}

/**
 * Load a single geocache.
 * @param waypoint Waypoint of the geocache (e.g. <em>GC132V6</em>)
 * @param buf Buffer that receives the retrieved geocache data. The calling
 *  context is responsible for freeing the members of this vector.
 * @return @c true if the geocache could be retrieved correctly, @c false
 *  otherwise
 */
bool GCSpider::single(QString waypoint, Geocache& buf) {
  // @todo
  // try to login
  if(!loggedIn_) {
    throw Failure(tr("Not logged in!"));
  }
  if(!waypoint.startsWith("GC", Qt::CaseInsensitive)) {
    throw Failure(tr("Waypoint must begin with \"GC\""));
  }

  QNetworkReply * reply = loadPage(QUrl("http://www.geocaching.com/seek/cache_"
    "details.aspx?wp=" + waypoint));

  QString content = reply->readAll();
  GCSpiderCachePage gcscp(content);
  return gcscp.all(buf);
}
