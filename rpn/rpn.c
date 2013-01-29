#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
char lookAhead = EMPTY;

#define isBinDigit(c) (('0' <= (c)) && ('1' >= (c)))
#define isOctDigit(c) (('0' <= (c)) && ('7' >= (c)))
#define isDecDigit(c) (('0' <= (c)) && ('9' >= (c)))
#define isHexDigit(c) ((('0' <= (c)) && ('9' >= (c))) || (('A' <= (c)) && ('F' >= (c))))
#define lexBuf(c) ((' ' == (c)) || ('\t' == (c)))
#define newline(c) (('\r' == (c)) || ('\n' == (c)))
#define isOp(c) (('+' == (c)) || ('-' == (c)) || ('*' == (c)) || ('/' == (c))|| ('%' == (c)) || ('^' == (c)))

#define toBinDigit(c) ((c) - '0')
#define toDecDigit(c) ((c) - '0')
#define toOctDigit(c) ((c) - '0')
#define toHexDigit(c) (isDecDigit(c) ? ((c) - '0') : ((c) - 'A' + 10))

#define addBinDigit(val) ((val) << 1)
#define addDecDigit(val) (((val) << 3) + ((val) << 1))
#define addOctDigit(val) (val) << 3)
#define addHexDigit(val) ((val) << 4)

