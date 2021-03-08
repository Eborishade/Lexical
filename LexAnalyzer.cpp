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
        vector<string> delimiters;


        // other private methods
    
        bool isADelimiter(char& c){
        //post: returns whether char exists in list of delimiters
            string s(1, c);

            if (find(delimiters, s) != -1){
                return true;
            }
            return false;
        }


        int find(vector<string>& v, string& val){
        //pre: 2nd param is a string that exists in vector
        //post: returns location of val; returns -1 if not found
            for (int i =0; i < v.size(); i++){
                if (v[i] == val){
                    return i;
                }
            }
            return -1; //not found
        }


        void delSpaces(string& str){
            //post: removes repeated spaces from string if there are any
            // leaves any trailing spaces at end of string
           int i = 0;

           while (i < str.length()-1){
                if (i > 0 && str[i-1] == ' ' && str[i] == ' '){
                    str.erase(i, 1); 
                    i--;
                }
                i++;
            }
        }


        bool lineParse(istream& infile, string line, ostream& outfile){
        /* -pre: 1st param refers to open file, string is a line from infile
           -post: calls addOutput to populate output file
           -note: 1st param is in case statement spans multiple lines
        */   
            bool error = false;
            string build;//will keep track of not-yet delimited elements
            int pos = 0; //will track position in line
            char ch = line[pos];//will keep track of char to be analyzed
            int ascii = int(ch);//ascii version of current char


            while(pos < line.length() && !error){
                ch = line[pos];
                ascii = int(ch);
                build = "";

                //SPACE
                if (ascii == 32){//space:  == 32 ascii
                    //do nothing, move on.


                //NUMBERS
                } else if(ch >= '0' && ch <= '9' ){
                    //until delimiter reached, add all chars to string
                    //test string as int, else error
                    
                    while (ch >= '0' && ch <= '9' && pos < line.length()){
                        ch = line[pos];
                        ascii = int(ch);
                        if(ch >= '0' && ch <= '9'){
                            build+= ch;
                            pos++;
                        }
                    }//will end when ch no longer equals a number. build will be string filled with numbers.
                    
                    if (isADelimiter(ch)){//success, print to outfile
                        int newint = stoi(build);
                        tokens.push_back(tokenmap["t_int"]);
                        lexemes.push_back(to_string(newint));

                        pos--; //allows delimiter to be futher analyzed later...

                    } else {//if last space is NOT a delimeter, THROW ERROR
                        error = true;
                        tokens.push_back(tokenmap["t_int"]);
                        lexemes.push_back("Error: Integer_Not_Delimited_Overflow.");
                    }

  
                //STRING:: "== 34 ascii
                }else if (ascii == '"'){
                    //may cause reading multiple lines
                    //until delimiter reached, add all chars to string
                    //test string as id or keyword, else error
                    pos++;
                    ch = line[pos];
                    ascii = int(ch);

                    while (ch != '"' && !error){
                        build += ch;
                        pos++;

                        //if line ends, read in new line.
                        if (pos >= line.length()){
                            getline(infile, line);

                            if (!infile.eof()){
                                pos = 0; 
                            } else {
                                error = true;
                                tokens.push_back(tokenmap["t_str"]);
                                lexemes.push_back("Error: String_Not_Delimited_Overflow. End of file reached with open string.");
                            }
                        }
                        
                        //set ch to new position, account for spaces
                        if (!error){
                            ch = line[pos];
                            ascii = int(ch);
                        }
                    }
                    //output
                    if (!error){
                        delSpaces(build);
                        tokens.push_back(tokenmap["t_str"]);
                        lexemes.push_back(build);
                    }


                /* DELMITERS:: = , ( ) ; < <= > >= == != + - * / % && || !  So... */ 
                } else if (isADelimiter(ch)){
                    pos++;
                    bool twochar = false;//tracks whether item is 2char delim or 1char delim

                    //first check 2 char-length  delimiters
                    if (pos < line.length()){
                        build += ch;
                        build += line[pos];
                        
                        //look for in token map, else not delimiter.
                        if (tokenmap.find(build) != tokenmap.end()){
                            tokens.push_back(tokenmap[build]);
                            lexemes.push_back(build);
                            twochar = true;      
                        } 
                    //KEEP PROGRESSION (pos++) to prevent double analysis
                    } 

                    //else check 1 char-length delimiters
                    if (!twochar){
                        build = ch;
                        pos--; //undo original progression for later analysis...

                        if (tokenmap.find(build) != tokenmap.end()){
                            tokens.push_back(tokenmap[build]);
                            lexemes.push_back(build);

                        } else {//should not be reachable...
                                error = true;
                                tokens.push_back(build);
                                lexemes.push_back("Error<catastrophic>: Unknown_Delimiter_Reached!");
                            }
                    }

                //OTHER
                } else {
                    /*it must be an identifier, keyword or error.
                     until delimiter reached, add all chars to string, 
                     test string as id or keyword, else error
                    */
                    while (!isADelimiter(ch) && pos < line.length()){
                        build+= ch;

                        pos++;
                        if (pos < line.length()){
                            ch = line[pos];
                            ascii = int(ch);                            
                        }
                    }
                    pos--;//so delimiter can be analyzed later...

                    //is key
                    if (tokenmap.find(build) != tokenmap.end()){
                            tokens.push_back(tokenmap[build]);
                            lexemes.push_back(build);
                   
                    //is id or invalid
                    } else {

                        int i = 0;
                        while (i < build.length() && !error){
                            //if it contains anything but letters & numbers then invalid
                            if ( !((build[i] >= '0' && build[i] <= '9') || (build[i] >= 'A' && build[i] <= 'Z') || (build[i] >= 'a' && build[i] <= 'z')) ){
                                error = true;
                                tokens.push_back("Null");
                                lexemes.push_back("Error: Illegal_Argument_Reached. ["+build+"]");
                            }
                            i++;
                        }
                        //is id
                        if (!error){
                            tokens.push_back(tokenmap["t_id"]);
                            lexemes.push_back(build);
                        }
                    }
                }
                //END ANALYSIS, loop again

                pos++;
            }
            return error;
        }


        void outprint(ostream& outfile){
            //post: prints contents of token and lexeme vectors
            for(int i = 0; i < tokens.size(); i++){
                outfile << tokens[i] << " : " << lexemes[i] << endl;
            }
        }


    public:
        LexAnalyzer(istream& infile){
        /*  -pre: parameter refers to open data file consisting of token and lexeme pairs
             i.e. s_and and t_begin begin t_int 27. Each pair appears on its own input line.
            File must include empty line at end to prevent premature eof trigger.
        
            -post: tokenmap has been populated 
        */

            //token = s_and,  lexeme = and 
            string token, lexeme; 
            infile >> token >> lexeme; //read file line: << a << b; map[a] = b
            delimiters.push_back(" ");//space is a delimiter
            tokenmap["t_id"] = "t_id";
            tokenmap["t_int"] = "t_int";
            tokenmap["t_str"] = "t_str";

            while (!infile.eof()){              
                tokenmap[lexeme] = token;//inserted 'backwards' so that 'and' returns lexeme s_and

                infile >> token >> lexeme; //read file line: << a << b; map[a] = b

                if (token[0] == 's'){//all symbols are delimeters
                    delimiters.push_back(lexeme);
                }

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

            getline(scfile, line);
            while (!scfile.eof() && !errorOccured){
            //note: will end before last line is parsed, unless empty new line present in file
                errorOccured = lineParse(scfile, line, outfile);
                getline(scfile, line);
            }

            outprint(outfile);

            if (errorOccured){
                cout << "Error occured: Error in file. terminating..." << endl;
                cout << "File not Processed completely. Program fail." << endl;
            } else {
                cout << "File Processed. Program Sucessful." << endl;
            }

        }

};



int main(){
    string source = "tokenlexemedata.txt";
    string codefile;
    string outputfile; 


    //setup source file

    ifstream sourcefile (source);  
      if (!sourcefile){
        cout << "error opening source file" << endl;
        exit(-1);
    }

    //setup code input file

    cout << "Enter Input File >> "; 
    cin >> codefile;

    ifstream inFile (codefile);
    if (!inFile){
        cout << "error opening code input file" << endl;
        exit(-1);
    }

    //setup output input file

    cout << "Enter Output File >> "; 
    cin >> outputfile;
    ofstream outFile (outputfile); //creates file if there is none found
  
    // LEX

    LexAnalyzer lex(sourcefile);
    lex.scanFile(inFile, outFile);


    //close out

    sourcefile.close();
    inFile.close();
    outFile.close();


    return 0;
}