#include "blog.h"

#include <tidy.h>

char           *
tidy_html(char *input)
{

	uint		len = 0;
	char           *output;
	TidyDoc		tdoc = tidyCreate();

	tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
	//--output - xhtml y | -asxhtml
		tidyOptSetBool(tdoc, TidyForceOutput, yes);
	//--force - output yes
		tidyOptSetInt(tdoc, TidyIndentContent, TidyAutoState);
	//--indent "auto"
		tidyOptSetBool(tdoc, TidyXmlDecl, yes);
	//--add - xml - decl yes
		tidyOptSetBool(tdoc, TidyQuiet, yes);
	//-q
		tidyOptSetBool(tdoc, TidyNumEntities, yes);
	//-n
		tidySetInCharEncoding(tdoc, "utf8");
	tidySetOutCharEncoding(tdoc, "utf8");

	tidyParseString(tdoc, input);
	tidyCleanAndRepair(tdoc);

	tidySaveString(tdoc, NULL, &len);
	output = malloc(len + 1);

	if (!output)
		return NULL;

	tidySaveString(tdoc, output, &len);
	//dbprintf("TIDY: Cleaned up %d bytes.\n", len);
	output[len] = '\0';
	//Make sure string is terminated.

		return output;
}
