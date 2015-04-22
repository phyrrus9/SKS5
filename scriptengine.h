#ifndef SCRIPTENGINE_H_INCLUDED
#define SCRIPTENGINE_H_INCLUDED

#include <vector>
#include "engine.h"

using namespace std;

template <class T>
struct script_var
{
	char *name;
	T value;
};

extern vector<script_var<int> *> script_ivars;
extern vector<script_var<float> *> script_fvars;

void scriptError(char *msg);
void parseScriptLine(char *str);

#endif // SCRIPTENGINE_H_INCLUDED
