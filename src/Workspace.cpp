#include "Workspace.h"
#include "WorkspacePrinter.h"
#include "CompilePrinter.h"

Workspace::Workspace() :
	directory(""),
	points(std::vector<Point*>()),
	shapes(std::vector<Shape*>()),
	transformations(std::vector<Transformation*>()),
	camera(new Camera()),
	pointsByIdentifier(std::unordered_map<std::string, Point*>()),
	shapesByIdentifier(std::unordered_map<std::string, Shape*>()),
	transformationsByIdentifier(std::unordered_map<std::string, Transformation*>()),
	mode(0),
	pointsIndex(0),
	shapesIndex(0),
	transformationsIndex(0),
	pointSequenceNumber(0),
	shapeSequenceNumber(0),
	transformationSequenceNumber(0),
	printers{new WorkspacePrinter(new IndentedStream(std::cout)), new CompilePrinter(new IndentedStream(std::cout))},
	printerIndex(0),
	animation(),
	isPlayingAnimation(false),
	animationTime(0.0f)
{}

int Workspace::getMode() const{
	return this->mode;
}

void Workspace::setMode(const int mode){
	if(mode < Workspace::MINIMUM_MODE){
		this->mode = Workspace::MINIMUM_MODE;
	}else if(mode > Workspace::MAXIMUM_MODE){
		this->mode = Workspace::MAXIMUM_MODE;
	}else{
		this->mode = mode;
	}
}

const std::vector<Point*>* Workspace::getPoints() const{
	return &this->points;
}

const std::vector<Shape*>* Workspace::getShapes() const{
	return &this->shapes;
}

const std::vector<Transformation*>* Workspace::getTransformations() const{
	return &this->transformations;
}

Camera* Workspace::getCamera(){
	return this->camera;
}

template <typename T, typename U>
void Workspace::findObject(const std::string& identifier, U* const byIdentifier, T** const output) const{
	if(output){
		*output = 0;
		typename U::const_iterator iterator = byIdentifier->find(identifier);
		if(iterator != byIdentifier->end()){
			*output = iterator->second;
		}
	}
}

void Workspace::findPoint(const std::string& identifier, Point** const point) const{
	this->findObject(identifier, &this->pointsByIdentifier, point);
}

void Workspace::findShape(const std::string& identifier, Shape** const shape) const{
	this->findObject(identifier, &this->shapesByIdentifier, shape);
}

void Workspace::findTransformation(const std::string& identifier, Transformation** const transformation) const{
	this->findObject(identifier, &this->transformationsByIdentifier, transformation);
}

int Workspace::nextPointSequenceNumber(){
	return ++this->pointSequenceNumber;
}

int Workspace::nextShapeSequenceNumber(){
	return ++this->shapeSequenceNumber;
}

int Workspace::nextTransformationSequenceNumber(){
	return ++this->transformationSequenceNumber;
}

void Workspace::selectPrevious(){
	switch(this->mode){
	case Workspace::MODE_POINTS:
		this->selectPrevious(&this->pointsIndex, this->points.size());
		break;
	case Workspace::MODE_SHAPES:
		this->selectPrevious(&this->shapesIndex, this->shapes.size());
		break;
	case Workspace::MODE_TRANSFORMATIONS:
		this->selectPrevious(&this->transformationsIndex, this->transformations.size());
		break;
	default:
		break;
	}
}

void Workspace::selectPrevious(int* const index, const size_t size){
	if(size > 0){
		--(*index);
		if(*index < 0){
			*index = size - 1;
		}
	}
}

void Workspace::selectNext(){
	switch(this->mode){
	case Workspace::MODE_POINTS:
		this->selectNext(&this->pointsIndex, this->points.size());
		break;
	case Workspace::MODE_SHAPES:
		this->selectNext(&this->shapesIndex, this->shapes.size());
		break;
	case Workspace::MODE_TRANSFORMATIONS:
		this->selectNext(&this->transformationsIndex, this->transformations.size());
		break;
	default:
		break;
	}
}

void Workspace::selectNext(int* const index, const size_t size){
	if(size > 0){
		++(*index);
		if((size_t)(*index) >= size){
			*index = 0;
		}
	}
}

void Workspace::selectNextPrinter(){
	this->selectNext(&this->printerIndex, Workspace::PRINTERS_SIZE);
}

