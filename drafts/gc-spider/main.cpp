#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(int argc, char * argv[]) {
  if (argc < 2) {
    cerr << "not enough parameters!" << endl;
    exit(EXIT_FAILURE);
  }

  // are we logged in?
  if(strstr(content.c_str(), "You are not logged in") == NULL) {
    // yes we are
    cout << "we are logged in, ok" << endl;
  } else {
    // no we're not
    cout << "we are not logged in :(" << endl;
  }
}
