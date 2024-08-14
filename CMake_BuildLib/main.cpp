#include <iostream>
#include <version.h>
using namespace std;

int main (int argc, char* argv[]){
    cout<<"Helloi"<<endl;
    cout<<"argc "<<argc<<endl;
    if (argc < 2) {
        cout<<"Helloiiiiiiii"<<endl;

    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    }

    return 0; 
}
