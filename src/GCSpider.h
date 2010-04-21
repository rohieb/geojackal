/*
 * @file GCSpider.h
 * @date 22.02.2010
 * @author Roland Hieber
 */

#ifndef GCSPIDER_H_
#define GCSPIDER_H_

#include <QString>

using namespace std;

namespace geojackal {

class GCSpider {
public:
  GCSpider();
  virtual ~GCSpider();

  bool loadCache(QString waypoint);
  bool isLoggedIn();

  // for testing
  void open(QString filename);

private:
  bool hasContent;
  QString file_content_;
};

}

#endif /* GCSPIDER_H_ */
