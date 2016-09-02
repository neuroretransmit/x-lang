#include "regex_utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

/// The following is the size of a buffer to contain any error messages
/// encountered when the regular expression is compiled. */
#define MAX_ERROR_MSG 0x1000

/**
  * @brief  Compile the regular expression described by "regex_text" into
  * "r".
  *
  * @return success
  *
  * @param r where to save compiled regex
  * @param regex_text regular expression
  */
int compile_regex(regex_t* r, const char* regex_text)
{
	int status = regcomp(r, regex_text, REG_EXTENDED | REG_NEWLINE);

	if (status != 0) {
		char error_message[MAX_ERROR_MSG];
		regerror(status, r, error_message, MAX_ERROR_MSG);
		printf("Regex error compiling '%s': %s\n",
			   regex_text, error_message);
		return 1;
	}

	return 0;
}

/**
  * @brief Match the string in "to_match" against regex in "r".
  *
  * @return match
  *
  * @param r Compiled regex expression
  * @param to_match String to match against
  */
bool match_regex(regex_t* r, const char* to_match)
{
	regmatch_t pm;

	bool match = regexec(r, to_match, 1, &pm, REG_EXTENDED) ? false : true;

	return match;
}
