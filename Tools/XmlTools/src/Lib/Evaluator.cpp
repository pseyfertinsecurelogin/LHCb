// -*- C++ -*-
// ---------------------------------------------------------------------------

#include "XmlTools/Evaluator.h"

#include <iostream>
#include <algorithm>
#include <cmath>	// for std::pow()
#include <stack>
#include <string>
#include <unordered_map>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>	// for strtod()

#include "boost/utility/string_ref.hpp"
#include "boost/variant.hpp"

#define EVAL XmlTools::Evaluator

//---------------------------------------------------------------------------
using FuncPtr = boost::variant<double(*)(),
                               double(*)(double),
                               double(*)(double,double),
                               double(*)(double,double,double),
                               double(*)(double,double,double,double),
                               double(*)(double,double,double,double,double)>;


using Item = boost::variant<boost::blank,double,   std::string, FuncPtr>;
enum                      { UNKNOWN,     VARIABLE, EXPRESSION,  FUNCTION }; // must match order in boost::variant


typedef const char * pchar;
typedef std::unordered_map<std::string,Item> dic_type;

struct EVAL::Struct final {
  dic_type theDictionary;
  std::unique_ptr<char[]> theExpression;
  pchar    thePosition = nullptr;
  int      theStatus = EVAL::OK;
  double   theResult = 0.0;
};

//---------------------------------------------------------------------------

namespace {

    class InvokeFuncPtrWith : public boost::static_visitor<double> {
        const double *p;
        template <typename F, std::size_t ... Is>
        double helper( F fun, std::index_sequence<Is...> ) const {
            return fun( p[sizeof...(Is)-1-Is]... ); // reverse the arguments...
        }
    public:
        InvokeFuncPtrWith(const double* pp) : p(pp) {}
        template <typename... Args>
        double operator()( double(*fun)(Args...) ) const {
            return helper( fun, std::index_sequence_for<Args...>{} );
        }
    };

    inline boost::string_ref remove_blanks(boost::string_ref str) {
        int n=str.size();
        int i=0;   while (i<n && isspace(str[i])) ++i;
        int j=n-1; while (j>i && isspace(str[j])) --j;
        return str.substr(i,j-i+1);
    }

    inline std::string to_string(boost::string_ref str) {
        return std::string( str.data(), str.size() );
    }
    inline std::string operator+(char s1, boost::string_ref s2) {
        return s1+to_string(s2);
    }
}
#define SKIP_BLANKS                      \
for(;;++pointer) {                       \
  c = (pointer > end) ? '\0' : *pointer; \
  if (!isspace(c)) break;                \
}

#define EVAL_EXIT(STATUS,POSITION) endp = POSITION; return STATUS

static constexpr int MAX_N_PAR = 5;
static const char sss[MAX_N_PAR+2] = "012345";

enum { ENDL, LBRA, OR, AND, EQ, NE, GE, GT, LE, LT,
       PLUS, MINUS, MULT, DIV, POW, RBRA, VALUE };

static int engine(pchar, pchar, double &, pchar &, const dic_type &);

static int variable(const std::string & name, double & result,
                    const dic_type & dictionary)
/***********************************************************************
 *                                                                     *
 * Name: variable                                    Date:    03.10.00 *
 * Author: Evgeni Chernyaev                          Revised:          *
 *                                                                     *
 * Function: Finds value of the variable.                              *
 *           This function is used by operand().                       *
 *                                                                     *
 * Parameters:                                                         *
 *   name   - name of the variable.                                    *
 *   result - value of the variable.                                   *
 *   dictionary - dictionary of available variables and functions.     *
 *                                                                     *
 ***********************************************************************/
{
  auto iter = dictionary.find(name);
  if (iter == dictionary.end())
    return EVAL::ERROR_UNKNOWN_VARIABLE;
  const auto& item = iter->second;
  switch (item.which()) {
  case VARIABLE:
    result = boost::get<double>(item);
    return EVAL::OK;
  case EXPRESSION: {
    auto exp_begin = boost::get<std::string>(item).c_str();
    auto exp_end   = std::next(exp_begin,strlen(exp_begin) - 1);
    if (engine(exp_begin, exp_end, result, exp_end, dictionary) == EVAL::OK)
      return EVAL::OK;
  }
  /* no break */
  default:
    return EVAL::ERROR_CALCULATION_ERROR;
  }
}

static int function(const std::string & name, std::stack<double> & par,
	                double & result, const dic_type & dictionary)
