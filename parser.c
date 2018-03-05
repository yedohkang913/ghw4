#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions in math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to convert those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file (char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
	
	color c;
	c.red = 255;
	c.green = 153;
	c.blue = 255;

  if (strcmp(filename, "stdin") == 0) {
  	f = stdin;
  }
  else {
  	f = fopen(filename, "r");
  }
  
  while (fgets(line, 255, f) != NULL) {
		
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
		
		if(!(strcmp(line, "line"))) {
			fgets(line, 255, f);
			double x0, y0, z0, x1, y1, z1;
			sscanf(line, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);
			add_edge(edges, x0, y0, z0, x1, y1, z1);
		}
		
		if(!(strcmp(line, "ident"))) {
			ident(transform);
		}
		
		if(!(strcmp(line, "scale"))) {
			fgets(line, 255, f);
			double x, y, z;
			sscanf(line, "%lf %lf %lf", &x, &y, &z);
			struct matrix * scale;
			scale = make_scale(x,y,z);
			matrix_mult(scale, transform);
		}
		
		if(!(strcmp(line, "move"))) {
			fgets(line, 255, f);
			double x,y,z;
			sscanf(line, "%lf %lf %lf", &x, &y, &z);
			struct matrix * move;
			move = make_translate(x,y,z);
			matrix_mult(move, transform);
		}
		
		if(!strcmp(line, "rotate")) {
			fgets(line, 255, f);
			double theta;
			char axis;
			sscanf(line, "%c %lf", &axis, &theta);
			struct matrix * rotation;
			if (axis == 'x') {
				rotation = make_rotX(theta);
				matrix_mult(rotation, transform);
			}
			if (axis == 'y') {
				rotation = make_rotY(theta);
				matrix_mult(rotation, transform);
			}
			if (axis == 'z') {
				rotation = make_rotZ(theta);
				matrix_mult(rotation, transform);
			}
		}
		
		if(!(strcmp(line, "apply"))) {
			matrix_mult(transform, edges);
		}
		
		if(!(strcmp(line, "display"))) {
			clear_screen(s);
			draw_lines(edges, s, c);
			display(s);
		}
		
		if(!(strcmp(line, "save"))) {
			clear_screen(s);
			draw_lines(edges, s, c);
			fgets(line, 255, f);
			char * file = malloc(255);
			sscanf(line, "%s", file);
			save_extension(s, file);
		}
		
		if(!(strcmp(line, "quit"))) {
			exit(1);
		}
  }
	
}
  