void Workspace::moveSelectedObjectUp(const float amount){
	Vec3f up(this->camera->getUpVector());
	up.multiply(amount);
	if((this->mode == Workspace::MODE_POINTS) && (this->points.size() > 0)){
		this->points[this->pointsIndex]->addPosition(&up);
	}else if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		this->shapes[this->shapesIndex]->addPosition(&up);
	}else if((this->mode == Workspace::MODE_TRANSFORMATIONS) && (this->transformations.size() > 0)){
		this->transformations[this->transformationsIndex]->addPosition(&up);
	}
}

void Workspace::moveSelectedObjectRight(const float amount){
	Vec3f right(this->camera->getLookAtVector());
	right.cross(this->camera->getUpVector());
	right.normalize();
	right.multiply(amount);
	if((this->mode == Workspace::MODE_POINTS) && (this->points.size() > 0)){
		this->points[this->pointsIndex]->addPosition(&right);
	}else if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		this->shapes[this->shapesIndex]->addPosition(&right);
	}else if((this->mode == Workspace::MODE_TRANSFORMATIONS) && (this->transformations.size() > 0)){
		this->transformations[this->transformationsIndex]->addPosition(&right);
	}
}

void Workspace::sizeSelectedObjectUp(const float amount){
	Vec3f up(this->camera->getUpVector());
	up.multiply(amount);
	if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		this->shapes[this->shapesIndex]->addSize(&up);
	}
}

void Workspace::sizeSelectedObjectRight(const float amount){
	Vec3f right(this->camera->getLookAtVector());
	right.cross(this->camera->getUpVector());
	right.normalize();
	right.multiply(amount);
	if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		this->shapes[this->shapesIndex]->addSize(&right);
	}
}

void Workspace::showOrHideSelectedObject(){
	if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		Shape* const shape = this->shapes[this->shapesIndex];
		shape->setIsVisible(!shape->getIsVisible());
	}
}

void Workspace::transformSelectedShape(Transformation* const transformation){
	if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		this->shapes[this->shapesIndex]->addTransformation(transformation);
	}
}

void Workspace::newPoint(Point* const point){
	this->fixIdentifier(point, &this->pointsByIdentifier, &Workspace::nextPointSequenceNumber);
	point->setWorkspace(this);
	point->workspaceIndex = this->points.size();
	this->pointsByIdentifier[point->getIdentifier()] = point;
	this->points.push_back(point);
}

void Workspace::newShape(Shape* const shape){
	this->fixIdentifier(shape, &this->shapesByIdentifier, &Workspace::nextShapeSequenceNumber);
	shape->setWorkspace(this);
	shape->workspaceIndex = this->shapes.size();
	this->shapesByIdentifier[shape->getIdentifier()] = shape;
	this->shapes.push_back(shape);
}

void Workspace::newTransformation(Transformation* const transformation){
	this->fixIdentifier(transformation, &this->transformationsByIdentifier, &Workspace::nextTransformationSequenceNumber);
	this->transformationsByIdentifier[transformation->getIdentifier()] = transformation;
	this->transformations.push_back(transformation);
}

void Workspace::newFrame(Frame* const frame){
	this->animation.addFrame(frame);
}

template <typename T, typename U>
void Workspace::fixIdentifier(T* const toFix, U* const byIdentifier, int (Workspace::*const nextSequenceNumber)()){
	while(true){
		toFix->setIdentifier((this->*nextSequenceNumber)());
		if(byIdentifier->count(toFix->getIdentifier()) == 0){
			break;
		}
	}
}

void Workspace::newPointFromParser(Point* const point){
	point->setWorkspace(this);
	point->workspaceIndex = this->points.size();
	this->points.push_back(point);
}

void Workspace::newShapeFromParser(Shape* const shape){
	shape->setWorkspace(this);
	shape->workspaceIndex = this->shapes.size();
	this->shapes.push_back(shape);
}

void Workspace::newTransformationFromParser(Transformation* const transformation){
	this->transformations.push_back(transformation);
}

void Workspace::newFrameFromParser(Frame* const frame){
	this->animation.addFrame(frame);
}

void Workspace::applyToSelected(const int function, const float argument){
	if((this->mode == Workspace::MODE_POINTS) && (this->points.size() > 0)){
		this->points[this->pointsIndex]->apply(function, argument);
	}else if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		this->shapes[this->shapesIndex]->apply(function, argument);
	}else if((this->mode == Workspace::MODE_TRANSFORMATIONS) && (this->transformations.size() > 0)){
		this->transformations[this->transformationsIndex]->apply(function, argument);
	}
}