/***********************************************************************
 *                                                                     *
 * Name: function                                    Date:    03.10.00 *
 * Author: Evgeni Chernyaev                          Revised:          *
 *                                                                     *
 * Function: Finds value of the function.                              *
 *           This function is used by operand().                       *
 *                                                                     *
 * Parameters:                                                         *
 *   name   - name of the function.                                    *
 *   par    - stack of parameters.                                     *
 *   result - value of the function.                                   *
 *   dictionary - dictionary of available variables and functions.     *
 *                                                                     *
 ***********************************************************************/
{
  int npar = par.size();
  if (npar > MAX_N_PAR) return EVAL::ERROR_UNKNOWN_FUNCTION;

  auto iter = dictionary.find(sss[npar]+name);
  if (iter == dictionary.end()) return EVAL::ERROR_UNKNOWN_FUNCTION;
  const auto& item = iter->second;

  double pp[MAX_N_PAR];
  for(int i=0; i<npar; i++) { pp[i] = par.top(); par.pop(); }
  errno = 0;
  if (item.which()!=FUNCTION) return EVAL::ERROR_CALCULATION_ERROR;
  const auto& fun = boost::get<FuncPtr>(item);
  if ( fun.which()!=npar)     return EVAL::ERROR_CALCULATION_ERROR;
  result = boost::apply_visitor( InvokeFuncPtrWith(pp), fun );
  return (errno == 0) ? EVAL::OK : EVAL::ERROR_CALCULATION_ERROR;
}

static int operand(pchar begin, pchar end, double & result,
                   pchar & endp, const dic_type & dictionary)
/***********************************************************************
 *                                                                     *
 * Name: operand                                     Date:    03.10.00 *
 * Author: Evgeni Chernyaev                          Revised:          *
 *                                                                     *
 * Function: Finds value of the operand. The operand can be either     *
 *           a number or a variable or a function.                     *
 *           This function is used by engine().                        *
 *                                                                     *
 * Parameters:                                                         *
 *   begin  - pointer to the first character of the operand.           *
 *   end    - pointer to the last character of the character string.   *
 *   result - value of the operand.                                    *
 *   endp   - pointer to the character where the evaluation stoped.    *
 *   dictionary - dictionary of available variables and functions.     *
 *                                                                     *
 ***********************************************************************/
{
  pchar pointer = begin;

  //   G E T   N U M B E R
  if (!isalpha(*pointer)) {
	errno = 0;
    result = strtod(pointer, (char **)(&pointer));
    if (errno == 0) {
      EVAL_EXIT( EVAL::OK, --pointer );
    }else{
      EVAL_EXIT( EVAL::ERROR_CALCULATION_ERROR, begin );
    }
  }

  //   G E T   N A M E
  char  c;
  while(pointer <= end) {
    c = *pointer;
    if (c != '_' && !isalnum(c)) break;
    pointer++;
  }
  c = *pointer;
  *const_cast<char*>(pointer) = '\0';
  std::string name(begin);
  *const_cast<char*>(pointer) = c;

  //   G E T   V A R I A B L E
  result = 0.0;
  SKIP_BLANKS;
  if (c != '(') {
    int EVAL_STATUS = variable(name, result, dictionary);
    EVAL_EXIT( EVAL_STATUS, (EVAL_STATUS == EVAL::OK) ? --pointer : begin);
  }

  //   G E T   F U N C T I O N
  std::stack<pchar>  pos;                // position stack
  std::stack<double> par;                // parameter stack
  pchar par_begin = pointer+1;

  for(;;pointer++) {
    c = (pointer > end) ? '\0' : *pointer;
    switch (c) {
    case '\0':
      EVAL_EXIT( EVAL::ERROR_UNPAIRED_PARENTHESIS, pos.top() );
    case '(':
      pos.push(pointer); break;
    case ',':
      if (pos.size() == 1) {
        double        value;
        auto par_end = pointer-1;
        int EVAL_STATUS = engine(par_begin, par_end, value, par_end, dictionary);
        if (EVAL_STATUS == EVAL::WARNING_BLANK_STRING)
          { EVAL_EXIT( EVAL::ERROR_EMPTY_PARAMETER, --par_end ); }
        if (EVAL_STATUS != EVAL::OK)
          { EVAL_EXIT( EVAL_STATUS, par_end ); }
        par.push(value);
        par_begin = pointer + 1;
      }
      break;
    case ')':
      if (pos.size() > 1) {
        pos.pop();
        break;
      }else{
        double        value;
        auto par_end = pointer-1;
        int EVAL_STATUS = engine(par_begin, par_end, value, par_end, dictionary);
        switch (EVAL_STATUS) {
        case EVAL::OK:
          par.push(value);
          break;
        case EVAL::WARNING_BLANK_STRING:
          if (par.size() != 0)
            { EVAL_EXIT( EVAL::ERROR_EMPTY_PARAMETER, --par_end ); }
          break;
        default:
          EVAL_EXIT( EVAL_STATUS, par_end );
        }
        EVAL_STATUS = function(name, par, result, dictionary);
        EVAL_EXIT( EVAL_STATUS, (EVAL_STATUS == EVAL::OK) ? pointer : begin);
      }
    }
  }
  return EVAL::ERROR_SYNTAX_ERROR; // just for the Eclipse static analyzer
}

