%module character

%{

#include "types.h"
#include "character.h"
#include "storage.h"

%}

#define _DLGEDIT_

class storage
{
public:
    void set (const char*, int);
    int get (const char*);
};

#define STAND_NORTH 0
#define STAND_SOUTH 1
#define STAND_WEST 2
#define STAND_EAST 3
#define WALK_NORTH 4
#define WALK_SOUTH 5
#define WALK_WEST 6
#define WALK_EAST 7
#define NBR_MOVES 8
#define NO_MOVE 65535

%include "../../types.h"
%include "../../character_base.h"
%include "../../character.h"
