#include <iostream>
#include <fstream>
#include <string.h>
#include "gis_convertor.h"

using namespace std;

/* Example of usage */
int main(int argc, char* argv[]) {

    /* Get file names for work. */
    string inFile, outFile;    
    switch(argc) {
        case 1: 
            cout << "Please, enter input filename." << endl;
            system("pause");
            exit(1);
        case 2:
            inFile  = argv[1];
            outFile = inFile + ".asc";
            break;
        case 3:
            inFile  = argv[1];
            outFile = argv[2];
            break;    
    }

    /* getYSize
     * Create file stream for reading info.
     * You can also use other iostream implementation
     * like string, console or socket streams.
     */
    ifstream * in  = new ifstream(inFile.c_str());
    ofstream * out = new ofstream(outFile.c_str());

    /* Creating source and target format handlers. */
    GisSource * gs = new GrdGisSource(in);
    GisTarget * gt = new ArcGisTarget(out);

    /* Creating default gis convertor and starting convert process. */
    GisConvertor gc(gs, gt);
    gc.convert();

    /*
     * Don't forget close streams to avoid memeory's leak.
     */
    in->close();
    out->close();

    system("pause");
    return 0;
}