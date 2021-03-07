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
        string error_message;


        // other private methods
    
        bool isADelimiter(char c){
        //post: returns whether char exists in list of delimiters
            string s(1, c);

            if (find(delimiters, s) != -1){
                return true;
            }
            return false;
        }


        int find(vector<string>& v, string val){
        //pre: 2nd param is a string that exists in vector
        //post: returns location of val; returns -1 if not found
            for (int i =0; i < v.size(); i++){
                if (v[i] == val){
                    return i;
                }
            }
            return -1; //not found
        }


        bool lineParse(istream& infile, string line, ostream& outfile){
        /* -pre: 1st param refers to open file, string is a line from infile
           -post: calls addOutput to populate output file
           -note: 1st param is in case statement spans multiple lines
        */   
            bool error = false;
            string output;//string to be added to output file
            string build;//will keep track of not-yet delimited elements
            int pos = 0; //will track position in line
            char ch = line[pos];//will keep track of char to be analyzed
            int ascii = int(ch);//ascii version of current char


            while(pos < line.length() && !error){
                ch = line[pos];
                ascii = int(ch);
                build = "";
                output = "";

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
                        output = "t_int : " + to_string(newint);
                        addOutput(outfile, output);
                        pos--; //allows delimiter to be futher analyzed later...

                    } else {//if last space is NOT a delimeter, THROW ERROR
                        error = true;
                        error_message = "Error: Integer_Not_Delimited_Overflow.";
                        addOutput(outfile, error_message);
                    }

  
                //STRING: "== 34 ascii
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
                        if (pos == line.length()){
                            getline(infile, line);

                            if (!infile.eof()){
                                pos = 0; 
                            } else {
                                error = true;
                                error_message = "Error: String_Not_Delimited_Overflow. End of file reached with open string.\n" +build;
                                addOutput(outfile, error_message);
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
                        output = "t_str : " + build;
                        addOutput(outfile, output);
                    }


                //SYMBOL: ,==44 (==40  )==41  ;==59 ascii
                }else if (ascii == ',' || ascii == '(' || ascii == ')' || ascii == ';'){
                    build = ch;
                    output = tokenmap[build] + " : " + ch;
                    addOutput(outfile, output);


                //ASSIGNMENT or RELATIONAL OP: <==60  ===61 >==62 !==33
                }else if (ascii == '<' || ascii == '>' || ascii == '=' || ascii == '!'){                    
                    pos++;

                    if (pos < line.length()){
                        if (line[pos-1] == '<' && line[pos] == '='){
                            output = tokenmap["<="] + " : <=";
                            addOutput(outfile, output);

                        } else if (line[pos-1] == '>' && line[pos] == '='){
                            output = tokenmap[">="] + " : >=";
                            addOutput(outfile, output);

                        } else if (line[pos-1] == '=' && line[pos] == '='){
                            output = tokenmap["=="] + " : ==";
                            addOutput(outfile, output);

                        } else if (line[pos-1] == '!' && line[pos] == '='){
                            output = tokenmap["!="] + " : !=";
                            addOutput(outfile, output);

                        } else {
                            pos--;
                            build = ch;
                            output = tokenmap[build] + " : " + ch;
                            addOutput(outfile, output);
                        }
                    }


                //ARITHMETIC OP: %==37 *==42 +==43 -==45 /==47
                }else if ( ascii == '%' || ascii == '*' || ascii == '+' || ascii == '-' || ascii == '/'){
                    build = ch;
                    output = tokenmap[build] + " : " + ch;
                    addOutput(outfile, output);


                //LOGICAL OP
                }else if (ascii == '&' || ascii == '|'){
                    //Make sure second char is same as first 
                    pos++;

                    if (pos <line.length()){
                        if (line[pos-1] == '&' && line[pos] == '&'){
                            output = tokenmap["&&"] + " : &&";
                            addOutput(outfile, output);

                        } else if (line[pos-1] == '|' && line[pos] == '|'){
                            output = tokenmap["||"] + " : ||";
                            addOutput(outfile, output);

                        } else {
                            error = true;
                            error_message = "Undefined Logic Operator reached";
                            addOutput(outfile, error_message);
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

                    //is key?
                    if (tokenmap.find(build) != tokenmap.end()){
                            output = tokenmap[build] + ": " + build;
                            addOutput(outfile, output);
                   
                    //is id? or invalid?
                    } else {

                        int i = 0;
                        while (i < build.length() && !error){
                            //if it contains anything but letters & numbers, not id
                            if ( !((build[i] >= '0' && build[i] <= '9') || (build[i] >= 'A' && build[i] <= 'Z') || (build[i] >= 'a' && build[i] <= 'z')) ){
                                error = true;
                                error_message = "Error: Undefined value reached.";
                                addOutput(outfile, error_message);
                            }
                            i++;
                        }
                        if (!error){
                            output = "t_id : " + build;
                            addOutput(outfile, output);

                        }
                    }

                }
                //END ANALYSIS, loop again

                pos++;
            }

            return error;
        }


        void addOutput(ostream& outfile, string output){
            outfile << output << endl;
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
            delimiters.push_back(" ");//space is a delimiter

            while (!infile.eof()){              
                tokenmap[lexeme] = token;//inserted 'backwards' so that 'and' returns lexeme s_and
                
                tokens.push_back(token);
                lexemes.push_back(lexeme);

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
            while (!scfile.eof() && !errorOccured){//will end before last line is parsed unless empty newline present
                errorOccured = lineParse(scfile, line, outfile);
                getline(scfile, line);
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
    
    ofstream outFile (outputfile);
    if (!outFile){
        cout << "error opening output file" << endl;
        exit(-1);
    }


    LexAnalyzer lex(sourcefile);
    lex.scanFile(inFile, outFile);

    sourcefile.close();
    inFile.close();
    outFile.close();


    return 0;
}