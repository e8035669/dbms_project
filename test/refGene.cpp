#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;
namespace fs = std::experimental::filesystem;

map<string, string> loadChromFa(const string& chromFaPath) {
    map<string, string> chromFa;
    for (const auto& fname : fs::directory_iterator(chromFaPath)) {
        cout << "Load " << fname << endl;
        ifstream ifs(fname.path());
        stringstream ss;
        string buf;
        getline(ifs, buf);  // Ignore first line
        while (getline(ifs, buf)) {
            ss << buf;
        }
        cout << fs::path(fname).stem().string() << " Done." << endl;
        chromFa[fs::path(fname).stem().string()] = ss.str();
    }

    return chromFa;
}

void split(const string& str, char delimiter, vector<string>& vec) {
    vec.clear();
    stringstream ss(str);
    for (string buf; getline(ss, buf, delimiter);) {
        vec.push_back(buf);
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cout << "Usage: " << argv[0]
             << " [refGene.txt] [chromFa/] [refGene.csv]" << endl;
        return -1;
    }

    string refGenePath(argv[1]);
    string chromFaPath(argv[2]);
    string outFilePath(argv[3]);
    auto chrom = loadChromFa(chromFaPath);

    ifstream ifs(refGenePath);
    ofstream ofs(outFilePath);

    ofs << "bin,name,txStart,name2,strand,cdsStartStat,cdsEndStat,score,ch_"
           "name,geneSeq\r\n";

    for (string buf; getline(ifs, buf);) {
        vector<string> items;
        split(buf, '\t', items);

        int geneStart = stoi(items[4]);
        int geneEnd = stoi(items[5]);
        string geneSeq = chrom[items[2]].substr(geneStart, geneEnd - geneStart);

        cout << items[0] << ", " << items[1] << ", " << items[4] << ", "
             << items[12] << ", " << items[3] << ", " << items[13] << ", "
             << items[14] << ", " << items[11] << ", " << items[2] << ", "
             << geneSeq << endl;

        ofs << items[0] << "," << items[1] << "," << items[4] << ","
            << items[12] << "," << items[3] << "," << items[13] << ","
            << items[14] << "," << items[11] << "," << items[2] << ","
            << geneSeq << "\r\n";
    }

    return 0;
}

