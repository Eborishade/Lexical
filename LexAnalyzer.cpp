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
        //symbols must be delimited by space, whitespace, id, int, or other symbol.


        // other private methods
    
        bool lineParse(string line){}

        bool lineParse(istream& infile, string line){
        /* -pre: 1st param refers to open file, string is a line from infile
           -post: calls addOutput to populate output file
           -note: 1st param is in case statement spans multiple lines
        */   

            int pos = 0;
            char ch = line[pos];//focus on first character in line
            int ascii = int(ch);
            cout << "ch: " << ch <<endl;
            cout << "ascii: " << ascii <<endl;

            while(pos < line.length()){
                ch = line[pos];
                ascii = int(ch);
                //********<testing>*************
                cout << "ch: " << ch <<endl;
                cout << "ascii: " << ascii <<endl;
                //********</testing>*************


                if (ascii == 32){//space:  == 32 ascii
                    //do nothing, move on OR DELIMITER, END PREV
                    pos++;


                } else if(ch >= '0' && ch <= '9' ){//number
                    //until delimiter reached, add all chars to string
                    //test string as int, else error

                    

                }else if (ascii == '"'){//string: "== 34 ascii
                    //may cause reading multiple lines
                    //until delimiter reached, add all chars to string
                    //test string as id or keyword, else error
                    

                }else if (ascii == ',' || ascii == '(' || ascii == ')' || ascii == ';'){//symbol: ,==44 (==40  )==41  ;==59 ascii


                }else if ((ascii >= 60 && ascii <=62) || ascii == '!'){//relational op: <==60  ===61 >==62 !==33


                }else if ( ascii == '%' || ascii == '*' || ascii == '+' || ascii == '-' || ascii == '/'){//arithmetic op: %==37 *==42 +==43 -==45 /==47


                }else if (ascii == '&'){//logical op


                } else {//it must be an identifier, keyword or error.
                    //until delimiter reached, add all chars to string
                    //test string as id or keyword, else error

                }

                pos++;//???? may create error/skip; WATCH
            }



        }
        


        void addOutput(ostream& outfile, string output){
            outfile << output;
        }


    public:
        LexAnalyzer(istream& infile){
        /*  -pre: parameter refers to open data file consisting of token and lexeme pairs
             i.e. s_and and t_begin begin t_int 27. Each pair appears on its own input line.
            File must include empty line at end to prevent premature eof trigger.
        
            -post: tokenmap has been populated 
        */

            string token, lexeme;
            infile >> token >> lexeme; //read file line: << a << b; map[a] = b

            while (!infile.eof()){              
                tokenmap[lexeme] = token;//inserted 'backwards' so that 'and' returns lexeme s_and
                
                tokens.push_back(token);
                lexemes.push_back(lexeme);

                infile >> token >> lexeme; //read file line: << a << b; map[a] = b
            }
        }


        void scanFile(istream& scfile, ostream& outfile){
        /*  -pre: 1st parameter refers to an open text file that contains source
             code in the language, 2nd parameter refers to an open empty output file
            File must include empty line at end to prevent premature eof trigger.

            -post: If no error, the token and lexeme pairs for the given input
             file have been written to the output file. If there is an error,
             the incomplete token/lexeme pairs, as well as an error message have
             written to the output file. A success or fail message has printed to the console.
        */
            string line;
            bool errorOccured = false;

            scfile >> line;
            while (!scfile.eof() && !errorOccured){//will end before last line is parsed unless empty newline present
                errorOccured = lineParse(scfile, line);
                scfile >> line;
            }

            if (errorOccured){
                cout << "Error occured: Error in file. terminating..." << endl;
                cout << "File not Processed completely. Program fail." << endl;
            } else {
                cout << "File Processed. Program Sucessful." << endl;
            }

        }

};



int main(){


    return 0;
}