void Workspace::applyToPointFromIdentifiers(const int function, const std::string& receiverIdentifier, const std::string& elementIdentifier){
	Point* point;
	this->findPoint(receiverIdentifier, &point);
	if(point){
		point->apply(function, elementIdentifier);
	}
}

void Workspace::applyToShapeFromIdentifiers(const int function, const std::string& receiverIdentifier, const std::string& elementIdentifier){
	Shape* shape;
	this->findShape(receiverIdentifier, &shape);
	if(shape){
		shape->apply(function, elementIdentifier);
	}
}

bool Workspace::applyToLastFrame(const int function, const float argument){
	return this->animation.applyToLastFrame(function, argument);
}

bool Workspace::processInterpolationFunction(const int type, const std::string& identifier, const int selector, const float value){
	void* object;
	this->findPoint(identifier, (Point**)&object);
	if(object){
		return ((Point*)object)->animateOn(&this->animation, type, selector, value);
	}
	this->findShape(identifier, (Shape**)&object);
	if(object){
		return ((Shape*)object)->animateOn(&this->animation, type, selector, value);
	}
	this->findTransformation(identifier, (Transformation**)&object);
	if(object){
		return ((Transformation*)object)->animateOn(&this->animation, type, selector, value);
	}
	return false;
}

void Workspace::prompt(const int function, const std::string& prompt){
	float value;
	std::cout << prompt << " ";
	std::cin >> value;
	this->applyToSelected(function, value);
}

void Workspace::promptFrameTime(const int function, const std::string& prompt){
	float value;
	std::cout << prompt << " ";
	std::cin >> value;
	this->applyToLastFrame(function, value);
}

void Workspace::prompt(const int function, const std::string& prompt1, const std::string& prompt2){
	std::string value1;
	std::string value2;
	std::cout << prompt1 << " ";
	std::cin >> value1;
	std::cout << prompt2 << " ";
	std::cin >> value2;
	switch(function){
	case Parser::FUNCTION_NAME_POINTS:
		this->applyToPointFromIdentifiers(function, value2, value1);
		break;
	case Parser::FUNCTION_NAME_SHAPES:
		this->applyToShapeFromIdentifiers(function, value2, value1);
		break;
	case Parser::FUNCTION_NAME_TRANSFORMATIONS:
		this->applyToShapeFromIdentifiers(function, value1, value2);
		break;
	default:
		break;
	}
}

void Workspace::promptFrameFunction(const int type, const std::string& prompt1, const std::string& prompt2, const std::string& prompt3){
	std::string identifier;
	std::string selector;
	float value;
	std::cout << prompt1 << " ";
	std::cin >> identifier;
	std::cout << prompt2 << " ";
	std::cin >> selector;
	std::cout << prompt3 << " ";
	std::cin >> value;
	this->processInterpolationFunction(type, identifier, Animation::selectorFromWorkspaceString(selector), value);
}

void Workspace::render() const{
	const Mat4f* const projection = this->camera->getProjectionMatrix();
	const Mat4f* const view = this->camera->getViewMatrix();
	const float* const cameraPositionComponents = this->camera->getPosition()->getComponents();
	Mat4f parentTransformations;
	parentTransformations.identity();
	for(size_t i = 0; i < this->shapes.size(); ++i){
		if(!this->shapes[i]->getIsVisible()){
			continue;
		}
		this->shapes[i]->render(projection, view, &parentTransformations, cameraPositionComponents, (this->mode == Workspace::MODE_SHAPES) && (i == (size_t)this->shapesIndex));
	}
}

void Workspace::buildAnimation(){
	if(this->animation.build()){
		std::cout << "Animation initialized successfully." << std::endl;
	}else{
		std::cout << "Animation failed to initialize." << std::endl;
	}
}

void Workspace::playAnimation(){
	this->isPlayingAnimation = true;
}

void Workspace::pauseAnimation(){
	this->isPlayingAnimation = false;
}

void Workspace::stopAnimation(){
	this->isPlayingAnimation = false;
	this->animationTime = 0.0f;
	this->animation.reset();
}

void Workspace::tickAnimation(){
	if(this->isPlayingAnimation){
		this->animationTime += 1.0f;
		if(!this->animation.tick(this->animationTime)){
			this->animationTime = 0.0f;
		}
		glutPostRedisplay();
	}
}