/***********************************************************************
 *                                                                     *
 * Name: maker                                       Date:    28.09.00 *
 * Author: Evgeni Chernyaev                          Revised:          *
 *                                                                     *
 * Function: Executes basic arithmetic operations on values in the top *
 *           of the stack. Result is placed back into the stack.       *
 *           This function is used by engine().                        *
 *                                                                     *
 * Parameters:                                                         *
 *   op  - code of the operation.                                      *
 *   val - stack of values.                                            *
 *                                                                     *
 ***********************************************************************/
static int maker(int op, std::stack<double> & val)
{
  if (val.size() < 2) return EVAL::ERROR_SYNTAX_ERROR;
  double val2 = val.top(); val.pop();
  double val1 = val.top();
  switch (op) {
  case OR:                                // operator ||
    val.top() = (val1 || val2) ? 1. : 0.;
    return EVAL::OK;
  case AND:                               // operator &&
    val.top() = (val1 && val2) ? 1. : 0.;
    return EVAL::OK;
  case EQ:                                // operator ==
    val.top() = (val1 == val2) ? 1. : 0.;
    return EVAL::OK;
  case NE:                                // operator !=
    val.top() = (val1 != val2) ? 1. : 0.;
    return EVAL::OK;
  case GE:                                // operator >=
    val.top() = (val1 >= val2) ? 1. : 0.;
    return EVAL::OK;
  case GT:                                // operator >
    val.top() = (val1 >  val2) ? 1. : 0.;
    return EVAL::OK;
  case LE:                                // operator <=
    val.top() = (val1 <= val2) ? 1. : 0.;
    return EVAL::OK;
  case LT:                                // operator <
    val.top() = (val1 <  val2) ? 1. : 0.;
    return EVAL::OK;
  case PLUS:                              // operator '+'
    val.top() = val1 + val2;
    return EVAL::OK;
  case MINUS:                             // operator '-'
    val.top() = val1 - val2;
    return EVAL::OK;
  case MULT:                              // operator '*'
    val.top() = val1 * val2;
    return EVAL::OK;
  case DIV:                               // operator '/'
    if (val2 == 0.0) return EVAL::ERROR_CALCULATION_ERROR;
    val.top() = val1 / val2;
    return EVAL::OK;
  case POW:                               // operator '^' (or '**')
    errno = 0;
    val.top() = pow(val1,val2);
    if (errno == 0) return EVAL::OK;
    // no break
  default:
    return EVAL::ERROR_CALCULATION_ERROR;
  }
}

/***********************************************************************
 *                                                                     *
 * Name: engine                                      Date:    28.09.00 *
 * Author: Evgeni Chernyaev                          Revised:          *
 *                                                                     *
 * Function: Evaluates arithmetic expression.                          *
 *                                                                     *
 * Parameters:                                                         *
 *   begin  - pointer to the character string with expression.         *
 *   end    - pointer to the end of the character string (it is needed *
 *            for recursive call of engine(), when there is no '\0').  *
 *   result - result of the evaluation.                                *
 *   endp   - pointer to the character where the evaluation stoped.    *
 *   dictionary - dictionary of available variables and functions.     *
 *                                                                     *
 ***********************************************************************/
