#ifndef SHAPEFINDER_H
#define SHAPEFINDER_H

#include <math.h>
#include <vector>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "cvutilities.h"
#include "foundshape.h"

typedef cv::Mat Mat;

class shapeFinder
{
public:
	shapeFinder();
	shapeFinder(Mat image);
	void setImage(Mat image);
	std::vector<foundShape> findRegPolygons(int numSides, int minApothem, int maxApothem, int maxShapes = 10, int voteThreshold = 0, int apothemStep = 2, int baseRotation = 0, int angleTolerance = 5);
	std::vector<foundShape> findRectangles(cv::Size aspectRatio, int minWidth, int maxWidth, int maxShapes = 10, int voteThreshold = 0, int widthStep = 2, int angleTolerance = 5);
	bool getSeenBefore();
	void setSeenBefore(bool seenBefore);

private:
	/**
	 * Vote for regular polygon centers.
	 *
	 * Create a vote image for a regular polygon with the given charateristics.
	 *
	 * @param numSides the polygon's number of sides.
	 * @param apothem the length of the shortest line from the center to a side. Though similar, it must not be confused with radius. See http://en.wikipedia.org/wiki/Apothem.
	 * @param rotation the rotation of the polygon, measured in degrees.
	 * @param angleTolerance the number of degrees the gradients' directions may fluctuate.
	 *
	 * @return a vote image with each pixel containing the number of votes it has.
	 */
	Mat voteRegPoly(int numSides, int apothem, int rotation, int angleTolerance);
	Mat voteRectangle(cv::Size aspectRatio, int width, int angleTolerance);
	void findGradients(Mat magnitudes, Mat xSobel, Mat ySobel, float* thresholdValue);
	void angleRanges(std::vector<cv::Range>* ranges, int numSides, int rotation, int angleTolerance);
	void findShapes(std::vector<foundShape>* bestShapes, std::vector<Mat> votes, std::vector<int> sizes, int maxShapes = 10, int voteThreshold = 0);
	void vote(Mat votes, cv::Vec2f unitGradient, cv::Point basePoint, int w, int distance);
	bool seenBefore(foundShape shape, int allowedShift = 5);

	Mat image;
	float currentThreshold;
	int callCounter;
	std::vector<foundShape> previousFindings;
	bool seenBeforeCheck;
};

#endif // SHAPEFINDER_H
