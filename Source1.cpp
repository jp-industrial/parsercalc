//calculator with parser, 21/2/23 jp-ind.

#include "c:\Users\james\Desktop\std_lib_facilities.h"

class Token { //we use these to represent chars, and 'read ahead'
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};

class Token_stream {
public:
    Token_stream() :full(false), buffer(0) {}; //don't know why this works, function doesn't work without it.
    Token get(); //get token from stream
    void putback(Token t); //put token back
private:
    bool full{ false }; //is there a token in the buffer?
    Token buffer; //putback() puts token here.
};

void Token_stream::putback(Token t) //class member function
{
    if (full) error("putback() into full buffer!\n"); //checks buffer condition.
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full) { //check buffer
        full = false;
        return buffer; //if full, get token from buffer.
    }

    char ch;
    cin >> ch;

    switch (ch) {
    case ';': //for printing.
    case 'q': //for 'quit'.
    case '(': case ')': case '+': case '-': case '*': case '/':
        return Token(ch); //Token will represent itself as 'kind'.

    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch); //put character back into input stream.
        double val;
        cin >> val; //feed from input stream.
        return Token('8', val);
    }
    default:
        error("Bad token.\n");
    }

}

Token_stream ts;

double expression(); //declaration
double primary()     // read and evaluate a Primary
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':    // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return d;
    }
    case '8':            // we use '8' to represent a number
        return t.value;  // return the number's value
    default:
        error("primary expected");
    }
}
double term()
{
    double left = primary();
    Token t = ts.get();     // get the next token

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();    // get the next token
    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;       // finally: no more + or -: return the answer
        }
    }
}

int main() {
    try {
        while (cin)
            cout << '=' << expression() << '\n';
        keep_window_open("~0");
    }
    catch (exception& e) {
        cerr << e.what() << endl;
        keep_window_open("~1");
        return 1;
    }
    catch (...) {
        cerr << "exception \n";
        keep_window_open("~2");
        return 2;
    }
}
