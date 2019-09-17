#include "../some/someClass.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
        cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }
    display(argv[1]);
    return 0;
}