/*
 * @file GCSpider.cpp
 * @date 22.02.2010
 * @author Roland Hieber
 */

#include "GCSpider.h"
#include "Failure.h"
#include <QNetworkRequest>
#include <QNetworkReply>

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
  pnam_(0), username_(username), password_(password), loggedIn(false) {
  pnam_ = new QNetworkAccessManager(this);
  if(!pnam_) {
    throw Failure("Could not create QNetworkAccessManager instance!");
  }
  login();
  if(!loggedIn)
    throw Failure("Not logged in!");
}

GCSpider::~GCSpider() {
  if(pnam_)
    delete pnam_;
}

/**
 * Log in to geocaching.com.
 * Logs the user in and saves the HTTP authentication cookie for later use.
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
  QNetworkRequest req(QUrl("/login/Default.aspx"));
  req.setRawHeader("User-Agent", USER_AGENT);
  req.setRawHeader("Host", "www.geocaching.com");
  connect(pnam_, SIGNAL(finished(QNetworkReply*)), this,
    SLOT(replyFinished(PHASE_LOGIN, QNetworkReply*)));
  pnam_->post(req, postData);
}

/**
 * Get nearest caches around a coordinate up to a specified distance.
 * @param center Center coordinates
 * @param maxDist Maximum distance of cache to center point
 * @param buf Buffer that receives the caches to be retrieved. The calling
 *  context is responsible for freeing the members of this vector.
 */
bool GCSpider::nearest(const Coordinate center, const float maxDist, QVector<
  Cache *>& buf) {
  return false;
}

/**
 * Called when a QNetworkAccessManager finishes the connection
 * @param phase current phase of the spider
 * @param reply Network reply
 * @throws Failure if anything goes wrong
 */
void GCSpider::replyFinished(SpiderPhase phase, QNetworkReply * reply) {
  if(reply->error()) {
    throw Failure(reply->errorString());
  }
  switch(phase) {
    case PHASE_LOGIN: {
      // extract cookies
      QVariant var = reply->header(QNetworkRequest::SetCookieHeader);
      if(var.isValid()) {
        loginCookies = qvariant_cast<QList<QNetworkCookie> > (var);
        QList<QNetworkCookie>::iterator it = loginCookies.begin();
        for(it = loginCookies.begin(); it != loginCookies.end(); ++it) {
          // FIXME: look for ASP.NET_SessionId and userid
        }
        //loggedIn = true;
      } else {
        loginCookies = QList<QNetworkCookie>(); // no cookies
        loggedIn = false;
        throw Failure("No valid cookies found!");
      }
      reply->deleteLater();
      break;
    }
    case PHASE_SEARCH:
    case PHASE_LOAD:
    default:
      break;
  }
}

