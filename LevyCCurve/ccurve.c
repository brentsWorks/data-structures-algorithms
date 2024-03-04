// 3 October 2023
// C Program to recursively draw Levy C Curves.

//
// References:
//
// https://en.wikipedia.org/wiki/L%C3%A9vy_C_curve
//
// https://sites.math.washington.edu/~morrow/336_15/papers/kelsey.pdf
// Department of Mathematics 
// University of Washington 
// Accessed 6 Oct. 2023. 

#include <string.h>
#include <cairo.h>
#include <stdio.h>

// define function to draw Levy C Curve using recursion
void drawLevyCurve(cairo_t *cr, double x1, double y1, double x2, double y2, int depth) {
	
	// Find midpoint of Levy C Curve (for X and Y coordinates)
	double midX = x1 + ((y2 - y1) + (x2 - x1)) /2; 
	double midY = y1 + ((y2 - y1) - (x2 - x1)) / 2;

	// Base case 
	if (depth < 1) {
		cairo_move_to(cr, x1, y1);
		cairo_line_to(cr, midX, midY);
		cairo_line_to(cr, x2, y2);
		cairo_stroke(cr);
		return;
	}

	// Recursive calls to draw both halves of the curve
	drawLevyCurve(cr, x1, y1, midX, midY, depth - 1);
	drawLevyCurve(cr, midX, midY, x2, y2, depth - 1);
}


int
main (int argc, char *argv[])
{
     
	// process command line input per specification
	char outputFile[128], eof = ".png";
	int width, height, rec_depth;
	double start_x, start_y, end_x, end_y;

	scanf("%s %d %d %d %lf %lf %lf %lf", &outputFile, &width, &height, &rec_depth,
			&start_x, &start_y, &end_x, &end_y); 

	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr              = cairo_create (surface);

	
	// set background to white and move point to origin
	cairo_set_source_rgb (cr, 1, 1, 1);		
	cairo_move_to (cr, 0, 0);				
	cairo_paint(cr);						

	cairo_set_source_rgb(cr, 1, 0, 0);				
	cairo_set_line_width(cr, 0.5);
	
	// Call draw function to recursively draw Levy C Curve
	drawLevyCurve(cr, start_x, start_y, end_x, end_y, rec_depth);

	// send output to png file
		strcat(outputFile, ".png");
        cairo_destroy (cr);
        cairo_surface_write_to_png (surface, outputFile);
        cairo_surface_destroy (surface);
        return 0;
}