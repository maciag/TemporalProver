import prefix;
import lexical;

import std.stdio;

void main()
{
	writeln("Podaj formułę:");
	char[] infix = readln()[0..$-1].dup;
	
	if(!validate(infix))
	{
		writeln("Formuła nie jest poprawna.");
		return;
	}
	
	writeln("Zapis prefiksowy:");
	writeln(toPrefix(infix));
}