#ifndef WORKSPACE_H
#define WORKSPACE_H
#include "vectors.h"
#include "Point.h"
#include "Shape.h"
#include "Transformation.h"
#include "Camera.h"
#include "Printer.h"
#include "Animation.h"
#include <vector>
#include <unordered_map>
#include <fstream>
class Parser;
class Workspace {
	public:
		Workspace();

		void changeDirectory(const char* const directory);
		void promptForDirectory();

		int getMode() const;
		void setMode(const int mode);
		static const int MINIMUM_MODE = 0;
		static const int MODE_POINTS = 0;
		static const int MODE_SHAPES = 1;
		static const int MODE_TRANSFORMATIONS = 2;
		static const int MAXIMUM_MODE = 2;

		const std::vector<Point*>* getPoints() const;
		const std::vector<Shape*>* getShapes() const;
		const std::vector<Transformation*>* getTransformations() const;
		Camera* getCamera();

		void findPoint(const std::string& identifier, Point** const point) const;
		void findShape(const std::string& identifier, Shape** const shape) const;
		void findTransformation(const std::string& identifier, Transformation** const transformation) const;

		int nextPointSequenceNumber();
		int nextShapeSequenceNumber();
		int nextTransformationSequenceNumber();

		void selectPrevious();
		void selectNext();
		void selectNextPrinter();

		void moveSelectedObjectUp(const float amount);
		void moveSelectedObjectRight(const float amount);
		void sizeSelectedObjectUp(const float amount);
		void sizeSelectedObjectRight(const float amount);
		void showOrHideSelectedObject();
		void transformSelectedShape(Transformation* const transformation);

		void newPoint(Point* const point);
		void newShape(Shape* const shape);
		void newTransformation(Transformation* const transformation);
		void newFrame(Frame* const frame);

		void applyToSelected(const int function, const float argument);
		void applyToPointFromIdentifiers(const int function, const std::string& receiverIdentifier, const std::string& elementIdentifier);
		void applyToShapeFromIdentifiers(const int function, const std::string& receiverIdentifier, const std::string& elementIdentifier);
		bool applyToLastFrame(const int function, const float argument);
		bool processInterpolationFunction(const int type, const std::string& identifier, const int selector, const float value);

		void prompt(const int function, const std::string& prompt);
		void promptFrameTime(const int function, const std::string& prompt);
		void prompt(const int function, const std::string& prompt1, const std::string& prompt2);
		void promptFrameFunction(const int type, const std::string& prompt1, const std::string& prompt2, const std::string& prompt3);

		void render() const;

		void buildAnimation();
		void playAnimation();
		void pauseAnimation();
		void stopAnimation();
		void tickAnimation();

		void compile() const;
		void backup() const;
		void save() const;

		void print() const;
		void printSelected() const;

		void reset();

		~Workspace();
	private:
		template <typename T, typename U> void findObject(const std::string& identifier, U* const byIdentifier, T** const output) const;

		void selectPrevious(int* const index, const size_t size);
		void selectNext(int* const index, const size_t size);

		friend class Parser;
		Parser* const parser;
		std::string directory;
		std::vector<Point*> points;
		std::vector<Shape*> shapes;
		std::vector<Transformation*> transformations;
		Camera* const camera;

		std::unordered_map<std::string, Point*> pointsByIdentifier;
		std::unordered_map<std::string, Shape*> shapesByIdentifier;
		std::unordered_map<std::string, Transformation*> transformationsByIdentifier;

		template <typename T, typename U> void fixIdentifier(T* const toFix, U* const byIdentifier, int (Workspace::*const nextSequenceNumber)());

		void newPointFromParser(Point* const point);
		void newShapeFromParser(Shape* const shape);
		void newTransformationFromParser(Transformation* const transformation);
		void newFrameFromParser(Frame* const frame);

		void saveToFile(const std::string& filename, const Animation* const animation) const;
		template <typename T> void saveToFile(const std::string& filename, T* const objects) const;

		void printOverview() const;
		template <typename T> void printAllOf(T* const objects) const;
		template <typename T> void printAllOf(T* const objects, Printer* const printer) const;

		int mode;
		int pointsIndex;
		int shapesIndex;
		int transformationsIndex;
		int pointSequenceNumber;
		int shapeSequenceNumber;
		int transformationSequenceNumber;
		static const size_t PRINTERS_SIZE = 2;
		Printer* const printers[Workspace::PRINTERS_SIZE];
		int printerIndex;
		static const int WORKSPACE_PRINTER = 0;
		static const int COMPILE_PRINTER = 1;
		Animation animation;
		bool isPlayingAnimation;
		float animationTime;
};
#endif // WORKSPACE_H
