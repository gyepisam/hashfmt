/*
 * formats lines of input into hash textual format for perl, json, python
 * Author: Gyepi Sam
 * Date: Jan 16, 2013
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

struct LanguageOpt {
    bool quote_key;   // does key need to be quoted?
    bool quote_value; // does value need to be quoted?
    string separator; // separates key and value
    string sigil;  //Variable designator. Only in perl...
};

typedef struct LanguageOpt LanguageOpt;
typedef map<string, LanguageOpt> Option;


Option options; /* List of languages and their hash syntax */

void init_options() {
    options["json"] = { true, true, ":", "" };
    options["perl"] = { false, true, "=>", "$" };
    options["python"] = { true, true, ":", "" };
}

string language_list() {
    string out;

    for(Option::iterator p = options.begin(); p != options.end(); p++) {
        if (p != options.begin())
            out += ", ";
        out += p->first;
    }

    return(out);
}

void usage(const string program) {
 cerr << "usage: " << program << " [OPTIONS] LANGUAGE\n\n";
 cerr << "OPTIONS:\n";
 cerr << "  --help | -h : show this text\n";
 cerr << "  --line-break N | -l N : print newline after every N elements\n";
 cerr << "  --variable XX | -v XX : assign the resulting hash to a variable named XX\n";
 cerr << "LANGUAGE is one of: " << language_list() << endl;
}

void usage(const string program, const string message) {
    cout << message << endl;
    usage(program);
}

string quote(const string value, bool requires_quotes) {
    string out;
    char quote = '"';

    if (requires_quotes)
        out += quote;

    for (string::size_type i = 0; i < value.size(); i++) {
        if (value[i] == quote)
            out += '\\';

        out += value[i];
    }

    if (requires_quotes)
        out += quote;

    return out;
}



int main(int argc, char *argv[]) {


    int linebreak = 0;
    string variable = "";
    int optind = 0;

    init_options();

    if (argc <  2) {
        usage(argv[0], "Incorrect usage");
        return 1;
    } else {
        for(int i = 1; i < argc; i++) {
            string val(argv[i]);

            if (val == "-h" || val == "--help") {
                usage(argv[0]);
                return 0;
            }
            else if (val == "-v" || val == "--variable") {
                variable = argv[++i];
            }
            else if (val == "-l" || val == "--line-break") {
                char *arg = argv[i+1];
                if ((linebreak = atoi(arg)) == 0) {
                    usage(argv[0], "cannot parse line-break argument: " + string(arg) + " as a number");
                    return(1);
                }
                else {
                    i++;
                }
            }
            else {
                optind = i;
                break;
            }
        }
    }

    if (optind == 0) {
        usage(argv[0]);
        return(1);
    }

    string language(argv[optind]);

    Option::iterator result = options.find(language);

    if (result == options.end()) {
        cerr << "Language " << language << " is not defined!\n";
        return 2;
    }

    LanguageOpt opt = result->second;
    
    vector<string> output;

    if (!variable.empty())
        cout << opt.sigil << variable << " = ";

    cout << "{";

    string key;
    string value;
    int lines = 0;

    while(cin) {
        cin >> key;

        if (key.empty())
            continue;

        cin >> value;
        
        if (++lines > 1) {
            cout << ", ";

            if (linebreak != 0 && (lines % linebreak == 1)) /* after every N, not on every N */
                cout << '\n';
        }

        cout << quote(key, opt.quote_key);
        cout << ' ' << opt.separator << ' ';
        cout << quote(value, opt.quote_value);

        key = "";
        value = "";
    }

    cout << "}\n";

    return 0;
}
