#include "types.h"
#include <vector>
#include <string.h>
#include <stdio.h>

class map;
class mapcharacter;

class instruction
{
   u_int16 type;
   u_int16 param_val[9];
   char string_val[30];
   u_int8 otherevent_val;

  public:
  instruction();
  instruction(const int &, char *);
  virtual void exec(mapcharacter*, map*, u_int16, u_int16)=0;
  u_int16 param(int);
  u_int16 otherevent();
  char* string();
  void get(FILE*);

  friend istream& operator>> (istream&, instruction&);
  friend ostream& operator<< (ostream&, instruction&);
};

