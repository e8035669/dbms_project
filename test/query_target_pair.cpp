#include <algorithm>
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
        cout << "Usage: " << argv[0]
             << " [all_mrna.txt] [query_target_pair.csv]" << endl;
        return -1;
    }

    string filename = argv[1];
    string outputFilename = argv[2];
    ifstream ifs(filename);
    ofstream ofs(outputFilename);

    vector<string> items, qStartOfBlock, blockSize, tStartOfBlock;

    cout << "qName,tStart,blockSize,qStartOfBlock,tStartOfBlock" << endl;
    ofs << "qName,tStart,blockSize,qStartOfBlock,tStartOfBlock\r\n";

    for (string line; getline(ifs, line);) {
        split(line, '\t', items);
        cout << items[10] << ", " << items[16] << endl;

        split(items[19], ',', blockSize);
        split(items[20], ',', qStartOfBlock);
        split(items[21], ',', tStartOfBlock);

        if (blockSize.size() != qStartOfBlock.size() ||
            blockSize.size() != tStartOfBlock.size()) {
            throw runtime_error("Error");
        }

        for (size_t i = 0; i < blockSize.size(); ++i) {
            cout << '\t' << blockSize[i] << ", " << qStartOfBlock[i] << ", "
                 << tStartOfBlock[i] << endl;
            ofs << items[10] << ',' << items[16] << ',' << blockSize[i] << ','
                << qStartOfBlock[i] << ',' << tStartOfBlock[i] << "\r\n";
        }
    }

    return 0;
}
