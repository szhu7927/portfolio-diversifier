#include "..\public\risktopointalgo.h"
#include "..\public\ymath.h"

//Uses the Shoelace Theorem to calculate the area of a convex polygon
//Points must be put in clockwise order;
double area(std::vector<std::pair<double, double>> vec) {
	double sum1 = 0;
	double sum2 = 0;

	for (int i = 0; i < vec.size(); ++i) {
		sum1 += vec[i].first * vec[(i + 1) % vec.size()].second;
		sum2 += vec[(i + 1) % vec.size()].first * vec[i].second;
	}

	return (sum1 - sum2) / 2;
}

Line::Line(double slope, double intercept) {
	this->slope = slope;
	this->intercept = intercept;
}

double gety(Line line, double x) {
	return line.slope * x + line.intercept;
}

//Total area under the curve by adding up individual areas (used like an integral in this case)
double totalarea(std::vector<std::pair<double, double>> frontier) {
	//Sort points from left to right
	std::sort(frontier.begin(), frontier.end());

	//Find equation of line from leftmost point to rightmost point
	std::pair<double, double> leftpoint = frontier[0];
	std::pair<double, double> rightpoint = frontier[frontier.size() - 1];
	
	double slope = (leftpoint.second - rightpoint.second) / (leftpoint.first - rightpoint.first);
	double intercept = leftpoint.second - leftpoint.first * slope;

	Line line = Line(slope, intercept);

	double totalarea = 0;

	//Add up areas
	for (int i = 0; i < frontier.size() - 1; ++i) {
		//First area, which is a triangle
		if (i == 0) {
			//Vector containing points in the polygon
			std::vector<std::pair<double, double>> polypoints;

			std::pair<double, double> left = frontier[0];
			polypoints.push_back(left);

			std::pair<double, double> topright;
			std::pair<double, double> bottomright;

			//If the frontier point is above the line (concave)
			if (gety(line, frontier[1].first) < frontier[1].second) {
				std::pair<double, double> topright = frontier[1];
				std::pair<double, double> bottomright = std::make_pair(frontier[1].first, gety(line, frontier[1].first));
			}
			//If the frontier point is below the line (convex)
			else {
				std::pair<double, double> topright = std::make_pair(frontier[1].first, gety(line, frontier[1].first));
				std::pair<double, double> bottomright = frontier[1];
			}
			polypoints.push_back(topright);
			polypoints.push_back(bottomright);

			totalarea += area(polypoints);
		}
		//Last area, which is a triangle (all others are quadrilaterals)
		else if (i == frontier.size() - 2) {
			std::vector<std::pair<double, double>> polypoints;

			std::pair<double, double> right = frontier[frontier.size() - 1];
			polypoints.push_back(right);

			std::pair<double, double> topleft;
			std::pair<double, double> bottomleft;

			//If the frontier point is above the line (concave)
			if (gety(line, frontier[frontier.size() - 2].first) < frontier[frontier.size() - 2].second) {
				std::pair<double, double> topleft = frontier[frontier.size() - 2];
				std::pair<double, double> bottomleft = std::make_pair(frontier[frontier.size() - 2].first, gety(line, frontier[frontier.size() - 2].first));
			}
			//If the frontier point is below the line (convex)
			else {
				std::pair<double, double> topleft = std::make_pair(frontier[frontier.size() - 2].first, gety(line, frontier[frontier.size() - 2].first));
				std::pair<double, double> bottomleft = frontier[frontier.size() - 2];
			}
			polypoints.push_back(bottomleft);
			polypoints.push_back(topleft);

			totalarea += area(polypoints);
		}
		//General case
		else {
			//Tracks if frontier points intersect line
			int flipped = 0;

			std::vector<std::pair<double, double>> polypoints;

			std::pair<double, double> topleft;
			std::pair<double, double> bottomleft;

			//If the left frontier point is above the line (concave)
			if (gety(line, frontier[i].first) < frontier[i].second) {
				std::pair<double, double> topleft = frontier[i];
				std::pair<double, double> bottomleft = std::make_pair(frontier[i].first, gety(line, frontier[i].first));
			}
			//If the left frontier point is below the line (convex)
			else {
				std::pair<double, double> topleft = std::make_pair(frontier[i].first, gety(line, frontier[i].first));
				std::pair<double, double> bottomleft = frontier[i];
				flipped++;
			}
			polypoints.push_back(bottomleft);
			polypoints.push_back(topleft);

			std::pair<double, double> topright;
			std::pair<double, double> bottomright;

			//If the right frontier point is above the line (concave)
			if (gety(line, frontier[i + 1].first) < frontier[i + 1].second) {
				std::pair<double, double> topright = frontier[i + 1];
				std::pair<double, double> bottomright = std::make_pair(frontier[i + 1].first, gety(line, frontier[i + 1].first));
			}
			//If the right frontier point is below the line (convex)
			else {
				std::pair<double, double> topright = std::make_pair(frontier[i + 1].first, gety(line, frontier[i + 1].first));
				std::pair<double, double> bottomright = frontier[i + 1];
				flipped++;
			}
			polypoints.push_back(topright);
			polypoints.push_back(bottomright);

			//Divide area by 2 if frontier intersects line
			if (flipped == 1) totalarea += area(polypoints) / 2;
			else totalarea += area(polypoints);
		}
	}

	return totalarea;
}