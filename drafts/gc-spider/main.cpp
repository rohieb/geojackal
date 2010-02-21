#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib> 

using namespace std;

main(int argc, char * argv[]) {
  if (argc < 2) {
    cerr << "not enough parameters!" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream file(argv[1]);
  string content, line;

  if(!file) {
    cerr << "input stream failed!" << endl;
    exit(EXIT_FAILURE);
  }

  while(getline(file, line)) {
    content += line;
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