#define handleDigit(type,done,next,val)                                 \
case type:                                                              \
    if(!(done = !is ## type ## Digit(next)))                            \
        val = add ## type ## Digit(val) + to ## Bin ## Digit(next);     \
    break

enum TOKENS {INTEGER, OPERATION, NEWLINE, ERROR};
enum OPERATION {ADD, SUB, MULT, DIV, MOD, POW};
enum BASES {Bin, Oct, Dec, Hex};

void read_char(char * result)
{
    int val = getchar();
    if (val == -1) exit(0);
    *result = (char)val;
}

int power(int a, int b)
{
    int out = 1;
    while(b != 0)
    {
        if(b%2)
        out *= a;
        a *= a;
        b >>= 1;
    }
    return out;
}

#define read_char_t(name) void (*name)(char*)
void lexer(read_char_t(nextChar), int* type, int* val)
{
    // there is nothing in the look ahead,
    if(lookAhead == EMPTY)
    nextChar(&lookAhead);
    // clear out all buffer characters
    for(;lexBuf(lookAhead); nextChar(&lookAhead)) ;
    // checks if it is a digit, all numbers must start with
    // a zero for a change of base, or a decimal number,
    // or a negative sign (which is matched later)
    if(isDecDigit(lookAhead))
    {
        // base is assumed decimal, and we are not done
        int base = Dec, done = 0, looked = 0;
        // the type is an int
        *type = INTEGER;
        // converts the character to its the integer digit
        // it represents
        *val = toDecDigit(lookAhead);
        // if val has a leading 0, then it is specifying it's base
        if(*val == 0)
        {
            // looks at the next character
            nextChar(&lookAhead);
            // switches on the bases
            switch(lookAhead)
            {
            case 'b':
            case 'B':
                base = Bin;
                break;
            case 'o':
            case 'O':
            case 'q':
            case 'Q':
                base = Oct;
                break;
            case 'd':
            case 'D':
                break;
            case 'x':
            case 'X':
            case 'h':
            case 'H':
                base = Hex;
                break;
            default:
                // a 0 followed by another digit is assumed to be octal
                if(isDecDigit(lookAhead))
                {
                    base = Oct;
                    looked = 1;
                }
                else
                    return;
            }
        }
        do
        {
            // loads the next character and switches on the base
            if(!looked)
                nextChar(&lookAhead);
            looked = 0;
            switch(base)
            {
            // checks if the character is of the appropriate type
            // and updates the value appropriately
            handleDigit(Bin,done,lookAhead,*val);
            handleDigit(Oct,done,lookAhead,*val);
            handleDigit(Dec,done,lookAhead,*val);
            handleDigit(Hex,done,lookAhead,*val);
            }
        } while(!done) ;
        // if its still a possible digit, then we want to throw in a error so
        // 09 doesn't split into 0 and 9
        if(isHexDigit(lookAhead))
        {
            *type = ERROR;
            *val = (int)lookAhead;
        }
    }
    else if(isOp(lookAhead))
    {
        // the character specifies an operation
        *type = OPERATION;
        switch(lookAhead)
        {
        case '+':
            *val = ADD;
            break;
        case '-':
            *val = SUB;
            // double checks to make sure it isn't a negative number
            nextChar(&lookAhead);
            if(isDecDigit(lookAhead))
            {
            // the lexer call will overwrite type and val
            lexer(nextChar,type,val);
            // indicate that it is a negative number, note that
            // this will work for -2^31
            *val *= -1;
            }
            // returns as to not set lookAhead = 0
            return;
        case '*':
            *val = MULT;
            break;
        case '/':
            *val = DIV;
            break;
        case '%':
            *val = MOD;
            break;
        case '^':
            *val = POW;
            break;
        }
        // nothing left in the lookahead buffer
        lookAhead = EMPTY;
    }
    else if(newline(lookAhead))
    {
        // we have a newline
        *type = NEWLINE;
        lookAhead = EMPTY;
    }
    else
    {
        // unrecognized character
        *type = ERROR;
        *val = (int)lookAhead;
        lookAhead = EMPTY;
    }
}
// error handling done through a global flag
char PARSE_ERROR = 0;
// *i and *j are the previous to values on the stack
void parser(read_char_t(nextChar), int* i, int* j)
{
    int type, val;
    // stores the next TOKEN in type and val
    for(lexer(nextChar, &type, &val); ; lexer(nextChar, &type, &val))
    {
        // switches on the type of the TOKEN
        switch(type)
        {
        case INTEGER:
            // if its an int, then we advance one place in the stack and continue
            // parsing, eventually it should reduce to an integer, the stack
            // is updated by the next level, see case OPERATION
            parser(nextChar, j, &val);
            // checks to make sure there isn't a parse error
            // if there is a parse error the error is caught at the first level
            // (when j is NULL)
            if(PARSE_ERROR && j == NULL)
                PARSE_ERROR = 0;
            else if(PARSE_ERROR)
                return;
            break;
        case OPERATION:
            // thers nothing to operate on, no need to return since this is the bottom wrung
            if(i == NULL && j == NULL)
            printf("Error:Operation put on the stack without any preceding numbers\n");
            // theres only one thing to operate on, and all operations are binary
            else if(i == NULL)
            {
                printf("Error:Operation put on the stack with only one preceding number\n");
                // no need to set PARSE_ERROR since we return to the bottom wrung
                return;
            }
            else
            {
                // completes the operation and stores the value in *i
                // which is equivalent to *j of the previous level
                // then returns back to the previous level
                switch(val)
                {
                case ADD:
                    *i = (*i)+(*j);
                    return;
                case SUB:
                    *i = (*i)-(*j);
                    return;
                case MULT:
                    *i = (*i)*(*j);
                    return;
                case DIV:
                    // divide by 0 error
                    if(*j == 0)
                    {
                        printf("Error: divide by 0\n");
                        PARSE_ERROR = 1;
                        return;
                    }
                    *i = (*i)/(*j);
                    return;
                case MOD:
                    // divide by 0 error
                    if(*j == 0)
                    {
                        printf("Error: mod by 0\n");
                        PARSE_ERROR = 1;
                        return;
                    }
                    *i = (*i)%(*j);
                    return;
                case POW:
                    // negative power error (possibly could just return 0)
                    if(*j < 0)
                    {
                        printf("Error: negative power\n");
                        PARSE_ERROR = 1;
                        return;
                    }
                    *i = power(*i,*j);
                    return;
                }
            }
            break;
        case NEWLINE:
            if(i == NULL && j == NULL) ;
                // newline without anything happening
            else if(i == NULL)
            {
                // newline with a single value
                printf("%d\n",*j);
                return;
            }
            else
            {
                // still stuff on the stack, its a problem
                printf("Error: newline character with numbers still on the stack\n");
                PARSE_ERROR = 1;
                return;
            }
            break;
        case ERROR:
            // theres a problem character
            printf("Error:Unexpected character '%c'\n",(char)val);
            // should check if j == NULL before returning
            PARSE_ERROR = 1;
            return;
        }
    }
}
int main()
{
    printf("Welcome to the RPN calculator\n");
    do
    {
        PARSE_ERROR = 0;
        parser(read_char, NULL, NULL);
    } while (PARSE_ERROR);
    return 0;
}


