#include <iostream>
#include <fstream>
#include <vector>

class tridata {
  public:
	int	v[3];  // indices to vertex list
	// texture and vertex normal info removed for this demo
};

void splitString(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    int in_string_bool = 0;
    size_t start_pos = 0;
    size_t it_pos = 0;
    strs.clear();
    // Decompose statement
    while( it_pos < txt.length()) {
        if(in_string_bool && txt[it_pos] == ch && it_pos > start_pos){
            //end of word
            strs.push_back( txt.substr( start_pos, it_pos - start_pos) );
            in_string_bool = 0;
        }
        if(!in_string_bool && txt[it_pos] != ch){
            in_string_bool = 1;
            start_pos = it_pos;
        }
        it_pos++;
    }
    // Add the last one
    if(in_string_bool && it_pos > start_pos){
        strs.push_back( txt.substr( start_pos, it_pos - start_pos) );
    }
}

void parseAndPrintLine(const std::string & line, const std::string & coordinateType){
    std::vector<std::string> parsed;
    splitString(line, parsed, ' ');
    if(parsed.size() != 4){
        std::cout << "error parsing vert: " << line << std::endl;
    }
    double double_x = ::atof(parsed[1].c_str());
    double double_y = ::atof(parsed[2].c_str());
    double double_z = ::atof(parsed[3].c_str());
    std::cout << coordinateType << " "<< double_x<< " "<< double_y << " "<<double_z << std::endl;
}

void parsePolyLine(const std::string & line){
    std::vector<std::string> spaceSplits;
    splitString(line, spaceSplits, ' ');
    std::vector<std::string> rep;
    std::vector <std::vector <std::string> > slashSplits(spaceSplits.size(), rep);
    for(size_t i = 0; i < spaceSplits.size(); i++){
        splitString(spaceSplits[i], slashSplits[i], '/');
    }
    int num_verts = slashSplits.size() - 1;
    int indicies[num_verts];
    for(int i=1; i < num_verts + 1; i++){
        indicies[i-1] = atoi(slashSplits[i][0].c_str()) - 1;
    }

    int num_triangles = num_verts - 2;
    for(int i = 0; i < num_triangles; i++){
        tridata td;
    	td.v[0]=indicies[0];
    	td.v[1]=indicies[1+i];
    	td.v[2]=indicies[2+i];
        std::cout << "f "<< td.v[0] << " " << td.v[1] << " " << td.v[2] << std::endl;
    }
}

int main (int argc, char* argv[]) {
    std::ifstream myfile ("models/minicooper.obj");
    if (myfile.is_open())
    {
        std::string line; 
        while ( getline (myfile,line) )
        {
            //std::cout<<line<<"\n";
            if(line[0] == 'v' && line[1] == 'n'){
                parseAndPrintLine(line, "vn");
            }
            else if(line[0] == 'v' && line[1] == 't'){
                parseAndPrintLine(line, "vt");
            }
            else if(line[0] == 'f'){
                parsePolyLine(line);
            }
            else if(line[0] == 'v'){
                parseAndPrintLine(line, "v");
            }
        }
    }
    return 0;
}