static int engine(pchar begin, pchar end, double & result,
                  pchar & endp, const dic_type & dictionary)
{
  static const int SyntaxTable[17][17] = {
    //E  (  || && == != >= >  <= <  +  -  *  /  ^  )  V - current token
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1 },   // E - previous
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1 },   // (   token
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // ||
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // &&
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // ==
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // !=
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // >=
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // >
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // <=
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // <
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // +
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // -
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // *
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // /
    { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },   // ^
    { 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0 },   // )
    { 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0 }    // V = {.,N,C}
  };
  static const int ActionTable[15][16] = {
    //E  (  || && == != >= >  <= <  +  -  *  /  ^  ) - current operator
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1 }, // E - top operator
    {-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 }, // (   in stack
    { 4, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4 }, // ||
    { 4, 1, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4 }, // &&
    { 4, 1, 4, 4, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4 }, // ==
    { 4, 1, 4, 4, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4 }, // !=
    { 4, 1, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1, 4 }, // >=
    { 4, 1, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1, 4 }, // >
    { 4, 1, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1, 4 }, // <=
    { 4, 1, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 1, 4 }, // <
    { 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1, 1, 1, 4 }, // +
    { 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1, 1, 1, 4 }, // -
    { 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1, 4 }, // *
    { 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1, 4 }, // /
    { 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }  // ^
  };

  std::stack<int>    op;                      // operator stack
  std::stack<pchar>  pos;                     // position stack
  std::stack<double> val;                     // value stack
  pchar         pointer = begin;
  int           iCur, iPrev = 0, iTop, EVAL_STATUS;
  char          c;

  op.push(0); pos.push(pointer);         // push EOL to the stack
  SKIP_BLANKS;
  if (c == '\0') { EVAL_EXIT( EVAL::WARNING_BLANK_STRING, begin ); }
  for(;;pointer++) {

    //   N E X T   T O K E N
    c = (pointer > end) ? '\0' : *pointer;
    if (isspace(c)) continue;            // skip space, tab etc.
    switch (c) {
    case '\0': iCur = ENDL; break;
    case '(':  iCur = LBRA; break;
    case '|':
      if (*(pointer+1) == '|') {
	    pointer++; iCur = OR; break;
      }else{
        EVAL_EXIT( EVAL::ERROR_UNEXPECTED_SYMBOL, pointer );
      }
    case '&':
      if (*(pointer+1) == '&') {
        pointer++; iCur = AND; break;
      }else{
        EVAL_EXIT( EVAL::ERROR_UNEXPECTED_SYMBOL, pointer );
      }
    case '=':
      if (*(pointer+1) == '=') {
        pointer++; iCur = EQ; break;
      }else{
        EVAL_EXIT( EVAL::ERROR_UNEXPECTED_SYMBOL, pointer );
      }
    case '!':
      if (*(pointer+1) == '=') {
        pointer++; iCur = NE; break;
      }else{
        EVAL_EXIT( EVAL::ERROR_UNEXPECTED_SYMBOL, pointer );
      }
    case '>':
      if (*(pointer+1) == '=') { pointer++; iCur = GE; } else { iCur = GT; }
      break;
    case '<':
      if (*(pointer+1) == '=') { pointer++; iCur = LE; } else { iCur = LT; }
      break;
    case '+':  iCur = PLUS;  break;
    case '-':  iCur = MINUS; break;
    case '*':
      if (*(pointer+1) == '*') { pointer++; iCur = POW; }else{ iCur = MULT; }
      break;
    case '/':  iCur = DIV;  break;
    case '^':  iCur = POW;  break;
    case ')':  iCur = RBRA; break;
    default:
      if (c == '.' || isalnum(c)) {
        iCur = VALUE; break;
      }else{
        EVAL_EXIT( EVAL::ERROR_UNEXPECTED_SYMBOL, pointer );
      }
    }

    //   S Y N T A X   A N A L I S Y S
    auto iWhat = SyntaxTable[iPrev][iCur];
    iPrev = iCur;
    switch (iWhat) {
    case 0:                             // systax error
      EVAL_EXIT( EVAL::ERROR_SYNTAX_ERROR, pointer );
    case 1: {                           // operand: number, variable, function
      double value;
      EVAL_STATUS = operand(pointer, end, value, pointer, dictionary);
      if (EVAL_STATUS != EVAL::OK) { EVAL_EXIT( EVAL_STATUS, pointer ); }
      val.push(value);
      continue; }
    case 2:                             // unary + or unary -
      val.push(0.0);
      break;
    case 3: default:                    // next operator
      break;
    }

    //   N E X T   O P E R A T O R
    for(;;) {
      if (op.size() == 0) { EVAL_EXIT( EVAL::ERROR_SYNTAX_ERROR, pointer ); }
      iTop = op.top();
      switch (ActionTable[iTop][iCur]) {
      case -1:                           // syntax error
        if (op.size() > 1) pointer = pos.top();
        EVAL_EXIT( EVAL::ERROR_UNPAIRED_PARENTHESIS, pointer );
      case 0:                            // last operation (assignment)
        if (val.size() == 1) {
          result = val.top();
          EVAL_EXIT( EVAL::OK, pointer );
        }else{
          EVAL_EXIT( EVAL::ERROR_SYNTAX_ERROR, pointer );
        }
      case 1:                           // push current operator in stack
        op.push(iCur); pos.push(pointer);
        break;
      case 2:                           // execute top operator
        EVAL_STATUS = maker(iTop, val); // put current operator in stack
        if (EVAL_STATUS != EVAL::OK) {
          EVAL_EXIT( EVAL_STATUS, pos.top() );
        }
        op.top() = iCur; pos.top() = pointer;
        break;
      case 3:                           // remove '(' from stack
        op.pop(); pos.pop();
        break;
      case 4: default:                  // execute top operator and
        EVAL_STATUS = maker(iTop, val); // remove it from stack
        if (EVAL_STATUS != EVAL::OK) {  // repeat with the same iCur
          EVAL_EXIT( EVAL_STATUS, pos.top() );
        }
        op.pop(); pos.pop();
        continue;
      }
      break;
    }
  }
}

