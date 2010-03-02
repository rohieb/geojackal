/*
 * @file GCSpider.h
 * @date 22.02.2010
 * @author Roland Hieber
 */

#ifndef GCSPIDER_H_
#define GCSPIDER_H_

#include <string>

using namespace std;

namespace geojackal {

class GCSpider {
public:
  GCSpider();
  virtual ~GCSpider();

  void open(string filename);
  bool isLoggedIn();

private:
  bool hasContent;
  string file_content_;
};

}

#endif /* GCSPIDER_H_ */
