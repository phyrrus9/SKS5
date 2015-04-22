#include "engine.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

vector<script_var<int> *> script_ivars;
vector<script_var<float> *> script_fvars;

void scriptError(const char *msg, ...)
{
	char *buf = new char[50];
	va_list args;
	va_start (args, msg);
	vsprintf (buf,msg, args);
	perror (buf);
	va_end (args);
     writeDebug("[Script error] %s", buf);
}

void parseScriptSet(char *str)
{
	char * pch = strtok(NULL, " ");
	if (pch == NULL)
		scriptError("No variable given in set");
	else
	{
		if (strcmp(pch, "view") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_view) == 0)
				scriptError("No view given (0-3)");
		}
		else if (strcmp(pch, "xpos") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_xpos) == 0)
				scriptError("No position given");
		}
		else if (strcmp(pch, "ypos") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_ypos) == 0)
				scriptError("No position given");
		}
		else if (strcmp(pch, "zpos") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_zpos) == 0)
				scriptError("No position given");
		}
		else if (strcmp(pch, "keys") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_keys) == 0)
				scriptError("No amount given");
		}
		else if (strcmp(pch, "coins") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_coins) == 0)
				scriptError("No amount given");
		}
		else if (strcmp(pch, "walls") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_walls) == 0)
				scriptError("No amount given");
		}
		else if (strcmp(pch, "level") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_lvl) == 0)
				scriptError("No level number given");
		}
		else if (strcmp(pch, "zkills") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_zkills) == 0)
				scriptError("No amount given");
		}
		else if (strcmp(pch, "health") == 0)
		{
			pch = strtok(NULL, " ");
			if (sscanf(pch, "%d", &pl_health) == 0)
				scriptError("No value given");
		}
		else
			scriptError("Unknown variable in set");
	}
}

void parseScriptNewInt(char *str)
{
	char *pch = strtok(NULL, " ");
	if (pch == NULL)
		scriptError("No name defined for variable");
	else
	{
		script_var<int> *var = new script_var<int>;
		var->name = strdup(pch);
		var->value = 0;
		script_ivars.push_back(var);
	}
}

void parseScriptNewFloat(char *str)
{
	char *pch = strtok(NULL, " ");
	if (pch == NULL)
		scriptError("No name defined for variable");
	else
	{
		script_var<float> *var = new script_var<float>;
		var->name = strdup(pch);
		var->value = 0.0f;
		script_fvars.push_back(var);
	}
}

void parseScriptLine(char *str)
{
	char *line = strdup(str);
	char *pch = strtok(str, " ");
	if (pch == NULL)
		scriptError("No command given in script");
	else
	{
		if (strcmp(pch, "set") == 0)
			parseScriptSet(line);
		else if (strcmp(pch, "int") == 0)
			parseScriptNewInt(line);
		else if (strcmp(pch, "float") == 0)
			parseScriptNewFloat(line);
		else
			scriptError("Unknown command: %s", str);
	}
	free(line);
}
