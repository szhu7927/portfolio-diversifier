#ifdef _WIN32
 #include "..\public\risktopointalgo.h"
 #include "..\public\ymath.h"
#else
 #include "../public/risktopointalgo.h"
 #include "../public/ymath.h"
#endif

//Uses the Shoelace Theorem to calculate the area of a convex polygon
//Points must be put in clockwise order;
double area(std::vector<std::pair<double, double> > vec) {
	double sum1 = 0;
	double sum2 = 0;

	for (int i = 0; i < vec.size(); ++i) {
		sum1 += vec[i].first * vec[(i + 1) % vec.size()].second;
		sum2 += vec[(i + 1) % vec.size()].first * vec[i].second;
	}

	return std::abs(sum1 - sum2) / 2.0;
}

Line::Line(double slope, double intercept) {
	this->slope = slope;
	this->intercept = intercept;
}

double gety(Line line, double x) {
	return line.slope * x + line.intercept;
}

//Find equation of line from leftmost point to rightmost point
Line getline(std::vector<std::pair<double, double> > frontier) {
	std::pair<double, double> leftpoint = frontier[0];
	std::pair<double, double> rightpoint = frontier[frontier.size() - 1];

	double slope = (leftpoint.second - rightpoint.second) / (leftpoint.first - rightpoint.first);
	double intercept = leftpoint.second - leftpoint.first * slope;

	return Line(slope, intercept);
}

//Get a single series of areas
double intervalarea(std::vector<std::pair<double, double> > frontier, Line line, int start, int end) {
	//Sort points from left to right
	std::sort(frontier.begin(), frontier.end());

	double intervalarea = 0;

	//Add up areas
	for (int i = start; i < end; ++i) {
		//First area, which is a triangle
		if (i == 0) {
			//Vector containing points in the polygon
			std::vector<std::pair<double, double> > polypoints;

			std::pair<double, double> left = frontier[0];
			polypoints.push_back(left);

			std::pair<double, double> topright;
			std::pair<double, double> bottomright;

			//If the frontier point is above the line (concave)
			if (gety(line, frontier[1].first) < frontier[1].second) {
				topright = frontier[1];
				bottomright = std::make_pair(frontier[1].first, gety(line, frontier[1].first));
			}
			//If the frontier point is below the line (convex)
			else {
				topright = std::make_pair(frontier[1].first, gety(line, frontier[1].first));
				bottomright = frontier[1];
			}
			polypoints.push_back(topright);
			polypoints.push_back(bottomright);

			intervalarea += area(polypoints);
		}
		//Last area, which is a triangle (all others are quadrilaterals)
		else if (i == frontier.size() - 2) {
			std::vector<std::pair<double, double> > polypoints;

			std::pair<double, double> right = frontier[frontier.size() - 1];
			polypoints.push_back(right);

			std::pair<double, double> topleft;
			std::pair<double, double> bottomleft;

			//If the frontier point is above the line (concave)
			if (gety(line, frontier[frontier.size() - 2].first) < frontier[frontier.size() - 2].second) {
				topleft = frontier[frontier.size() - 2];
				bottomleft = std::make_pair(frontier[frontier.size() - 2].first, gety(line, frontier[frontier.size() - 2].first));
			}
			//If the frontier point is below the line (convex)
			else {
				topleft = std::make_pair(frontier[frontier.size() - 2].first, gety(line, frontier[frontier.size() - 2].first));
				bottomleft = frontier[frontier.size() - 2];
			}
			polypoints.push_back(bottomleft);
			polypoints.push_back(topleft);

			intervalarea += area(polypoints);
		}
		//General case
		else {
			//Tracks if frontier points intersect line
			int flipped = 0;

			std::vector<std::pair<double, double> > polypoints;

			std::pair<double, double> topleft;
			std::pair<double, double> bottomleft;

			//If the left frontier point is above the line (concave)
			if (gety(line, frontier[i].first) < frontier[i].second) {
				topleft = frontier[i];
				bottomleft = std::make_pair(frontier[i].first, gety(line, frontier[i].first));
			}
			//If the left frontier point is below the line (convex)
			else {
				topleft = std::make_pair(frontier[i].first, gety(line, frontier[i].first));
				bottomleft = frontier[i];
				flipped++;
			}
			polypoints.push_back(bottomleft);
			polypoints.push_back(topleft);

			std::pair<double, double> topright;
			std::pair<double, double> bottomright;

			//If the right frontier point is above the line (concave)
			if (gety(line, frontier[i + 1].first) < frontier[i + 1].second) {
				topright = frontier[i + 1];
				bottomright = std::make_pair(frontier[i + 1].first, gety(line, frontier[i + 1].first));
			}
			//If the right frontier point is below the line (convex)
			else {
				topright = std::make_pair(frontier[i + 1].first, gety(line, frontier[i + 1].first));
				bottomright = frontier[i + 1];
				flipped++;
			}
			polypoints.push_back(topright);
			polypoints.push_back(bottomright);

			//Divide area by 2 if frontier intersects line
			if (flipped == 1) intervalarea += area(polypoints) / 2;
			else intervalarea += area(polypoints);
		}
	}

	return intervalarea;
}

//Adding areas up to a certain point
double cumarea(std::vector<std::pair<double, double> > frontier, Line line, int point) {
	return intervalarea(frontier, line, 0, point);
}

//Total area under the curve by adding up individual areas (used like an integral in this case)
double totalarea(std::vector<std::pair<double, double> > frontier) {
	//Another sort because the line depends on it
	std::sort(frontier.begin(), frontier.end());
	Line line = getline(frontier);	
	
	return cumarea(frontier, line, frontier.size() - 1);
}

//Returns a vector with the points and a corresponding cumulative area
std::vector<std::pair<std::pair<double, double>, double> > cumareavector(std::vector<std::pair<double, double>> frontier) {
	//Another sort because the line depends on it
	std::sort(frontier.begin(), frontier.end());
	
	double totarea = totalarea(frontier);
	Line line = getline(frontier);

	std::vector<std::pair<std::pair<double, double>, double> > areadata;
	double cumarearatio = 0;

	for (int i = 0; i < frontier.size(); ++i) {
		std::pair<std::pair<double, double>, double> area = std::make_pair(frontier[i], cumarearatio);
		areadata.push_back(area);
		
		if (i != frontier.size() - 1) cumarearatio += intervalarea(frontier, line, i, i + 1) / totarea;
	}

	return areadata;
}

std::pair<double, double> bestpoint(std::vector<std::pair<std::pair<double, double>, double> > areadata, double risk) {
	//Binary search algorithm
	int left = 0;
	int right = areadata.size() - 1;
	int mid;

	std::pair<double, double> nearest;
	bool nearestfound = false;

	while (!nearestfound) {
		mid = (left + right - 1) / 2;

		if (areadata[mid].second <= risk && risk <= areadata[mid + 1].second) nearestfound = true;
		else if (risk > areadata[mid].second) left = mid + 1;
		else right = mid - 1;
	}

	//Find out which surrounding point is closer, to the left or right
	if (risk - areadata[mid].second < areadata[mid + 1].second - risk) return areadata[mid].first;
	else return areadata[mid + 1].first;
}

void printareas(std::vector<std::pair<std::pair<double, double>, double> > areadata) {
	std::cout << "Cumulative Proportion of Area by Point: \n";

	for (auto area : areadata) {
		std::cout << "Point: (" << area.first.first << ", " << area.first.second << "); Cumulative Area: " << area.second << "\n";
	}
}
