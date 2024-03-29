#include "Solver.h"
#include "vectors.h"
#include "units.h"

float Solver::imageReferenceLinePoints[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float Solver::imageOrigin[2] = {0.0f, 0.0f};
float Solver::mmPerPx = 0.0f;

Solver::Solver(){}

void Solver::setImageDistanceRatio(){
	SOLVER_PROMPT(x1, Solver::imageReferenceLinePoints[0])
	SOLVER_PROMPT(y1, Solver::imageReferenceLinePoints[1])
	SOLVER_PROMPT(x2, Solver::imageReferenceLinePoints[2])
	SOLVER_PROMPT(y2, Solver::imageReferenceLinePoints[3])
	SOLVER_PROMPT(length of reference line in mm, Solver::mmPerPx)
	if(Solver::mmPerPx < 0.0f){
		Solver::mmPerPx = -Solver::mmPerPx;
	}
	const float xDifference = Solver::imageReferenceLinePoints[0] - Solver::imageReferenceLinePoints[2];
	const float yDifference = Solver::imageReferenceLinePoints[1] - Solver::imageReferenceLinePoints[3];
	const float pxDistance = sqrt((xDifference * xDifference) + (yDifference * yDifference));
	if(pxDistance > 0.0f){
		Solver::mmPerPx /= pxDistance;
		std::cout << "ratio = " << Solver::mmPerPx << " mm/px" << std::endl;
	}else{
		Solver::mmPerPx = 0.0f;
		std::cout << "ratio = undefined mm/px" << std::endl;
	}
}

void Solver::setImageOriginToAbsoluteCoordinates(){
	SOLVER_PROMPT(x, Solver::imageOrigin[0])
	SOLVER_PROMPT(y, Solver::imageOrigin[1])
	std::cout << "Set image origin to (" << Solver::imageOrigin[0] << " px, " << Solver::imageOrigin[1] << " px)" << std::endl;
}

void Solver::setImageOriginToMidpointOfLine(){
	SOLVER_PROMPT(x1, Solver::imageReferenceLinePoints[0])
	SOLVER_PROMPT(y1, Solver::imageReferenceLinePoints[1])
	SOLVER_PROMPT(x2, Solver::imageReferenceLinePoints[2])
	SOLVER_PROMPT(y2, Solver::imageReferenceLinePoints[3])
	Solver::imageOrigin[0] = (Solver::imageReferenceLinePoints[0] + Solver::imageReferenceLinePoints[2]) / 2.0f;
	Solver::imageOrigin[1] = (Solver::imageReferenceLinePoints[1] + Solver::imageReferenceLinePoints[3]) / 2.0f;
	std::cout << "Set image origin to (" << Solver::imageOrigin[0] << " px, " << Solver::imageOrigin[1] << " px)" << std::endl;
}

void Solver::convertImageDistanceToMM(){
	if(Solver::mmPerPx > 0.0f){
		SOLVER_PROMPT(x1, Solver::imageReferenceLinePoints[0])
		SOLVER_PROMPT(y1, Solver::imageReferenceLinePoints[1])
		SOLVER_PROMPT(x2, Solver::imageReferenceLinePoints[2])
		SOLVER_PROMPT(y2, Solver::imageReferenceLinePoints[3])
		const float xDifference = Solver::imageReferenceLinePoints[0] - Solver::imageReferenceLinePoints[2];
		const float yDifference = Solver::imageReferenceLinePoints[1] - Solver::imageReferenceLinePoints[3];
		std::cout << "The distance from (x1, y1) to (x2, y2) = " << (sqrt((xDifference * xDifference) + (yDifference * yDifference)) * Solver::mmPerPx) << " mm" << std::endl;
	}else{
		std::cout << "Please set the mm/px ratio." << std::endl;
	}
}

void Solver::imageCoordinatesRelativeToImageOrigin(){
	float components[2];
	SOLVER_PROMPT(x, components[0])
	SOLVER_PROMPT(y, components[1])
	components[0] -= Solver::imageOrigin[0];
	components[1] -= Solver::imageOrigin[1];
	if(Solver::mmPerPx > 0.0f){
		std::cout << "The relative coordinate is (" << (components[0] * Solver::mmPerPx) << " mm, " << (components[1] * Solver::mmPerPx) << " mm)" << std::endl;
	}else{
		std::cout << "The relative coordinate is (" << components[0] << " px, " << components[1] << " px)" << std::endl;
	}
}

void Solver::trigonometricRatiosFromAnAngle(){
	float angle;
	SOLVER_PROMPT(angle, angle)
	const float sinAngle = sin(DEGREES_TO_RADIANS(angle));
	const float cosAngle = cos(DEGREES_TO_RADIANS(angle));
	std::cout << "sin(" << angle << ") = " << sinAngle << std::endl;
	std::cout << "cos(" << angle << ") = " << cosAngle << std::endl;
	if(cosAngle != 0.0f){
		std::cout << "tan(" << angle << ") = " << (sinAngle / cosAngle) << std::endl;
	}
	if(sinAngle != 0.0f){
		std::cout << "csc(" << angle << ") = " << (1.0f / sinAngle) << std::endl;
	}
	if(cosAngle != 0.0f){
		std::cout << "sec(" << angle << ") = " << (1.0f / cosAngle) << std::endl;
	}
	if(sinAngle != 0.0f){
		std::cout << "cot(" << angle << ") = " << (cosAngle / sinAngle) << std::endl;
	}
}

void Solver::fourQuadrantInverseTangent(){
	float vertical;
	float horizontal;
	SOLVER_PROMPT(vertical component, vertical)
	SOLVER_PROMPT(horizontal component, horizontal)
	std::cout << "atan2(" << vertical << ", " << horizontal << ") = " << RADIANS_TO_DEGREES(atan2(vertical, horizontal)) << std::endl;
}

void Solver::distanceBetweenTwoPoints(){
	float components[6];
	SOLVER_PROMPT(x1, components[0])
	SOLVER_PROMPT(y1, components[1])
	SOLVER_PROMPT(z1, components[2])
	SOLVER_PROMPT(x2, components[3])
	SOLVER_PROMPT(y2, components[4])
	SOLVER_PROMPT(z2, components[5])
	components[0] -= components[3];
	components[1] -= components[4];
	components[2] -= components[5];
	components[0] *= components[0];
	components[1] *= components[1];
	components[2] *= components[2];
	std::cout << "distance = " << sqrt(components[0] + components[1] + components[2]) << std::endl;
}

void Solver::vectorFromSubtractionOfTwoPoints(){
	float components[6];
	SOLVER_PROMPT(x1, components[0])
	SOLVER_PROMPT(y1, components[1])
	SOLVER_PROMPT(z1, components[2])
	SOLVER_PROMPT(x2, components[3])
	SOLVER_PROMPT(y2, components[4])
	SOLVER_PROMPT(z2, components[5])
	components[0] = components[3] - components[0];
	components[1] = components[4] - components[1];
	components[2] = components[5] - components[2];
	SOLVER_PRINT_COMPONENTS("The vector originating from (x1, y1, z1) pointing towards (x2, y2, z2)", components)
}

void Solver::angleBetweenTwoVectors(){
	Vec3f u;
	Vec3f v;
	SOLVER_PROMPT(x1, u.components[0])
	SOLVER_PROMPT(y1, u.components[1])
	SOLVER_PROMPT(z1, u.components[2])
	SOLVER_PROMPT(x2, v.components[0])
	SOLVER_PROMPT(y2, v.components[1])
	SOLVER_PROMPT(z2, v.components[2])
	u.normalize();
	v.normalize();
	SOLVER_PRINT_ANGLE(u, v)
}

void Solver::crossProduct(){
	Vec3f u;
	Vec3f v;
	SOLVER_PROMPT(x1, u.components[0])
	SOLVER_PROMPT(y1, u.components[1])
	SOLVER_PROMPT(z1, u.components[2])
	SOLVER_PROMPT(x2, v.components[0])
	SOLVER_PROMPT(y2, v.components[1])
	SOLVER_PROMPT(z2, v.components[2])
	u.cross(&v);
	SOLVER_PRINT_VECTOR("(x1, y1, z1) cross (x2, y2, z2)", u)
}

void Solver::parametersToRotateOneVectorToAnother(){
	Vec3f u;
	Vec3f v;
	SOLVER_PROMPT(x1, u.components[0])
	SOLVER_PROMPT(y1, u.components[1])
	SOLVER_PROMPT(z1, u.components[2])
	SOLVER_PROMPT(x2, v.components[0])
	SOLVER_PROMPT(y2, v.components[1])
	SOLVER_PROMPT(z2, v.components[2])
	u.normalize();
	v.normalize();
	SOLVER_PRINT_ANGLE(u, v)
	u.cross(&v);
	SOLVER_PRINT_VECTOR("(x, y, z)", u)
}

void Solver::parametersToRotateTwoVectorsDefinedByThreePoints(){
	Vec3f from;
	Vec3f origin;
	Vec3f to;
	SOLVER_PROMPT(from.x, from.components[0])
	SOLVER_PROMPT(from.y, from.components[1])
	SOLVER_PROMPT(from.z, from.components[2])
	SOLVER_PROMPT(origin.x, origin.components[0])
	SOLVER_PROMPT(origin.y, origin.components[1])
	SOLVER_PROMPT(origin.z, origin.components[2])
	SOLVER_PROMPT(to.x, to.components[0])
	SOLVER_PROMPT(to.y, to.components[1])
	SOLVER_PROMPT(to.z, to.components[2])
	from.components[0] -= origin.components[0];
	from.components[1] -= origin.components[1];
	from.components[2] -= origin.components[2];
	to.components[0] -= origin.components[0];
	to.components[1] -= origin.components[1];
	to.components[2] -= origin.components[2];
	from.normalize();
	to.normalize();
	SOLVER_PRINT_ANGLE(from, to)
	from.cross(&to);
	SOLVER_PRINT_VECTOR("(x, y, z)", from)
}

void Solver::orthonormalBasisVectorsFromPositiveZ(){}

void Solver::orthonormalBasisMatrixFromPositiveZ(){}

void Solver::inverseMatrix(){}

void Solver::transposeMatrix(){}

void Solver::multiplyTwoMatrices(){}

Solver::~Solver(){}
