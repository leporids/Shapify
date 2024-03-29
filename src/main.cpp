#include <GL/glew.h>
#include <GL/glut.h>
#include "Grid.h"
#include "Parser.h"
#include "Axis2D.h"
#include "Loft.h"
#include "Solver.h"

Grid grid(FROM_MM(1), FROM_MM(100));
Workspace workspace;
Parser parser(&workspace);
Axis2D axis;

void displayFunc();
void reshapeFunc(int width, int height);
void keyFunc(unsigned char key, int x, int y);
void timerFunc(int value);
void shapesMenuFunc(int option);
void methodsMenuFunc(int option);
void transformationsMenuFunc(int option);
void measurementsMenuFunc(int option);
void solverMenuFunc(int option);
void cameraMenuFunc(int option);
void animationMenuFunc(int option);
void menuFunc(int option);

namespace MENU {
	enum {
		BOX,
		CONE,
		CYLINDER,
		SPHERE,
		COMPOUND,
		LOFT,

		METHOD_X,
		METHOD_Y,
		METHOD_Z,
		METHOD_LENGTH,
		METHOD_WIDTH,
		METHOD_HEIGHT,
		METHOD_RADIUS,
		METHOD_ANGLE,
		METHOD_TIME,
		METHOD_TYPE,
		METHOD_TRANSFORM_WITH,
		METHOD_ATTACH_SHAPE_TO,
		METHOD_ATTACH_POINT_TO,
		METHOD_DETACH,

		TRANSLATE,
		ROTATE,
		SCALE,

		GRID,
		MEASURE_DISTANCE,
		MEASURE_ANGLE,

		SET_IMAGE_DISTANCE_RATIO,
		SET_IMAGE_ORIGIN_POINT_ABSOLUTE,
		SET_IMAGE_ORIGIN_POINT_MIDPOINT,
		CONVERT_IMAGE_DISTANCE_TO_MM,
		RELATIVE_IMAGE_COORDINATES,
		TRIGONOMETRIC_RATIOS,
		FOUR_QUADRANT_INVERSE_TANGENT,
		DISTANCE_BETWEEN_POINTS,
		VECTOR_FROM_TWO_POINTS,
		ANGLE_BETWEEN_VECTORS,
		CROSS_PRODUCT,
		ROTATION_MATRIX_PARAMETERS_ABSOLUTE_ORIGIN,
		ROTATION_MATRIX_PARAMETERS_RELATIVE_ORIGIN,
		ORTHONORMAL_BASIS_VECTORS,
		ORTHONORMAL_BASIS_MATRIX,
		INVERSE_MATRIX,
		TRANSPOSE_MATRIX,
		MATRIX_MULTIPLICATION,

		ANTERIOR,
		LATERAL_LEFT,
		POSTERIOR,
		LATERAL_RIGHT,
		DORSAL,
		VENTRAL,

		NEW_FRAME,
		SET_TO_INTERPOLATION,
		LINEAR_INTERPOLATION,
		BUILD_ANIMATION,
		PLAY_ANIMATION,
		PAUSE_ANIMATION,
		STOP_ANIMATION,

		NEW_POINT,
		NEW_PLANE,
		COMPILE,
		BACKUP,
		SAVE,
		QUIT
	};
}

void createMenu();

int main(int argc, char* argv[]){
	if(argc < 2){
		puts("Please enter the workspace directory as a command line argument.");
		return 1;
	}
	const char* const directory = argv[1];
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(Camera::DEFAULT_WINDOW_WIDTH, Camera::DEFAULT_WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shape Editor");
	glEnable(GL_DEPTH);
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyFunc);
	glutTimerFunc(100, timerFunc, 0);
	createMenu();
	GLenum glewError = glewInit();
	if(glewError != GLEW_OK){
		printf("Error: %s\n", glewGetErrorString(glewError));
		return 1;
	}
	if(parser.parseDirectory(directory)){
		printf("\nParser has parsed directory \"%s\".\nAllocated %lu points, %lu shapes, and %lu transformations.\n", directory, workspace.getPoints()->size(), workspace.getShapes()->size(), workspace.getTransformations()->size());
		glutMainLoop();
	}else{
		puts("\nParser has encountered an error.");
	}
	return 0;
}