void Workspace::compile() const{
	std::string filename(this->directory);
	filename += "/output/compiled.cpp";
	std::ofstream file(filename.c_str(), std::ofstream::out);
	CompilePrinter printer(new IndentedStream(file));
	this->printAllOf(&this->points, &printer);
	this->printAllOf(&this->transformations, &printer);
	this->printAllOf(&this->shapes, &printer);
	this->animation.print(&printer);
	delete printer.getStream();
	file.close();
}

void Workspace::backup() const{
	std::string points(this->directory);
	std::string shapes(this->directory);
	std::string transformations(this->directory);
	std::string animations(this->directory);
	points += "/backup/points.txt";
	shapes += "/backup/shapes.txt";
	transformations += "/backup/transformations.txt";
	animations += "/backup/animations.txt";
	this->saveToFile(points, &this->points);
	this->saveToFile(shapes, &this->shapes);
	this->saveToFile(transformations, &this->transformations);
	this->saveToFile(animations, &this->animation);
}

void Workspace::save() const{
	std::string points(this->directory);
	std::string shapes(this->directory);
	std::string transformations(this->directory);
	std::string animations(this->directory);
	points += "/points.txt";
	shapes += "/shapes.txt";
	transformations += "/transformations.txt";
	animations += "/animations.txt";
	this->saveToFile(points, &this->points);
	this->saveToFile(shapes, &this->shapes);
	this->saveToFile(transformations, &this->transformations);
	this->saveToFile(animations, &this->animation);
}

void Workspace::saveToFile(const std::string& filename, const Animation* const animation) const{
	std::ofstream file(filename.c_str(), std::ofstream::out);
	WorkspacePrinter printer(new IndentedStream(file));
	animation->print(&printer);
	delete printer.getStream();
	file.close();
}

template <typename T>
void Workspace::saveToFile(const std::string& filename, T* const objects) const{
	std::ofstream file(filename.c_str(), std::ofstream::out);
	WorkspacePrinter printer(new IndentedStream(file));
	this->printAllOf(objects, &printer);
	delete printer.getStream();
	file.close();
}

void Workspace::printOverview() const{
	printf("// Workspace: %lu points, %lu shapes, and %lu transformations.\n", this->points.size(), this->shapes.size(), this->transformations.size());
}

void Workspace::print() const{
	this->printOverview();
	puts("\n// Points");
	this->printAllOf(&this->points);
	puts("\n// Transformations");
	this->printAllOf(&this->transformations);
	puts("\n// Shapes");
	this->printAllOf(&this->shapes);
	puts("\n// Animation");
	this->animation.print(this->printers[this->printerIndex]);
}

template <typename T>
void Workspace::printAllOf(T* const objects) const{
	for(size_t i = 0; i < objects->size(); ++i){
		objects->at(i)->print(this->printers[this->printerIndex]);
	}
}

template <typename T>
void Workspace::printAllOf(T* const objects, Printer* const printer) const{
	for(size_t i = 0; i < objects->size(); ++i){
		objects->at(i)->print(printer);
	}
}

void Workspace::printSelected() const{
	this->printOverview();
	if((this->mode == Workspace::MODE_POINTS) && (this->points.size() > 0)){
		printf("\n// Point [%d]\n", this->pointsIndex);
		this->points[this->pointsIndex]->print(this->printers[this->printerIndex]);
	}else if((this->mode == Workspace::MODE_SHAPES) && (this->shapes.size() > 0)){
		printf("\n// Shape [%d]\n", this->shapesIndex);
		this->shapes[this->shapesIndex]->print(this->printers[this->printerIndex]);
	}else if((this->mode == Workspace::MODE_TRANSFORMATIONS) && (this->transformations.size() > 0)){
		printf("\n// Transformation [%d]\n", this->transformationsIndex);
		this->transformations[this->transformationsIndex]->print(this->printers[this->printerIndex]);
	}
}

#define DELETE_ALL(X) for(i = 0; i < this->X.size(); ++i){delete this->X[i];}

Workspace::~Workspace(){
	size_t i;
	DELETE_ALL(shapes)
	DELETE_ALL(transformations)
	DELETE_ALL(points)
	for(i = 0; i < Workspace::PRINTERS_SIZE; ++i){
		delete this->printers[i]->getStream();
		delete this->printers[i];
	}
	delete this->camera;
	puts("\nCleanup complete.");
}
