#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void split(const string& str, char delimiter, vector<string>& vec) {
    vec.clear();
    stringstream ss(str);
    for (string buf; getline(ss, buf, delimiter);) {
        vec.push_back(buf);
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " [refGene.txt] [exon.csv]" << endl;
        return -1;
    }

    string filename = argv[1];
    string outputFilename = argv[2];

    ifstream ifs(filename);
    ofstream ofs(outputFilename);

    vector<string> items, exonStarts, exonEnds, exonFrames;

    ofs << "exonStart,exonEnd,exonFrame,name,txStart\r\n";

    for (string buf; getline(ifs, buf);) {
        split(buf, '\t', items);

        cout << items[1] << ", " << items[4] << endl;

        split(items[9], ',', exonStarts);
        split(items[10], ',', exonEnds);
        split(items[15], ',', exonFrames);

        if (exonStarts.size() != exonEnds.size() ||
            exonStarts.size() != exonFrames.size()) {
            throw runtime_error("Error");
        }

        for (size_t i = 0; i < exonStarts.size(); ++i) {
            cout << '\t' << exonStarts[i] << ", " << exonEnds[i] << ", "
                 << exonFrames[i] << endl;

            ofs << exonStarts[i] << ',' << exonEnds[i] << ',' << exonFrames[i]
                << ',' << items[1] << ',' << items[4] << "\r\n";
        }
    }

    return 0;
}