//---------------------------------------------------------------------------
static void setItem(const char * prefix, const char * name,
		            Item&& item, XmlTools::Evaluator::Struct& s) {

  if ( !name || *name == '\0') {
    s.theStatus = EVAL::ERROR_NOT_A_NAME;
    return;
  }

  //   R E M O V E   L E A D I N G   A N D   T R A I L I N G   S P A C E S
  auto nam = remove_blanks(name);

  //   C H E C K   N A M E
  if (nam.empty() || std::any_of( nam.begin(), nam.end(),
                                  [](const char& i) { return i != '_' && !isalnum(i) ; } ) ) {
    s.theStatus = EVAL::ERROR_NOT_A_NAME;
    return;
  }

  //   A D D   I T E M   T O   T H E   D I C T I O N A R Y
  auto item_name = prefix + to_string(nam);
#if  __cplusplus > 201402L
  // C++17
  auto r = s.theDictionary.insert_or_assign(item_name, std::move(item));
  s.theStatus = ( r.second          ? EVAL::OK
                : item_name == name ? EVAL::WARNING_EXISTING_VARIABLE
                                    : EVAL::WARNING_EXISTING_FUNCTION );
#else
  auto iter = s.theDictionary.find(item_name);
  if (iter != s.theDictionary.end()) {
    iter->second = std::move(item);
    s.theStatus = ( item_name == name ? EVAL::WARNING_EXISTING_VARIABLE
                                      : EVAL::WARNING_EXISTING_FUNCTION );
  }else{
    s.theDictionary.insert( std::make_pair(item_name, std::move(item)) );
    s.theStatus = EVAL::OK;
  }
#endif
}

