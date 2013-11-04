
/*

    Copyright 2013 Darry Danzig

    This file is part of Fezela. http://www.fezela.net

    Fezela is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fezela is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fezela.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <string.h>
#include "str_utl.h"

using namespace std;

string StrUtl::normalize_str(string str) {

  string new_str = str;

  new_str = trim_spaces_beginning(new_str);
  new_str = trim_spaces_ending(new_str);
  new_str = squeeze_spaces(new_str);
  new_str = normalize_spaces(new_str);
  new_str = to_lower(new_str);

  return new_str;
}

string StrUtl::trim_spaces_beginning(string str) {

  string new_str;
  char *c_str = (char *) str.c_str();
  char *n_str;

  for (n_str = c_str; *n_str != '\0'; n_str++)
    if ( !isspace(*n_str) )
      break;

  new_str = n_str;
  return new_str;
}

string StrUtl::trim_spaces_ending(string str) {

  string new_str;
  char *c_str = (char *) str.c_str();
  char *new_c_str;

  new_c_str = (char *) malloc(strlen(c_str) + 1);

  if (new_c_str == NULL)
    cerr << "Error malloc..No memory!" << endl;

  strcpy(new_c_str, c_str);

  for (int i=strlen(new_c_str)-1; i >= 0; i--) {

    if ( !isspace(new_c_str[i]) )
      break;

    new_c_str[i] = '\0';
  } 

  new_str = new_c_str;
  free(new_c_str);

  return new_str;
}

string StrUtl::squeeze_spaces(string str) {

  string new_str;
  char *c_str = (char *) str.c_str();
  char *new_c_str;

  new_c_str = (char *) malloc(strlen(c_str) + 1);
  
  if ( new_c_str == NULL )
    cerr << "Error malloc. No memory!" << endl;

  strcpy(new_c_str, c_str);

  // Do the squeeze.
  char *i;

  i = new_c_str;

  while ( *i != '\0' ) {

    if ( isspace(*i) ) {
      // Is the character after i a space too?
      while ( isspace(*(i+1))) {

	// Copy everthing over one to the left.
	for (char *j= i + 1; *j != '\0'; j++) 
	  *j = *(j+1);
      }	
    }

    i++;
  }

  new_str = new_c_str;
  free(new_c_str);

  return new_str;
}

string StrUtl::to_lower(string str) {

  string new_str;
  char *c_str = (char *) str.c_str();
  char *new_c_str;

  new_c_str = (char *) malloc(strlen(c_str) + 1);

  // Do the lower case.
  
  for (int i=0; c_str[i] != '\0'; i++) 
    new_c_str[i] = tolower(c_str[i]);

  new_str = new_c_str;
  free(new_c_str);

  return new_str;
}

string StrUtl::normalize_spaces(string str) {

  string new_str;
  char *c_str = (char *) str.c_str();
  char *new_c_str;

  new_c_str = (char *) malloc (strlen(c_str) + 1);
  
  if ( new_c_str == NULL ) 
    cerr << "Error malloc. No memory." << endl;

  strcpy(new_c_str, c_str);

  for (int i=0; new_c_str[i] != '\0'; i++)
    if ( isspace(new_c_str[i] ) )
      new_c_str[i] = STR_UTL_WHITE_SPACE;

  new_str = new_c_str;
  free(new_c_str);

  return new_str;
}