void displayFunc(){
	glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(workspace.getCamera()->getFovAngle(), workspace.getCamera()->getRatio(), workspace.getCamera()->getZNear(), workspace.getCamera()->getZFar());
	glDisable(GL_DEPTH_TEST);
	workspace.getCamera()->loadMatrix();
	grid.render();
	glEnable(GL_DEPTH_TEST);
	workspace.render();
	axis.render(workspace.getCamera()->getWindowWidth(), workspace.getCamera()->getWindowHeight());
	glutSwapBuffers();
}

void reshapeFunc(int width, int height){
	glViewport(0, 0, width, height);
	workspace.getCamera()->reshape(width, height);
}

void keyFunc(unsigned char key, int x, int y){
	switch(key){
	case '1':
		cameraMenuFunc(MENU::ANTERIOR);
		break;
	case '2':
		cameraMenuFunc(MENU::LATERAL_LEFT);
		break;
	case '3':
		cameraMenuFunc(MENU::POSTERIOR);
		break;
	case '4':
		cameraMenuFunc(MENU::LATERAL_RIGHT);
		break;
	case '5':
		cameraMenuFunc(MENU::DORSAL);
		break;
	case '6':
		cameraMenuFunc(MENU::VENTRAL);
		break;
	case '8':
		workspace.setMode(Workspace::MODE_POINTS);
		break;
	case '9':
		workspace.setMode(Workspace::MODE_SHAPES);
		break;
	case '0':
		workspace.setMode(Workspace::MODE_TRANSFORMATIONS);
		break;
	case 'w':
		workspace.moveSelectedObjectUp(5.0f);
		break;
	case 'W':
		workspace.sizeSelectedObjectUp(5.0f);
		break;
	case 'a':
		workspace.moveSelectedObjectRight(-5.0f);
		break;
	case 'A':
		workspace.sizeSelectedObjectRight(-5.0f);
		break;
	case 's':
		workspace.moveSelectedObjectUp(-5.0f);
		break;
	case 'S':
		workspace.sizeSelectedObjectUp(-5.0f);
		break;
	case 'd':
		workspace.moveSelectedObjectRight(5.0f);
		break;
	case 'D':
		workspace.sizeSelectedObjectRight(5.0f);
		break;
	case 'n':
		workspace.selectNext();
		break;
	case 'N':
		workspace.selectPrevious();
		break;
	case 'o':
		workspace.print();
		break;
	case 'O':
		workspace.selectNextPrinter();
		break;
	case 'p':
		workspace.printSelected();
		break;
	case ' ':
		workspace.showOrHideSelectedObject();
		break;
	case 'h':
		workspace.getCamera()->moveRight(-5.0f);
		break;
	case 'j':
		workspace.getCamera()->moveUp(-5.0f);
		break;
	case 'k':
		workspace.getCamera()->moveUp(5.0f);
		break;
	case 'l':
		workspace.getCamera()->moveRight(5.0f);
		break;
	case ',':
		workspace.getCamera()->moveForwards(5.0f);
		break;
	case '.':
		workspace.getCamera()->moveForwards(-5.0f);
		break;
	case 'z':
		workspace.getCamera()->zoomIn(5.0f);
		break;
	case 'Z':
		workspace.getCamera()->zoomIn(-5.0f);
		break;
	case '<':
		grid.decrease();
		break;
	case '>':
		grid.increase();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void timerFunc(int value){
	workspace.tickAnimation();
	glutTimerFunc(100, timerFunc, 0);
}

void shapesMenuFunc(int option){
	switch(option){
	case MENU::BOX:
		workspace.newShape((Shape*)(new Box()));
		break;
	case MENU::CONE:
		workspace.newShape((Shape*)(new Cone()));
		break;
	case MENU::CYLINDER:
		workspace.newShape((Shape*)(new Cylinder()));
		break;
	case MENU::SPHERE:
		workspace.newShape((Shape*)(new Sphere()));
		break;
	case MENU::COMPOUND:
		workspace.newShape((Shape*)(new Compound()));
		break;
	case MENU::LOFT:
		workspace.newShape((Shape*)(new Loft()));
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void methodsMenuFunc(int option){
	switch(option){
	case MENU::METHOD_X:
		workspace.prompt(Parser::FUNCTION_NAME_X, "x:");
		break;
	case MENU::METHOD_Y:
		workspace.prompt(Parser::FUNCTION_NAME_Y, "y:");
		break;
	case MENU::METHOD_Z:
		workspace.prompt(Parser::FUNCTION_NAME_Z, "z:");
		break;
	case MENU::METHOD_LENGTH:
		workspace.prompt(Parser::FUNCTION_NAME_LENGTH, "length:");
		break;
	case MENU::METHOD_WIDTH:
		workspace.prompt(Parser::FUNCTION_NAME_WIDTH, "width:");
		break;
	case MENU::METHOD_HEIGHT:
		workspace.prompt(Parser::FUNCTION_NAME_HEIGHT, "height:");
		break;
	case MENU::METHOD_RADIUS:
		workspace.prompt(Parser::FUNCTION_NAME_RADIUS, "radius:");
		break;
	case MENU::METHOD_ANGLE:
		workspace.prompt(Parser::FUNCTION_NAME_ANGLE, "angle:");
		break;
	case MENU::METHOD_TIME:
		workspace.promptFrameTime(Parser::FUNCTION_NAME_TIME, "time:");
		break;
	case MENU::METHOD_TYPE:
		workspace.prompt(Parser::FUNCTION_NAME_TYPE, "type:");
		break;
	case MENU::METHOD_TRANSFORM_WITH:
		workspace.prompt(Parser::FUNCTION_NAME_TRANSFORMATIONS, "transform:", "with:");
		break;
	case MENU::METHOD_ATTACH_SHAPE_TO:
		workspace.prompt(Parser::FUNCTION_NAME_SHAPES, "attachShape:", "to:");
		break;
	case MENU::METHOD_ATTACH_POINT_TO:
		workspace.prompt(Parser::FUNCTION_NAME_POINTS, "attachPoint:", "to:");
		break;
	case MENU::METHOD_DETACH:
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void transformationsMenuFunc(int option){
	switch(option){
	case MENU::TRANSLATE:
		workspace.newTransformation((Transformation*)(new Translate(0.0f, 0.0f, 0.0f)));
		break;
	case MENU::ROTATE:
		workspace.newTransformation((Transformation*)(new Rotate(0.0f, 0.0f, 0.0f, 0.0f)));
		break;
	case MENU::SCALE:
		workspace.newTransformation((Transformation*)(new Scale(0.0f, 0.0f, 0.0f)));
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void measurementsMenuFunc(int option){
	switch(option){
	case MENU::GRID:
		if(grid.isEnabled()){
			grid.disable();
		}else{
			grid.enable();
		}
		glutPostRedisplay();
		break;
	case MENU::MEASURE_DISTANCE:
		break;
	case MENU::MEASURE_ANGLE:
		break;
	default:
		break;
	}
}

void solverMenuFunc(int option){
	switch(option){
	case MENU::TRIGONOMETRIC_RATIOS:
		Solver::trigonometricRatiosFromAnAngle();
		break;
	case MENU::SET_IMAGE_DISTANCE_RATIO:
		Solver::setImageDistanceRatio();
		break;
	case MENU::SET_IMAGE_ORIGIN_POINT_ABSOLUTE:
		Solver::setImageOriginToAbsoluteCoordinates();
		break;
	case MENU::SET_IMAGE_ORIGIN_POINT_MIDPOINT:
		Solver::setImageOriginToMidpointOfLine();
		break;
	case MENU::CONVERT_IMAGE_DISTANCE_TO_MM:
		Solver::convertImageDistanceToMM();
		break;
	case MENU::RELATIVE_IMAGE_COORDINATES:
		Solver::imageCoordinatesRelativeToImageOrigin();
		break;
	case MENU::FOUR_QUADRANT_INVERSE_TANGENT:
		Solver::fourQuadrantInverseTangent();
		break;
	case MENU::DISTANCE_BETWEEN_POINTS:
		Solver::distanceBetweenTwoPoints();
		break;
	case MENU::VECTOR_FROM_TWO_POINTS:
		Solver::vectorFromSubtractionOfTwoPoints();
		break;
	case MENU::ANGLE_BETWEEN_VECTORS:
		Solver::angleBetweenTwoVectors();
		break;
	case MENU::CROSS_PRODUCT:
		Solver::crossProduct();
		break;
	case MENU::ROTATION_MATRIX_PARAMETERS_ABSOLUTE_ORIGIN:
		Solver::parametersToRotateOneVectorToAnother();
		break;
	case MENU::ROTATION_MATRIX_PARAMETERS_RELATIVE_ORIGIN:
		Solver::parametersToRotateTwoVectorsDefinedByThreePoints();
		break;
	case MENU::ORTHONORMAL_BASIS_VECTORS:
		Solver::orthonormalBasisVectorsFromPositiveZ();
		break;
	case MENU::ORTHONORMAL_BASIS_MATRIX:
		Solver::orthonormalBasisMatrixFromPositiveZ();
		break;
	case MENU::INVERSE_MATRIX:
		Solver::inverseMatrix();
		break;
	case MENU::TRANSPOSE_MATRIX:
		Solver::transposeMatrix();
		break;
	case MENU::MATRIX_MULTIPLICATION:
		Solver::multiplyTwoMatrices();
		break;
	default:
		break;
	}
}

void cameraMenuFunc(int option){
	switch(option){
	case MENU::ANTERIOR:
		grid.setMode(Grid::XY);
		axis.setMode(Axis2D::ANTERIOR_AXES);
		workspace.getCamera()->loadAnterior();
		break;
	case MENU::LATERAL_LEFT:
		grid.setMode(Grid::YZ);
		axis.setMode(Axis2D::LATERAL_LEFT_AXES);
		workspace.getCamera()->loadLateralLeft();
		break;
	case MENU::POSTERIOR:
		grid.setMode(Grid::XY);
		axis.setMode(Axis2D::POSTERIOR_AXES);
		workspace.getCamera()->loadPosterior();
		break;
	case MENU::LATERAL_RIGHT:
		grid.setMode(Grid::YZ);
		axis.setMode(Axis2D::LATERAL_RIGHT_AXES);
		workspace.getCamera()->loadLateralRight();
		break;
	case MENU::DORSAL:
		grid.setMode(Grid::XZ);
		axis.setMode(Axis2D::DORSAL_AXES);
		workspace.getCamera()->loadDorsal();
		break;
	case MENU::VENTRAL:
		grid.setMode(Grid::XZ);
		axis.setMode(Axis2D::VENTRAL_AXES);
		workspace.getCamera()->loadVentral();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void animationMenuFunc(int option){
	switch(option){
	case MENU::NEW_FRAME:
		workspace.newFrame((Frame*)(new Frame(0.0f)));
		break;
	case MENU::METHOD_TIME:
		workspace.promptFrameTime(Parser::FUNCTION_NAME_TIME, "time:");
		break;
	case MENU::SET_TO_INTERPOLATION:
		workspace.promptFrameFunction(FrameFunction::SET_TO, "identifier:", "selector:", "value:");
		break;
	case MENU::LINEAR_INTERPOLATION:
		workspace.promptFrameFunction(FrameFunction::LINEAR_TO, "identifier:", "selector:", "value:");
		break;
	case MENU::BUILD_ANIMATION:
		workspace.buildAnimation();
		break;
	case MENU::PLAY_ANIMATION:
		workspace.playAnimation();
		break;
	case MENU::PAUSE_ANIMATION:
		workspace.pauseAnimation();
		break;
	case MENU::STOP_ANIMATION:
		workspace.stopAnimation();
		break;
	default:
		break;
	}
}

void menuFunc(int option){
	switch(option){
	case MENU::NEW_POINT:
		workspace.newPoint((Point*)(new Point()));
		break;
	case MENU::NEW_PLANE:
		workspace.newPoint((Point*)(new PlanePoints()));
		break;
	case MENU::COMPILE:
		workspace.compile();
		break;
	case MENU::BACKUP:
		workspace.backup();
		break;
	case MENU::SAVE:
		workspace.save();
		break;
	case MENU::QUIT:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void createMenu(){
	int subMenuShapes = glutCreateMenu(shapesMenuFunc);
	glutAddMenuEntry("Box", MENU::BOX);
	glutAddMenuEntry("Cone", MENU::CONE);
	glutAddMenuEntry("Cylinder", MENU::CYLINDER);
	glutAddMenuEntry("Sphere", MENU::SPHERE);
	glutAddMenuEntry("Compound", MENU::COMPOUND);
	glutAddMenuEntry("Loft", MENU::LOFT);

	int subMenuMethods = glutCreateMenu(methodsMenuFunc);
	glutAddMenuEntry("x:", MENU::METHOD_X);
	glutAddMenuEntry("y:", MENU::METHOD_Y);
	glutAddMenuEntry("z:", MENU::METHOD_Z);
	glutAddMenuEntry("length:", MENU::METHOD_LENGTH);
	glutAddMenuEntry("width:", MENU::METHOD_WIDTH);
	glutAddMenuEntry("height:", MENU::METHOD_HEIGHT);
	glutAddMenuEntry("radius:", MENU::METHOD_RADIUS);
	glutAddMenuEntry("angle:", MENU::METHOD_ANGLE);
	glutAddMenuEntry("time:", MENU::METHOD_TIME);
	glutAddMenuEntry("type:", MENU::METHOD_TYPE);
	glutAddMenuEntry("transform:with:", MENU::METHOD_TRANSFORM_WITH);
	glutAddMenuEntry("attachShape:to:", MENU::METHOD_ATTACH_SHAPE_TO);
	glutAddMenuEntry("attachPoint:to:", MENU::METHOD_ATTACH_POINT_TO);
	glutAddMenuEntry("detach", MENU::METHOD_DETACH);

	int subMenuTransformations = glutCreateMenu(transformationsMenuFunc);
	glutAddMenuEntry("Translate", MENU::TRANSLATE);
	glutAddMenuEntry("Rotate", MENU::ROTATE);
	glutAddMenuEntry("Scale", MENU::SCALE);

	int subMenuMeasurements = glutCreateMenu(measurementsMenuFunc);
	glutAddMenuEntry("Grid", MENU::GRID);
	glutAddMenuEntry("Distance", MENU::MEASURE_DISTANCE);
	glutAddMenuEntry("Angle", MENU::MEASURE_ANGLE);

	int subMenuSolver = glutCreateMenu(solverMenuFunc);
	glutAddMenuEntry("Set Image Distance Ratio", MENU::SET_IMAGE_DISTANCE_RATIO);
	glutAddMenuEntry("Set Image Origin Point from Point", MENU::SET_IMAGE_ORIGIN_POINT_ABSOLUTE);
	glutAddMenuEntry("Set Image Origin Point from Midpoint", MENU::SET_IMAGE_ORIGIN_POINT_MIDPOINT);
	glutAddMenuEntry("Convert Image Distance to mm", MENU::CONVERT_IMAGE_DISTANCE_TO_MM);
	glutAddMenuEntry("Relative Image Coordinates", MENU::RELATIVE_IMAGE_COORDINATES);
	glutAddMenuEntry("Trigonometric Ratios", MENU::TRIGONOMETRIC_RATIOS);
	glutAddMenuEntry("Four Quadrant Inverse Tangent", MENU::FOUR_QUADRANT_INVERSE_TANGENT);
	glutAddMenuEntry("Distance Between Two Points", MENU::DISTANCE_BETWEEN_POINTS);
	glutAddMenuEntry("Vector From Subtraction of Two Points", MENU::VECTOR_FROM_TWO_POINTS);
	glutAddMenuEntry("Angle Between Two Vectors", MENU::ANGLE_BETWEEN_VECTORS);
	glutAddMenuEntry("Cross Product", MENU::CROSS_PRODUCT);
	glutAddMenuEntry("Rotation Parameters By Two Vectors", MENU::ROTATION_MATRIX_PARAMETERS_ABSOLUTE_ORIGIN);
	glutAddMenuEntry("Rotation Parameters By Three Points", MENU::ROTATION_MATRIX_PARAMETERS_RELATIVE_ORIGIN);
	glutAddMenuEntry("Orthonormal Basis Vectors", MENU::ORTHONORMAL_BASIS_VECTORS);
	glutAddMenuEntry("Orthonormal Basis Matrix", MENU::ORTHONORMAL_BASIS_MATRIX);
	glutAddMenuEntry("Inverse Matrix", MENU::INVERSE_MATRIX);
	glutAddMenuEntry("Transpose Matrix", MENU::TRANSPOSE_MATRIX);
	glutAddMenuEntry("Multiply Two Matrices", MENU::MATRIX_MULTIPLICATION);

	int subMenuCamera = glutCreateMenu(cameraMenuFunc);
	glutAddMenuEntry("Anterior", MENU::ANTERIOR);
	glutAddMenuEntry("Left Lateral", MENU::LATERAL_LEFT);
	glutAddMenuEntry("Posterior", MENU::POSTERIOR);
	glutAddMenuEntry("Right Lateral", MENU::LATERAL_RIGHT);
	glutAddMenuEntry("Dorsal", MENU::DORSAL);
	glutAddMenuEntry("Ventral", MENU::VENTRAL);

	int subMenuAnimation = glutCreateMenu(animationMenuFunc);
	glutAddMenuEntry("New Frame", MENU::NEW_FRAME);
	glutAddMenuEntry("time:", MENU::METHOD_TIME);
	glutAddMenuEntry("setTo:", MENU::SET_TO_INTERPOLATION);
	glutAddMenuEntry("linearTo:", MENU::LINEAR_INTERPOLATION);
	glutAddMenuEntry("Initialize", MENU::BUILD_ANIMATION);
	glutAddMenuEntry("Play", MENU::PLAY_ANIMATION);
	glutAddMenuEntry("Pause", MENU::PAUSE_ANIMATION);
	glutAddMenuEntry("Stop", MENU::STOP_ANIMATION);

	glutCreateMenu(menuFunc);
	glutAddMenuEntry("New Point", MENU::NEW_POINT);
	glutAddMenuEntry("New Plane", MENU::NEW_PLANE);
	glutAddSubMenu("Shapes", subMenuShapes);
	glutAddSubMenu("Methods", subMenuMethods);
	glutAddSubMenu("Transformations", subMenuTransformations);
	glutAddSubMenu("Measurements", subMenuMeasurements);
	glutAddSubMenu("Solver", subMenuSolver);
	glutAddSubMenu("Camera", subMenuCamera);
	glutAddSubMenu("Animation", subMenuAnimation);
	glutAddMenuEntry("Compile", MENU::COMPILE);
	glutAddMenuEntry("Backup", MENU::BACKUP);
	glutAddMenuEntry("Save", MENU::SAVE);
	glutAddMenuEntry("Quit", MENU::QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
