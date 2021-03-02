/*
By:
 2/16/20
 Emmanuel Borishade
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class LexAnalyzer{

private:
    vector<string> lexemes; // source code file lexemes
    vector<string> tokens;// source code file tokens
    map<string, string> tokenmap; // valid lexeme/token pairs
    // other private methods

    bool lineParse(string);//calls addOutput to populate output file
    void addOutput(string);

public:
    LexAnalyzer(istream& infile);
    /*  -pre: parameter refers to open data file consisting of token and lexeme pairs
         i.e. s_and and t_begin begin t_int 27. Each pair appears on its own input line.
        
        -post: tokenmap has been populated    */

    void scanFile(istream& infile, ostream& outfile){
    /*  -pre: 1 st parameter refers to an open text file that contains source
         code in the language, 2 nd parameter refers to an open empty output file

        -post: If no error, the token and lexeme pairs for the given input
         file have been written to the output file. If there is an error,
         the incomplete token/lexeme pairs, as well as an error message have
         written to the output file. A success or fail message has printed to the console.
    */
        string value;
        bool errorOccured = false;

        infile >> value;
        while (!infile.eof()){//will end before last line is parsed
            errorOccured = lineParse(value);
            infile >> value;
        }
        errorOccured = lineParse(value); //parse final line of file
        if (errorOccured){
            cout << "Error occured: Error in file. terminating..." << endl;
            cout << "File not Processed completely. Program fail." << endl;

        } else {
            cout << "File Processed. Program Sucessful." << endl;

        }

    }

};