/*
 * @file GCSpider.cpp
 * @date 22.02.2010
 * @author Roland Hieber
 */

#include "GCSpider.h"
#include <fstream>
#include <stdexcept>

#include <QFile>
//#include <QHttp>

using namespace std;
using namespace geojackal;

GCSpider::GCSpider() :
  hasContent(false) {
}

GCSpider::~GCSpider() {
}

/**
 * Open a (HTML) file and read its contents into the memory
 * @param filename
 */
void GCSpider::open(QString filename) {
//  QFile file(filename.c_str());
//  string line;
//
//  if(!file) {
//    throw runtime_error("input stream failed!");
//  }
//
//  while(getline(file, line)) {
//    file_content_ += line;
//  }
//
//  hasContent = true;
}
