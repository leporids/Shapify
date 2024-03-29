#ifndef SOLVER_H
#define SOLVER_H
#include <iostream>
#define SOLVER_PROMPT(type, variable) \
	std::cout << "Please enter " #type ": "; \
	std::cin >> variable;
#define SOLVER_DOT_PRODUCT(u, v) (u.components[0] * v.components[0]) + (u.components[1] * v.components[1]) + (u.components[2] * v.components[2])
#define SOLVER_PRINT_ANGLE(u, v) \
	if((u.length() > 0.0f) && (v.length() > 0.0f)){ \
		std::cout << "angle = " << RADIANS_TO_DEGREES(acos(SOLVER_DOT_PRODUCT(u, v))) << std::endl; \
	}else{ \
		std::cout << "angle = undefined" << std::endl; \
	}
#define SOLVER_PRINT_COMPONENTS(name, xyz) std::cout << name << " = (" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << ")" << std::endl;
#define SOLVER_PRINT_VECTOR(name, vector) SOLVER_PRINT_COMPONENTS(name, vector.components)
class Solver {
	public:
		static float imageReferenceLinePoints[4];
		static float imageOrigin[2];
		static float mmPerPx;

		static void setImageDistanceRatio();
		static void setImageOriginToAbsoluteCoordinates();
		static void setImageOriginToMidpointOfLine();
		static void convertImageDistanceToMM();
		static void imageCoordinatesRelativeToImageOrigin();

		static void trigonometricRatiosFromAnAngle();
		static void fourQuadrantInverseTangent();

		static void distanceBetweenTwoPoints();
		static void vectorFromSubtractionOfTwoPoints();
		static void angleBetweenTwoVectors();
		static void crossProduct();

		static void parametersToRotateOneVectorToAnother();
		static void parametersToRotateTwoVectorsDefinedByThreePoints();
		static void orthonormalBasisVectorsFromPositiveZ();

		static void orthonormalBasisMatrixFromPositiveZ();
		static void inverseMatrix();
		static void transposeMatrix();
		static void multiplyTwoMatrices();

		~Solver();

	private:
		Solver();
};
#endif // SOLVER_H