//---------------------------------------------------------------------------
namespace XmlTools {

//---------------------------------------------------------------------------
Evaluator::Evaluator()
: m_p{ std::make_unique<Struct>() } {
}

//---------------------------------------------------------------------------
Evaluator::~Evaluator() = default;

//---------------------------------------------------------------------------
double Evaluator::evaluate(const char * expression) {
  m_p->theExpression.reset();
  m_p->thePosition   = nullptr;
  m_p->theStatus     = WARNING_BLANK_STRING;
  m_p->theResult     = 0.0;
  if (expression) {
    m_p->theExpression = std::make_unique<char[]>(strlen(expression)+1);
    strcpy(m_p->theExpression.get(), expression);
    m_p->theStatus = engine(m_p->theExpression.get(),
			  m_p->theExpression.get()+strlen(expression)-1,
			  m_p->theResult,
			  m_p->thePosition,
			  m_p->theDictionary);
  }
  return m_p->theResult;
}

//---------------------------------------------------------------------------
int Evaluator::status() const {
  return m_p->theStatus;
}

//---------------------------------------------------------------------------
int Evaluator::error_position() const {
  return m_p->thePosition - m_p->theExpression.get();
}

//---------------------------------------------------------------------------
void Evaluator::print_error() const {
  static const char* prefix = "Evaluator : ";
  switch (m_p->theStatus) {
  case ERROR_NOT_A_NAME:
    std::cerr << prefix << "invalid name"         << std::endl;
    return;
  case ERROR_SYNTAX_ERROR:
    std::cerr << prefix << "systax error"         << std::endl;
    return;
  case ERROR_UNPAIRED_PARENTHESIS:
    std::cerr << prefix << "unpaired parenthesis" << std::endl;
    return;
  case ERROR_UNEXPECTED_SYMBOL:
    std::cerr << prefix << "unexpected symbol"    << std::endl;
    return;
  case ERROR_UNKNOWN_VARIABLE:
    std::cerr << prefix << "unknown variable"     << std::endl;
    return;
  case ERROR_UNKNOWN_FUNCTION:
    std::cerr << prefix << "unknown function"     << std::endl;
    return;
  case ERROR_EMPTY_PARAMETER:
    std::cerr << prefix << "empty parameter in function call"
		 << std::endl;
    return;
  case ERROR_CALCULATION_ERROR:
    std::cerr << prefix << "calculation error"    << std::endl;
    return;
  default:
    return;
  }
}

//---------------------------------------------------------------------------
void Evaluator::setVariable(const char * name, double value)
{ setItem("", name, value, *m_p); }

void Evaluator::setVariable(const char * name, const char * expression)
{ setItem("", name, std::string(expression), *m_p); }

//---------------------------------------------------------------------------
// TODO: could remove the prefix for functions now that they are stored
//       as variant, ie. the same info (# of args) is now present in the
//       state of the FuncPtr variant...
//       But that would change the semantics, as now the functions are in
//       a different 'namespace' which depends on the # of arguments...
//       so one could get clashes which were not there before.
//       In addition, checks for existing function vs. variable would have
//       to be changed, as those two are currently distinguished by the prefix.
//       But that could easily be changed to use Item::which() instead.
void Evaluator::setFunction(const char * name,
                            double (*fun)())
{ setItem("0", name, FuncPtr{fun}, *m_p); }

void Evaluator::setFunction(const char * name,
                            double (*fun)(double))
{ setItem("1", name, FuncPtr{fun}, *m_p); }

void Evaluator::setFunction(const char * name,
                            double (*fun)(double,double))
{ setItem("2", name, FuncPtr{fun}, *m_p); }

void Evaluator::setFunction(const char * name,
                            double (*fun)(double,double,double))
{ setItem("3", name, FuncPtr{fun}, *m_p); }

void Evaluator::setFunction(const char * name,
                            double (*fun)(double,double,double,double))
{ setItem("4", name, FuncPtr{fun}, *m_p); }

void Evaluator::setFunction(const char * name,
                            double (*fun)(double,double,double,double,double))
{ setItem("5", name, FuncPtr{fun}, *m_p); }

//---------------------------------------------------------------------------
bool Evaluator::findVariable(const char * name) const {
  if ( !name || *name == '\0') return false;
  auto nam = remove_blanks(name);
  return !nam.empty() && m_p->theDictionary.find(to_string(nam)) != m_p->theDictionary.end();
}

//---------------------------------------------------------------------------
bool Evaluator::findFunction(const char * name, int npar) const {
  if (npar < 0  || npar > MAX_N_PAR) return false;
  if ( !name || *name == '\0')    return false;
  auto nam = remove_blanks(name);
  return !nam.empty() && m_p->theDictionary.find(sss[npar]+nam) != m_p->theDictionary.end();
}

//---------------------------------------------------------------------------
void Evaluator::removeVariable(const char * name) {
  if ( !name || *name == '\0') return;
  auto nam = remove_blanks(name);
  if (!nam.empty()) m_p->theDictionary.erase(to_string(nam));
}

//---------------------------------------------------------------------------
void Evaluator::removeFunction(const char * name, int npar) {
  if (npar < 0  || npar > MAX_N_PAR) return;
  if ( !name || *name == '\0')    return;
  auto nam = remove_blanks(name);
  if (!nam.empty()) m_p->theDictionary.erase(sss[npar]+nam);
}

//---------------------------------------------------------------------------
void Evaluator::clear() {
  m_p->theDictionary.clear();
  m_p->theExpression.reset();
  m_p->thePosition   = nullptr;
  m_p->theStatus     = OK;
  m_p->theResult     = 0.0;
}

//---------------------------------------------------------------------------
} // namespace XmlTools
