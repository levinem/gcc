// { dg-do assemble  }

char *a="a?";

class A
{
public:
	A()
	{
		char *b="a?";
	}
};

char *c="a?";
