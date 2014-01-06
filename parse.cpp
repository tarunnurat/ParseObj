#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <cstring>

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

void parseAndPrintLine(const std::string & line, const char * coordinateType, FILE * f) {
    std::vector<std::string> parsed;
    splitString(line, parsed, ' ');
    if(parsed.size() != 4){
        std::cout << "error parsing vert: " << line << std::endl;
    }
    float float_x = ::atof(parsed[1].c_str());
    float float_y = ::atof(parsed[2].c_str());
    float float_z = ::atof(parsed[3].c_str());
    
    //out_file << coordinateType << " "<< float_x << " "<< float_y << " "<<float_z << std::endl;

    /*
    out_file << coordinateType << " ";
    out_file << float_x;
    out_file << " ";
    out_file << float_y; out_file << " ";
    out_file << float_z;
    out_file << std::endl;
    */
    fwrite(coordinateType, strlen(coordinateType), sizeof(char), f);
    fwrite((char*)&float_x, sizeof(float), 1, f);
    fwrite((char*)&float_y, sizeof(float), 1, f);
    fwrite((char*)&float_z, sizeof(float), 1, f);
    //printf("%s %a %a %a\n", coordinateType, float_x, float_y, float_z); 
}

void parsePolyLine(const std::string & line, FILE * f){
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
    for(int i = 0; i < num_triangles; i++) {
        tridata td;
    	td.v[0]=indicies[0];
    	td.v[1]=indicies[1+i];
    	td.v[2]=indicies[2+i];
        //out_file << "f " << td.v[0] << " " << td.v[1] << " " << td.v[2] << std::endl;
    fwrite("f", strlen("f"), sizeof(char), f);
    fwrite((char*)&td.v[0], sizeof(float), 1, f);
    fwrite((char*)&td.v[1], sizeof(float), 1, f);
    fwrite((char*)&td.v[2], sizeof(float), 1, f);
    /*
    out_file << "f ";
    out_file << td.v[0];
    out_file << " ";
    out_file << td.v[1];
    out_file << " ";
    out_file << td.v[2];
    out_file << std::endl;
    */
    }
}

int main () {
    std::ifstream myfile ("models/cube.obj");
    if (myfile.is_open())
    {
        /*
        std::ofstream out_file;
        out_file.open("out.obj");
        */
        FILE * f = fopen("out.obj", "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        std::string line; 
        while ( getline (myfile,line) )
        {
            //std::cout<<line<<"\n";
            if(line[0] == 'v' && line[1] == 'n'){
                parseAndPrintLine(line, "vn", f);
            }
            else if(line[0] == 'v' && line[1] == 't'){
                parseAndPrintLine(line, "vt", f);
            }
            else if(line[0] == 'f'){
                parsePolyLine(line, f);
            }
            else if(line[0] == 'v'){
                parseAndPrintLine(line, "v", f);
            }
        }
        fclose(f);
        //out_file.close();
    }
    return 0;
}
